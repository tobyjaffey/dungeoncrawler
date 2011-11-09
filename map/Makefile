SDL_CFLAGS = `sdl-config --cflags`
SDL_LDFLAGS = `sdl-config --libs` -LSDL_gfx -lSDL_gfx 

all:
	gcc -o rawmaptoc -Wall rawmaptoc.c
	./tmxtoc.sh map.tmx && cp map.h ../include && cp map.c ../common
	gcc -o tiles2hdr -Wall tiles2hdr.c  $(SDL_CFLAGS) $(SDL_LDFLAGS)
	./tiles2hdr tiles.bmp && cp tiles.h ../include && cp tiles.c ../common

clean:
	rm -f rawmaptoc map.c map.h tiles.c tiles.h tiles2hdr

