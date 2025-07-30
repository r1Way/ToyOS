/* filepath: g:\project\2025\os_step\ToyOS\kernel.ld */
OUTPUT_ARCH(riscv)
ENTRY(_entry)

SECTIONS
{
    . = 0x80200000;
    
    .text : {
        *(.text.entry)
        *(.text .text.*)
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
        stack0 = .;
        . += 4096 * 16;
    }
}