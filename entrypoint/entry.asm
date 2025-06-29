#To be inserted at 80410168
.include "header.s"

b START

.include "reloc.s"

START:
.set  REG_HeapLo,31
.set  REG_FileSize,28
.set  REG_File,27
.set  REG_HeapID,26
.set  REG_mexData,24

backup

# quickly init some audio callbacks
  load r3, 0x80059d28
  load r4, 0x80059d50
  branchl r0, 0x804479d4

lwz REG_HeapLo, 0x11B0 (r13)

#Check if file exists
  bl  FileName
  mflr  r3
  branchl r12, DVDConvertPathToEntrynum
  cmpwi r3,-1
  beq NoFile
#Get size of hoshi
  bl  FileName
  mflr  r3
  branchl r12,File_GetSize
  addi  REG_FileSize,r3,0
#Align
  addi  REG_FileSize,REG_FileSize,31
  rlwinm	REG_FileSize, REG_FileSize, 0, 0, 26
#Create heap of this size
  add r4,REG_HeapLo,REG_FileSize     #heap hi = start + filesize
  addi  r4,r4,32*5              #plus header
  mr  r3,REG_HeapLo                  #heap lo = start
  mr  REG_HeapLo,r4             #new start = heap hi
  branchl r12, OSCreateHeap
  mr  REG_HeapID,r3
#Alloc from this heap
  mr  r3,REG_HeapID
  mr  r4,REG_FileSize
  branchl r12, OSAllocFromHeap
  mr  REG_File,r3
#Load file here
  bl  FileName
  mflr  r3
  mr r4, REG_File
  addi	r5, sp, 0x80
  branchl	r12, File_LoadSync
#Reloc
  mr r3, REG_File
  bl Reloc
#Get Function
  mr  r3,REG_File
  addi  r4,sp,0x84
  bl  Overload
mexPatch_Skip:

#Flush instruction cache so code can be run from this file
  mr  r3,REG_File
  mr  r4,REG_FileSize
  branchl r12, TRK_FlushCache

#Destroy this heap
  mr  r3,REG_HeapID
  branchl r12, OSDestroyHeap

  b UpdateHeapLo

FileName:
blrl
.string "hoshi.dat"
.align 2
SymbolName:
blrl
.string "mdFunction"
.align 2
############################################
NoFile:
#OSReport
  bl  ErrorString
  mflr  r3
  branchl r12, OSReport
  b Skip_OnFileLoad



/*
#Assert
  bl  Assert_Name
  mflr  r3
  li  r4,0
  load  r5, 0
  branchl r12, assert
*/
#############################################
Assert_Name:
blrl
.string "m-ex"
.align 2
ErrorString:
blrl
.string "error: hoshi.dat not found!\n"
.align 2
###############################################


UpdateHeapLo:
  stw	REG_HeapLo, 0x11B0 (r13)

#Grab OnFileLoadCallback before we free the stack frame
  lwz r12,0x84(sp)

#Run TM_OnFileLoad
  cmpwi r12,0
  beq Skip_OnFileLoad
  mr  r3,REG_File
  mtctr r12
  bctrl
Skip_OnFileLoad:
  restore

Exit:
  lwz	r3, 0x11B0 (r13)
  lwz	r4, 0x11B4 (r13)
