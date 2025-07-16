#ifndef MEX_H_INLINE
#define MEX_H_INLINE

#include "structs.h"
#include "obj.h"
#include "datatypes.h"
#include "gx.h"
#include "hsd.h"
#include "os.h"
#include "scene.h"
#include "preload.h"

#include <string.h>
// #include <math.h>

/*** Macros ***/
#define GetElementsIn(arr) sizeof(arr) / sizeof(arr[0])
#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)

/*** Functions ***/

// float fabs(float x)
// {
//     if (x < 0)
//         return -x;
//     else
//         return x;
// }

static void OSClearReports()
{
    memset(osreport_data->text, 0, osreport_data->size);
    osreport_data->cursor = 0;
    osreport_data->cursor_temp = 0;
    osreport_data->newline_num = 0;
}

static int Preload_GetEntryIndexFromEntrynum(int entry_num)
{
    Preload *preload = Preload_GetTable();
    for (int i = 0; i < GetElementsIn(preload->entry); i++)
    {
        PreloadEntry *this_entry = &preload->entry[i];

        // if this preload entry is in use and contains our file
        if (this_entry->load_state > 0 &&
            this_entry->file_entry_num == entry_num)
        {
            return i;
        }
    }

    return -1;
}
static PreloadEntry *Preload_GetEntryFromEntrynum(int entry_num)
{
    int preload_idx = Preload_GetEntryIndexFromEntrynum(entry_num);

    if (preload_idx != -1)
        return &Preload_GetTable()->entry[preload_idx];

    return 0;
}

static void MatAnimDesc_CopyTextures(MatAnimDesc *dst, MatAnimDesc *src)
{
    if (src->texture_anim->n_imagetbl > 0)
    {
        memcpy(&dst->texture_anim->imagetbl[0], // copy over texture image pointers
               &src->texture_anim->imagetbl[0],
               src->texture_anim->n_imagetbl * sizeof(void *));
    }

    if (src->texture_anim->n_tluttbl > 0)
    {
        memcpy(&dst->texture_anim->tluttbl[0], // copy over texture lookup pointers
               &src->texture_anim->tluttbl[0],
               src->texture_anim->n_tluttbl * sizeof(void *));
    }
}

static MatAnimJointDesc *MatAnimJointDesc_GetMatAnimDescByIndex(MatAnimJointDesc *j, int joint_idx, int *n)
{
    if (!j)
        return 0;

    if ((*n) == joint_idx)
        return j;

    if (j->child)
    {
        (*n)++;

        MatAnimJointDesc *j_check = MatAnimJointDesc_GetMatAnimDescByIndex(j->child, joint_idx, n);
        if (j_check)
            return j_check;
    }

    if (j->next)
    {
        (*n)++;

        MatAnimJointDesc *j_check = MatAnimJointDesc_GetMatAnimDescByIndex(j->next, joint_idx, n);
        if (j_check)
            return j_check;
    }

    return 0;
}
static MatAnimDesc *Matanimjoint_GetMatAnimDescByIndex(MatAnimJointDesc *root, int joint_idx, int dobj_idx)
{
    int n = 0;
    MatAnimJointDesc *j = MatAnimJointDesc_GetMatAnimDescByIndex(root, joint_idx, &n);

    if (!j)
        return 0;

    MatAnimDesc *this_dobj = j->matanim;

    if (!this_dobj)
        return 0;

    for (int this_idx = 0; this_idx < dobj_idx; this_idx++)
    {
        if (this_dobj->next)
            this_dobj = this_dobj->next;

        else
            return 0;
    }

    return this_dobj;
}

static int hashstr_32(char *str)
{

    unsigned int h;
    unsigned char *p;

    h = 0;
    for (p = (unsigned char *)str; *p != '\0'; p++)
        h = 31 * h + *p;
    return h; // or, h % ARRAY_SIZE;
}
static u16 hashstr_16(char *str)
{
    return hashstr_32(str) >> 16; // Ensure the output is 16-bit
}

