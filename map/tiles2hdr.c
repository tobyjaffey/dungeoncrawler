#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main(int argc, char *argv[])
{
    SDL_Surface *image, *imageRGB;
    uint32_t i, y, x;
    uint32_t *pixels;
    FILE *tiles_c, *tiles_h;

    if (argc < 2)
    {
        fprintf(stderr, "Usage %s <filename>\n", argv[0]);
        return 1;
    }

    if (NULL == (image = SDL_LoadBMP(argv[1])))
    {
        fprintf(stderr, "Unable to load bitmap: %s\n", SDL_GetError());
        return 1;
    }

    if (image->w % 8 != 0 || image->h != 8 || image->w > 256*8)
    {
        fprintf(stderr, "Expecting an (n*8) x 8 image (got %dx%d)\n", image->w, image->h);
        return 1;
    }

    if (NULL == (imageRGB = SDL_CreateRGBSurface(0, image->w, image->h, 32, 0, 0, 0, 0)))
    {
        fprintf(stderr, "Unable to create surface: %s\n", SDL_GetError());
        return 1;
    }

    SDL_BlitSurface(image, NULL, imageRGB, NULL);

    pixels = imageRGB->pixels;

#if 0
    for (y=0;y<imageRGB->h;y++)
    {
        for (x=0;x<imageRGB->w;x++)
        {
            printf("%c", pixels[y*imageRGB->w + x] == 0 ? '0' : '1');
        }
        printf("\n");
    }
#endif

    if (NULL == (tiles_c = fopen("tiles.c", "w")))
    {
        fprintf(stderr, "Can't open tiles.c\n");
        return 1;
    }

    if (NULL == (tiles_h = fopen("tiles.h", "w")))
    {
        fprintf(stderr, "Can't open tiles.h\n");
        return 1;
    }

    fprintf(tiles_h, "#ifndef TILES_H\n");
    fprintf(tiles_h, "#define TILES_H 1\n");
    fprintf(tiles_h, "#include \"common.h\"\n");
    fprintf(tiles_h, "extern const uint8_t tiles[%d][8];\n", imageRGB->w / 8);
    fprintf(tiles_h, "#define HORZ_ENEMY_TILE 6\n");
    fprintf(tiles_h, "#define HORZ_ENEMY_TILE_MASK 7\n");
    fprintf(tiles_h, "#define VERT_ENEMY_TILE 8\n");
    fprintf(tiles_h, "#define VERT_ENEMY_TILE_MASK 9\n");
    fprintf(tiles_h, "#define CHASER_ENEMY_TILE 10\n");
    fprintf(tiles_h, "#define CHASER_ENEMY_TILE_MASK 11\n");
    fprintf(tiles_h, "#define PLAYER_TILE 12\n");
    fprintf(tiles_h, "#define PLAYER_TILE_MASK 13\n");
    fprintf(tiles_h, "#define COIN_TILE 14\n");
    fprintf(tiles_h, "#define COIN_TILE_MASK 15\n");
    fprintf(tiles_h, "#define ZERO_TILE 16\n");
    fprintf(tiles_h, "#define A_TILE 26\n");
    fprintf(tiles_h, "#define AT_TILE 52\n");
    fprintf(tiles_h, "#define DOT_TILE 53\n");
    fprintf(tiles_h, "#endif\n");
    fclose(tiles_h);

    fprintf(tiles_c, "#include \"common.h\"\n");
    fprintf(tiles_c, "#include \"tiles.h\"\n");
    fprintf(tiles_c, "const uint8_t tiles[%d][8] = \n", imageRGB->w / 8);
    fprintf(tiles_c, "{\n");
    
    for (i=0;i<imageRGB->w;i+=8)
    {
        fprintf(tiles_c, "\t{");
        for (x=0;x<8;x++)
        {
            uint8_t line = 0;
            uint8_t mask = 1;
            for (y=0;y<8;y++)
            {
                line |= pixels[ (y * imageRGB->w) + (i + x) ] == 0 ? 0 : mask;
                mask <<= 1;
            }
            fprintf(tiles_c, "0x%02X,", line);
        }
        fprintf(tiles_c, "},\n");
    }

    fprintf(tiles_c, "};\n");
 
    fclose(tiles_c);

    SDL_FreeSurface(imageRGB);
    SDL_FreeSurface(image);
    return 0;
}

