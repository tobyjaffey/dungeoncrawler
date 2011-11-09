#include "common.h"
#include "lcd.h"
#include "spi.h"
#include "clk.h"
#include "pio.h"

void lcd_txData(uint8_t ch)
{
    A0 = 1;
    spi_tx(ch);
}

static void txCtl(uint8_t ch)
{
    A0 = 0;
    spi_tx(ch);
}

void lcd_setPos(uint8_t col, uint8_t row)
{
    txCtl(0xb0 + row); // set cursor row
    txCtl(0x00 + (col & 0x0f)); // set cursor col low
    txCtl(0x10 + ( (col>>4) & 0x0f)); // set cursor col high
}

void lcd_setAddr(uint8_t start)
{
    txCtl(0x40 | (start & 0x3f)); // set Display start address
}

void lcd_setNormalReverse(uint8_t normal)
{  // 0 = Normal, 1 = Reverse
    txCtl(0xa6 | (normal & 0x01) );
}

void lcd_cls(uint8_t ch)
{
    uint8_t row, i;
    for (row=0;row<9;row++)
    {
        lcd_setPos(0, row);
        for (i=0;i<132;i++)
            lcd_txData(ch);
    }
}

void lcd_init(void)
{
    LCDRst = 0; // hold down the RESET line to reset the display
    clk_delayms(1);
    LCDRst = 1;

    SSN = 0;
    // send the initialisation commands to the LCD display
    txCtl(0xe2); // RESET cmd
    txCtl(0x24); // set internal resistor ratio
    txCtl(0x81); // set Vol Control
    txCtl(0x60); // set Vol Control - ctd
    txCtl(0xe6); // ?? -- don't know what this command is
    txCtl(0x00); // ?? -- don't know what this command is
    txCtl(0x2f); // set internal PSU operating mode
    txCtl(0xa1); // LCD bias set
    txCtl(0xaf); // Display ON
    txCtl(0xa4); // Normal (not all pixels) mode.
    lcd_setNormalReverse(0); //Non-inverted screen.
    SSN = 1;

    lcd_setAddr(0);
    SSN = 0;
    lcd_cls(0xFF);
    SSN = 1;
}

void lcd_cs(uint8_t on)
{
    SSN = !on;
}



