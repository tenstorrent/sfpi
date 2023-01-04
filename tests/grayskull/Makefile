TARGETS := all clean test promote
SUBDIRS := gcc/. sfpi/.

$(TARGETS): $(SUBDIRS)
$(SUBDIRS):
	@$(MAKE) -C $@ $(MAKECMDGOALS)

.PHONY: $(TARGETS) $(SUBDIRS)
