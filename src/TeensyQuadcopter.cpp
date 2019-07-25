/*
 Name:		TeensyQuadcopter.cpp
 Created:	5/3/2017 9:52:41 PM
 Author:	Michael Langford
*/

/*

prop1       prop2
|           |
[][] front [][]
[][]   |   [][]
    \  |  /
    {[[[]]}-----microcontroller
    {[[[]]}
    /     \
[][]       [][]
[][]       [][]
|          |
prop3      prop4

*/

#include "WProgram.h"
#include "MPU9250.h"
#include "RadioReciever.h"
#include "PID.h"
#include "Motors.h"
#include "Debug.h"
#include "wiring.h"

#include "unit_tests.h"
#ifdef TEENSYQUADCOPTER

#define THROTTLE_THRESHOLD 1.5f

#define THROTTLE_UNLOCK   -90.0f
#define YAW_UNLOCK        90.0f
#define PITCH_UNLOCK      -90.0f
#define ROLL_UNLOCK       -90.0f
#define GEAR_SET          90.0f

#define SAFE              0
#define ARMED             1

//PID loops
PIDClass YawPID;
PIDClass PitchPID;
PIDClass RollPID;

//PID outputs
float yaw_out, pitch_out, roll_out, throttle_out;

//motor outputs
float motor_output[4];

//debug values
int loop_time = 0;
int l_loop_time = 0;
int count = 0;

//SAFEing value
int safety = SAFE;

/*//Kalman filters
Kalman pitch_kalman;
Kalman roll_kalman;*/

//main angles
float yaw_angle = 0.0f;
float main_pitch = 0.0f;
float main_roll = 0.0f;

void arm();

// the setup function runs once when you press reset or power the board
void setup() {
	Serial.begin(9600);
	Serial.clear();

	//init debug helper
	init_Debug();

	init_MPU9250();
	init_RadioReciever();
	init_motors();

	//init pid loops

	YawPID.init_PID();
	PitchPID.init_PID();
	RollPID.init_PID();

	YawPID.Set_Constants  (0.00f, 0.0f, 1200.0f);
	PitchPID.Set_Constants(0.59f, 0.0f, 1200.0f);
	RollPID.Set_Constants (0.59f, 0.0f, 1200.0f);

	/*//init kalman filters

	pitch_kalman.init_Kalman();
	roll_kalman.init_Kalman();*/

	//
	//calibrate and load initial values
	//

	ClearAngles(0.0f, 0.0f, 0.0f);

	yaw_angle = 0.0f;
	main_pitch = 0.0f;
	main_roll = 0.0f;

	/*pitch_kalman.init_Kalman();
	roll_kalman.init_Kalman();*/

	ClearAngles(0.0f, 0.0f, 0.0f);
	update_MPU9250();
	ClearAngles(0.0f, 0.0f, 0.0f);
}

int angle_reset_count = 0;
int resets = 0;

