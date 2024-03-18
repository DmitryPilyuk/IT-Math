.PHONY: all clean

SUBDIRS := resultO0 resultO3

all: $(SUBDIRS)

clean:
	rm result*/*.o result*/main result*/block_size_test

$(SUBDIRS):
	$(MAKE) -C $@

.PHONY: all $(SUBDIRS)