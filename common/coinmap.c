#include "common.h"
#include "coinmap.h"
#include "map.h"
#include "tiles.h"

static uint32_t coinmap = 0x00000000;   // bit field of coins collected

void coinmap_init(void)
{
    uint8_t bitIndex = 0;
    uint16_t x, y;

    for (y=0;y<MAP_HEIGHT;y++)
    {
        for (x=0;x<MAP_WIDTH;x++)
        {
            if (COIN_TILE == map[y*MAP_WIDTH+x]-1)
            {
                coinmap |= 1 << bitIndex;
                bitIndex++;
            }
        }
    }
}


uint8_t coinmap_remaining(void)
{
    uint8_t i;
    uint8_t coins = 0;
    for (i=0;i<32;i++)
    {
        if (coinmap & (1 << i))
            coins++;
    }
    return coins;
}

bool coinmap_isTaken(uint16_t coinx, uint16_t coiny)
{
    uint8_t bitIndex = 0;
    uint16_t x, y;

    for (y=0;y<MAP_HEIGHT;y++)
    {
        for (x=0;x<MAP_WIDTH;x++)
        {
            if ( coinx == x && coiny == y)
                return !(coinmap & (1 << bitIndex));
            if (COIN_TILE == map[y*MAP_WIDTH+x]-1)
                bitIndex++;
        }
    }
    return false;
}

void coinmap_take(uint16_t coinx, uint16_t coiny)
{
    uint8_t bitIndex = 0;
    uint16_t x, y;

    for (y=0;y<MAP_HEIGHT;y++)
    {
        for (x=0;x<MAP_WIDTH;x++)
        {
            if ( coinx == x && coiny == y)
            {
                coinmap &= ~(1 << bitIndex);
                return;
            }
            if (COIN_TILE == map[y*MAP_WIDTH+x]-1)
                bitIndex++;
        }
    }
}


