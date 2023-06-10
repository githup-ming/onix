BUILD:=./build
SRC:=.

MULTIBOOT2:=0x10000
ENTRYPOINT:=$(shell python -c "print(f'0x{$(MULTIBOOT2) + 64:x}')")

CFLAGS:= -m32 # 32位程序
CFLAGS+= -fno-builtin # 不需要gcc的内置函数
CFLAGS+= -nostdinc # 不需要标志头文件
CFLAGS+= -fno-pic # 不需要位置无关代码
CFLAGS+= -fno-pie # 不需要位置无关的可执行程序
CFLAGS+= -nostdlib # 不需要标准库
CFLAGS+= -fno-stack-protector # 不需要栈保护
CFLAGS:= $(strip ${CFLAGS})

DEBUG:= -g

INCLUDE:= -I$(SRC)/include

LDFLAGS:= -m elf_i386 \
		-static \
		-Ttext $(ENTRYPOINT) \
		--section-start=.multiboot2=$(MULTIBOOT2)
LDFLAGS:=$(strip ${LDFLAGS})

$(BUILD)/boot/%.bin: $(SRC)/boot/%.asm
	$(shell mkdir -p $(dir $@))
	nasm -f bin $< -o $@

$(BUILD)/%.o: $(SRC)/%.asm
	$(shell mkdir -p $(dir $@))
	nasm -f elf32 $(DEBUG) $< -o $@

$(BUILD)/%.o: $(SRC)/%.c
	$(shell mkdir -p $(dir $@))
	gcc $(CFLAGS) $(DEBUG) $(INCLUDE) -c $< -o $@


$(BUILD)/kernel/kernel.bin: \
	$(BUILD)/kernel/start.o \
	$(BUILD)/kernel/main.o \
	$(BUILD)/kernel/io.o \
	$(BUILD)/kernel/console.o \
	$(BUILD)/kernel/printk.o \
	$(BUILD)/kernel/assert.o \
	$(BUILD)/kernel/debug.o \
	$(BUILD)/kernel/global.o \
	$(BUILD)/kernel/task.o \
	$(BUILD)/kernel/thread.o \
	$(BUILD)/kernel/schedule.o \
	$(BUILD)/kernel/mutex.o \
	$(BUILD)/kernel/interrupt.o \
	$(BUILD)/kernel/handler.o \
	$(BUILD)/kernel/clock.o \
	$(BUILD)/kernel/time.o \
	$(BUILD)/kernel/rtc.o \
	$(BUILD)/kernel/memory.o \
	$(BUILD)/kernel/bitmap.o \
	$(BUILD)/lib/string.o \
	$(BUILD)/lib/vsprintf.o \
	$(BUILD)/lib/stdlib.o \
	$(BUILD)/lib/syscall.o \
	$(BUILD)/lib/list.o \

	$(shell mkdir -p $(dir $@))
	ld ${LDFLAGS} $^ -o $@

$(BUILD)/system.bin: $(BUILD)/kernel/kernel.bin
	objcopy -O binary $< $@

$(BUILD)/system.map: $(BUILD)/kernel/kernel.bin
	nm $< | sort > $@


$(BUILD)/master.img: $(BUILD)/boot/boot.bin \
	$(BUILD)/boot/loader.bin \
	$(BUILD)/system.bin \
	$(BUILD)/system.map \

# 创建一个16MB 的硬盘镜像
	yes | bximage -q -hd=16 -func=create -sectsize=512 -imgmode=flat $@

# 将boot.bin 写入硬盘，从头开始（0x00）写一个扇区
	dd if=$(BUILD)/boot/boot.bin of=$@ bs=512 count=1 conv=notrunc

# 将loader.bin 写入硬盘，从第二个扇区（512*2=0x400）开始写4个扇区
	dd if=$(BUILD)/boot/loader.bin of=$@ bs=512 count=4 seek=2 conv=notrunc

# 测试system.bin 小于100k， 如果大于100k则编译报错，需要修改下面的count
	test -n "$$(find $(BUILD)/system.bin -size -100k)"

# 将system.bin 写入硬盘，从第10个扇区(512*10=0x1400)开始写200个扇区
	dd if=$(BUILD)/system.bin of=$@ bs=512 count=200 seek=10 conv=notrunc

$(BUILD)/kernel.iso : $(BUILD)/kernel/kernel.bin $(SRC)/utils/grub.cfg 

# 检测内核文件是否合法
	grub-file --is-x86-multiboot2 $<
# 创建iso目录
	mkdir -p $(BUILD)/iso/boot/grub
# 拷贝内核文件
	cp $< $(BUILD)/iso/boot
# 拷贝grub文件
	cp $(SRC)/utils/grub.cfg $(BUILD)/iso/boot/grub
# 生成 iso 文件
	grub-mkrescue -o $@ $(BUILD)/iso


.PHONY: test
test: $(BUILD)/master.img


.PHONY: usb
usb: boot.bin /dev/sdb
	sudo dd if=/dev/sdb of=tmp.bin bs=512 count=1 conv=notrunc
	cp tmp.bin usb.bin
	sudo rm tmp.bin
	dd if=boot.bin of=usb.bin bs=446 count=1 conv=notrunc
	sudo dd if=usb.bin of=/dev/sdb bs=512 count=1 conv=notrunc
	# rm usb.bin

.PHONY: clean
clean:
	rm -rf $(BUILD)

.PHONY: bochs
bochs: $(BUILD)/master.img
	bochs -q -f bochs/bochsrc -unlock

.PHONY: bochsg
bochsg: $(BUILD)/master.img
	bochs-gdb -q -f bochs/bochsrc.gdb -unlock

.PHONY: bochsb
bochsb: $(BUILD)/kernel.iso
	bochs -q -f bochs/bochsrc.grub -unlock

QEMU:=qemu-system-i386 \
	-m 32M \
	-audiodev pa,id=hda \
	-machine pcspk-audiodev=hda \
	-rtc base=localtime \

QEMU_DISK:=-boot c \
	-drive file=$(BUILD)/master.img,index=0,media=disk,format=raw \

QEMU_CDROM:=-boot d \
	-drive file=$(BUILD)/kernel.iso,media=cdrom \

QEMU_DEBUG:= -s -S

.PHONY: qemu
qemu: $(BUILD)/master.img
	$(QEMU) $(QEMU_DISK)

.PHONY:qemub 
qemub: $(BUILD)/kernel.iso
	$(QEMU) $(QEMU_CDROM) 
	# $(QEMU_DEBUG)

.PHONY: qemug
qemug: $(BUILD)/master.img
		$(QEMU) $(QEMU_DISK) $(QEMU_DEBUG)

$(BUILD)/master.vmdk :$(BUILD)/master.img
	qemu-img convert -O vmdk $< $@

.PHONY: vmdk
vmdk: $(BUILD)/master.vmdk