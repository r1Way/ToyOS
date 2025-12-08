#include "param.h"
#include "spinlock.h"
#ifndef PROC_H
#define PROC_H

// 保存内核上下文切换的寄存器。
// 从一个进程的内核态切换到另一个进程的内核态
struct context {
  uint64 ra;
  uint64 sp;

  // callee-saved
  uint64 s0;
  uint64 s1;
  uint64 s2;
  uint64 s3;
  uint64 s4;
  uint64 s5;
  uint64 s6;
  uint64 s7;
  uint64 s8;
  uint64 s9;
  uint64 s10;
  uint64 s11;
};

// Per-CPU state.
struct cpu {
  struct proc *proc;          // The process running on this cpu, or null.
  struct context context;     // swtch() here to enter scheduler().
  int noff;                   // Depth of push_off() nesting.
  int intena;                 // Were interrupts enabled before push_off()?
};

extern struct cpu cpus[NCPU];


// 每个进程用于陷阱(trap)处理的数据，供 trampoline.S 使用。
// trapframe 结构体被分配在用户页表中的一个独立页面上，紧挨着 trampoline 页面。
// trapframe 不会被特殊映射到内核页表。
// 在 trampoline.S 中，uservec 会将用户寄存器保存到 trapframe，
// 然后根据 trapframe 中的 kernel_sp、kernel_hartid、kernel_satp 初始化内核环境，并跳转到 kernel_trap。
// usertrapret() 和 trampoline.S 中的 userret 会设置 trapframe 的 kernel_* 字段，
// 恢复用户寄存器，从 trapframe 切换到用户页表，进入用户空间。
// trapframe 包含了所有被调用者保存的寄存器（如 s0-s11），
// 因为通过 usertrapret() 返回用户态时不会经过完整的内核调用栈。
struct trapframe {
/*   0 */ uint64 kernel_satp;   // 内核页表地址
/*   8 */ uint64 kernel_sp;     // 进程内核栈顶指针
/*  16 */ uint64 kernel_trap;   // usertrap() 的入口地址
/*  24 */ uint64 epc;           // 保存的用户程序计数器（返回用户态时用）
/*  32 */ uint64 kernel_hartid; // 保存的内核线程号（tp寄存器）
  /*  40 */ uint64 ra;
  /*  48 */ uint64 sp;
  /*  56 */ uint64 gp;
  /*  64 */ uint64 tp;
  /*  72 */ uint64 t0;
  /*  80 */ uint64 t1;
  /*  88 */ uint64 t2;
  /*  96 */ uint64 s0;
  /* 104 */ uint64 s1;
  /* 112 */ uint64 a0;
  /* 120 */ uint64 a1;
  /* 128 */ uint64 a2;
  /* 136 */ uint64 a3;
  /* 144 */ uint64 a4;
  /* 152 */ uint64 a5;
  /* 160 */ uint64 a6;
  /* 168 */ uint64 a7;
  /* 176 */ uint64 s2;
  /* 184 */ uint64 s3;
  /* 192 */ uint64 s4;
  /* 200 */ uint64 s5;
  /* 208 */ uint64 s6;
  /* 216 */ uint64 s7;
  /* 224 */ uint64 s8;
  /* 232 */ uint64 s9;
  /* 240 */ uint64 s10;
  /* 248 */ uint64 s11;
  /* 256 */ uint64 t3;
  /* 264 */ uint64 t4;
  /* 272 */ uint64 t5;
  /* 280 */ uint64 t6;
};

enum procstate { UNUSED, USED, SLEEPING, RUNNABLE, RUNNING, ZOMBIE };

// 每个进程的状态结构体
struct proc {
  struct spinlock lock;           // 进程锁，保护进程状态

  // 需要持有 p->lock 时才能访问的字段：
  enum procstate state;           // 进程状态
  void *chan;                     // 睡眠时等待的通道指针
  int killed;                     // 非零表示进程被杀死
  int xstate;                     // 进程退出时的状态码，供父进程 wait 使用
  int pid;                        // 进程 ID

  // 需要持有 wait_lock 时才能访问的字段：
  struct proc *parent;            // 父进程指针

  // 以下字段为进程私有，访问时无需持有锁
  uint64 kstack;                  // 内核栈的虚拟地址
  uint64 sz;                      // 进程内存空间大小（字节数）
  pagetable_t pagetable;          // 用户页表
  struct trapframe *trapframe;    // trapframe，供 trampoline.S 使用
  struct context context;         // 内核上下文切换时保存的寄存器
  // struct file *ofile[NOFILE];     // 打开的文件列表
  // struct inode *cwd;              // 当前工作目录
  char name[16];                  // 进程名（用于调试）
};

#endif // PROC_H