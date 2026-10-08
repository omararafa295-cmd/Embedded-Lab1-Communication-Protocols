#include "USART_Driver.h"

void USART_init()
{
  UCSRA &= ~(1 << U2X);

  // 9600 baud @ 8 MHz
  UBRRH = 0;
  UBRRL = 51;

  // Enable receiver and transmitter
  UCSRB =
    (1 << RXEN) |
    (1 << TXEN);
  UCSRC =
    (1 << URSEL) |
    (1 << UCSZ1) |
    (1 << UCSZ0);
}

void USART_send(uint8_t data)
{
  while (!(UCSRA & (1 << UDRE)))
  {
  }

  UDR = data;
}

uint8_t USART_receive()
{
  while (!(UCSRA & (1 << RXC)))
  {
  }

  return UDR;
}

void USART_sendString(const char *str)
{
  while (*str != '\0')
  {
    USART_send(*str);
    str++;
  }
}

void USART_send2Digits(uint8_t value)
{
  USART_send('0' + (value / 10));
  USART_send('0' + (value % 10));
}
