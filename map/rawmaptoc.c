#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    uint32_t x, y, width = 0, height = 0;
    uint32_t tile;
    FILE *map_h, *map_c;

    if (argc < 3)
    {
        fprintf(stderr, "Usage %s <width> <height>\n", argv[0]);
        exit(1);
    }

    width = atoi(argv[1]);
    height = atoi(argv[2]);

    if ((width % 16 != 0) || (height % 8 != 0))
    {
        fprintf(stderr, "Invalid map size %dx%d: Only multiples of 16x8 supported", width, height);
        exit(1);
    }

    if (NULL == (map_h = fopen("map.h", "w")))
    {
        fprintf(stderr, "Can't open map.h\n");
        exit(1);
    }

    fprintf(map_h, "#ifndef MAP_H\n");
    fprintf(map_h, "#define MAP_H 1\n");
    fprintf(map_h, "#include \"common.h\"\n");
    fprintf(map_h, "#define MAP_WIDTH %d\n", width);
    fprintf(map_h, "#define MAP_HEIGHT %d\n", height);
    fprintf(map_h, "#define MAP_EMPTY 1\n");
    fprintf(map_h, "extern const uint8_t map[MAP_WIDTH*MAP_HEIGHT];\n");
    fprintf(map_h, "#endif\n");

    fclose(map_h);

    if (NULL == (map_c = fopen("map.c", "w")))
    {
        fprintf(stderr, "Can't open map.c\n");
        exit(1);
    }

    fprintf(map_c, "#include \"common.h\"\n");
    fprintf(map_c, "#include \"map.h\"\n\n");

    fprintf(map_c, "const uint8_t map[] = \n");
    fprintf(map_c, "{\n\t");

    for (y=0;y<height;y++)
    {
        for (x=0;x<width;x++)
        {
            if (4 != read(0, &tile, 4))
            {
                fprintf(stderr, "Read failed\n");
                exit(1);
            }
            fprintf(map_c, "0x%02X,", tile);
        }
        fprintf(map_c, "\n");
        if (y != height-1)
            fprintf(map_c, "\t");
    }
    fprintf(map_c, "};\n");

    fclose(map_c);

    return 0;
}
