KDIR = $(shell realpath linux-stable)
MDIR = $(shell realpath module)
UDIR = $(shell realpath user)

.PHONY: kernel module user clean

all: module user

kernel:
	$(MAKE) -C $(KDIR)

module:
	$(MAKE) -C $(MDIR) KDIR=$(KDIR)

user:
	$(MAKE) -C $(UDIR) KDIR=$(KDIR) MDIR=$(MDIR)

clean-module:
	$(MAKE) -C $(MDIR) KDIR=$(KDIR) clean

clean-user:
	$(MAKE) -C $(UDIR) clean

clean: clean-module clean-user

distclean: clean-module
	$(MAKE) -C $(UDIR) distclean
    
