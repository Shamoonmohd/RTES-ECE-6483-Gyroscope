#include "driver.h"
#include "mbed.h"





int Get_whoAmI(SPI *spi)
{  

    int address = (read_spi | who_am_i_reg);
    spi->write(address);                        // Send 0x8f, the command to read the WHOAMI register  
    int who = spi->write(0x00);        // Send a dummy byte to receive the contents of the WHOAMI register     
    return who;

}

void write(SPI *spi, int address, int write_bit)
{
    spi->write(address);
    spi->write(write_bit);
}

