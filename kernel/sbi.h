#ifndef SBI_H
#define SBI_H
#include "types.h"
int sbi_call(uint64 which, uint64 arg0, uint64 arg1, uint64 arg2);
#endif // SBI_H