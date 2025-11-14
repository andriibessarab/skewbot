#pragma once
#include "vex.h"

using namespace vex;

const int MOTOR_SPEED = 80;        //%
const int MOTOR_MAX_TORQUE = 100;  //%

const int TOP_MOTOR_PORT = PORT5;
const int LEFT_MOTOR_PORT = PORT1;
const int RIGHT_MOTOR_PORT = PORT7;
const int BACK_MOTOR_PORT = PORT9;
const int DISTANCE_SENSOR_PORT = PORT8;
const int TOUCH_LED_PORT = PORT12;
const int OPTICAL_SENSOR_PORT = PORT4;
 
struct custom_motor
{
    const int port_number; 
    motor motor_object;
    double ideal_position; 
    double adjustment_degrees_neg;
    double adjustment_degrees_pos;

    custom_motor(int port) 
        : port_number(port), 
          motor_object(port),
          ideal_position(0.0),
          adjustment_degrees_neg(0.0),
          adjustment_degrees_pos(0.0)
    {
        motor_object.resetPosition();                          // reset encoder
        motor_object.setStopping(hold);                        // hold mode
        motor_object.setMaxTorque(MOTOR_MAX_TORQUE, percent);  // max out torque
        motor_object.setVelocity(MOTOR_SPEED, percent);        // max velocity that doesn't stall
    }

    void set_adjustment_degrees_neg(int d)
    {
        adjustment_degrees_neg = d;
    }

    void set_adjustment_degrees_pos(int d)
    {
        adjustment_degrees_pos = d;
    }
        

    void spin_degrees(double degrees_to_spin)
    {
        motor_object.resetPosition();
        motor_object.setStopping(hold);                        // hold mode
        motor_object.setMaxTorque(MOTOR_MAX_TORQUE, percent);  // max out torque
        motor_object.setVelocity(MOTOR_SPEED, percent);        // max velocity that doesn't stall

        if(degrees_to_spin < 0)
            motor_object.spinToPosition(degrees_to_spin - adjustment_degrees_neg, degrees, true);
        else
            motor_object.spinToPosition(degrees_to_spin + adjustment_degrees_pos, degrees, true);
        
    }
};

extern brain Brain;
extern inertial BrainInertial;
extern custom_motor top_motor;
extern custom_motor left_motor;
extern custom_motor right_motor;
extern custom_motor back_motor;
extern distance distance_sensor;
extern touchled touch_led;
extern optical optical_sensor;

void vexcode_init(void);
