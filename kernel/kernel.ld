/* filepath: g:\project\2025\os_step\ToyOS\kernel.ld */
OUTPUT_ARCH(riscv)
ENTRY(_entry)

SECTIONS
{
    . = 0x82000000;
    
    .text : {
        *(.text.entry)
        *(.text .text.*)
        *(trampsec)
    }
    
    .rodata : {
        *(.rodata .rodata.*)
    }
    
    .data : {
        *(.data .data.*)
    }
    
    .bss : {
        *(.bss .bss.*)
        . = ALIGN(8);
        . += 4096 * 16;
        stack0 = .;
    }
}