gdb-multiarch kernel/kernel \
  -ex "set architecture riscv:rv64" \
  -ex "target remote localhost:26000" \
  -ex "layout split" \
  -ex "break usertrap" \
  -ex "c"