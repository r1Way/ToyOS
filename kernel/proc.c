#include"types.h"
#include "riscv.h"
#include "memlayout.h"
#include "defs.h"
#include "param.h"
#include "proc.h"
#include "spinlock.h"

struct cpu cpus[NCPU];

struct proc proc[NPROC];

struct proc *initproc;

// 为每个进程分配一个内核栈页面，并将其映射到高地址，后面跟一个无效的保护页。
void
proc_mapstacks(pagetable_t kpgtbl)
{
  struct proc *p;
  
  for(p = proc; p < &proc[NPROC]; p++) {
    char *pa = kalloc();
    if(pa == 0)
      panic("kalloc");
    uint64 va = KSTACK((int) (p - proc));
    kvmmap(kpgtbl, va, (uint64)pa, PGSIZE, PTE_R | PTE_W);
  }
}