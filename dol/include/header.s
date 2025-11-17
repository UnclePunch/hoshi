.set AUDIO_HEAP_SIZE, 0x00034140

/*
.set DVDConvertPathToEntrynum, 0x803c4ed4
.set File_GetSize, 0x8005915c
.set OSCreateHeap, 0x803d3804
.set OSDestroyHeap, 0x803d3870
.set OSAllocFromHeap, 0x803d360c
.set HSD_MemAlloc, 0x8041cee4
.set HSD_Free, 0x8041ceb4
.set File_LoadSync, 0x80059364
.set HSD_ArchiveInit, 0x8041e224
.set HSD_GetArchivePublicAddress, 0x8041e390
.set TRK_FlushCache, 0x803bffd8
.set OSReport, 0x803d4ce8
.set assert, 0x804284b8
*/

.macro branchl reg, address
lis \reg, \address @h
ori \reg,\reg,\address @l
mtctr \reg
bctrl
.endm

.macro branch reg, address
lis \reg, \address @h
ori \reg,\reg,\address @l
mtctr \reg
bctr
.endm

.macro load reg, name
lis \reg, (\name >> 16)
ori \reg, \reg, (\name & 0xFFFF)
.endm

.macro backup
mflr r0
stw r0, 0x4(r1)
stwu	r1,-0x100(r1)	# make space for 12 registers
stmw  r20,0x8(r1)
.endm

 .macro restore
lmw  r20,0x8(r1)
lwz r0, 0x104(r1)
addi	r1,r1,0x100	# release the space
mtlr r0
.endm

.macro bp
branchl r12,0x8021b2d8
.endm

#xFunction struct
  .set  ftX_Code,0x0
  .set  ftX_InstructionRelocTable,0x4
  .set  ftX_InstructionRelocTableCount,0x8
  .set  ftX_FunctionRelocTable,0xC
    .set  FunctionRelocTable_ReplaceThis,0x0
    .set  FunctionRelocTable_ReplaceWith,0x4
  .set  ftX_FunctionRelocTableCount,0x10
  .set  ftX_CodeSize,0x14
  .set  ftX_DebugSymCount,0x18
  .set  ftX_DebugSym,0x1C
    .set ftX_DebugSym_Start, 0x0
    .set ftX_DebugSym_End, ftX_DebugSym_Start + 0x4
    .set ftX_DebugSym_FuncName, ftX_DebugSym_End + 0x4
    .set ftX_DebugSym_Size, ftX_DebugSym_FuncName + 0x4
