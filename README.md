# Lab 1 - Communication Protocols

## MCUs
- Arduino UNO
- ATmega32

## Communication Protocols
- I2C
- SPI
- UART

## Project Description
The Arduino UNO reads date and time from a DS1307 RTC using I2C.

The Arduino sends the date and time to the ATmega32 using SPI.

The ATmega32 receives the data and displays it on a Virtual Terminal using UART.

## System Architecture
DS1307 RTC -> I2C -> Arduino UNO -> SPI -> ATmega32 -> UART -> Virtual Terminal