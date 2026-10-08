#ifndef SPI_DRIVER_H
#define SPI_DRIVER_H

#include <Arduino.h>
#include <avr/io.h>
#include <stdint.h>

void SPI_Init();
uint8_t SPI_ReceiveData();
uint8_t SPI_SendReceiveData(uint8_t data);

#endif
