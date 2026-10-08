#include "SPI_Driver.h"

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

uint8_t SPI_SendReceiveData(uint8_t data)
{
  SPDR = data;

  while (!(SPSR & (1 << SPIF)))
  {
  }

  return SPDR;
}
