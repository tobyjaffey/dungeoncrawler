#include "common.h"
#include "splash.h"
#include "clk.h"
#include "lcd.h"
#include "tiles.h"
#include "sprite.h"
#include "key.h"

const uint8_t banner_bg[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

static void pause(void)
{
    uint16_t i;
    for (i=0;i<1500/30;i++)
    {
        // a hack to get SDL to dump frames
        clk_delayms(30);
        lcd_cs(1);
        lcd_cs(0);
    }
    while(key_get() == 0)
    {
        // a hack to get SDL to dump frames
        lcd_cs(1);
        lcd_cs(0);
    }
}

void splash_intro(void)
{
    lcd_cs(1);
    lcd_setNormalReverse(1);

    lcd_cls(0x00);
    splash_puts(1, 0, "IN THE DARK...");
    splash_puts(3, 2, "THEY LURK");

    sprite_drawTile(6<<3, 5<<3, tiles[CHASER_ENEMY_TILE]);
    sprite_drawTile(8<<3, 5<<3, tiles[HORZ_ENEMY_TILE]);
    sprite_drawTile(10<<3, 5<<3, tiles[VERT_ENEMY_TILE]);

    splash_puts(0, 7, "JRT@HODGEPIG.ORG");

    lcd_cs(0);
    pause();
}

void splash_player(uint8_t x, uint8_t y)
{
    lcd_cs(1);
    lcd_setNormalReverse(1);
    lcd_cls(0x00);
    sprite_drawTile(x, y, tiles[PLAYER_TILE]);

    sprite_drawTile(x, y+16, tiles[A_TILE + ('L' - 'A')]);
    sprite_drawTile(x-12, y, tiles[A_TILE + ('Q' - 'A')]);
    sprite_drawTile(x+12, y, tiles[A_TILE + ('W' - 'A')]);
    sprite_drawTile(x, y-16, tiles[A_TILE + ('P' - 'A')]);

    splash_puts(3, (y == 0) ? 6 : 0, "CONTROLS");

    lcd_cs(0);
    pause();
}

void splash_dead(void)
{
    uint8_t x, y;

    lcd_cs(1);
    lcd_setNormalReverse(1);
    for (y=2;y<=4;y++)
    {
        for (x=0;x<16;x++)
        {
            sprite_drawTile(x<<3, y<<3, banner_bg);
        }
    }
    splash_puts(6, 3, "DEAD");
    lcd_cs(0);
    pause();
}

void splash_win(void)
{
    lcd_cs(1);
    lcd_setNormalReverse(1);
    lcd_cls(0x00);
    splash_puts(4, 3, "YOU WIN");
    lcd_cs(0);
    pause();
}

void splash_coins(uint8_t coins)
{
    char buf[] = "XX COINS LEFT";
    uint8_t x, y;

    if (coins < 10)
    {
        buf[0] = ' ';
        buf[1] = '0' + coins;
    }
    else
    {
        buf[0] = '0' + coins/10;
        buf[1] = '0' + coins % 10;
    }

    lcd_cs(1);
    lcd_setNormalReverse(1);

    for (y=2;y<=4;y++)
    {
        for (x=0;x<16;x++)
        {
            sprite_drawTile(x<<3, y<<3, banner_bg);
        }
    }

    splash_puts(1,3, buf);
    lcd_cs(0);
    pause();
}

void splash_puts(uint8_t x, uint8_t y, const char *msg)
{
    while(*msg)
    {
        if (*msg >= '0' && *msg <= '9')
            sprite_drawTile(x<<3, y<<3, tiles[ZERO_TILE + (*msg - '0')]);
        else
        if (*msg >= 'A' && *msg <= 'Z')
            sprite_drawTile(x<<3, y<<3, tiles[A_TILE + (*msg - 'A')]);
        else
        if (*msg == '.')
            sprite_drawTile(x<<3, y<<3, tiles[DOT_TILE]);
        else
        if (*msg == '@')
            sprite_drawTile(x<<3, y<<3, tiles[AT_TILE]);

        msg++;
        x++;
    }
}

