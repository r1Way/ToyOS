gdb-multiarch kernel/kernel \
  -ex "set architecture riscv:rv64" \
  -ex "target remote localhost:26000" \
  -ex "layout split" \
  -ex "b kinit" \
  -ex "b freerange" \
  -ex "b kvminit" \
  -ex "b kvmmake" \
  -ex "b kalloc" \
  # -ex "b memset" \
  -ex "b kvmmap" \
  -ex "b mappages" 