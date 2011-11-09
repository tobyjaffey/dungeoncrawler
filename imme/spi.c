#include "common.h"
#include "spi.h"

void spi_init(void)
{
    U0CSR = 0;  //Set SPI Master operation
    U0BAUD =  SPI_BAUD_M; // set Mantissa
    U0GCR = U0GCR_ORDER | SPI_BAUD_E; // set clock on 1st edge, -ve clock polarity, MSB first, and exponent
}

void spi_tx(uint8_t ch)
{
    U0DBUF = ch;
    while(!(U0CSR & U0CSR_TX_BYTE)); // wait for byte to be transmitted
    U0CSR &= ~U0CSR_TX_BYTE;         // Clear transmit byte status
}