void loop() {

	loop_time = micros();

	//update sensors and radio		
	//update_LIS3DH();
	update_MPU9250();
	update_RadioReciever();

	/*//Kalman Filter
	main_pitch = pitch_kalman.Filter(Get_Acc_Pitch(), GetPitchRate() * GetGyroElapsedTime());
	main_roll = roll_kalman.Filter(Get_Acc_Roll(), GetRollRate() * GetGyroElapsedTime());
	*/

	/*
	main_pitch = ((double)(main_pitch + GetPitchRate()*GetGyroElapsedTime()) * 0.999) + (double)Get_Acc_Pitch()*0.001;
	main_roll = ((double)(main_roll + GetRollRate()*GetGyroElapsedTime()) * 0.999) + (double)Get_Acc_Roll() * 0.001;
	*/


	main_pitch = GetPitch();// (main_pitch + GetPitchRate()*egyrotime);// +Get_Acc_Pitch()*0.000f;
	main_roll = GetRoll();// (main_roll + GetRollRate()*egyrotime);// +Get_Acc_Roll()*0.0001f;


	/*angle_reset_count++;
	if (angle_reset_count > 30)
	{
		angle_reset_count = 0;
		if (abs(GetPitchRate()) < 3.7f && abs(GetRollRate()) < 3.7f)
		{
			if (abs(Get_Acc_Pitch()) < 10.0f && abs(Get_Acc_Roll()) < 10.0f)
			{
				resets++;
				main_pitch = Get_Acc_Pitch();
				main_roll = Get_Acc_Roll();
				ClearAngles(GetYaw(), Get_Acc_Pitch(), Get_Acc_Roll());
				debug_flash();
			}
		}
	}*/

	/*if (abs(GetPitchRate()*GetGyroElapsedTime()) < 5.0f && abs(GetRollRate()*GetGyroElapsedTime()) < 5.0f)
	{
		if (abs(Get_Acc_Pitch()) < 2.0f && abs(Get_Acc_Roll()) < 2.0f)
		{
			angle_reset_count++;
		}
		else
		{
			angle_reset_count = 0;
		}
	}
	else
	{
		angle_reset_count = 0;
	}

	if (angle_reset_count > 8)
	{
		angle_reset_count = 0;
		resets++;
		main_pitch = Get_Acc_Pitch();
		main_roll = Get_Acc_Roll();
		ClearAngles(GetYaw(), Get_Acc_Pitch(), Get_Acc_Roll());
		debug_flash();
	}*/

	if (safety == SAFE)
	{
		//update debug helper
		update_Debug_SAFE();

		//zero motors
		update_motors(0.0f, 0.0f, 0.0f, 0.0f);

		//check ARM
		arm();
	}
	else if (safety == ARMED)
	{
		//update debug helper
		update_Debug_ARMED();

		//update PID loops
		yaw_angle += GetChannel(RUD_YAW) / 1500.0f;
		yaw_out = YawPID.update_PID(GetYaw(), yaw_angle);
		pitch_out = PitchPID.update_PID(main_pitch, -GetChannel(ELEV_PITCH) / 4.0f);
		roll_out = RollPID.update_PID(main_roll, GetChannel(AIL_ROLL) / 4.0f);

		//update motor values
		throttle_out = GetChannel(THROTTLE);
		throttle_out = (throttle_out + 100.0f) / 2.0f;

		//check safety value
		if (GetChannel(GEAR) < GEAR_SET && throttle_out < THROTTLE_THRESHOLD)
		{
			safety = SAFE;
			return;
		}

		motor_output[0] = throttle_out;
		motor_output[1] = throttle_out;
		motor_output[2] = throttle_out;
		motor_output[3] = throttle_out;

		motor_output[0] += yaw_out;
		motor_output[1] += -yaw_out;
		motor_output[2] += -yaw_out;
		motor_output[3] += yaw_out;

		motor_output[0] += pitch_out;
		motor_output[1] += pitch_out;
		motor_output[2] += -pitch_out;
		motor_output[3] += -pitch_out;

		motor_output[0] += -roll_out;
		motor_output[1] += roll_out;
		motor_output[2] += -roll_out;
		motor_output[3] += roll_out;

		//clamp motor values

		motor_output[0] = max(0.0f, motor_output[0]);
		motor_output[1] = max(0.0f, motor_output[1]);
		motor_output[2] = max(0.0f, motor_output[2]);
		motor_output[3] = max(0.0f, motor_output[3]);

		motor_output[0] = min(100.0f, motor_output[0]);
		motor_output[1] = min(100.0f, motor_output[1]);
		motor_output[2] = min(100.0f, motor_output[2]);
		motor_output[3] = min(100.0f, motor_output[3]);

		if (throttle_out < THROTTLE_THRESHOLD)
		{
			motor_output[0] = 0.0f;
			motor_output[1] = 0.0f;
			motor_output[2] = 0.0f;
			motor_output[3] = 0.0f;
		}

		//send motor values to driver
		update_motors(motor_output[0], motor_output[1], motor_output[2], motor_output[3]);
		//update_motors(0.0f, 0.0f, 0.0f, 0.0f);
	}
	else
	{
		update_Debug_ARMED();
	}

	//////                       ///////
	//////        display        ///////
	//////                       ///////
	count++;
	if (count > 100)
	{
		count = 0;
		//Serial.printf("%f, %f, %f, %f, ", motor_output[0], motor_output[1], motor_output[2], motor_output[3]);
		Serial.printf("%f, %f, %f\n", GetYaw(), GetPitch(), GetRoll());
		//Serial.printf("%f, ", 1.0f / ((loop_time - l_loop_time) / 1000000.0f));

		//Serial.printf("%f, %f, %f, 00, %f, %f, %f, 00, %d, %d, %d\n", GetYaw(), GetPitch(), GetRoll(), get_cal_x(), get_cal_y(), get_cal_z(), getX(), getY(), getZ());
		//Serial.printf("%f, %f, ", Get_Acc_Pitch(), Get_Acc_Roll());
		//Serial.printf("%f, %f, %f, %f, ", GetChannel(THROTTLE), GetChannel(RUD_YAW), GetChannel(AIL_ROLL), GetChannel(ELEV_PITCH));
		//Serial.printf("%f, %f\n", GetRoll(), GetPitch());
	}
	l_loop_time = loop_time;
}

void arm()
{
	if (GetChannel(THROTTLE) < THROTTLE_UNLOCK)
	{
		if (GetChannel(RUD_YAW) > YAW_UNLOCK)
		{
			if (GetChannel(AIL_ROLL) < ROLL_UNLOCK)
			{
				if (GetChannel(ELEV_PITCH) < PITCH_UNLOCK)
				{
					if (GetChannel(GEAR) > GEAR_SET)
					{
						calibrate_Accelerometer();
						calibrate_Gyro();
						ClearAngles(0.0f, 0.0f, 0.0f);

						//delay(100);
						yaw_angle = 0.0f;

						main_pitch = 0.0f;
						main_roll = 0.0f;

						/*pitch_kalman.init_Kalman();
						roll_kalman.init_Kalman();*/

						ClearAngles(0.0f, 0.0f, 0.0f);
						update_MPU9250();
						ClearAngles(0.0f, 0.0f, 0.0f);
						
						safety = ARMED;
					}
				}
			}
		}
	}
}

#endif