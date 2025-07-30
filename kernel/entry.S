.section .data
message:
  .string "Hello World, ToyOS is Loading.\n"

.section .text
.global _entry
_entry:
        # output the string first
        la t0, message
print_loop:
        lbu a0, 0(t0)   # load a char 
        beqz a0, end    # jump to end if a0 is 0
        li a7, 1 
        ecall
        addi t0, t0, 1  # add a0, jump to the next char
        j print_loop

end:
        # set up a stack for c
        # stack0 is declared in start.c
        # with a 4096-byte stack per CPU
        # sp = stack0 + ( hartid * 4090 * 16 )
        la sp, stack0
        li a0, 4096*16
        add sp, sp, a0
        # jump to start() in start.c
        call start