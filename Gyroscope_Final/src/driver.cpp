#include "driver.h"
#include "mbed.h"
int zero_count = 0;         //global variables
float average_velocity = 0; 
int front = -1;
int rear = -1;
int queue_count = 0;
int is_queue_full = 0;
int queue[30];

int Get_whoAmI(SPI *spi)  //implemnet to verfiy gysoscope is communicating
{

  int address = (read_spi | who_am_i_reg);
  spi->write(address);        // Send 0x8f, the command to read the WHOAMI register
  int who = spi->write(0x00); // Send a dummy byte to receive the contents of the WHOAMI register

  return who;
}

void write(SPI *spi, int address, int write_bit) //write function
{

  spi->write(address);               
  spi->write(write_bit);
}

void init_gyro(SPI *spi, DigitalOut *cs)  //initializing the gyro
{
  int Address;
  int write_bit;
  cs->write(1);            // Chip must be deselected
                           // Setup the spi for 8 bit data, high steady state clock,
                           // second edge capture, with a 1MHz clock rate
  spi->format(8, 3);       // 8 bit data format and 3 is spi mode
  spi->frequency(1000000); // 1MHz
  cs->write(0);            // here we want to start communicating with the chip;
  int who = Get_whoAmI(spi);
  printf("who_am_i = 0x%X\n", who);
  cs->write(1); // Chip must be deselected

  cs->write(0);
  Address = write_spi | ctrl_reg_1; 
  write_bit = ENABLE_GYRO_REG;
  write(spi, Address, write_bit); //enable the gyroscope in normal mode
  cs->write(1);

  cs->write(0);
  Address = write_spi | ctrl_reg_2;
  write_bit = DUMMY;
  write(spi, Address, write_bit);
  cs->write(1);

  cs->write(0);
  Address = write_spi | ctrl_reg_3;
  write_bit = DUMMY;
  write(spi, Address, write_bit);
  cs->write(1);

  cs->write(0);
  Address = write_spi | ctrl_reg_4;
  write_bit = DUMMY;
  write(spi, Address, write_bit);
  cs->write(1);

  cs->write(0);
  Address = write_spi | ctrl_reg_5;
  write_bit = 0x03;
  write(spi, Address, write_bit);
  cs->write(1);
}


void queue_init()       //initializing the queue 
{
  front = -1;
  rear = -1;
  queue_count = 0;
  is_queue_full = 0;
  
}

void push(float gyro_X)   //Implementing the circular queue for taking average of gyro_x values
{
  if (gyro_X > 0)
  {
    if (queue_count >= 30 && is_queue_full == 0)
    {
      is_queue_full = 1;
      front = 0;
    }
    if (!is_queue_full)
    {
      rear = rear + 1;
      queue[rear] = gyro_X;
      average_velocity = ((average_velocity * queue_count) + gyro_X) / (queue_count + 1);
      queue_count++;

      
    }
    else
    {
      average_velocity = ((average_velocity * queue_count) - queue[front]) / (queue_count - 1);
      rear = front;
      front = (front + 1) % 30;
      queue[rear] = gyro_X;
      average_velocity = ((average_velocity * (queue_count - 1)) + gyro_X) / (queue_count);
      
  }
}
}

float noise_filter_averageVelocity(float gyro_X)   
{
  
  if ((int)gyro_X == 0)            //checking how many values are coming lower than 1;
  {
    zero_count += 1;              //count number of zero for rest case if we are not moving, count number of zero's
    if (zero_count >= 50)         //if zero is more than 50 we are returning the zero as when we are in idle state our average velocity become zero
    {
      return 0;                   //returning average velocity as 0
    }
    return average_velocity;      //if zero count is smaller than 50 we are sening the average velocity;
  }

  zero_count = 0;                 //again initializing zero count =0 if values are greater than 0
  push(gyro_X);                   //pushing X in queue so that we acn take the average
  return average_velocity;        //returning the average velocity.
}