static void AOBJ_CheckEnded(AOBJ *a, int *is_done)
{
    if (a->flags != AOBJ_NO_ANIM)
        *is_done = 0;
}
static inline int JObj_CheckAnimEnded(JOBJ *j)
{
    int is_anim_done = 1;
    JObj_ForEachAnim(j, 6, 0xffff, AOBJ_CheckEnded, 2, &is_anim_done);

    return is_anim_done;
}

static void AOBJ_GetFrame(AOBJ *a, float *out_frame)
{
    *out_frame = a->curr_frame;
}
static inline float JObj_GetAOBJFrame(JOBJ *j)
{
    float cur_frame = 1;
    JObj_ForEachAnim(j, 6, 0xffff, AOBJ_GetFrame, 2, &cur_frame);

    return cur_frame;
}

static inline GOBJ *GOBJ_EZCreator(int entity_class, int p_link, int flags, int data_size, void *data_destructor, int obj_kind, void *obj_desc, void *proc_cb, int proc_pri, void *gx_cb, int gx_link, int gx_pri)
{
    GOBJ *g = GObj_Create(entity_class, p_link, flags);

    if (data_size)
    {
        void *gp = HSD_MemAlloc(data_size);
        memset(gp, 0, data_size);
        GObj_AddUserData(g, 14, data_destructor, gp);
    }

    if (obj_kind != HSD_OBJKIND_NONE)
    {
        void *obj;
        if (obj_kind == HSD_OBJKIND_JOBJ)
        {
            if (obj_desc == 0)
                obj = JObj_Alloc();
            else
                obj = JObj_LoadJoint(obj_desc);
        }
        else if (obj_kind == HSD_OBJKIND_FOG)
            obj = Fog_LoadDesc(obj_desc);
        else if (obj_kind == HSD_OBJKIND_LOBJ)
            obj = LObj_LoadDesc(obj_desc);
        else if (obj_kind == HSD_OBJKIND_COBJ)
            obj = COBJ_LoadDescSetScissor(obj_desc);

        GObj_AddObject(g, obj_kind, obj);

        if (obj_kind == HSD_OBJKIND_COBJ)
        {
            GOBJ_InitCamera(g, gx_cb, gx_pri);
            g->cobj_links = gx_link;
        }
        else if (obj_kind == HSD_OBJKIND_JOBJ)
            GObj_AddGXLink(g, gx_cb, gx_link, gx_pri);
    }
    else if (gx_cb)
        GObj_AddGXLink(g, gx_cb, gx_link, gx_pri);

    if (proc_cb)
        GObj_AddProc(g, proc_cb, proc_pri);

    return g;
}

static inline GOBJ *JObj_LoadSet_SetPri(int is_hidden, JOBJSet *set, int anim_id, float frame, int p_link, int gx_link, int is_add_anim, void *cb, int pri)
{
    void (*JObj_AnimUnk)(JOBJ *j, int flags) = (void *)0x800550bc;

    GOBJ *g = GOBJ_EZCreator(14, p_link, 0,
                             0, 0,
                             HSD_OBJKIND_JOBJ, set->jobj,
                             cb, pri,
                             JObj_GX, gx_link, 0);

    JObj_SetAllMOBJFlags(g->hsd_object, 0x28000000);

    if (is_add_anim)
    {
        // add anims
        // JObj_AddSetAnim(g->hsd_object, set, anim_id);

        JObj_AddSetAnim(g->hsd_object, anim_id, set, frame, 1);

        // AnimJointDesc *animjoint = 0;
        // MatAnimJointDesc *matanim = 0;
        // void *shapeanim = 0;
        // if (set->animjoint)
        //     animjoint = set->animjoint[anim_id];
        // if (set->matanimjoint)
        //     matanim = set->matanimjoint[anim_id];
        // if (set->shapeaninjoint)
        //     shapeanim = set->shapeaninjoint[anim_id];

        // JObj_AddAnimAll(g->hsd_object, animjoint, matanim, shapeanim);
        // JObj_ReqAnimAll(g->hsd_object, 0);
        // JObj_AnimAll(g->hsd_object);

        // set to last frame?
        // JObj_AnimUnk(g->hsd_object, 0xffff);

        if (is_hidden)
            JObj_SetFlagsAll(g->hsd_object, JOBJ_HIDDEN);
    }

    return g;
}

