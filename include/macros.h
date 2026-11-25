#ifndef KAR_H_MACROS
#define KAR_H_MACROS

/*** Misc Useful Macros ***/
#define GetElementsIn(arr) sizeof(arr) / sizeof(arr[0])
#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)
#define _SHIFTL(v, s, w) ((u32)(((u32)(v) & ((0x01 << (w)) - 1)) << (s))) // mask the first w bits of v before lshifting
#define _SHIFTR(v, s, w) ((u32)(((u32)(v) >> (s)) & ((0x01 << (w)) - 1))) // rshift v and mask the first w bits afterwards
#define tostr(s) xstr(s)
#define xstr(s) #s

#ifndef GIT_COMMIT
#define GIT_COMMIT "unknown"
#endif

#endif