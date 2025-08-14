#ifndef DEFS_H
#define DEFS_H

#include "types.h"
#include "riscv.h"

// 计算静态数组的元素个数，检验系统调用号是否越界
#define NELEM(x) (sizeof(x) / sizeof((x)[0]))

// kalloc.c - 内存分配相关
void kinit(void);
void kfree(void *pa);
void *kalloc(void);
void freerange(void *pa_start, void *pa_end);

// vm.c - 虚拟内存管理
void kvminit(void);
void kvminithart(void);
pagetable_t kvmmake(void);
void kvmmap(pagetable_t kpgtbl, uint64 va, uint64 pa, uint64 sz, int perm);
int mappages(pagetable_t pagetable, uint64 va, uint64 size, uint64 pa, int perm);
pte_t *walk(pagetable_t pagetable, uint64 va, int alloc);

// printf.c - 打印和输出
void printf(const char *fmt, ...);
void printstr(const char *str);
void printint(int num, int base);
void console_putc(char c);
void sbi_console_putchar(char c);
void panic(char *s);
uint64 sys_myHelloWorld(void);

// string.c - 字符串操作
void *memset(void *dst, int c, uint n);

// sbi.c - SBI调用
//奇了怪了，用inline就是链接不了报错。
int sbi_call(uint64 which, uint64 arg0, uint64 arg1, uint64 arg2);

// start.c - 启动相关
void start(void);

//main.c - 主函数
void main(void);

// syscall.c - 系统调用
void syscall(void);

// trap.c - 中断和异常处理
// void trap(void);
// void usertrap(void);
// void kerneltrap(void);

// 外部符号声明（来自链接脚本）
extern char etext[];      // kernel.ld 中定义的内核代码段结束
extern char end[];        // kernel.ld 中定义的内核结束
extern char trampoline[]; // trampoline.S 中定义

// 全局变量声明
extern pagetable_t kernel_pagetable;
extern volatile int panicked;

#endif // DEFS_H