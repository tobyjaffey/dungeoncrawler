#include "common.h"
#include "pio.h"

void pio_init(void)
{
    //No need to set PERCFG or P2DIR as default values on reset are fine
    P0SEL |= (BIT5 | BIT3 ); // set SCK and MOSI as peripheral outputs
    P0DIR |= BIT4 | BIT2; // set SSN and A0 as outputs
    P1DIR |= BIT1; // set LCDRst as output
    P2DIR = BIT3 | BIT4; // set LEDs  as outputs
    //LED_GREEN = LOW; // Turn the Green LED on (LEDs driven by reverse logic: 0 is ON)
}
