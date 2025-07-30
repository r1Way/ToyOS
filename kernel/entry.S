.section .data
message:
  .string "Hello World, ToyOS is Loading.\n"

.section .text
.global _entry
_entry:
        la t0, message
print_loop:
        lbu a0, 0(t0)
        beqz a0, end
        li a7, 1 
        ecall
        addi t0, t0, 1
        j print_loop

end:
        la sp, stack0
        li a0, 4096*16
        add sp, sp, a0
        call start