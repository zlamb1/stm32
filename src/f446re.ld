SECTIONS
{
    . = 0x08000000;

    .reset_vectors ALIGN(0x4) : { *(.reset_vectors) }
    .text ALIGN(0x4) : { _stext = .; *(.text); *(.text.*) _etext = .; }
    .rodata ALIGN(0x4) : { _srodata = .; *(.rodata); *(.rodata.*) _erodata = .; }

    . = 0x20000000;

    .data ALIGN(0x200) : AT(_erodata) { _sdata = .; *(.vectors); *(.data); *(.data.*) _edata = .; }
    .bss ALIGN(0x4) : { _sbss = .; *(.bss); *(.bss.*) _ebss = .; }
}