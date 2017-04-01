ifneq ($(KERNELRELEASE),)

ccflags-y :=	-I$M/include						\
		-std=gnu11						\
		-Werror -Wno-declaration-after-statement		\
		-Wno-unused-function					\
		-DDEBUG

obj-m := playground.o
playground-y :=	src/playground.o					\
		src/ioc.o						\
		src/cmd/work.o						\
		src/cmd/hello.o						\
		src/cmd/list.o

else

all:
	$(MAKE) -C $(KDIR) M=$$PWD modules

clean:
	$(MAKE) -C $(KDIR) M=$$PWD clean

endif
