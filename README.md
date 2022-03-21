# C Skate Teacher

With this project I wanted to design a movement tracking device that could help skateboarders learn new tricks. The aim is for the device to be attached to a skateboard, and wirelessly provide feedback on trick attempts to the user's phone or PC. For example feedback may include telling the user to apply more/less force to an area of the board. 
Currently, the device and a PC application have been developed to visualise the movement of the skateboard. This application uses the Processing IDE (based on Java). The device itself is made up of a SAMD21 microcontroller on a QtPy breakout board, programmed in C. A BNO055 IMU is used for tracking, and data is sent to the application via a serial connection. 

This project was originally a university project written in Arduino. It is being ported to C and improved. For the original Arduino version, check [here](https://github.com/samueloneill/skateteacher).

### Current State
- Reads orientation data from IMU and feeds back to PC via serial
- PC application displays real-time movement of the board
- Integrated LED control depending on IMU sensor calibration and current state-of-charge

### Todo
- Machine learning based trick feedback
- Accurate state-of-charge estimation using voltage translation and Coulomb counting
- Wireless connectivity using IoT and/or Bluetooth
