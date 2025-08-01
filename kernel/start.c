#include "types.h"
#include "riscv.h"

// 函数声明，定义在main.c中
void main();

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
  // 现在还没有dual mode，暂时直接调用 main 函数
  main();
}
