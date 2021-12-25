/*
* Project:    Embedded Challenge(Need for Speed)
* Subject:    ECE-GY 6483 RTES
* File: main.cpp
* Author(s):  Mohammad Shamoon(ms12736), Dhruva Hunusekatte(dh3357)
*/
#include "mbed.h"
#include "driver.h"
#include <iostream>
using namespace std;

SPI spi(mosi, miso, sclk);                                    // mosi, miso, sclk
DigitalOut cs(CS);                                            //chip select (CS)
InterruptIn button(button_reset);                             //Interrupt button for resstting the gyroscope
DigitalOut led(LED1);
DigitalOut flash(LED_RED);
Ticker tick;                                                  //Ticker object
static float distance_travel = 0;                           
bool flag_clock = false;                                      //flag Clock for printing values at 1 seconds
static int clock_count = 0;
float average_distance = 0;
static float total_distance = 0;
int temp_count = 0;
float av_x;
float linear_velocity;

void clock_timer()                                              //Clock timer work for every 0.5 seconds and calulate the total distance
{
  if (temp_count % 2 == 0)
  {
    clock_count += 1;
    average_distance = linear_velocity * 1;
    total_distance = total_distance + average_distance;
    flag_clock = true;
  }
  temp_count += 1;
}

void reset_gyro()                                           // reset gyroscope with reset button, initailizing all veriable to initial state;
{
  init_gyro(&spi, &cs);
  distance_travel = 0;
  total_distance = 0;
  av_x = 0;
  clock_count = 0;
}

int main()
{
  
 
  queue_init();                                               //initializing the queue maintained for  moving average of 30 values of angular velocity X   
  init_gyro(&spi, &cs);                                       //inititlizing the gyroscope
  button.rise(&reset_gyro);                                   // attach the address of the flip function to the rising edge
  tick.attach(&clock_timer, 0.5);                             //sampling for 0.5 seconds

  while (1)
  {

    flash = !flash;

    wait_us(100000);                                            // wait

    //-----------------------------OutPut register------------------------------------------------------------------//
    //read operation for the X axis
    cs = 0;                                                   // here we want to start communicating with the chip;
                                                              // read opertion for the x-axs
    spi.write(0xA8);                                          // read opertaion 0x80 | 0x28
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

    float X = xAxis * 0.00875f;   //sensitivity value for X values
    float Y = yAxis * 0.00875f;   //sensitivity value for X values
    float Z = zAxis * 0.00875f;   //sensitivity value for X values
   
    if (X <= Nois_rate)                         //nullifying the X values which are smaller than 0.6
    {
      X = 0;                              //inittializing those values zero
    }
 
    X = X * 0.0174533;                              //converting into radian meter per second
    av_x = noise_filter_averageVelocity(X);         //passing gyro values to calculate average angular velocity X
    linear_velocity = radius_Arc_length * av_x;     //calculating the linear velocity 


    if (flag_clock)
    {
      printf("================================Gyroscope___Output==================================\n");
      printf("Time elapsed:%ds\n", clock_count);
      printf("Speed(m/s):%0.4f\n", linear_velocity);
      printf("Total Distance Travelled:%0.4f\n", total_distance);

      flag_clock = false;
    }

    
  }
}