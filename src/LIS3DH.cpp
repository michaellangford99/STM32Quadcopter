/*
Name:		LIS3DH.ino
Created:	3/2/2018 4:50 PM
Author:	Michael Langford
*/

#include "LIS3DH.h"

#define STATUS_REG_AUX			   0x07

#define WHO_AM_I				   0x0f

#define CTRL_REG1				   0x20
#define CTRL_REG2				   0x21
#define CTRL_REG3				   0x22
#define CTRL_REG4				   0x23
#define CTRL_REG5				   0x24
#define CTRL_REG6				   0x25

#define STATUS_REG_2               0x27

#define OUT_X_L                    0x28
#define OUT_X_H                    0x29
#define OUT_Y_L                    0x2a
#define OUT_Y_H                    0x2b
#define OUT_Z_L                    0x2c
#define OUT_Z_H                    0x2d

#define READ                       0b10111111  
#define WRITE                      0b00111111

#define LIS3DH_CS_PIN              9

#define CLOCK_SPEED                4000000

#define LIS3DH_CALIBRATE_TIME 50

float x_acc_offset = 0.0f;
float y_acc_offset = 0.0f;
float z_acc_offset = 0.0f;

float acc_pitch_offset = 0.0f;
float acc_roll_offset = 0.0f;

float X_raw_out;
float Y_raw_out;
float Z_raw_out;

float acc_roll_angle;
float acc_pitch_angle;

#define BUFFERSIZE 20
float xdata[BUFFERSIZE];
float ydata[BUFFERSIZE];
float zdata[BUFFERSIZE];

int readAccRegister(byte address);
void writeAccRegister(byte address, byte data);

float Get_raw_X()
{
	return X_raw_out;
}

float Get_raw_Y()
{
	return Y_raw_out;
}

float Get_raw_Z()
{
	return Z_raw_out;
}

float Get_Acc_Pitch()
{
	return acc_pitch_angle;
}

float Get_Acc_Roll()
{
	return acc_roll_angle;
}

bool Calibrating = false;

void update_LIS3DH()
{
	if (Calibrating)
	{
		X_raw_out = ((float)((int16_t)((readAccRegister(OUT_X_H) << 8) | (readAccRegister(OUT_X_L)))) / 100.0f) - x_acc_offset;
		Y_raw_out = ((float)((int16_t)((readAccRegister(OUT_Y_H) << 8) | (readAccRegister(OUT_Y_L)))) / 100.0f) - y_acc_offset;
		Z_raw_out = ((float)((int16_t)((readAccRegister(OUT_Z_H) << 8) | (readAccRegister(OUT_Z_L)))) / 100.0f) - z_acc_offset;
	}
	else
	{
		X_raw_out += ((((float)((int16_t)((readAccRegister(OUT_X_H) << 8) | (readAccRegister(OUT_X_L)))) / 100.0f) - x_acc_offset) - X_raw_out)*0.02f;
		Y_raw_out += ((((float)((int16_t)((readAccRegister(OUT_Y_H) << 8) | (readAccRegister(OUT_Y_L)))) / 100.0f) - y_acc_offset) - Y_raw_out)*0.02f;
		Z_raw_out += ((((float)((int16_t)((readAccRegister(OUT_Z_H) << 8) | (readAccRegister(OUT_Z_L)))) / 100.0f) - z_acc_offset) - Z_raw_out)*0.02f;

		/*for (int i = 0; i < BUFFERSIZE - 1; i++)
		{
			xdata[i] = xdata[i + 1];
			ydata[i] = ydata[i + 1];
			zdata[i] = zdata[i + 1];
		}

		xdata[BUFFERSIZE - 1] = ((float)((int16_t)((readAccRegister(OUT_X_H) << 8) | (readAccRegister(OUT_X_L)))) / 100.0f) - x_acc_offset;
		ydata[BUFFERSIZE - 1] = ((float)((int16_t)((readAccRegister(OUT_Y_H) << 8) | (readAccRegister(OUT_Y_L)))) / 100.0f) - x_acc_offset;
		zdata[BUFFERSIZE - 1] = ((float)((int16_t)((readAccRegister(OUT_Z_H) << 8) | (readAccRegister(OUT_Z_L)))) / 100.0f) - x_acc_offset;

		float xa, ya, za;
		xa = ya = za = 0.0f;

		for (int i = 0; i < BUFFERSIZE; i++)
		{
			xa += xdata[i];
			ya += ydata[i];
			za += zdata[i];
		}

		X_raw_out = xa / (float)BUFFERSIZE;
		Y_raw_out = ya / (float)BUFFERSIZE;
		Z_raw_out = za / (float)BUFFERSIZE;*/
	}

	acc_pitch_angle += (degrees(atan2f(Y_raw_out, -Z_raw_out)) - acc_pitch_angle) * 1.0f;
	acc_roll_angle += (degrees(atan2f(X_raw_out, -Z_raw_out)) - acc_roll_angle) * 1.0f;
}

