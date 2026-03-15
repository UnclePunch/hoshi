# inject @ 0x80059df8

/*
    this bit of code only exists to serve rollback.
    
    unfortunately the game doesn't keep any log of the audio
    heap's boundaries so we're doing that here.
*/

.include "include/header.s"

    lis r3, AUDIO_LOG@ha
    ori r3, r3, AUDIO_LOG@l

// store heap start address and size
    stw r30, 0x0(r3)
    lis r4, AUDIO_HEAP_SIZE@ha
    ori r4, r4, AUDIO_HEAP_SIZE@l
    stw r4, 0x4(r3)

// original instruction
    li r3, 0