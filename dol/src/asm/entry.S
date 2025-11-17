# inject @ 0x80410168

.include "include/header.s"

.global func

# call our c function
  bl func

# restore clobbered r3 register
  lwz	r3, 0x11B0 (r13)
  lwz	r4, 0x11B4 (r13)