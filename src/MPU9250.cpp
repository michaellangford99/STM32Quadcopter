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
#define CALIBRATE_TIME             2000

//register definitions
#define SELF_TEST_X_GYRO 0x00
#define SELF_TEST_Y_GYRO 0x01
#define SELF_TEST_Z_GYRO 0x02
#define SELF_TEST_X_ACCEL 0x0D
#define SELF_TEST_Y_ACCEL 0x0E
#define SELF_TEST_Z_ACCEL 0x0F
#define XG_OFFSET_H 0x13
#define XG_OFFSET_L 0x14
#define YG_OFFSET_H 0x15
#define YG_OFFSET_L 0x16
#define ZG_OFFSET_H 0x17
#define ZG_OFFSET_L 0x18
#define SMPLRT_DIV 0x19
#define CONFIG 0x1A
#define GYRO_CONFIG 0x1B
#define ACCEL_CONFIG 0x1C
#define ACCEL_CONFIG_2 0x1D
#define LP_ACCEL_ODR 0x1E
#define WOM_THR 0x1F
#define FIFO_EN 0x23
#define I2C_MST_CTRL 0x24
#define I2C_SLV0_ADDR 0x25
#define I2C_SLV0_REG 0x26
#define I2C_SLV0_CTRL 0x27
#define I2C_SLV1_ADDR 0x28
#define I2C_SLV1_REG 0x29
#define I2C_SLV1_CTRL 0x2A
#define I2C_SLV2_ADDR 0x2B
#define I2C_SLV2_REG 0x2C
#define I2C_SLV2_CTRL 0x2D
#define I2C_SLV3_ADDR 0x2E
#define I2C_SLV3_REG 0x2F
#define I2C_SLV3_CTRL 0x30
#define I2C_SLV4_ADDR 0x31
#define I2C_SLV4_REG 0x32
#define I2C_SLV4_DO 0x33
#define I2C_SLV4_CTRL 0x34
#define I2C_SLV4_DI 0x35
#define I2C_MST_STATUS 0x36
#define INT_PIN_CFG 0x37
#define INT_ENABLE 0x38
#define INT_STATUS 0x3A
#define ACCEL_XOUT_H 0x3B
#define ACCEL_XOUT_L 0x3C
#define ACCEL_YOUT_H 0x3D
#define ACCEL_YOUT_L 0x3E
#define ACCEL_ZOUT_H 0x3F
#define ACCEL_ZOUT_L 0x40
#define TEMP_OUT_H 0x41
#define TEMP_OUT_L 0x42
#define GYRO_XOUT_H 0x43
#define GYRO_XOUT_L 0x44
#define GYRO_YOUT_H 0x45
#define GYRO_YOUT_L 0x46
#define GYRO_ZOUT_H 0x47
#define GYRO_ZOUT_L 0x48
#define EXT_SENS_DATA_00 0x49
#define EXT_SENS_DATA_01 0x4A
#define EXT_SENS_DATA_02 0x4B
#define EXT_SENS_DATA_03 0x4C
#define EXT_SENS_DATA_04 0x4D
#define EXT_SENS_DATA_05 0x4E
#define EXT_SENS_DATA_06 0x4F
#define EXT_SENS_DATA_07 0x50
#define EXT_SENS_DATA_08 0x51
#define EXT_SENS_DATA_09 0x52
#define EXT_SENS_DATA_10 0x53
#define EXT_SENS_DATA_11 0x54
#define EXT_SENS_DATA_12 0x55
#define EXT_SENS_DATA_13 0x56
#define EXT_SENS_DATA_14 0x57
#define EXT_SENS_DATA_15 0x58
#define EXT_SENS_DATA_16 0x59
#define EXT_SENS_DATA_17 0x5A
#define EXT_SENS_DATA_18 0x5B
#define EXT_SENS_DATA_19 0x5C
#define EXT_SENS_DATA_20 0x5D
#define EXT_SENS_DATA_21 0x5E
#define EXT_SENS_DATA_22 0x5F
#define EXT_SENS_DATA_23 0x60
#define I2C_SLV0_DO 0x63
#define I2C_SLV1_DO 0x64
#define I2C_SLV2_DO 0x65
#define I2C_SLV3_DO 0x66
#define I2C_MST_DELAY_CTRL 0x67
#define SIGNAL_PATH_RESET 0x68
#define MOT_DETECT_CTRL 0x69
#define USER_CTRL 0x6A
#define PWR_MGMT_1 0x6B
#define PWR_MGMT_2 0x6C
#define FIFO_COUNTH 0x72
#define FIFO_COUNTL 0x73
#define FIFO_R_W 0x74
#define WHO_AM_I 0x75
#define XA_OFFSET_H 0x77
#define XA_OFFSET_L 0x78
#define YA_OFFSET_H 0x7A
#define YA_OFFSET_L 0x7B
#define ZA_OFFSET_H 0x7D
#define ZA_OFFSET_L 0x7E

union Gyro_Config {
	struct {
		int XGYRO_Cten : 1;//X Gyro self-test
		int YGYRO_Cten : 1;//Y Gyro self-test
		int ZGYRO_Cten : 1;//Z Gyro self-test
		
		int GYRO_FS_SEL : 2;//Gryo full scale select

		int reserved : 1;//Reserved
		int Fchoice_b : 2;//Used to bypass DLPF

	};
	uint8_t raw;
};

enum Gyro_FS_Sel {
	_250DPS = 0,
	_500DPS,
	_1000DPS,
	_2000DPS
};

Gyro_FS_Sel gyro_fs = _250DPS;
Gyro_Config gyro_config;
//puts data in correct units after scale selector applied
const float GYRO_MULTIPLIER = 250.0f;

union Accel_Config {
	struct{
		int ax_st_en : 1;//X Accel self-test
		int ay_st_en : 1;//Y Accel self-test
		int az_st_en : 1;//Z Accel self-test

		int ACCEL_FS_SEL : 2;//Accel Full Scale Select
		int res : 3;//Reserved
	};
	uint8_t raw;
};

enum Accel_FS_Sel {
	_2g = 0,
	_4g,
	_8g,
	_16g
};

Accel_FS_Sel accel_fs = _4g;
Accel_Config accel_config;
//puts data in correct units after scale selector applied
const float ACC_MULTIPLIER = 2.0f;

#define BEGIN_SPI_MPU9250()		   SPI.beginTransaction(SPISettings(CLOCK_SPEED, MSBFIRST, SPI_MODE0));

#define END_SPI_MPU9250()          SPI.endTransaction();

float microseconds, lms;
float seconds;
float pre_update_seconds;

float getGyroTime()
{
	return pre_update_seconds;
}

int readRegister(byte address);
void writeRegister(byte address, byte data);

void init_MPU9250() {
	SPI.begin();

	pinMode(10, OUTPUT);
	digitalWrite(10, HIGH);

	BEGIN_SPI_MPU9250();
	
	writeRegister(CONFIG, 0);

	accel_config.ACCEL_FS_SEL = accel_fs;
	writeRegister(ACCEL_CONFIG, accel_config.raw);

	gyro_config.GYRO_FS_SEL = gyro_fs;
	writeRegister(GYRO_CONFIG, accel_config.raw);

	END_SPI_MPU9250();

	delay(10);

	calibrate_Gyro();
	calibrate_Accelerometer();
}

int dbg_count = 0;

void update_MPU9250()
{
	update_Gyro();
	update_Accelerometer();

	/*dbg_count++;
	//Serial.printf("gx: %f\tgy: %f\tgz: %f\tax: %f\tay: %f\taz: %f\n", gyro_x, gyro_y, gyro_z, acc_x, acc_y, acc_z);

	if (dbg_count > 100)
	{
		Serial.printf("%f, %f, %f, %f,%f,%f,%f,%f,%f,%f,%f\n", GetYawRate(), GetPitchRate(), GetRollRate(), GetYaw(), GetPitch(), GetRoll(), GetXAcc(), GetYAcc(), GetZAcc(), GetRollAcc(), GetPitchAcc());
		dbg_count = 0;
	}*/
}

#define GYRO_RAW_RATE_X (((int16_t)((readRegister(GYRO_XOUT_H) & 0xFF) << 8) | (int16_t)(readRegister(GYRO_XOUT_L) & 0xFF))/65536.0f)
#define GYRO_RAW_RATE_Y (((int16_t)((readRegister(GYRO_YOUT_H) & 0xFF) << 8) | (int16_t)(readRegister(GYRO_YOUT_L) & 0xFF))/65536.0f)
#define GYRO_RAW_RATE_Z (((int16_t)((readRegister(GYRO_ZOUT_H) & 0xFF) << 8) | (int16_t)(readRegister(GYRO_ZOUT_L) & 0xFF))/65536.0f)

