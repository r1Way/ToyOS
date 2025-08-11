gdb-multiarch kernel/kernel \
  -ex "set architecture riscv:rv64" \
  -ex "target remote localhost:26000" \
  # -ex "hbreak _entry" \
  # -ex "hbreak spin" \
  # -ex "layout split" \
  # -ex "continue" 
