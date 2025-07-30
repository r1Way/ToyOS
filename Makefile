CROSS_COMPILE = riscv64-linux-gnu-
CC = $(CROSS_COMPILE)gcc
LD = $(CROSS_COMPILE)ld
OBJCOPY = $(CROSS_COMPILE)objcopy
QEMU = qemu-system-riscv64

CFLAGS = -march=rv64g -mabi=lp64d -mcmodel=medany -fno-common -g -fno-stack-protector
CFLAGS += -fno-pie -no-pie -nostdlib -nostartfiles -ffreestanding -Wl,--build-id=none

LDFLAGS = -z max-page-size=4096

kernel/kernel: kernel/entry.S kernel/main.c kernel/start.c kernel/kernel.ld
	$(CC) $(CFLAGS) -T kernel/kernel.ld -o kernel/kernel kernel/entry.S kernel/main.c kernel/start.c

kernel/kernel.bin: kernel/kernel
	$(OBJCOPY) kernel/kernel --strip-all -O binary kernel/kernel.bin

qemu: kernel/kernel
	qemu-system-riscv64 \
		-machine virt \
		-bios default \
		-kernel kernel/kernel \
		-m 128M \
		-nographic \
		-global virtio-mmio.force-legacy=false

# try to generate a unique GDB port
GDBPORT = 26000 
# QEMU's gdb stub command line changed in 0.11
QEMUGDB = $(shell if $(QEMU) -help | grep -q '^-gdb'; \
	then echo "-gdb tcp::$(GDBPORT)"; \
	else echo "-s -p $(GDBPORT)"; fi)

# -bios default 表示qemu自带的opensbi固件
qemu-gdb: kernel/kernel
	@echo "------------------------------------------"
	@echo "Starting QEMU with GDB support..."
	@echo "run \"sh gdb.sh in another bash\" to debug"
	@echo "------------------------------------------"
	qemu-system-riscv64 \
		-machine virt \
		-bios default \
		-kernel kernel/kernel \
		-m 128M \
		-nographic \
		-global virtio-mmio.force-legacy=false \
		-S $(QEMUGDB)

clean:
	rm -f kernel/kernel kernel/kernel.bin
	rm -f *.tex *.dvi *.idx *.aux *.log *.ind *.ilg \
	*/*.o */*.d */*.asm */*.sym 

.PHONY: qemu qemu-gdb clean
