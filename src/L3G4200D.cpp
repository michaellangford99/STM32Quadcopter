/*
Name:		L3G4200D.cpp
Created:	5/25/2017 12:53 PM
Author:	Michael Langford
*/

#include "L3G4200D.h"

#ifdef L3G4200D_IN_USE

#define CNTRL_REG_4                0x23
#define DPS500_4WIRESPI_CONTUPDT   0x10

#define CNTRL_REG_2                0x21
#define HP_FILTER_MODE30           0x20

#define CNTRL_REG_3                0x22
#define DATA_READY                 0x08

#define CNTRL_REG_1                0x20
#define SPS400_25CUTTOFF           0xff

#define READ                       0b10111111  
#define WRITE                      0b00111111

#define GYRO_CS                    10

#define BEGIN_SPI_GYRO()		   SPI.beginTransaction(SPISettings(CLOCK_SPEED, MSBFIRST, SPI_MODE0));

#define END_SPI_GYRO()             SPI.endTransaction();

float cal_x = 0.0f;
float cal_y = 0.0f;
float cal_z = 0.0f;

float x = 0;
float y = 0;
float z = 0;

float xr = 0.0f, yr = 0.0f, zr = 0.0f;

int readRegister_L3G4200D(byte address);
void writeRegister_L3G4200D(byte address, byte data);

float microseconds, lms;
float seconds;
float pre_update_seconds;

float GetYaw()
{
	return z;
}

float GetPitch()
{
	return x;
}

float GetRoll()
{
	return y;
}

float GetYawRate()
{
	return zr;
}

float GetPitchRate()
{
	return xr;
}

float GetRollRate()
{
	return yr;
}

float GetGyroElapsedTime()
{
	return pre_update_seconds;
}

void ClearAngles(float yaw, float pitch, float roll)
{
	x = pitch;
	y = roll;
	z = yaw;

	seconds = 0.0f;
	lms = micros();
}

void SetGyroAngles(float yaw, float pitch, float roll)
{
	x = pitch;
	y = roll;
	z = yaw;
}

float get_cal_x()
{
	return cal_x;
}

float get_cal_y()
{
	return cal_y;
}

float get_cal_z()
{
	return cal_z;
}

void init_L3G4200D() {
	SPI.begin();

	pinMode(10, OUTPUT);
	digitalWrite(10, HIGH);

	BEGIN_SPI_GYRO();
	writeRegister_L3G4200D(CNTRL_REG_1, SPS400_25CUTTOFF);
	writeRegister_L3G4200D(CNTRL_REG_4, DPS500_4WIRESPI_CONTUPDT);
	writeRegister_L3G4200D(CNTRL_REG_2, HP_FILTER_MODE30);
	writeRegister_L3G4200D(CNTRL_REG_3, DATA_READY);
	END_SPI_GYRO();

	Calibrate_Gyro();
}

void Calibrate_Gyro()
{
	BEGIN_SPI_GYRO();
	delay(100);
	float cx = 0, cy = 0, cz = 0;
	for (int i = 0; i < CALIBRATE_TIME; i++)
	{
		while (true)
		{
			if ((readRegister_L3G4200D(0x27) & 8) == 8)
				break;
		}
		cx += (float)getX();
		cy += (float)getY();
		cz += (float)getZ();	
	}
	cal_x = cx / (float)CALIBRATE_TIME;
	cal_y = cy / (float)CALIBRATE_TIME;
	cal_z = cz / (float)CALIBRATE_TIME;

	x = y = z = 0.0f;
	seconds = 0.0f;
	lms = micros();
	END_SPI_GYRO();
}

void update_L3G4200D() {
	BEGIN_SPI_GYRO();
	xr = ((float)getX() - cal_x) * 0.0175f;
	yr = ((float)getY() - cal_y) * 0.0175f;
	zr = ((float)getZ() - cal_z) * 0.0175f;

	x += xr*seconds;
	y += yr*seconds;
	z += zr*seconds;

	//iffy code :)
	//x += y*sinf(radians(zr*seconds));
	//y -= x*sinf(radians(zr*seconds));

	pre_update_seconds = seconds;
	microseconds = (float)micros() - lms;
	lms = micros();
	seconds = microseconds / 1000000.0f;
	if (seconds < 0)
		seconds = -seconds;
	END_SPI_GYRO();
}

int16_t getX()
{
	int16_t xval = (int16_t)(((readRegister_L3G4200D(0x29) & 0xFF) << 8) | (readRegister_L3G4200D(0x28) & 0xFF));
	return xval;
}

int16_t getY()
{
	return (int16_t)(((readRegister_L3G4200D(0x2B) & 0xFF) << 8) | (readRegister_L3G4200D(0x2A) & 0xFF));
}

int16_t getZ()
{
	return (int16_t)(((readRegister_L3G4200D(0x2D) & 0xFF) << 8) | (readRegister_L3G4200D(0x2C) & 0xFF));
}

int readRegister_L3G4200D(byte address)
{
	digitalWrite(GYRO_CS, LOW);
	int toRead;

	address |= 0x80;

	SPI.transfer(address);
	toRead = SPI.transfer(0);
	digitalWrite(GYRO_CS, HIGH);
	return toRead;
}

void writeRegister_L3G4200D(byte address, byte data)
{
	digitalWrite(GYRO_CS, LOW);
	address &= 0x7F;
	SPI.transfer(address);
	SPI.transfer(data); 
	digitalWrite(GYRO_CS, HIGH);
}

#endif