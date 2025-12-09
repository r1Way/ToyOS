.section .data
message:
  .string "Hello World, ToyOS is Loading.\n"

.section .text
.global _entry

_entry:
        # now we are in the S mode
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
        # a0存放着hartid，参考opensbi的sbi_hart.c
        # 对于单cpu，hartid=0
        # 对于多cpu，每次启动时的hartid可能是不同的，我们需要将sp设置到该cpu对应的栈空间上
        # CPU取决于makefile，不过param.h中的宏NCPU也要记得修改
        la sp, stack0
        la t1, hartid # 将hartid地址写入t1
        sw a0, 0(t1) # 将a0写入hartid，方便后续使用
        mv a2, a0
        li a0, 1024*4
        addi a2, a2, 1
        mul a0, a0, a2
        add sp, sp, a0

        # jump to start() in start.c
        call start