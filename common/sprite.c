#include "common.h"
#include "sprite.h"
#include "lcd.h"

#include "map.h"
#include "tiles.h"

extern uint16_t mapx;
extern uint16_t mapy;

#define IS_EMPTY(x) (((x) == MAP_EMPTY) || ((x) > HORZ_ENEMY_TILE))

bool gridAt(uint8_t x, uint8_t y, uint8_t *tile)
{
    if (x >= SCREEN_WIDTH || y >= SCREEN_HEIGHT)
        return false;

    x >>= 3;
    y >>= 3;

    *tile = map[(mapy+y)*MAP_WIDTH+(mapx+x)];

    return true;
}

bool static ptInRect(int16_t rx, int16_t ry, int16_t rw, int16_t rh, int16_t px, int16_t py)
{
    if (px >= rx && px < rx+rw && py >= ry && py < ry+rh)
        return true;
    return false;
}

bool sprite_hit(const sprite_t *a, const sprite_t *b)
{
    if (ptInRect(a->x, a->y, 8, 8, b->x, b->y))
        return true;
    if (ptInRect(a->x, a->y, 8, 8, b->x+7, b->y))
        return true;
    if (ptInRect(a->x, a->y, 8, 8, b->x, b->y+7))
        return true;
    if (ptInRect(a->x, a->y, 8, 8, b->x+7, b->y+7))
        return true;
    return false;
}


bool sprite_mapscroll(sprite_t *spr)
{
    int16_t screenx = spr->x + spr->xv;
    int16_t screeny = spr->y + spr->yv;
    int16_t worldx = (mapx << 3) + screenx;
    int16_t worldy = (mapy << 3) + screeny;
    uint8_t sprx = worldx >> 3;
    uint8_t spry = worldy >> 3;

    if (!ptInRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, screenx, screeny) ||
        !ptInRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, screenx+8, screeny) ||
        !ptInRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, screenx, screeny+8) ||
        !ptInRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, screenx+8, screeny+8))
    {
        if (worldx >= 0 && worldx <= (MAP_WIDTH << 3) && worldy >= 0 && worldy <= (MAP_HEIGHT << 3))
        {
//            printf("OUT worldx=%d worldy=%d mapx=%d mapy=%d\n", worldx, worldy, mapx, mapy);
            if (worldy >= ((mapy + 7)<<3) && IS_EMPTY(map[(spry+1)*MAP_WIDTH+sprx]) )
            {
//                printf("DOWN\n");
                mapy += 8;
                spr->y = 0;
                return true;
            }
            if (worldy < (mapy<<3) && IS_EMPTY(map[(spry-1)*MAP_WIDTH+sprx]))
            {
//                printf("UP\n");
                mapy -= 8;
                spr->y = SCREEN_HEIGHT - 8;
                return true;
            }
            if (worldx > ((mapx + 7)<<3) && IS_EMPTY(map[(spry)*MAP_WIDTH+(sprx+1)]))
            {
//                printf("RIGHT\n");
                mapx += 16;
                spr->x = 0;
                return true;
            }
            if (worldx < (mapx<<3) && IS_EMPTY(map[(spry)*MAP_WIDTH+(sprx-1)]))
            {
//                printf("LEFT\n");
                mapx -= 16;
                spr->x = SCREEN_WIDTH - 8;
                return true;
            }
        }
    }
    return false;
}

void sprite_array_chase(sprite_t *target, sprite_t *spr, uint8_t n)
{
    while(n--)
    {
        if (!(spr->flags & SPRITE_FLAG_IGNORE) && spr->flags & SPRITE_FLAG_CHASE)
            sprite_chase(target, spr);
        spr++;
    }
}

void sprite_chase(sprite_t *target, sprite_t *chaser)
{
    chaser->xv = 0;
    chaser->yv = 0;

    if (target->x > chaser->x)
        chaser->xv = 1;
    if (target->x < chaser->x)
        chaser->xv = -1;

    // follow horizontally first
    if (chaser->xv != 0)
        return;

    if (target->y > chaser->y)
        chaser->yv = 1;
    if (target->y < chaser->y)
        chaser->yv = -1;
}

