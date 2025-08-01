#include "types.h"
#include "riscv.h"
#include "sbi.h"
#include "syscall.h"

// 声明userret函数
extern void userret(void);

// 声明usertrapret函数
void __attribute__((naked)) usertrapret(void);

void __attribute__((naked))
usertrap(void)
{
    // if (r_scause() == 8) {
        intr_on();
        syscall();
    // } 
    // //如果是硬中断（设备中断）
    // else if (0 == 1) {
        
    // } 
    // /* 如果是其他中断 */
    // else {
        
    // }
    usertrapret();
}

void
__attribute__((naked))
usertrapret(void)
{
    intr_off();
    unsigned long x = r_sstatus();
    x &= ~SSTATUS_SPP;      // clear SPP to 0 for user mode
    x |= SSTATUS_SPIE;      // enable interrupts in user mode
    w_sstatus(x);
    ((void (*)(void))userret)();
}