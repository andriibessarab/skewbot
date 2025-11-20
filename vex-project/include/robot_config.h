//----------------------------------------------------------------------------
//
//  Module:       robot_config.g
//  Description:  Header for initializing sensors and default vex stuff
//  Authors:      MTE 100 & 121 Group 15
//
//----------------------------------------------------------------------------

#ifndef ROBOT_CONFIG_H
#define ROBOT_CONFIG_H

#pragma once

#include "vex.h"
#include "skewb_motor.h"

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
extern distance distance_sensor;
extern touchled touch_led;
extern optical optical_sensor;

extern skewb_motor top_motor;
extern skewb_motor left_motor;
extern skewb_motor right_motor;
extern skewb_motor back_motor;

void vexcode_init(void);


#endif ROBOT_CONFIG_H
