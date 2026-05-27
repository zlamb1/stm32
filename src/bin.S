.global frames_START
.global frames_END
.section .rodata

frames_START:
.align 0x2
.incbin "src/sky.raw"
frames_END: