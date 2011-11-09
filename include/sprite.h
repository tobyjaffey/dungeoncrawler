#ifndef SPRITE_H
#define SPRITE_H 1


enum
{
    SPRITE_FLAG_BOUNCE = 0x1,
    SPRITE_FLAG_DEADLY = 0x2,
    SPRITE_FLAG_IGNORE = 0x4,
    SPRITE_FLAG_CHASE  = 0x8
};

typedef struct
{
    uint8_t x, y;
    int8_t xv:4;
    int8_t yv:4;
    const uint8_t *pix;
    uint8_t flags;
} sprite_t;

void sprite_move(sprite_t *spr);
void sprite_array_move(sprite_t *spr, uint8_t n);

void sprite_array_chase(sprite_t *target, sprite_t *spr, uint8_t n);
void sprite_chase(sprite_t *target, sprite_t *chaser);

void sprite_drawTile(uint8_t x, uint8_t y, const uint8_t *pix);
void sprite_draw(sprite_t *spr);
void sprite_array_draw(sprite_t *spr, uint8_t n);
bool sprite_hit(const sprite_t *a, const sprite_t *b);
bool sprite_mapscroll(sprite_t *spr);


#endif

