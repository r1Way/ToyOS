# ToyOS Makefile
# RISC-V 64-bit operating system

# Cross-compilation toolchain
CROSS_COMPILE = riscv64-linux-gnu-
CC = $(CROSS_COMPILE)gcc
LD = $(CROSS_COMPILE)ld
OBJCOPY = $(CROSS_COMPILE)objcopy
QEMU = qemu-system-riscv64

# Compiler flags
CFLAGS = -march=rv64g -mabi=lp64d -mcmodel=medany -fno-common -g -fno-stack-protector
CFLAGS += -fno-pie -no-pie -nostdlib -nostartfiles -ffreestanding -Wl,--build-id=none

# Linker flags
LDFLAGS = -z max-page-size=4096

# Kernel source files
K = kernel
KERNEL_SRCS = \
	$(K)/entry.S \
	$(K)/main.c \
	$(K)/start.c \
	$(K)/printf.c \

KERNEL_LD = $(K)/kernel.ld

# Build targets
$(K)/kernel: $(KERNEL_SRCS) $(KERNEL_LD)
	$(CC) $(CFLAGS) -T $(KERNEL_LD) -o $(K)/kernel $(KERNEL_SRCS)

$(K)/kernel.bin: $(K)/kernel
	$(OBJCOPY) $(K)/kernel --strip-all -O binary $(K)/kernel.bin

# QEMU configuration
CPUS = 1
QEMU_MACHINE = virt
QEMU_BIOS = default
QEMU_MEM = 128M

# QEMU options
QEMUOPTS = -machine $(QEMU_MACHINE) -bios $(QEMU_BIOS) -kernel $(K)/kernel
QEMUOPTS += -m $(QEMU_MEM) -nographic
QEMUOPTS += -global virtio-mmio.force-legacy=false

# Run QEMU without debugging
qemu: $(K)/kernel
	$(QEMU) $(QEMUOPTS)

# GDB debugging configuration
GDBPORT = 26000
# QEMU's gdb stub command line changed in 0.11
QEMUGDB = $(shell if $(QEMU) -help | grep -q '^-gdb'; \
	then echo "-gdb tcp::$(GDBPORT)"; \
	else echo "-s -p $(GDBPORT)"; fi)

# Run QEMU with GDB debugging support
qemu-gdb: $(K)/kernel
	@echo ""
	@echo "------------------------------------------"
	@echo "Starting QEMU with GDB support..."
	@echo "GDB port: $(GDBPORT)"
	@echo "Run \"./gdb.sh\" in another terminal to debug"
	@echo "------------------------------------------"
	@echo ""
	$(QEMU) $(QEMUOPTS) -S $(QEMUGDB)

# Clean build artifacts
clean:
	rm -f $(K)/kernel $(K)/kernel.bin
	rm -f *.tex *.dvi *.idx *.aux *.log *.ind *.ilg
	rm -f */*.o */*.d */*.asm */*.sym

# Phony targets
.PHONY: qemu qemu-gdb clean