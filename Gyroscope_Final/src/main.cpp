#include "mbed.h"
#include "driver.h"
using namespace std;
#include <iostream>

SPI spi(mosi, miso, sclk); // mosi, miso, sclk
DigitalOut cs(CS);         //chip select (CS)

int main()
{
  int Address;
  int write_bit;
  cs = 1;                 // Chip must be deselected
                          // Setup the spi for 8 bit data, high steady state clock,
                          // second edge capture, with a 1MHz clock rate
  spi.format(8, 3);       // 8 bit data format and 3 is spi mode
  spi.frequency(1000000); // 1MHz
  cs = 0;                 // here we want to start communicating with the chip;
  int who = Get_whoAmI(&spi);
  printf("who_am_i = 0x%X\n", who);
  cs = 1;                 // Chip must be deselected

  cs = 0;
  Address = write_spi | ctrl_reg_1;
  write_bit = ENABLE_GYRO_REG;
  write(&spi, Address, write_bit); //enable the gyroscope in normal mode
  cs = 1;

  cs = 0;
  Address = write_spi | ctrl_reg_2;
  write_bit = DUMMY;
  write(&spi, Address, write_bit);
  cs = 1;

  cs = 0;
  Address = write_spi | ctrl_reg_3;
  write_bit = DUMMY;
  write(&spi, Address, write_bit);
  cs = 1;

  cs = 0;
  Address = write_spi | ctrl_reg_4;
  write_bit = DUMMY;
  write(&spi, Address, write_bit);
  cs = 1;

  cs = 0;
  Address = write_spi | ctrl_reg_5;
  write_bit = DUMMY;
  write(&spi, Address, write_bit);
  cs = 1;

  while (1)
  {

    wait_us(100000);                                                             // wait for 1 minute to take new sample

    //-----------------------------OutPut register------------------------------------------------------------------//

    cs = 0;          // here we want to start communicating with the chip;
                     // read opertion for the x-axs
    spi.write(0xA8); // read opertaion 0x80 | 0x28
    short xAxisLowBit = spi.write(0x00);
    cs = 1;
    cs = 0;
    spi.write(0xA9);
    short xAxisHighBit = spi.write(0x00);
    cs = 1;
    short xAxis = xAxisHighBit << 8 | xAxisLowBit;

    // read opertion for the y-axs
    cs = 0;
    spi.write(0xAA); // read opertaion 0x80 | 0x28
    short yAxisLowBit = spi.write(0x00);
    cs = 1;
    cs = 0;
    spi.write(0xAB);
    short yAxisHighBit = spi.write(0x00);
    cs = 1;
    short yAxis = yAxisHighBit << 8 | yAxisLowBit;

    // read opertion for the z-axs

    cs = 0;
    spi.write(0xAC); // read opertaion 0x80 | 0x28
    short zAxisLowBit = spi.write(0x00);
    cs = 1;
    cs = 0;
    spi.write(0xAD);
    short zAxisHighBit = spi.write(0x00);
    cs = 1;
    short zAxis = zAxisHighBit << 8 | zAxisLowBit;
    
    int X = xAxis * 0.00875f;
    int Y = yAxis * 0.00875f;
    int Z = zAxis * 0.00875f;

    int average_velocity = (X * 0.017453292519943);                    //converting milli degree into radian;
    printf("average_velocity:%d\n",average_velocity);
    int distance  = (radius_Arc_length) * average_velocity;                 //distance travel in one minute in ft.
    printf("Distance(inches):%d\n",distance);
    distance = (distance/12.0);  //convert inches into ft;
    printf("Distance(ft):%d\n",distance);
    int steps = distance * 0.4; // convert ft to steps;
    printf("steps:%d\n",steps);
    

    // printf("X axis:%d  Y axis:%d   Z axis:%d\n", X, Y, Z);
    // if(distance!=0.0 && distance>0.0)
    // {
    //   printf("Distance(ft):%d\n",distance);
    //   printf("Steps:%d\n",(distance * 0.4));
      
    // }
    
  }
}