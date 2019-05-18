// 
// 
// 

#include "MMA7455L.h"
/*
float x_acc_offset = 0.0f, y_acc_offset = 0.0f, z_acc_offset = 0.0f;
float acc_pitch_offset = 0.0f;
float acc_roll_offset = 0.0f;

float X_raw_out, Y_raw_out, Z_raw_out;
float acc_roll_angle, acc_pitch_angle;

void init_MMA7455L()
{
	Wire.begin();

	Wire.setClock(40000);
	
	Wire.beginTransmission(MMA_7455_ADDRESS);
	Wire.write(MMA_7455_MODE_CONTROLL);
	Wire.write(MMA_7455_2G_MODE);  //Set Sensitivity to 2g Detection
	Wire.endTransmission();

	delay(100);

	Calibrate_Accelerometer();
}

void Calibrate_Accelerometer()
{
	float cx, cy, cz;
	cx = cy = cz = 0.0f;

	float croll = 0.0f, cpitch = 0.0f;

	x_acc_offset = y_acc_offset = z_acc_offset = 0.0f;

	for (int i = 0; i < MMA_7455L_CALIBRATE_TIME; i++)
	{
		delay(1);
		X_raw_out = GetAxis(X_OUT);
		Y_raw_out = GetAxis(Y_OUT);
		Z_raw_out = GetAxis(Z_OUT);

		cx += X_raw_out;
		cy += Y_raw_out;
		cz += Z_raw_out + 63.0f;
	}

	x_acc_offset = cx / MMA_7455L_CALIBRATE_TIME;
	y_acc_offset = cy / MMA_7455L_CALIBRATE_TIME;
	z_acc_offset = cz / MMA_7455L_CALIBRATE_TIME;

	for (int i = 0; i < MMA_7455L_CALIBRATE_TIME; i++)
	{
		delay(1);
		X_raw_out = GetAxis(X_OUT)-x_acc_offset;
		Y_raw_out = GetAxis(Y_OUT)-y_acc_offset;
		Z_raw_out = GetAxis(Z_OUT)-z_acc_offset;

		cpitch += degrees(atan2f(-X_raw_out, -Z_raw_out));
		croll += degrees(atan2f(-Y_raw_out, -Z_raw_out));
	}

	acc_pitch_offset = cpitch / MMA_7455L_CALIBRATE_TIME;
	acc_roll_offset = croll / MMA_7455L_CALIBRATE_TIME;
}

int acc_update = 0;

void update_MMA7455L()
{
	acc_update++;

	if (acc_update < 10)
		return;

	acc_update = 0;
		
	//
	// get raw acceleration values
	//

	//Get X axis out
	Wire.beginTransmission(MMA_7455_ADDRESS);
	Wire.write(X_OUT);
	Wire.endTransmission();
	Wire.beginTransmission(MMA_7455_ADDRESS);
	Wire.requestFrom(MMA_7455_ADDRESS, 1);
	while (Wire.available()==false)
	{
	}
	X_raw_out = (float)((int8_t)Wire.read()) - x_acc_offset;
	Wire.endTransmission();

	//Get Y axis out
	Wire.beginTransmission(MMA_7455_ADDRESS);
	Wire.write(Y_OUT);
	Wire.endTransmission();
	Wire.beginTransmission(MMA_7455_ADDRESS);
	Wire.requestFrom(MMA_7455_ADDRESS, 1);
	while (Wire.available() == false)
	{
	}
	Y_raw_out = (float)((int8_t)Wire.read()) - y_acc_offset;
	Wire.endTransmission();

	//Get Z axis out
	Wire.beginTransmission(MMA_7455_ADDRESS);
	Wire.write(Z_OUT);
	Wire.endTransmission();
	Wire.beginTransmission(MMA_7455_ADDRESS);
	Wire.requestFrom(MMA_7455_ADDRESS, 1);
	while (Wire.available() == false)
	{
	}
	Z_raw_out = (float)((int8_t)Wire.read()) - z_acc_offset;
	Wire.endTransmission();

	//
	// generate angles
	//

	acc_pitch_angle += (degrees(atan2f(-X_raw_out, -Z_raw_out)) - acc_pitch_angle) * 0.2f;
	acc_roll_angle += (degrees(atan2f(-Y_raw_out, -Z_raw_out)) - acc_roll_angle) * 0.2f;
}

float GetAxis(int reg)
{
	Wire.beginTransmission(MMA_7455_ADDRESS);
	Wire.write(reg);

	Wire.endTransmission();
	Wire.beginTransmission(MMA_7455_ADDRESS);
	Wire.requestFrom(MMA_7455_ADDRESS, 1);
	while (Wire.available() == false)
	{
	}
	return (float)((int8_t)Wire.read());
}

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
	return -(acc_pitch_angle - acc_pitch_offset);
}

float Get_Acc_Roll()
{
	return (acc_roll_angle - acc_roll_offset);
}
*/