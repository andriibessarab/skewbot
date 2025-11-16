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


extern brain Brain;
extern inertial BrainInertial;
extern distance distance_sensor;
extern touchled touch_led;
extern optical optical_sensor;

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
        motor_object.setStopping(brake);                       // hold mode
        motor_object.setMaxTorque(MOTOR_MAX_TORQUE, percent); // max out torque       // max velocity that doesn't stall
    }
    void stop(){
        motor_object.stop();
    }
    void set_angle_offset(int offset)
    {
        ANGLE_OFFSET = offset;
    }
    void PrintPosition(){
        Brain.Screen.setCursor(6,1);
        Brain.Screen.clearLine();
        Brain.Screen.print(" Pos: %.6f", motor_object.position(degrees));
    }

    double PID_Step(double setpoint, double meas, double dt, int min, int max, bool resetPI)
{
    static double I = 0.0;
    static double prev_err = 0.0;   

    double Kp = 2;
    double Ki = 0.015;
    double Kd = 0.1;

    double u = 0.0;
    double err = setpoint - meas;

    if (resetPI)
    {
        I = 0.0;
        prev_err = err;
    }
    else
    {
        // ----- Integral -----
        // if (fabs(err) > 3.0)   // your “don’t integrate near target” rule
        // {
        I += Ki * err * dt;
        //}

        // Anti-windup
        if (I > max) I = max;
        if (I < min) I = min;

        // ----- Derivative -----
        double D = 0.0;
        if (dt > 0.0)
            D = Kd * (err - prev_err) / dt;

        prev_err = err;

        // ----- Total Output -----
        u = Kp * err + I + D;

        // Clamp output
        if (u > max) u = max;
        if (u < min) u = min;
    }

    return u;
}

    // True: CW 90°
    // False: CCW 90°
    void spin_motor(int motorPower, bool dir)
    {
        PID_Step(0, 0, 0, 0, 0, true);

        motor_object.setPosition(0, degrees);

        const int period_ms = 20;
        const double dt = period_ms / 1000.0;

        const double MIN_ERROR = 0.15;
        const double MIN_EFFORT = 8.0;

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
                double power = PID_Step(targetAngle, current, dt, minSpeed, maxSpeed, false);

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



extern custom_motor top_motor;
extern custom_motor left_motor;
extern custom_motor right_motor;
extern custom_motor back_motor;


void vexcode_init(void);
