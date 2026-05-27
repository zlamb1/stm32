.global frames_START
.global frames_END
.section .rodata

frames_START:
.align 0x2
.incbin "src/nyan.raw"
frames_END: