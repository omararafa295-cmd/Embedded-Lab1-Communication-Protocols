#include <Arduino.h>
#include <avr/io.h>
#include <stdint.h>

#define PACKET_START 0xAA

void USART_init()
{
  UCSRA &= ~(1 << U2X);

  UBRRH = 0;
  UBRRL = 51;

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
  USART_send(
    '0' + (value / 10)
  );

  USART_send(
    '0' + (value % 10)
  );
}

void SPI_Init()
{
  DDRB &= ~(
    (1 << PB4) |
    (1 << PB5) |
    (1 << PB7)
  );

  DDRB |= (1 << PB6);

  SPCR = (1 << SPE);
}

uint8_t SPI_ReceiveData()
{
  while (!(SPSR & (1 << SPIF)))
  {
  }

  return SPDR;
}

void printDateTime(
  uint8_t day,
  uint8_t month,
  uint8_t year,
  uint8_t hour,
  uint8_t minute,
  uint8_t second
)
{
  USART_sendString("Date: ");

  USART_send2Digits(day);
  USART_send('/');

  USART_send2Digits(month);
  USART_send('/');

  USART_sendString("20");

  USART_send2Digits(year);

  USART_sendString("   Time: ");

  USART_send2Digits(hour);
  USART_send(':');

  USART_send2Digits(minute);
  USART_send(':');

  USART_send2Digits(second);

  USART_sendString("\r\n");
}

void setup()
{
  USART_init();

  SPI_Init();

  delay(500);

  USART_sendString("\r\n");
  USART_sendString("============================\r\n");
  USART_sendString("RTC Communication System\r\n");
  USART_sendString("ATmega32 SPI Slave Ready\r\n");
  USART_sendString("============================\r\n\r\n");
}

void loop()
{
  uint8_t startByte;

  uint8_t day;
  uint8_t month;
  uint8_t year;

  uint8_t hour;
  uint8_t minute;
  uint8_t second;

  uint8_t receivedChecksum;
  uint8_t calculatedChecksum;

  startByte = SPI_ReceiveData();

  if (startByte != PACKET_START)
  {
    return;
  }

  day = SPI_ReceiveData();

  month = SPI_ReceiveData();

  year = SPI_ReceiveData();

  hour = SPI_ReceiveData();

  minute = SPI_ReceiveData();

  second = SPI_ReceiveData();

  receivedChecksum =
    SPI_ReceiveData();

  calculatedChecksum =
    day ^
    month ^
    year ^
    hour ^
    minute ^
    second;

  if (
    receivedChecksum ==
    calculatedChecksum
  )
  {
    printDateTime(
      day,
      month,
      year,
      hour,
      minute,
      second
    );
  }
  else
  {
    USART_sendString(
      "Packet Error\r\n"
    );
  }
}
