#include "common.h"
#include "wdt.h"
#include "clk.h"
#include "pio.h"
#include "spi.h"
#include "lcd.h"
#include "key.h"
#include "sprite.h"
#include "coinmap.h"
#include "splash.h"

#include "tiles.h"
#include "map.h"

#ifdef PLATFORM_SDL
#include "cmdline.h"
#endif

uint16_t mapx = 0, mapy = 0;    // coordinates of map to show

#define MAX_SPRITES 4
static sprite_t sprites[MAX_SPRITES];
static uint8_t num_sprites = 0;

static void setup_player(void)
{
    uint16_t x, y;
    // scan map for player start
    for (y=0;y<MAP_HEIGHT;y++)
    {
        for (x=0;x<MAP_WIDTH;x++)
        {
            if (PLAYER_TILE == map[y*MAP_WIDTH+x]-1)
            {
                mapx = (x / 16) * 16;
                mapy = (y / 8) * 8;

                sprites[0].x = (x - mapx) << 3;
                sprites[0].y = (y - mapy) << 3;
                sprites[0].xv = 0;
                sprites[0].yv = 0;
                sprites[0].pix = tiles[PLAYER_TILE];
                sprites[0].flags = 0;

                num_sprites = 1;
                return;
            }
        }
    }
}

static void setup_sprites(void)
{
    uint16_t x, y;

    num_sprites = 1;

    // scan room for enemies
    for (y=mapy;y<mapy+8;y++)
    {
        for (x=mapx;x<mapx+16;x++)
        {
            switch(map[y*MAP_WIDTH+x]-1)
            {
                case HORZ_ENEMY_TILE:
                sprites[num_sprites].x = (x - mapx) << 3;
                sprites[num_sprites].y = (y - mapy) << 3;
                sprites[num_sprites].xv = 1;
                sprites[num_sprites].yv = 0;
                sprites[num_sprites].pix = tiles[HORZ_ENEMY_TILE];
                sprites[num_sprites].flags = SPRITE_FLAG_BOUNCE | SPRITE_FLAG_DEADLY;
                num_sprites++;
                break;

                case VERT_ENEMY_TILE:
                sprites[num_sprites].x = (x - mapx) << 3;
                sprites[num_sprites].y = (y - mapy) << 3;
                sprites[num_sprites].xv = 0;
                sprites[num_sprites].yv = 1;
                sprites[num_sprites].pix = tiles[VERT_ENEMY_TILE];
                sprites[num_sprites].flags = SPRITE_FLAG_BOUNCE | SPRITE_FLAG_DEADLY;
                num_sprites++;
                break;

                case CHASER_ENEMY_TILE:
                sprites[num_sprites].x = (x - mapx) << 3;
                sprites[num_sprites].y = (y - mapy) << 3;
                sprites[num_sprites].xv = 0;
                sprites[num_sprites].yv = 0;
                sprites[num_sprites].pix = tiles[CHASER_ENEMY_TILE];
                sprites[num_sprites].flags = SPRITE_FLAG_CHASE | SPRITE_FLAG_DEADLY;
                num_sprites++;
                break;

                case COIN_TILE:
                if (!coinmap_isTaken(x, y))
                {
                    sprites[num_sprites].x = (x - mapx) << 3;
                    sprites[num_sprites].y = (y - mapy) << 3;
                    sprites[num_sprites].xv = 0;
                    sprites[num_sprites].yv = 0;
                    sprites[num_sprites].pix = tiles[COIN_TILE];
                    sprites[num_sprites].flags = 0;
                    num_sprites++;
                }
                break;

            }
            if (num_sprites == MAX_SPRITES)
                return;
        }
    }
}

#ifdef PLATFORM_SDL
int main(int argc, char *argv[])
#else
int main(void)
#endif
{
    uint16_t x, y;
    uint8_t i;

#ifdef PLATFORM_SDL
    cmdline_parse(argc, argv);
#endif

    wdt_init();
    clk_init();
    pio_init();
    spi_init();
    lcd_init();
    key_init();

start_game:
    coinmap_init();
    setup_player();
    setup_sprites();

    splash_intro();
    splash_player(sprites[0].x, sprites[0].y);

    while(1)
    {
        lcd_cs(1);
        lcd_setNormalReverse(1);

        for (y=0;y<8;y++)
        {
            for (x=0;x<16;x++)
            {
                uint8_t tile = map[(y+mapy)*MAP_WIDTH+(x+mapx)]-1;
                if (tile < HORZ_ENEMY_TILE)
                    sprite_drawTile(x<<3, y<<3, tiles[tile]);
                else
                    sprite_drawTile(x<<3, y<<3, tiles[0]);
            }
        }
            
        if (sprite_mapscroll(&sprites[0]))
            setup_sprites();

        sprite_array_draw(sprites, num_sprites);
        sprite_array_chase(&sprites[0], &sprites[1], num_sprites-1);
        sprite_array_move(sprites, num_sprites);

        sprites[0].xv = 0;
        sprites[0].yv = 0;
        switch(key_get())
        {
            case 'Q':
                sprites[0].xv = -1;
            break;
            case 'W':
                sprites[0].xv = 1;
            break;
            case 'L':
                sprites[0].yv = 1;
            break;
            case 'P':
                sprites[0].yv = -1;
            break;
        }

        lcd_cs(0);

        for (i=1;i<num_sprites;i++)
        {
            if (sprite_hit(&sprites[0], &sprites[i]))
            {
                if (sprites[i].flags & SPRITE_FLAG_DEADLY)
                {
                    splash_dead();
                    goto start_game;
                }
                else
                {
                    // it's a coin
                    if (!(sprites[i].flags & SPRITE_FLAG_IGNORE))
                    {
                        uint8_t remaining;
                        coinmap_take(mapx + (sprites[i].x >> 3), mapy + (sprites[i].y >> 3));
                        sprites[i].flags |= SPRITE_FLAG_IGNORE;
                        remaining = coinmap_remaining();
                        if (remaining > 0)
                            splash_coins(coinmap_remaining());
                        else
                        {
                            splash_win();
                            goto start_game;
                        }
                    }
                }
            }
        }

        clk_delayms(1000/30);
    }
}
