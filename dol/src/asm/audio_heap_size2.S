# inject @ 0x80059dc8

.include "include/header.s"

/*
.section .rodata
.align 2
my_string:
    .asciz "hello from asm!\n"
    .align 2

.section .text

    lis r3, my_string@ha
    ori r3, r3, my_string@l
    bl  OSReport
*/
    lis r4, AUDIO_HEAP_SIZE@ha
    ori r4, r4, AUDIO_HEAP_SIZE@l
    add r4, r30, r4