void sprite_move(sprite_t *spr)
{
    // screen edge x
    if (spr->x + spr->xv > (SCREEN_WIDTH - 8) || spr->x + spr->xv < 0)
    {
        if (spr->flags & SPRITE_FLAG_BOUNCE)
            spr->xv = -spr->xv;
        else
            spr->xv = 0;
    }
    else
    {
        uint8_t tile;
        if ( (!gridAt(spr->x + spr->xv + 0, spr->y + spr->yv + 0, &tile)) || !IS_EMPTY(tile) ||
             (!gridAt(spr->x + spr->xv + 7, spr->y + spr->yv + 0, &tile)) || !IS_EMPTY(tile) ||
             (!gridAt(spr->x + spr->xv + 0, spr->y + spr->yv + 7, &tile)) || !IS_EMPTY(tile) ||
             (!gridAt(spr->x + spr->xv + 7, spr->y + spr->yv + 7, &tile)) || !IS_EMPTY(tile) )
        {
            // FIXME, budge edge
            if (spr->flags & SPRITE_FLAG_BOUNCE)
                spr->xv = -spr->xv;
        }
        else
        {
            spr->x += spr->xv;
        }
    }

    // screen edge y
    if (spr->y + spr->yv > (SCREEN_HEIGHT - 8) || spr->y + spr->yv < 0)
    {
        if (spr->flags & SPRITE_FLAG_BOUNCE)
            spr->yv = -spr->yv;
        else
            spr->yv = 0;
    }
    else
    {
        uint8_t tile;
        if ( (!gridAt(spr->x + spr->xv + 0, spr->y + spr->yv + 0, &tile)) || !IS_EMPTY(tile) ||
             (!gridAt(spr->x + spr->xv + 7, spr->y + spr->yv + 0, &tile)) || !IS_EMPTY(tile) ||
             (!gridAt(spr->x + spr->xv + 0, spr->y + spr->yv + 7, &tile)) || !IS_EMPTY(tile) ||
             (!gridAt(spr->x + spr->xv + 7, spr->y + spr->yv + 7, &tile)) || !IS_EMPTY(tile) )
        {
            // FIXME, budge edge
            if (spr->flags & SPRITE_FLAG_BOUNCE)
                spr->yv = -spr->yv;
        }
        else
        {
            spr->y += spr->yv;
        }
    }
}

void sprite_array_move(sprite_t *spr, uint8_t n)
{
    while(n--)
        sprite_move(spr++);
}

// y is quantized
void sprite_drawTile(uint8_t x, uint8_t y, const uint8_t *pix)
{
    lcd_setPos(x, y >> 3);
    lcd_txData(pix[0]);
    lcd_txData(pix[1]);
    lcd_txData(pix[2]);
    lcd_txData(pix[3]);
    lcd_txData(pix[4]);
    lcd_txData(pix[5]);
    lcd_txData(pix[6]);
    lcd_txData(pix[7]);
}

void sprite_draw(sprite_t *spr)
{
    uint8_t yoff = spr->y & 0x7;
    uint8_t xoff = spr->x & 0x7;
    const uint8_t *background = tiles[MAP_EMPTY-1];
    const uint8_t *mask = spr->pix + 8;

    if (yoff)
    {
        lcd_setPos(spr->x, (spr->y >> 3)+1);
        lcd_txData((spr->pix[0] >> (8-yoff)) | (((background[(0 + xoff) & 0x7])) & ~((mask[0] >> (8-yoff))) ) );
        lcd_txData((spr->pix[1] >> (8-yoff)) | (((background[(1 + xoff) & 0x7])) & ~((mask[1] >> (8-yoff))) ) );
        lcd_txData((spr->pix[2] >> (8-yoff)) | (((background[(2 + xoff) & 0x7])) & ~((mask[2] >> (8-yoff))) ) );
        lcd_txData((spr->pix[3] >> (8-yoff)) | (((background[(3 + xoff) & 0x7])) & ~((mask[3] >> (8-yoff))) ) );
        lcd_txData((spr->pix[4] >> (8-yoff)) | (((background[(4 + xoff) & 0x7])) & ~((mask[4] >> (8-yoff))) ) );
        lcd_txData((spr->pix[5] >> (8-yoff)) | (((background[(5 + xoff) & 0x7])) & ~((mask[5] >> (8-yoff))) ) );
        lcd_txData((spr->pix[6] >> (8-yoff)) | (((background[(6 + xoff) & 0x7])) & ~((mask[6] >> (8-yoff))) ) );
        lcd_txData((spr->pix[7] >> (8-yoff)) | (((background[(7 + xoff) & 0x7])) & ~((mask[7] >> (8-yoff))) ) );
    }

    lcd_setPos(spr->x, (spr->y >> 3));
    lcd_txData((spr->pix[0] << (yoff)) | ((background[(xoff + 0) & 0x7]) & ~(mask[0] << (yoff))  ));
    lcd_txData((spr->pix[1] << (yoff)) | ((background[(xoff + 1) & 0x7]) & ~(mask[1] << (yoff))  ));
    lcd_txData((spr->pix[2] << (yoff)) | ((background[(xoff + 2) & 0x7]) & ~(mask[2] << (yoff))  ));
    lcd_txData((spr->pix[3] << (yoff)) | ((background[(xoff + 3) & 0x7]) & ~(mask[3] << (yoff))  ));
    lcd_txData((spr->pix[4] << (yoff)) | ((background[(xoff + 4) & 0x7]) & ~(mask[4] << (yoff))  ));
    lcd_txData((spr->pix[5] << (yoff)) | ((background[(xoff + 5) & 0x7]) & ~(mask[5] << (yoff))  ));
    lcd_txData((spr->pix[6] << (yoff)) | ((background[(xoff + 6) & 0x7]) & ~(mask[6] << (yoff))  ));
    lcd_txData((spr->pix[7] << (yoff)) | ((background[(xoff + 7) & 0x7]) & ~(mask[7] << (yoff))  ));
}

void sprite_array_draw(sprite_t *spr, uint8_t n)
{
    while(n--)
    {
        if (!(spr->flags & SPRITE_FLAG_IGNORE))
            sprite_draw(spr);
        spr++;
    }
}


