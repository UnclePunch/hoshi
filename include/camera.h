#ifndef KAR_H_CAM
#define KAR_H_CAM

#include "datatypes.h"

typedef struct CameraParam
{
    Vec3 interest;         // 0x00
    Vec3 eye;              // 0x0c
    Vec3 up;               // 0x18
    float fov;             // 0x24
} CameraParam;

#endif