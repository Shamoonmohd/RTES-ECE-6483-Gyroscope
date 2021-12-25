#include <mbed.h>
# define read_spi 0x80              //enable spi_read
# define write_spi 0x00             // enable spi_write
# define mosi PF_9                  //mosi
# define miso PF_8                  //miso
# define sclk PF_7                  //sclk
# define CS PC_1                    //CS
# define who_am_i_reg 0x0f          // address of who_am_i register
# define ctrl_reg_1 0x20            // address of ctrl_reg_1 register
# define ctrl_reg_2 0x21            // address of ctrl_reg_2 register
# define ctrl_reg_3 0x22            // address of ctrl_reg_3 register
# define ctrl_reg_4 0x23            // address of ctrl_reg_4 register
# define ctrl_reg_5 0x24            // address of ctrl_reg_5 register
# define ENABLE_GYRO_REG 0x0F       //enable gyroscope
# define DUMMY 0x00                 //Dummy variables
# define radius_Arc_length 0.8      //Assuming person height and diving by 2
# define button_reset PA_0          //reset 
# define Nois_rate 0.6            //threshold





int Get_whoAmI(SPI *spi);                               //Get_whoAmI function(check gyroscope is communincating)
void write(SPI *spi, int address, int write_bit);       //write function
void init_gyro(SPI *spi, DigitalOut *cs);               ////init gyroscope
float noise_filter_averageVelocity(float x);            //noise_filter_average Velocity
void queue_init();                                      //initialize circular queue implement for taking average of angular velocity 
void push(float gyro_X);                           //push function for taking average values of gyro X