#define ACCEL_RAW_X (((int16_t)((readRegister(ACCEL_XOUT_H) & 0xFF) << 8) | (int16_t)(readRegister(ACCEL_XOUT_L) & 0xFF))/65536.0f)
#define ACCEL_RAW_Y (((int16_t)((readRegister(ACCEL_YOUT_H) & 0xFF) << 8) | (int16_t)(readRegister(ACCEL_YOUT_L) & 0xFF))/65536.0f)
#define ACCEL_RAW_Z (((int16_t)((readRegister(ACCEL_ZOUT_H) & 0xFF) << 8) | (int16_t)(readRegister(ACCEL_ZOUT_L) & 0xFF))/65536.0f)

//Gyroscope

float Yaw = 0.0f;
float Pitch = 0.0f;
float Roll = 0.0f;

float YawRateCal = 0.0f;
float PitchRateCal = 0.0f;
float RollRateCal = 0.0f;

float YawRate = 0.0f;
float PitchRate = 0.0f;
float RollRate = 0.0f;

long last_gtime;

void calibrate_Gyro()
{
	float start_time = millis();
	
	float YawRateSum = 0.0f;
	float PitchRateSum = 0.0f;
	float RollRateSum = 0.0f;

	long count = 0;
	
	while((millis() - start_time) < 1000.0f)
	{
		update_Gyro();

		YawRateSum += YawRate;
		PitchRateSum += PitchRate;
		RollRateSum += RollRate;

		count++;
	}

	YawRateCal =  YawRateSum / (float)count;
	PitchRateCal = PitchRateSum / (float)count;
	RollRateCal = RollRateSum / (float)count;

	Yaw = 0.0f;
	Pitch = 0.0f;
	Roll = 0.0f;
}

void update_Gyro()
{
	YawRate = ((float)GYRO_RAW_RATE_Z) * (float)(2 << gyro_fs) * 2.0f * GYRO_MULTIPLIER - YawRateCal;
	PitchRate = ((float)GYRO_RAW_RATE_Y) * (float)(2 << gyro_fs) * 2.0f * GYRO_MULTIPLIER - PitchRateCal;
	RollRate = ((float)GYRO_RAW_RATE_X) * (float)(2 << gyro_fs) * 2.0f * GYRO_MULTIPLIER - RollRateCal;
	
	Yaw += YawRate * seconds;
	Pitch += PitchRate * seconds;
	Roll += RollRate * seconds;

	pre_update_seconds = seconds;
	microseconds = (float)micros() - lms;
	lms = micros();
	seconds = microseconds / 1000000.0f;
}

float GetYaw()
{
	return Yaw;
}

float GetPitch()
{
	return Pitch;
}

float GetRoll()
{
	return Roll;
}

float GetYawRate() {
	return YawRate;
}

float GetPitchRate() {
	return PitchRate;
}

float GetRollRate() {
	return RollRate;
}

void ClearAngles(float yaw, float pitch, float roll)
{
	Yaw = yaw;
 	Pitch = pitch;
	Roll = roll;

	seconds = 0.0f;
	lms = micros();
}

//Accelerometer

float XAcc = 0.0f;
float YAcc = 0.0f;
float ZAcc = 0.0f;

float XAccCal = 0.0f;
float YAccCal = 0.0f;
float ZAccCal = 0.0f;

float PitchAcc = 0.0f;
float RollAcc = 0.0f;

void calibrate_Accelerometer()
{
	float start_time = millis();
	
	float XAccSum = 0.0f;
	float YAccSum = 0.0f;
	float ZAccSum = 0.0f;

	long count = 0;
	
	while((millis() - start_time) < 1000.0f)
	{
		update_Accelerometer();

		XAccSum += XAcc;
		YAccSum += YAcc-1.0f;
		ZAccSum += ZAcc;

		count++;
	}

	XAccCal =  XAccSum / (float)count;
	YAccCal = YAccSum / (float)count;
	ZAccCal = ZAccSum / (float)count;
}

void update_Accelerometer()
{
	XAcc = ((float)ACCEL_RAW_Y) * (float)(2 << accel_fs) * ACC_MULTIPLIER - XAccCal;
	YAcc = ((float)ACCEL_RAW_Z) * (float)(2 << accel_fs) * ACC_MULTIPLIER - YAccCal;
	ZAcc = ((float)ACCEL_RAW_X) * (float)(2 << accel_fs) * ACC_MULTIPLIER - ZAccCal;

	RollAcc = degrees(atan2f(XAcc, YAcc));
	PitchAcc = degrees(atan2f(-ZAcc, YAcc));
}

float GetPitchAcc()
{
	return PitchAcc;
}

float GetRollAcc()
{
	return RollAcc;
}

float GetXAcc() {
	return XAcc;
}

float GetYAcc() {
	return YAcc;
}

float GetZAcc() {
	return ZAcc;
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