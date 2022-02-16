TARGETS := all clean test
SUBDIRS := gcc/. sfpi/.

$(TARGETS): $(SUBDIRS)
$(SUBDIRS):
	@$(MAKE) -C $@ $(MAKECMDGOALS)

.PHONY: $(TARGETS) $(SUBDIRS)
