TARGETS := all clean test promote
SUBDIRS := grayskull/. wormhole/. riscv/.

$(TARGETS): $(SUBDIRS)
$(SUBDIRS):
	@$(MAKE) -C $@ $(MAKECMDGOALS)

.PHONY: $(TARGETS) $(SUBDIRS)