void Calibrate_Accelerometer()
{
	Calibrating = true;

	float cx, cy, cz;
	cx = cy = cz = 0.0f;

	//float croll = 0.0f, cpitch = 0.0f;

	x_acc_offset = y_acc_offset = z_acc_offset = 0.0f;

	for (int i = 0; i < LIS3DH_CALIBRATE_TIME; i++)
	{
		delay(1);
		
		update_LIS3DH();

		cx += X_raw_out;
		cy += Y_raw_out;
		cz += Z_raw_out + 163.84f;
	}

	x_acc_offset = cx / LIS3DH_CALIBRATE_TIME;
	y_acc_offset = cy / LIS3DH_CALIBRATE_TIME;
	z_acc_offset = cz / LIS3DH_CALIBRATE_TIME;

	/*for (int i = 0; i < LIS3DH_CALIBRATE_TIME; i++)
	{
		delay(1);
		update_LIS3DH();

		cpitch += degrees(atan2f(-X_raw_out, -Z_raw_out));
		croll += degrees(atan2f(-Y_raw_out, -Z_raw_out));
	}

	acc_pitch_offset = cpitch / LIS3DH_CALIBRATE_TIME;
	acc_roll_offset = croll / LIS3DH_CALIBRATE_TIME;*/

	Calibrating = false;
}

void init_LIS3DH() {
	SPI.begin();

	pinMode(LIS3DH_CS_PIN, OUTPUT);
	digitalWrite(LIS3DH_CS_PIN, HIGH);

	delay(100);

	//set up initial state, disable pwr down, set data rate
	writeAccRegister(CTRL_REG1, 0b10010111);

	x_acc_offset = 0.0f;
	y_acc_offset = 0.0f;
	z_acc_offset = 0.0f;

	acc_pitch_offset = 0.0f;
	acc_roll_offset = 0.0f;

	X_raw_out = 0.0f;
	Y_raw_out = 0.0f;
	Z_raw_out = 0.0f;

	acc_roll_angle = 0.0f;
	acc_pitch_angle = 0.0f;

	delay(100);

	Calibrate_Accelerometer();
}

int readAccRegister(byte address)
{
	SPI.beginTransaction(SPISettings(CLOCK_SPEED, MSBFIRST, SPI_MODE0));

	int toRead;

	address |= 0x80;

	digitalWrite(LIS3DH_CS_PIN, LOW);
	SPI.transfer(address);
	toRead = SPI.transfer(0);
	digitalWrite(LIS3DH_CS_PIN, HIGH);
	SPI.endTransaction();
	return toRead;
}

void writeAccRegister(byte address, byte data)
{
	SPI.beginTransaction(SPISettings(CLOCK_SPEED, MSBFIRST, SPI_MODE0));

	address &= 0x7F;

	digitalWrite(LIS3DH_CS_PIN, LOW);
	SPI.transfer(address);
	SPI.transfer(data);
	digitalWrite(LIS3DH_CS_PIN, HIGH);

	SPI.endTransaction();
}