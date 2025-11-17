# inject @ 0x80059d84

.include "include/header.s"

    lis r3, AUDIO_HEAP_SIZE@ha
    ori r3, r3, AUDIO_HEAP_SIZE@l
