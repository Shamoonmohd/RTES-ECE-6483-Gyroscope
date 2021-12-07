#include <mbed.h>
# define read_spi 0x80              //enable spi_read
# define write_spi 0x00             // enable spi_write
# define mosi PF_9                  //mosi
# define miso PF_8                  //miso
# define sclk PF_7                  //sclk
# define CS PC_1                    //CS
# define who_am_i_reg 0x0f          // address of who_am_i register
# define ctrl_reg_1 0x20
# define ctrl_reg_2 0x21
# define ctrl_reg_3 0x22
# define ctrl_reg_4 0x23
# define ctrl_reg_5 0x24
# define ENABLE_GYRO_REG 0x0F
# define DUMMY 0x00
# define radius_Arc_length 20


int Get_whoAmI(SPI *spi);
void write(SPI *spi, int address, int write_bit);
