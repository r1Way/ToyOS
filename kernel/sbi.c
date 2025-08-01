#include "types.h"
#include "sbi.h"
// sBI 调用函数
int inline sbi_call(uint64 which, uint64 arg0, uint64 arg1, uint64 arg2)
{
    register uint64 a0 asm("a0") = arg0;
    register uint64 a1 asm("a1") = arg1;
    register uint64 a2 asm("a2") = arg2;
    register uint64 a7 asm("a7") = which;
    asm volatile("ecall"
    : "=r"(a0) // 输出约束：a0 寄存器的值作为返回值
    : "r"(a0),"r"(a1),"r"(a2),"r"(a7) // 输入约束：使用这些寄存器作为参数
    : "memory") ; // 副作用：告诉编译器内存可能被修改
    return a0;
}