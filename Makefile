KDIR = $(shell realpath linux-stable)
MDIR = $(shell realpath module)
UDIR = $(shell realpath user)
COMMON = $(shell realpath include)

.PHONY: kernel module user clean clean-module clean-user distclean distclean-user

all: module user

kernel:
	$(MAKE) -C $(KDIR)

module:
	$(MAKE) -C $(MDIR) KDIR=$(KDIR) COMMON=$(COMMON)

user:
	$(MAKE) -C $(UDIR) KDIR=$(KDIR) MDIR=$(MDIR) COMMON=$(COMMON)

clean-module:
	$(MAKE) -C $(MDIR) KDIR=$(KDIR) clean

clean-user:
	$(MAKE) -C $(UDIR) clean

clean: clean-module clean-user

distclean: clean-module distclean-user

distclean-user:
	$(MAKE) -C $(UDIR) distclean
