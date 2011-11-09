.PHONY: sdl
.PHONY: imme

all:
	@echo Use "make imme" or "make sdl" or "make world" for everything

imme:
	make -C map
	make -f Makefile-imme

install:
	make -C map
	make -f Makefile-imme install

sdl:
	make -C map
	make -f Makefile-sdl

world: imme sdl

clean:
	make -C map clean
	make -f Makefile-imme clean
	make -f Makefile-sdl clean
