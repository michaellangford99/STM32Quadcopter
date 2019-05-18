/*
Name:		MPU9250.cpp
Created:	5/18/2019 11:37 PM
Author:	Michael Langford
*/

#include "MPU9250.h"

#define READ                       0b10111111  
#define WRITE                      0b00111111

#define CS                         10
#define CLOCK_SPEED                1000000
#define CALIBRATE_TIME             1000

//register definitions
#define MPU9250_WHO_AM_I           0x75

#define BEGIN_SPI_MPU9250()		   SPI.beginTransaction(SPISettings(CLOCK_SPEED, MSBFIRST, SPI_MODE0));

#define END_SPI_MPU9250()          SPI.endTransaction();

int readRegister(byte address);
void writeRegister(byte address, byte data);

void init_MPU9250() {
	SPI.begin();

	pinMode(10, OUTPUT);
	digitalWrite(10, HIGH);

	BEGIN_SPI_MPU9250();
	/*
    setup
    */
	END_SPI_MPU9250();
}

int readWHOAMI()
{
    return readRegister(MPU9250_WHO_AM_I) & 0xFF;
}

int readRegister(byte address)
{
	digitalWrite(CS, LOW);
	int toRead;

	address |= 0x80;

	SPI.transfer(address);
	toRead = SPI.transfer(0);
	digitalWrite(CS, HIGH);
	return toRead;
}

void writeRegister(byte address, byte data)
{
	digitalWrite(CS, LOW);
	address &= 0x7F;
	SPI.transfer(address);
	SPI.transfer(data); 
	digitalWrite(CS, HIGH);
}