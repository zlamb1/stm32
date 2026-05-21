.global _start
.syntax unified
.thumb

.section .reset_vectors, "a"

_reset_vectors:
    .long 0x2001C000         // stack pointer
    .long _start             // reset handler

.text

.thumb_func
_start:
    ldr r0, =_erodata
    ldr r1, =_sdata
    ldr r2, =_edata
1:
    cmp r1, r2
    bge 2f
    ldrb r3, [r0]
    strb r3, [r1]            // copy data section from flash into RAM
    add r0, #1
    add r1, #1
    b 1b
2:
    ldr r0, =_sbss
    ldr r1, =_ebss
    ldr r2, =0
1:
    cmp r0, r1
    bge 2f
    strb r2, [r0]            // zero bss
    add r0, #1
    b 1b
2:
    ldr r0, =vector_table
    ldr r1, =0xE000ED08
    str r0, [r1]             // store RAM vector table into VTOR
    dsb
    isb
    bl main                  // branch to main
1:
    wfi                      // enter low power state
    b 1b                     // infinite loop

.section .vectors, "aw"
.global vector_table

vector_table:
    .rept 113
    .long 0
    .endr