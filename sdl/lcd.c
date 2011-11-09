#include "common.h"
#include "lcd.h"
#include "cmdline.h"

#include <SDL.h>
#include <SDL_rotozoom.h>
#include <SDL_gfxPrimitives.h>

#define SCALEX 4
#define SCALEY 2

#define FATALERR { fprintf(stderr, "%s:%s\n", __func__, SDL_GetError()); exit(-1); }

static uint8_t cursor_x = 0, cursor_y = 0;
static SDL_Surface *screen = NULL;
static SDL_Surface *offscreen = NULL;
static uint32_t frameIndex = 0;

void lcd_init(void)
{
    if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
        FATALERR;

    SDL_EnableKeyRepeat(1, 1);

    if (NULL == (screen = SDL_SetVideoMode(SCREEN_WIDTH*SCALEX, SCREEN_HEIGHT*SCALEY, 32, 0)))
        FATALERR;

    offscreen = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, screen->format->BitsPerPixel, screen->format->Rmask, screen->format->Gmask, screen->format->Bmask, screen->format->Amask);
    SDL_SetColorKey(offscreen, SDL_SRCCOLORKEY, 0x00000000);
}

void lcd_cs(uint8_t on)
{
    if (!on)
    {
        SDL_Surface *scaled;
        scaled = zoomSurface(offscreen, SCALEX, SCALEY, 0);
        SDL_BlitSurface(scaled, NULL, screen, NULL);
        SDL_FreeSurface(scaled);
        SDL_UpdateRect(screen, 0, 0, screen->w, screen->h);

        if (NULL != cmdline_saveframes())
        {
            char fname[1024];
            snprintf(fname, sizeof(fname), "%s-%08d.bmp", cmdline_saveframes(), frameIndex++);
            if (0 != SDL_SaveBMP(screen, fname))
            {
                fprintf(stderr, "Failed to save %s\n", fname);
                exit(1);
            }
        }
    }
}

void lcd_cls(uint8_t ch)
{
    uint8_t row, i;
    for (row=0;row<8;row++)
    {
        lcd_setPos(0, row);
        for (i=0;i<128;i++)
            lcd_txData(ch);
    }
}

void lcd_setNormalReverse(uint8_t mode)
{
}


void lcd_setPos(uint8_t row, uint8_t col)
{
    cursor_x = row;
    cursor_y = col;
}

void lcd_txData(uint8_t c)
{
    uint32_t *pix = offscreen->pixels;
    uint8_t i;

    for (i=0;i<8;i++)
    {
        if (c & (1<<i))
            *(pix + ((cursor_y * 8 + i) * SCREEN_WIDTH) + cursor_x) = 0xFFFFFFFF;
        else
            *(pix + ((cursor_y * 8 + i) * SCREEN_WIDTH) + cursor_x) = 0x00000000;
    }
    cursor_x++;
}


void key_init(void)
{
}

uint16_t key_get(void)
{
    SDL_Event event;
    while(SDL_PollEvent(&event))
    {
        switch(event.type)
        {
            case SDL_KEYDOWN:
            switch(event.key.keysym.sym)
            {
                case SDLK_ESCAPE:
                SDL_Quit();
                exit(0);
                break;

                case SDLK_LEFT: return 'Q'; break;
                case SDLK_RIGHT: return 'W'; break;
                case SDLK_UP: return 'P'; break;
                case SDLK_DOWN: return 'L'; break;

                default:
                break;
            }
        }
    }
    return 0;
}


