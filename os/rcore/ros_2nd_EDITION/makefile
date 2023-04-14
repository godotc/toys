
CFG=debug
ARCH=x86_64
NAME=ros
TARGET=$(ARCH)-$(NAME)

FILE=target/$(TARGET)/$(CFG)/bootimage-$(NAME).bin


run: 
	qemu-system-x86_64 -drive format=raw,file=$(FILE)


