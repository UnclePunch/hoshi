#ifndef MEX_H_EXI
#define MEX_H_EXI

#include "datatypes.h"

typedef enum
{
    EXI_READ,
    EXI_WRITE,
} EXIMode;

typedef enum
{
    EXI_BUSY      = -2,
    EXI_ERROR     = -1,
    EXI_OK        = 0,
} EXIStatus;

s32 EXIInit(void); // Initialization
s32 EXISelect(s32 chan, s32 device, s32 frequency); // Select a device on an EXI chan
s32 EXIDeselect(s32 chan); // Deselect the currently selected device
s32 EXILock(s32 chan, s32 lockTimeout); // Lock access to an EXI channel
s32 EXIUnlock(s32 chan); // Unlock access to an EXI channel
s32 EXIDma(s32 chan, void *buffer, s32 length, EXIMode direction); // Start a DMA transfer
s32 EXIImm(s32 chan, void *buffer, s32 length, EXIMode direction); // Immediate transfer (no DMA)
s32 EXIImmEx(s32 chan, void *buffer, s32 length, EXIMode direction); // Immediate transfer (no DMA)
s32 EXISync(s32 chan); // Wait for the EXI transfer to complete
s32 EXIProbe(s32 chan); // Probe whether a device is present on an EXI chan
s32 EXIGetID(s32 chan, s32 device, u32 *out_id);

#endif