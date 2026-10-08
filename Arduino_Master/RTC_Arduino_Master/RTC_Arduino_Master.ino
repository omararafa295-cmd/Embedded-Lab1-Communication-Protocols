#include <Wire.h>
#include <SPI.h>

#define RTC_ADDRESS 0x68
#define SS_PIN 10

byte decToBcd(byte val)
{
  return ((val / 10 * 16) + (val % 10));
}

byte bcdToDec(byte val)
{
  return ((val / 16 * 10) + (val % 16));
}

void setRTC()
{
  Wire.beginTransmission(RTC_ADDRESS);

  Wire.write(0);

  Wire.write(decToBcd(0));    // Seconds
  Wire.write(decToBcd(30));   // Minutes
  Wire.write(decToBcd(17));   // Hours

  Wire.write(decToBcd(1));    // Day of week

  Wire.write(decToBcd(7));    // Day
  Wire.write(decToBcd(10));   // Month
  Wire.write(decToBcd(26));   // Year

  Wire.endTransmission();
}

void readRTC(
  byte &day,
  byte &month,
  byte &year,
  byte &hour,
  byte &minute,
  byte &second
)
{
  Wire.beginTransmission(RTC_ADDRESS);

  Wire.write(0);

  Wire.endTransmission();

  Wire.requestFrom(RTC_ADDRESS, 7);

  second =
    bcdToDec(
      Wire.read() & 0x7F
    );

  minute =
    bcdToDec(
      Wire.read()
    );

  hour =
    bcdToDec(
      Wire.read() & 0x3F
    );

  Wire.read();

  day =
    bcdToDec(
      Wire.read()
    );

  month =
    bcdToDec(
      Wire.read()
    );

  year =
    bcdToDec(
      Wire.read()
    );
}

void sendByte(byte data)
{
  SPI.transfer(data);

  delayMicroseconds(150);
}

void sendPacket(
  byte day,
  byte month,
  byte year,
  byte hour,
  byte minute,
  byte second
)
{
  byte checksum =
    day ^
    month ^
    year ^
    hour ^
    minute ^
    second;

  digitalWrite(
    SS_PIN,
    LOW
  );

  delayMicroseconds(100);

  sendByte(0xAA);

  sendByte(day);
  sendByte(month);
  sendByte(year);

  sendByte(hour);
  sendByte(minute);
  sendByte(second);

  sendByte(checksum);

  delayMicroseconds(100);

  digitalWrite(
    SS_PIN,
    HIGH
  );
}

void setup()
{
  Wire.begin();

  pinMode(
    SS_PIN,
    OUTPUT
  );

  digitalWrite(
    SS_PIN,
    HIGH
  );

  SPI.begin();

  SPI.beginTransaction(
    SPISettings(
      125000,
      MSBFIRST,
      SPI_MODE0
    )
  );

  delay(500);

  setRTC();

  delay(500);
}

void loop()
{
  byte day;
  byte month;
  byte year;

  byte hour;
  byte minute;
  byte second;

  readRTC(
    day,
    month,
    year,
    hour,
    minute,
    second
  );

  sendPacket(
    day,
    month,
    year,
    hour,
    minute,
    second
  );

  delay(1000);
}
