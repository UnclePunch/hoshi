// #include <math.h>
#include "hsd.h"
#include "inline.h"
#include "useful.h"
#include "obj.h"
#include "text.h"

#include "text_joint.h"

static GOBJ *text_joint_gobj;

void TextJoint_Init(int canvas_idx)
{

    text_joint_gobj = GOBJ_EZCreator(0, 0, 0,
                                     sizeof(TextJointLookup), TextJoint_Cleanup,
                                     0, 0,
                                     TextJoint_Think, 20,
                                     0, 0, 0);

    TextJointLookup *lookup = text_joint_gobj->userdata;
    lookup->num = 0;

    return;
}
void TextJoint_Cleanup()
{
}

Text *TextJoint_Create(JOBJ *j, int child_idx, int canvas_idx, char *string, int align_kind, float width, float height, GXColor *color)
{
    TextJointLookup *lookup = text_joint_gobj->userdata;

    JOBJ *attach_jobj;
    if (child_idx == 0)
        attach_jobj = j;
    else
        attach_jobj = JObj_GetIndex(j, child_idx);

    if (!attach_jobj)
    {
        OSReport("jobj %x does not have child %d\n", j, child_idx);
        assert("text_joint");
    }

    if (!string)
        string = attach_jobj->desc->class_name;

    Text *text = Text_CreateText(0, canvas_idx);
    text->kerning = 1;
    text->align = align_kind;
    text->gobj->gx_cb = TextJoint_GX;
    text->trans = (Vec3){0, -0.4, 0};
    text->viewport_scale = (Vec2){0.05 * 1, 0.05 * 1};

    if (width == 0 && height == 0)
        text->use_aspect = 0;
    else
    {
        text->use_aspect = 1;

        // scale aspect with joint
        Vec3 j_scale;
        JObj_SetupMtxSub(attach_jobj);
        HSD_MtxGetScale(&attach_jobj->rotMtx, &j_scale);

        text->aspect = (Vec2){width / j_scale.X, height / j_scale.Y};
    }

    text->color = *color;
    // text->viewport_color = (GXColor){255, 0, 0, 255};

    Text_AddSubtext(text, 0, -12, string);

    // add to lookup
    if (lookup->num >= GetElementsIn(lookup->entry))
    {
        OSReport("text_joint lookup num over\n");
        assert("0");
    }

    lookup->entry[lookup->num].text = text;
    lookup->entry[lookup->num].joint = attach_jobj;
    lookup->num++;

    return text;
}
void TextJoint_Destroy(Text *text)
{
    TextJointLookup *lookup = text_joint_gobj->userdata;

    // find this text on the lookup
    for (int i = 0; i < lookup->num; i++)
    {
        if (lookup->entry[i].text == text)
        {
            Text_Destroy(lookup->entry[i].text);

            // shift the rest up
            for (int j = i; j < lookup->num; j++)
                lookup->entry[j] = lookup->entry[j + 1];

            lookup->num--;

            break;
        }
    }
}
void TextJoint_Think()
{

    TextJointLookup *lookup = text_joint_gobj->userdata;

    // update text pos, scale, alpha
    for (int i = 0; i < lookup->num; i++)
    {
        JOBJ *j = lookup->entry[i].joint;
        Text *text = lookup->entry[i].text;

        // Vec3 pos, scale;
        // JObj_GetWorldPosition(j, 0, &pos);
        // HSD_MtxGetScale(&j->rotMtx, &scale);

        // text->trans = (Vec3){pos.X, -pos.Y - (0.4 * scale.Y), pos.Z};
        // text->viewport_scale = (Vec2){0.05 * scale.X, 0.05 * scale.Y};

        (j->flags & JOBJ_HIDDEN) ? (text->hidden = 1) : (text->hidden = 0);

        if (j->dobj &&
            j->dobj->mobj)
            text->color.a = j->dobj->mobj->mat->alpha * 255;
    }
}
void TextJoint_GX(GOBJ *g, int pass)
{

    if (pass != 2)
        return;

    COBJ *c = COBJ_GetCurrent();
    Text *t = g->userdata;

    JOBJ *j = TextJoint_FindJointFromText(t);

    JObj_SetupMtxSub(j);

    // use jobj matrix
    Mtx backup_mtx;
    PSMTXCopy(&c->view_mtx, &backup_mtx);
    PSMTXConcat(&c->view_mtx, &j->rotMtx, &c->view_mtx);

    // render text
    Text_GX(g, pass);

    // restore cobj mtx
    PSMTXCopy(&backup_mtx, &c->view_mtx);

    if (TJ_DEBUGWIDTH)
    {
        float width, height;
        Text_GetWidthAndHeight(t, &width, &height);
        if (t->use_aspect && width > t->aspect.X)
            width = t->aspect.X;

        Vec3 scale, trans;
        HSD_MtxGetTranslate(&j->rotMtx, &trans);
        HSD_MtxGetScale(&j->rotMtx, &scale);

        Vec3 start, end;
        start = (Vec3){trans.X + (t->trans.X * scale.X), trans.Y, trans.Z};
        end = (Vec3){start.X + (width * t->viewport_scale.X * scale.X), start.Y, start.Z};
        GX_DrawLine(&start,
                    &end,
                    16,
                    &(GXColor){255, 0, 0, 255});

        OSReport("text_joint %p has width %.2f\n", t, (width * t->viewport_scale.X * scale.X));
    }

    return;
}
JOBJ *TextJoint_FindJointFromText(Text *t)
{
    // find this text on the lookup
    JOBJ *j = 0;
    TextJointLookup *lookup = text_joint_gobj->userdata;
    for (int i = 0; i < lookup->num; i++)
    {
        if (lookup->entry[i].text == t)
        {
            j = lookup->entry[i].joint;
            break;
        }
    }

    if (!j)
    {
        OSReport("no joint found for text %x\n", t);
        assert("text_joint");
    }

    return j;
}
float TextJoint_GetWidth(Text *t)
{
    JOBJ *j = TextJoint_FindJointFromText(t);

    // get width as reported by text lib
    float width, height;
    Text_GetWidthAndHeight(t, &width, &height);

    // if using aspect, clamp width
    if (t->use_aspect && width > t->aspect.X)
        width = t->aspect.X;

    // return text width in jobj coordinates
    return (width * t->viewport_scale.X);
}