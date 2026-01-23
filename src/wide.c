#include "text.h"
#include "os.h"
#include "hsd.h"

#include "hoshi/wide.h"
#include "hoshi.h"

typedef struct
{
    float top;
    float bot;
    float left;
    float right;
} CamBounds;
void Wide_GetBounds(COBJ *cam, f32 depth, CamBounds *out) 
{
    depth -= (cam->eye->pos.Z - cam->interest->pos.Z);

    switch (cam->projection_type)
    {
        /* -------------------- */
        /* Perspective camera   */
        /* -------------------- */
        case 1:
        {
            float fov = cam->projection_param.perspective.fov;
            float fov_radians = fov * (M_PI / 180.0f);

            float aspect = cam->projection_param.perspective.aspect;

            // Horizontal FOV derived from engine FOV
            float fov_x = 2.0f * atan(tan(fov_radians * 0.5f) * aspect);

            float width  = 2.0f * depth * tan(fov_x * 0.5f);
            float height = width / aspect;

            out->left  = -width  * 0.5f;
            out->right =  width  * 0.5f;
            out->bot   = -height * 0.5f;
            out->top   =  height * 0.5f;
            break;
        }

        /* -------------------- */
        /* Custom frustum       */
        /* -------------------- */
        case 2:
        {
            // Frustum params are usually specified at near plane
            float near = cam->near;
            float scale = depth / near;

            out->left   = cam->projection_param.frustrum.left   * scale;
            out->right  = cam->projection_param.frustrum.right  * scale;
            out->bot    = cam->projection_param.frustrum.bottom * scale;
            out->top    = cam->projection_param.frustrum.top    * scale;
            break;
        }

        /* -------------------- */
        /* Orthographic camera  */
        /* -------------------- */
        case 3:
        {
            out->left   = cam->projection_param.ortho.left;
            out->right  = cam->projection_param.ortho.right;
            out->bot = cam->projection_param.ortho.bottom;
            out->top    = cam->projection_param.ortho.top;
            break;
        }
    }
}

float Wide_AdjustPos(float x_pos)
{
    float adjust_ratio = (*stc_cobj_aspect) / 1.25538;
    return x_pos * adjust_ratio;
}

void Wide_Init()
{
    CODEPATCH_REPLACEFUNC(Hoshi_AdjustWidePos, Wide_AdjustPos);   
}