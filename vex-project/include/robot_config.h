#pragma once
#include "vex.h"

using namespace vex;

const int TOP_MOTOR_PORT = 4;
const int LEFT_MOTOR_PORT = 0;
const int RIGHT_MOTOR_PORT = 6;
const int BACK_MOTOR_PORT = 5;

extern brain Brain;
extern inertial BrainInertial;
extern motor top_motor;
extern motor left_motor;
extern motor right_motor;
extern motor back_motor;

void vexcodeInit(void);
