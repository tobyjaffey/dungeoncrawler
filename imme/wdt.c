#include "common.h"
#include "wdt.h"

void wdt_init(void)
{
    //Disable WDT
    IEN2 &= ~IEN2_WDTIE;
    IEN0 &= ~EA;
}

