# Clock_System-Nucleo

Welcome to our advanced timekeeping system, engineered using the Nucleo-F446RE and Panda board, powered by the STM32F103C8T6 microcontroller. This cutting-edge project seamlessly connects your Nucleo board to your PC through UART, effectively converting your PC's serial monitor into a login gateway. Our clock system provides alot of functionalities, including displaying the current time and date, configuring date and time settings, and setting up alarms.

## Intro

Our flagship Project, represents an innovative clock system that harmoniously integrates a multitude of components to provide an engaging and highly functional user experience. 
Key highlights of this project :

- Robust alarm management, capable of accommodating multiple alarms.
- SPI communication with the Panda board, enabling control of LEDs and information display on 
  an LCD screen.
- Buzzer integration for effective and timely alarm notifications.
- Real-time clock (RTC) functionality powered by the DS1307 RTC module via I2C communication.
- An intuitive and user-friendly login system accessible through the PC's serial monitor.

## Hardware Components

- Nucleo-F446RE board ( based on STM32F446RE MCU )
- DS1307 RTC module
- Panda board (based on STM32F103C8T6 MCU )
- LCD display
- Buzzer
- LEDs

## System Configuration

1. Establish a UART connection between the Nucleo-F446RE board and your PC, enabling seamless 
   serial communication.
2. Connect the DS1307 RTC module to the Nucleo board through I2C communication.
3. Enable SPI communication between the Nucleo board and the Panda board.
4. Attach the LCD screen to the Panda board.
5. Install the buzzer on the Panda board to receive alarm notifications.
6. Utilize green and red LEDs on the Panda board to indicate login status.

## Usage Guide

1. Initiate the serial monitor on your PC to establish a direct connection with the Nucleo board.
2. Provide your unique username and password when prompted for secure authentication.
3. A successful login will be indicated by the green LED on the Panda board, while a red LED will signal a login failure.
4. After logging in, explore various options, including displaying the current time and date, configuring date and time preferences, and setting alarms with personalized names and numbers.
5. The clock system will elegantly showcase the current time and date on the LCD screen if you choose the option of display.
6. When the time arrives for an alarm, the buzzer will sound, accompanied by the display of the alarm name & alarm number on the LCD screen.

## Contributing

Feel Free to Fork and Submit a Pull Request if you find any issues or Bugs , Or Even if you have improvements . Make sure you Provide Full Descriptions about changes you have done.  
