CROSS_COMPILE = riscv64-linux-gnu-
CC = $(CROSS_COMPILE)gcc
LD = $(CROSS_COMPILE)ld
OBJCOPY = $(CROSS_COMPILE)objcopy

CFLAGS = -march=rv64g -mabi=lp64d -mcmodel=medany -fno-common -g -fno-stack-protector
CFLAGS += -fno-pie -no-pie -nostdlib -nostartfiles -ffreestanding -Wl,--build-id=none

LDFLAGS = -z max-page-size=4096

# OpenSBI 构建配置
OPENSBI_DIR = opensbi
OPENSBI_BUILD_DIR = $(OPENSBI_DIR)/build
OPENSBI_PLATFORM = generic
FIRMWARE_DIR = firmware

all: kernel firmware

# 构建 OpenSBI firmware
firmware: $(FIRMWARE_DIR)/fw_jump.elf

$(FIRMWARE_DIR)/fw_jump.elf: 
	mkdir -p $(FIRMWARE_DIR)
	$(MAKE) -C $(OPENSBI_DIR) \
		CROSS_COMPILE=$(CROSS_COMPILE) \
		PLATFORM=$(OPENSBI_PLATFORM) \
		O=$(OPENSBI_BUILD_DIR)
	cp $(OPENSBI_BUILD_DIR)/platform/$(OPENSBI_PLATFORM)/firmware/fw_jump.elf $(FIRMWARE_DIR)/

kernel: kernel/entry.S kernel/main.c kernel.ld
	$(CC) $(CFLAGS) -T kernel.ld -o kernel kernel/entry.S kernel/main.c

kernel.bin: kernel
	$(OBJCOPY) kernel --strip-all -O binary kernel.bin

qemu: kernel firmware
	qemu-system-riscv64 \
		-machine virt \
		-nographic \
		-bios $(FIRMWARE_DIR)/fw_jump.elf \
		-device loader,file=kernel,addr=0x80200000

qemu-gdb: kernel firmware
	qemu-system-riscv64 \
		-machine virt \
		-nographic \
		-bios $(FIRMWARE_DIR)/fw_jump.elf  \
		-device loader,file=kernel,addr=0x80200000 \
		-s -S

clean:
	rm -f kernel kernel.bin
	rm -rf $(FIRMWARE_DIR)
	$(MAKE) -C $(OPENSBI_DIR) clean O=$(OPENSBI_BUILD_DIR)

.PHONY: all firmware kernel qemu qemu-gdb clean
