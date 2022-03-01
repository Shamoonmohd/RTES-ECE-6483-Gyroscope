# RTES-ECE-6483-Gyroscope
Aim:- To build a wearable speedometer which can calculate velocity by measuring angular velocities available from our built-in gyroscope present in STM32f429discovery Board.
<img width="373" alt="image" src="https://user-images.githubusercontent.com/45068287/156235665-7e1f8b01-5b17-4f69-90c5-bdf80bd1d831.png">
The above gyroscope (I3G4250D) present on the Gyroscope was used to measure the X, Y, Z axis on the STM32429board.![image](https://user-images.githubusercontent.com/45068287/156235687-a2a4d68c-55f0-4d9b-8f93-3beb5243e2d5.png)

<img width="452" alt="image" src="https://user-images.githubusercontent.com/45068287/156235705-6f5013cf-38e5-4e09-b33e-aca067021480.png">

## Method :- 

a) We attach the board on a Person’s body below the knee.

b) The Gyroscope on the board is used to measure the values of X axis .

c) The Readings of the Gyroscope is measured at the rate of 100 hz(approx.), these readings are read using SPI communication between gyroscope and the board.

d) To measure the values, we first read the values on the SPI to get the lower byte and the upper byte of the z axis value of the Gyroscope. Then we combine them to get the entire reading

e) We then multiply it with sensitivity (Sensitivity describes the gain of the sensor and can be determined by applying a defined angular velocity to it. This value changes very little over temperature and time.) get the values of Angular change in Radians per second.

f) If X values being measured is less than noise_rate 0.6 , we take those values as zero as these values are due to the noise readings from the gyroscope, when the board or the gyroscope is not in motion.

g) We then multiply the readings with 0.0174533 which is (π/180) to convert Radians into degrees.

## Measurement:-

a) For accurate measurement , for every 0.5 seconds we take 30 such values and put them in a circular queue , and take the average of all the readings for that duration of 0.5 second.

b) Then we take measurement of across 20 seconds (40 measurements of across the entire duration (for every 0.5 second)). And find the average velocity

c) As we know the duration and the average velocity, we calculate the distance travelled in the 20 second period.

d) we multiply these values with the height of the person from his hip to the toe ( we use this measurement as arc length to multiply with the X values got above ( degrees) to get the distance travelled.

e) We export these outputs through the USB and print it on the Terminal.

## Outputs for Different kinds of motion:-


![image](https://user-images.githubusercontent.com/45068287/156236066-0251607a-70ef-4c3a-b770-9f1874467da5.png)

## We conducted the demonstration on a set distance of the Football field D Area ( 16.63 meters) 

From Point A to Point B

<img width="301" alt="image" src="https://user-images.githubusercontent.com/45068287/156237021-a799c05f-337c-426f-9a4e-6fdc86718394.png"><img width="452" alt="image" src="https://user-images.githubusercontent.com/45068287/156237047-a47ce0a2-7bd3-479f-b0c0-e6443feda89f.png">



