#pragma once
#include "vex.h"

using namespace vex;

const int MOTOR_SPEED = 80;       //%
const int MOTOR_MAX_TORQUE = 100; //%

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
    int ANGLE_OFFSET = 0;

    custom_motor(int port)
        : port_number(port),
          motor_object(port)
    {
        motor_object.resetPosition();                         // reset encoder
        motor_object.setStopping(hold);                       // hold mode
        motor_object.setMaxTorque(MOTOR_MAX_TORQUE, percent); // max out torque       // max velocity that doesn't stall
    }

    void set_angle_offset(int offset)
    {
        ANGLE_OFFSET = offset;
    }

    double proportional_step(double setpoint, double meas, double dt, int min, int max, bool resetPI)
    {
        static double I = 0.0;

        double Kp = 0.8;
        double Ki = 0.01;
        double u = 0.0;

        if (resetPI)
        {
            I = 0.0;
        }
        else
        {
            double err = setpoint - meas;

            // Only integrate when we're not super close to avoid overshoot
            if (fabs(err) > 3.0)
            {
                I += Ki * err * dt;
            }

            // Anti-windup
            if (I > max)
                I = max;
            if (I < min)
                I = min;

            u = Kp * err + I;

            if (u > max)
                u = max;
            if (u < min)
                u = min;
        }

        return u;
    }

    // True: CW 90°
    // False: CCW 90°
    void spin_motor(int motorPower, bool dir)
    {
        proportional_step(0, 0, 0, 0, 0, true);

        motor_object.setPosition(0, degrees);

        const int period_ms = 20;
        const double dt = period_ms / 1000.0;

        const double MIN_ERROR = 1;
        const double MIN_EFFORT = 5.0;

        const double ANGLE = 120 + ANGLE_OFFSET;

        int minSpeed = -motorPower;
        int maxSpeed = motorPower;

        bool reachedTarget = false;

        double targetAngle = dir ? ANGLE : -ANGLE;

        while (!reachedTarget)
        {
            double current = motor_object.position(degrees);

            double error = targetAngle - current;

            if (fabs(error) < MIN_ERROR)
            {
                motor_object.stop();
                reachedTarget = true;
            }
            else
            {
                double power = proportional_step(targetAngle, current, dt, minSpeed, maxSpeed, false);

                if (fabs(power) < MIN_EFFORT)
                {
                    power = (power >= 0) ? MIN_EFFORT : -MIN_EFFORT;
                }

                double speed = fabs(power);
                motor_object.setVelocity(speed, percent);

                if (power >= 0)
                {
                    motor_object.spin(forward);
                }
                else
                {
                    motor_object.spin(reverse);
                }

                wait(period_ms, msec);
            }
        }
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
