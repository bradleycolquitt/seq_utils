default: all

all: release

debug:
	$(MAKE) -C debug

release:
	$(MAKE) -C bin

clean:
	$(MAKE) -C debug clean
	$(MAKE) -C release clean

.PHONY: default all debug release clean
