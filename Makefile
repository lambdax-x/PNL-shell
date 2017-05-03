KDIR = $(shell realpath linux-stable)
MDIR = $(shell realpath module)
UDIR = $(shell realpath user)
COMMON = $(shell realpath include)

MAKE-KERNEL=$(MAKE) -C $(KDIR)
MAKE-MODULE=$(MAKE) -C $(MDIR) KDIR=$(KDIR) COMMON=$(COMMON)
MAKE-USER=$(MAKE) -C $(UDIR) COMMON=$(COMMON)

.PHONY: kernel module user clean clean-module clean-user distclean distclean-user

all: module user

kernel:
	$(MAKE-KERNEL)

module:
	$(MAKE-MODULE)

user:
	$(MAKE-USER)

clean: clean-module clean-user

clean-module:
	$(MAKE-MODULE) clean

clean-user:
	$(MAKE-USER) clean

distclean: clean-module distclean-user

distclean-user:
	$(MAKE-USER) distclean
