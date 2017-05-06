ifneq ($(KERNELRELEASE),)

ccflags-y :=	-I$(M)/include						\
		-I$(COMMON)						\
		-std=gnu11						\
		-Werror							\
		-Wno-declaration-after-statement			\
		-Wno-strict-prototypes					\
		-Wno-unused-function					\
		-DDEBUG

obj-m := playground.o

cmds := hello sleep list fg kill meminfo modinfo exec lsmod
cmd-objs := $(addprefix src/cmd/handlers/,$(addsuffix .o,$(cmds)))

playground-y :=	src/playground.o					\
		src/ioc.o						\
		src/cmd/work.o						\
		$(cmd-objs)
else

all:
	$(MAKE) -C $(KDIR) M=$$PWD modules

clean:
	$(MAKE) -C $(KDIR) M=$$PWD clean

endif