static void GOBJ_Anim(GOBJ *g)
{
    JObj_AnimAll(g->hsd_object);
    return;
}

static inline float lerp(float start, float end, float t)
{
    // clamp
    if (t < 0)
        t = 0;
    else if (t > 1)
        t = 1;

    return start + t * (end - start);
}

static DOBJ *JObj_GetDObjIndex(JOBJ *root, int joint_idx, int dobj_idx)
{
    if (!root)
    {
        OSReport("jobj %d not found\n", 0);
        assert("0");
        return 0;
    }

    JOBJ *this_joint = root;

    for (int this_idx = 0; this_idx < joint_idx; this_idx++)
    {
        if (this_joint->child)
            this_joint = this_joint->child;

        else if (this_joint->sibling)
            this_joint = this_joint->sibling;

        else
        {
            OSReport("jobj %d not found\n", joint_idx);
            assert("0");

            return 0;
        }
    }

    DOBJ *this_dobj = this_joint->dobj;

    if (!this_dobj)
    {
        OSReport("jobj %d has no dobj\n", joint_idx);
        assert("0");
        return 0;
    }

    for (int this_idx = 0; this_idx < dobj_idx; this_idx++)
    {
        if (this_dobj->next)
            this_dobj = this_dobj->next;

        else
        {
            OSReport("dobj %d not found\n", dobj_idx);
            assert("0");

            return 0;
        }
    }

    return this_dobj;
}

static inline int abs(int x)
{
    if (x < 0)
        return -x;
    else
        return x;
}

static inline void null()
{
    return;
}

static void C_QUATMtx(Vec4 *r, Mtx m)
{
    /*---------------------------------------------------------------------------*
   Name:         QUATMtx

   Description:  Converts a matrix to a unit quaternion.

   Arguments:    r   - result quaternion
                m   - the matrix

   Returns:      NONE
    *---------------------------------------------------------------------------*/

    float tr, s;
    int i, j, k;
    int nxt[3] = {1, 2, 0};
    float q[3];

    tr = m[0][0] + m[1][1] + m[2][2];
    if (tr > 0.0f)
    {
        s = (float)sqrtf(tr + 1.0f);
        r->W = s * 0.5f;
        s = 0.5f / s;
        r->X = (m[1][2] - m[2][1]) * s;
        r->Y = (m[2][0] - m[0][2]) * s;
        r->Z = (m[0][1] - m[1][0]) * s;
    }
    else
    {
        i = 0;
        if (m[1][1] > m[0][0])
            i = 1;
        if (m[2][2] > m[i][i])
            i = 2;
        j = nxt[i];
        k = nxt[j];
        s = (float)sqrtf((m[i][i] - (m[j][j] + m[k][k])) + 1.0f);
        q[i] = s * 0.5f;

        if (s != 0.0f)
            s = 0.5f / s;

        r->W = (m[j][k] - m[k][j]) * s;
        q[j] = (m[i][j] + m[j][i]) * s;
        q[k] = (m[i][k] + m[k][i]) * s;

        r->X = q[0];
        r->Y = q[1];
        r->Z = q[2];
    }
}

static HSD_Pad *PadGet(int playerIndex, int padType)
{
    HSD_Pads *pads = 0;

    // get the correct pad
    if (padType == PADGET_MASTER)
        pads = (HSD_Pads *)0x804c1fac;
    else if (padType == PADGET_ENGINE)
        pads = (HSD_Pads *)0x804c21cc;

    if (pads == 0)
        return 0;

    return (&pads->pad[playerIndex]);
}
static int Pad_GetDownSys(int pad_idx)
{
    HSD_Pad *pads = (HSD_Pad *)0x804c1fac;

    int down;

    if (pad_idx < 4)
        down = pads[pad_idx].down;
    else
    {
        for (int i = 0; i < 4; i++)
            down |= pads[i].down;
    }

    return down;
}

