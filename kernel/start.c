#include "types.h"
#include "riscv.h"
#include "defs.h"

#define NCPU          8  // maximum number of CPUs
// entry.S needs one stack per CPU.
__attribute__ ((aligned (16))) char stack0[4096*16 * NCPU];


/**
 * 内核启动函数
 * 
 * 这个函数由汇编代码 entry.S 调用
 *
 * 调用链：
 * _entry (entry.S) -> start() (start.c) -> main() (main.c)
 */
void start() 
{
  kinit(); // 初始化内核的内存分配器
  kvminit(); // 初始化内核的页表
  kvminithart(); // 切换到内核页表并启用分页
  // 现在还没有dual mode，暂时直接调用 main 函数
  main();
}
