.set AUDIO_HEAP_SIZE, 0x00034140
.set AUDIO_LOG, 0x804adb2c

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
