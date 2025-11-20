//----------------------------------------------------------------------------
//
//  Module:       skewb_motor.h
//  Description:  Header class with PID for motors.
//  Authors:      MTE 100 & 121 Group 15
//
//----------------------------------------------------------------------------

#ifndef SKEWB_MOTOR_H
#define SKEWB_MOTOR_H

#pragma once

#include "vex.h"

using namespace vex;

class skewb_motor
{
private:
    motor motor_object;

    double Kp = 0, Ki = 0, Kd = 0;
    double integral = 0.0;
    double prev_error = 0.0;
    double target_position = 0.0;
    const double MAX_INTEGRAL = 25.0; 

    // returns velocity percent 
    double calculate_pid_output(double current_position)
    {
        double error = target_position - current_position;

        // integral
        // only if close to target
        if (fabs(error) < 15.0) 
            integral += error;
        else
            integral = 0; 
        
        // cap integral
        double integral_term = integral * Ki;
        if (integral_term > MAX_INTEGRAL)
            integral_term = MAX_INTEGRAL;
        if (integral_term < -MAX_INTEGRAL)
            integral_term = -MAX_INTEGRAL;

        // derivative
        double derivative = error - prev_error;
        prev_error = error;

        // generate output
        double output = (Kp * error) + integral_term + (Kd * derivative);
        
        // clamp
        if (output > 100.0) output = 100.0;
        if (output < -100.0) output = -100.0;

        return output;
    }

        // execut actual move using pid
    void execute_move() {
        const int dt_ms = 10; 
        bool settled = false;
        int settled_count = 0;
        int timeout_counter = 0;
        const int MAX_TIMEOUT = 200;  // 2 sec

        while (!settled && timeout_counter < MAX_TIMEOUT)
        {
            double current_pos = motor_object.position(degrees);
            double error = target_position - current_pos;

            double velocity_percent = calculate_pid_output(current_pos);

            motor_object.spin(forward, velocity_percent, percent);

            // onlyc consider settled when no error
            if (fabs(error) <= 0 && fabs(motor_object.velocity(percent)) <= 0)
                settled_count++;
            else
                settled_count = 0;

            if (settled_count > 5) // each count means been settled for 50ms
                settled = true;

            timeout_counter++;
            wait(dt_ms, msec);
        }

        motor_object.stop(); // engage hold
    }

public:
    skewb_motor(int port, double p, double i, double d)
        : motor_object(port),
          Kp(p), Ki(i), Kd(d)
    {
        motor_object.resetPosition();
        motor_object.setStopping(hold); 
        motor_object.setMaxTorque(100, percent);
        target_position = 0.0;
    }

    // update pid
    void setPID(double p, double i, double d)
    {
        Kp = p; Ki = i; Kd = d;
    }

    // move some relative amount
    void move_relative(double degrees)
    {
        target_position += degrees;
        execute_move();
    }

    // absolute position move
    void move_absolute(double degrees)
    {
        target_position = degrees;
        execute_move();
    }

    // reset
    void hard_reset_zero()
    {
        motor_object.resetPosition();
        target_position = 0;
        prev_error = 0;
        integral = 0;
    }
};

#endif SKEWB_MOTOR_H
