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
		src/cmd/handlers/hello.o				\
		src/cmd/handlers/list.o					\
		src/cmd/handlers/modinfo.o				\
		src/cmd/handlers/meminfo.o				\
		src/cmd/handlers/kill.o                                 \
		src/cmd/handlers/wait.o
else

all:
	$(MAKE) -C $(KDIR) M=$$PWD modules

clean:
	$(MAKE) -C $(KDIR) M=$$PWD clean

endif
