#ifndef USART_DRIVER_H
#define USART_DRIVER_H

#include <Arduino.h>
#include <avr/io.h>
#include <stdint.h>

void USART_init();
void USART_send(uint8_t data);
uint8_t USART_receive();
void USART_sendString(const char *str);
void USART_send2Digits(uint8_t value);

#endif
