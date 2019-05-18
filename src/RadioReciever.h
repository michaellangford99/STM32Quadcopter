
// L3G4200D

#ifndef _RADIORECIEVER_h
#define _RADIORECIEVER_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "wprogram.h"
#else
#include "WProgram.h"
#endif

#include "avr/io.h"
#include "avr/interrupt.h"

#define RADIO_RX   23

//channel definitions
#define CHANNELS   9
#define THROTTLE   1
#define AIL_ROLL   2
#define ELEV_PITCH 3
#define RUD_YAW    4
#define GEAR       5
#define AUX4_LEFT  6
#define AUX5_RIGHT 7
#define UNKNOWN    8
#define TIME       0

#define TIME_MINIMUM 5800 

//pulsewidth timing constants
#define CENTER_PW  1512
#define DELTA_PW   4.78f

//smoothing rate for percentages
#define RADIO_DELTA_SPEED 0.01f

void init_RadioReciever();
float GetChannel(int channel);
void update_RadioReciever();

void wait_for_pulse();
void interrupt_method();
float get_percent(int pulse);

#endif