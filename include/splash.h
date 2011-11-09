#ifndef SPLASH_H
#define SPLASH_H 1

void splash_intro(void);
void splash_player(uint8_t x, uint8_t y);
void splash_win(void);
void splash_coins(uint8_t coins);
void splash_dead(void);
void splash_puts(uint8_t x, uint8_t y, const char *msg);

#endif

