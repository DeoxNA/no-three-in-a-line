# This makefile builds or cleans all binaries in each sub-directory
SUBDIRS := $(wildcard */.)

COMMAND :=

all: $(SUBDIRS)

debug: COMMAND += debug
debug: all

clean: COMMAND += clean
clean: all

$(SUBDIRS):
	$(MAKE) -C $@ $(COMMAND)

# Forces commands to run
.PHONY: all $(SUBDIRS)
.PHONY: debug
.PHONY: clean
