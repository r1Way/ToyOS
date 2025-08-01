#include "riscv.h"
#include "types.h"
#include "syscall.h"
#include "defs.h"

extern uint64 sys_myHelloWorld(void);

// 一个函数指针数组，数组中每个元素都是指向函数的指针
static uint64 (*syscalls[])(void) = {
    [SYS_myHelloWorld] sys_myHelloWorld,
};

void 
syscall(void)
{
    int num;
    asm volatile("mv %0, a7" : "=r"(num));  // 获取系统调用号
    if(num>0 && num < NELEM(syscalls) && syscalls[num]) {
        syscalls[num]();  // 调用对应的系统调用函数
    }
    else
    {
        
    }
}