static JOBJ *JObj_GetIndex(JOBJ *j, int idx)
{
    if (idx <= 0)
        return (idx == 0) ? j : NULL;

    int count = idx;

    while (count)
    {
        if (j)
        {
            if (j->child)
            {
                j = j->child;
            }
            else if (j->sibling)
            {
                j = j->sibling;
            }
            else
            {
                // Go up until we find a node with a sibling
                while (j)
                {
                    j = j->parent;
                    if (j && j->sibling)
                    {
                        j = j->sibling;
                        break;
                    }
                }
            }
        }
        else
        {
            return NULL;
        }

        if (!j)
            return NULL;
        count--;
    }

    return j;
}

static float JObj_GetAnimFrame(JOBJ *joint)
{
    // check for AOBJ in jobj
    JOBJ *jobj;
    jobj = joint;
    while (jobj != 0)
    {
        if (jobj->aobj != 0)
        {
            return jobj->aobj->curr_frame;
        }

        // check for AOBJ in dobj
        DOBJ *dobj;
        dobj = jobj->dobj;
        while (dobj != 0)
        {
            if (dobj->aobj != 0)
            {
                return dobj->aobj->curr_frame;
            }

            // check for AOBJ in mobj
            MOBJ *mobj;
            mobj = dobj->mobj;
            if (mobj->aobj != 0)
            {
                return mobj->aobj->curr_frame;
            }

            // check for AOBJ in tobj
            TOBJ *tobj;
            tobj = mobj->tobj;
            while (tobj != 0)
            {
                if (tobj->aobj != 0)
                {
                    return tobj->aobj->curr_frame;
                }
                tobj = tobj->next;
            }

            dobj = dobj->next;
        }

        jobj = jobj->child;
    }

    // no aobj found, return -1
    return -1;
}

static AOBJ *JObj_GetFirstAOBJ(JOBJ *jobj)
{
    // check for AOBJ in this jobj

    if (jobj->aobj != 0)
    {
        return jobj->aobj;
    }
    // check for AOBJ in dobj
    DOBJ *dobj;
    dobj = jobj->dobj;
    while (dobj != 0)
    {
        if (dobj->aobj != 0)
        {
            return dobj->aobj;
        }

        // check for AOBJ in mobj
        MOBJ *mobj;
        mobj = dobj->mobj;
        if (mobj->aobj != 0)
        {
            return mobj->aobj;
        }

        // check for AOBJ in tobj
        TOBJ *tobj;
        tobj = mobj->tobj;
        while (tobj != 0)
        {
            if (tobj->aobj != 0)
            {
                return tobj->aobj;
            }
            tobj = tobj->next;
        }

        dobj = dobj->next;
    }

    // this jobj doesnt have an aobj, check the child
    if (jobj->child != 0)
    {
        AOBJ *aobj = JObj_GetFirstAOBJ(jobj->child);
        if (aobj != 0)
            return aobj;
    }

    // child did not have an aobj, check the sibling
    if (jobj->sibling != 0)
    {
        AOBJ *aobj = JObj_GetFirstAOBJ(jobj->sibling);
        if (aobj != 0)
            return aobj;
    }

    // no aobj found, return 0
    return 0;
}

static AOBJ *JObj_GetJointAOBJ(JOBJ *jobj)
{
    // check for AOBJ in this jobj

    if (jobj->aobj != 0)
    {
        return jobj->aobj;
    }

    // this jobj doesnt have an aobj, check the child
    if (jobj->child != 0)
    {
        AOBJ *aobj = JObj_GetJointAOBJ(jobj->child);
        if (aobj != 0)
            return aobj;
    }

    // child did not have an aobj, check the sibling
    if (jobj->sibling != 0)
    {
        AOBJ *aobj = JObj_GetJointAOBJ(jobj->sibling);
        if (aobj != 0)
            return aobj;
    }

    // no aobj found, return 0
    return 0;
}

static DOBJ *JObj_GetDObjChild(JOBJ *joint, int dobj_index)
{

    int count = 0;
    DOBJ *dobj = joint->dobj;

    while (count < dobj_index)
    {
        if (dobj->next == 0)
            assert("dobj not found!");

        else
            dobj = dobj->next;

        count++;
    }

    return dobj;
}

