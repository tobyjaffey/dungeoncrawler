#ifndef COINMAP_H
#define COINMAP_H 1

void coinmap_init(void);
uint8_t coinmap_remaining(void);
bool coinmap_isTaken(uint16_t coinx, uint16_t coiny);
void coinmap_take(uint16_t coinx, uint16_t coiny);

#endif
