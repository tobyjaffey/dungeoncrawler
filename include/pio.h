#ifndef PIO_H
#define PIO_H 1

#ifdef PLATFORM_IMME
#define A0 P0_2
#define SSN P0_4
#define LCDRst P1_1
#endif

void pio_init(void);

#endif