static TOBJ *JObj_FindTOBJFromImageDesc(JOBJ *root, _HSD_ImageDesc *image_desc)
{

    JOBJ *this_j = root;

    // check this level of jobjs
    while (this_j)
    {
        // check this jobj
        DOBJ *this_d = this_j->dobj;
        while (this_d)
        {
            if (this_d->mobj &&
                this_d->mobj->tobj &&
                this_d->mobj->tobj->imagedesc == image_desc)
                return this_d->mobj->tobj;

            this_d = this_d->next;
        }

        // check any children before moving onto next sibling
        if (this_j->child)
        {
            // check child for the tobj
            TOBJ *tobj = JObj_FindTOBJFromImageDesc(this_j->child, image_desc);

            // if found return it
            if (tobj)
                return tobj;
        }

        // go next sibling
        this_j = this_j->sibling;
    }

    return 0;
}

static float Math_Vec2Angle(Vec2 *a, Vec2 *b)
{
    // get angle
    // float angle = atan((b->Y - a->Y) / (b->X - a->X));
    float angle = atan2((b->Y - a->Y), (b->X - a->X));

    /*
    //Ensure above 0 and below 6.28319
    while (angle < 0)
    {
        angle += M_PI;
    }
    while (angle > M_PI * 2)
    {
        angle -= M_PI;
    }
    */

    return angle;
}

static void C_MTXRotAxisRad(Mtx *m, Vec3 *axis, f32 rad)
{
    Vec3 vN;
    f32 s, c;          // sinTheta, cosTheta
    f32 t;             // ( 1 - cosTheta )
    f32 x, y, z;       // x, y, z components of normalized axis
    f32 xSq, ySq, zSq; // x, y, z squared

    s = sin(rad);
    c = cos(rad);
    t = 1.0f - c;

    VECNormalize(axis, &vN);

    x = vN.X;
    y = vN.Y;
    z = vN.Z;

    xSq = x * x;
    ySq = y * y;
    zSq = z * z;

    (*m)[0][0] = (t * xSq) + (c);
    (*m)[0][1] = (t * x * y) - (s * z);
    (*m)[0][2] = (t * x * z) + (s * y);
    (*m)[0][3] = 0.0f;

    (*m)[1][0] = (t * x * y) + (s * z);
    (*m)[1][1] = (t * ySq) + (c);
    (*m)[1][2] = (t * y * z) - (s * x);
    (*m)[1][3] = 0.0f;

    (*m)[2][0] = (t * x * z) - (s * y);
    (*m)[2][1] = (t * y * z) + (s * x);
    (*m)[2][2] = (t * zSq) + (c);
    (*m)[2][3] = 0.0f;
}

// static float Math_Vec2Distance(Vec2 *a, Vec2 *b)
// {
//     return sqrtf(pow((a->X - b->X), 2) + pow((a->Y - b->Y), 2));
// }

// static float Math_Vec3Distance(Vec3 *a, Vec3 *b)
// {
//     return sqrtf(pow((a->X - b->X), 2) + pow((a->Y - b->Y), 2) + pow((a->Z - b->Z), 2));
// }

// float fmin(float a, float b)
// {
//     return (((a) < (b)) ? (a) : (b));
// }

// float fmax(float a, float b)
// {
//     return (((a) > (b)) ? (a) : (b));
// }

static void GX_DrawLine(Vec3 *start, Vec3 *end, u8 width, GXColor *color)
{
    HSD_StateInitDirect(GX_VTXFMT0, 2);
    GXLoadPosMtxImm(&COBJ_GetCurrent()->view_mtx, GX_PNMTX0);
    GXSetLineWidth(width, 5);
    GXBegin(GX_LINES, GX_VTXFMT0, 2);

    // draw line
    GXPosition3f32(start->X, start->Y, start->Z);
    GXColor4u8(color->r, color->g, color->b, color->a);
    GXPosition3f32(end->X, end->Y, end->Z);
    GXColor4u8(color->r, color->g, color->b, color->a);

    HSD_StateInvalidate(-1);
}

#endif