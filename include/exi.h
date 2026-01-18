#ifndef MEX_H_EXI
#define MEX_H_EXI

#include "datatypes.h"

#define EXI_CHANNEL_0 0
#define EXI_CHANNEL_1 1
#define EXI_CHANNEL_2 2

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

typedef struct 
{
    u32 CR;   // Control register
    u32 BR;   // Bus register
    u32 DR;   // Data register
    u32 ESR;  // Status register
} EXIChannelReg;

typedef void (*EXICallback)(s32 chan, OSContext* context);

// volatile EXIChannelReg *exi_reg = (EXIChannelReg *)0xCC006800;

s32 EXIInit(void); // Initialization
s32 EXISelect(s32 chan, s32 device, s32 frequency); // Select a device on an EXI chan
s32 EXIDeselect(s32 chan); // Deselect the currently selected device
s32 EXILock(s32 chan, s32 lockTimeout); // Lock access to an EXI channel
s32 EXIUnlock(s32 chan); // Unlock access to an EXI channel
s32 EXIDma(s32 chan, void *buffer, s32 length, EXIMode direction, EXICallback cb); // Start a DMA transfer
s32 EXIImm(s32 chan, void *buffer, s32 length, EXIMode direction, EXICallback cb); // Immediate transfer (no DMA)
s32 EXIImmEx(s32 chan, void *buffer, s32 length, EXIMode direction); // Immediate transfer (no DMA)
s32 EXISync(s32 chan); // Wait for the EXI transfer to complete
s32 EXIProbe(s32 chan); // Probe whether a device is present on an EXI chan
s32 EXIGetID(s32 chan, s32 device, u32 *out_id);
s32 EXIGetType(s32 chan, s32 device, u32 *type);

#endif