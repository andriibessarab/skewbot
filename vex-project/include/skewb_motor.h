//----------------------------------------------------------------------------
//
//  Module:       skewb_motor.h
//  Description:  Header class with separate PID for CW and CCW.
//  Authors:      MTE 100 & 121 Group 15
//
//----------------------------------------------------------------------------

#ifndef SKEWB_MOTOR_H
#define SKEWB_MOTOR_H

#pragma once

#include "vex.h"

using namespace vex;

const int TURN_ANGLE = 120;

class skewb_motor
{
private:
    motor motor_object;

    // CW
    double Kp_cw = 0, Ki_cw = 0, Kd_cw = 0;
    // CCW
    double Kp_ccw = 0, Ki_ccw = 0, Kd_ccw = 0;
    // Active
    double active_Kp = 0, active_Ki = 0, active_Kd = 0;

    double integral = 0.0;
    double prev_position = 0.0;
    double target_position = 0.0;
    const double MAX_INTEGRAL = 25.0; 
    const double TOLERANCE = 2.5;

    // returns velocity percent 
    double calculate_pid_output(double current_position)
    {
        double error = target_position - current_position;

        // integral
        // only if close to target but outside tolerance
        if (fabs(error) < 15.0 && fabs(error) > TOLERANCE) 
            integral += error;
        else if (fabs(error) >= 15.0)
            integral = 0; 
        
        // cap integral
        double integral_term = integral * active_Ki;
        if (integral_term > MAX_INTEGRAL)
            integral_term = MAX_INTEGRAL;
        if (integral_term < -MAX_INTEGRAL)
            integral_term = -MAX_INTEGRAL;

        // derivative
        double derivative = current_position - prev_position;
        prev_position = current_position;

        // generate output using ACTIVE constants
        double output = (active_Kp * error) + integral_term - (active_Kd * derivative);
        
        // clamp
        if (output > 100.0)
            output = 100.0;
        if (output < -100.0)
            output = -100.0;

        return output;
    }

    // execute actual move using pid
    void execute_move() {
        const int dt_ms = 10; 
        bool settled = false;
        int settled_count = 0;
        int timeout_counter = 0;
        const int MAX_TIMEOUT = 500; 

        prev_position = motor_object.position(degrees);

        // Check direction to use correct pid
        if (target_position >= prev_position) {
            // Moving Positive (Clockwise)
            active_Kp = Kp_cw;
            active_Ki = Ki_cw;
            active_Kd = Kd_cw;
        } else {
            // Moving Negative (Counter-Clockwise)
            active_Kp = Kp_ccw;
            active_Ki = Ki_ccw;
            active_Kd = Kd_ccw;
        }

        while (!settled && timeout_counter < MAX_TIMEOUT)
        {
            double current_pos = motor_object.position(degrees);
            double error = target_position - current_pos;

            double velocity_percent = calculate_pid_output(current_pos);

            motor_object.spin(forward, velocity_percent, percent);

            // only consider settled when no error
            if (fabs(error) <= TOLERANCE && fabs(motor_object.velocity(dps)) < 2)
                settled_count++;
            else
                settled_count = 0;

            if (settled_count > 10) 
                settled = true;

            timeout_counter++;
            wait(dt_ms, msec);

            // Always listen for pause or emerg. stop
            if(touch_led_pointer->pressing())
            {
                // stop motor
                motor_object.stop();

                // wait for person to let go and press again
                touch_led_pointer->setColor(white);
                while(!touch_led_pointer->pressing())
                {}
                while(touch_led_pointer->pressing())
                {}
                while(!touch_led_pointer->pressing())
                {}
                touch_led_pointer->setColor(blue);

                wait(500, msec);
            } 
            else if(brain_pointer->buttonUp.pressing())
            {
                emergency_stopped = true;
                settled = true; // so that we exit
            }
        }

        motor_object.stop(); 
    }

public:
    static brain* brain_pointer;          // for debugging
    static touchled* touch_led_pointer;   // for pausing
    static bool emergency_stopped;

    skewb_motor(int port, double p_cw, double i_cw, double d_cw, double p_ccw, double i_ccw, double d_ccw)
        : motor_object(port),
          Kp_cw(p_cw), Ki_cw(i_cw), Kd_cw(d_cw),
          Kp_ccw(p_ccw), Ki_ccw(i_ccw), Kd_ccw(d_ccw)

    {
        motor_object.resetPosition();
        motor_object.setStopping(hold); 
        motor_object.setMaxTorque(100, percent);
        target_position = 0.0;
        
        active_Kp = Kp_cw;
        active_Ki = Ki_cw;
        active_Kd = Kd_cw;
    }

    void setPID(bool is_clockwise, double p, double i, double d)
    {
        if(is_clockwise) {
            Kp_cw = p; Ki_cw = i; Kd_cw = d;
        } else {
            Kp_ccw = p; Ki_ccw = i; Kd_ccw = d;
        }
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
        prev_position = 0;
        integral = 0;
    }
};

#endif SKEWB_MOTOR_H