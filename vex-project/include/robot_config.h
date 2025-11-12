#pragma once
#include "vex.h"

using namespace vex;

const int TOP_MOTOR_PORT = PORT5;
const int LEFT_MOTOR_PORT = PORT1;
const int RIGHT_MOTOR_PORT = PORT7;
const int BACK_MOTOR_PORT = PORT9;
const int DISTANCE_SENSOR_PORT = PORT8;
const int TOUCH_LED_PORT = PORT12;
const int OPTICAL_SENSOR_PORT = PORT4;
 
extern brain Brain;
extern inertial BrainInertial;
extern motor top_motor;
extern motor left_motor;
extern motor right_motor;
extern motor back_motor;
extern distance distance_sensor;
extern touchled touch_led;
extern optical optical_sensor;

void vexcodeInit(void);
