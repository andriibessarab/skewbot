//----------------------------------------------------------------------------
//                                                                          
//  Module:       robot_config.cpp                                                    
//  Description:  Initializing sensors and default vex stuff                                                                                 
//                                                                          
//----------------------------------------------------------------------------

#include "robot_config.h"

brain Brain;
inertial BrainInertial = inertial();
custom_motor top_motor(TOP_MOTOR_PORT);
custom_motor left_motor(LEFT_MOTOR_PORT);
custom_motor right_motor(RIGHT_MOTOR_PORT);
custom_motor back_motor(BACK_MOTOR_PORT);
distance distance_sensor = distance(DISTANCE_SENSOR_PORT);
touchled touch_led = touchled(TOUCH_LED_PORT);
optical optical_sensor = optical(OPTICAL_SENSOR_PORT);

void initalize_random_seed()
{
  wait(100, msec);
  double xAxis = BrainInertial.acceleration(xaxis) * 1000;
  double yAxis = BrainInertial.acceleration(yaxis) * 1000;
  double zAxis = BrainInertial.acceleration(zaxis) * 1000;
  
  int seed = int(xAxis + yAxis + zAxis);
  
  srand(seed);
}

void vexcode_init(void)
{
  initalize_random_seed();

  //initialize sensors
  touch_led.setColor(white);

  // make font smaller
  Brain.Screen.setFont(mono12);

  top_motor.set_adjustment_degrees_neg(5);
  top_motor.set_adjustment_degrees_pos(8);
  left_motor.set_adjustment_degrees_neg(8.5);
  left_motor.set_adjustment_degrees_pos(8);
  right_motor.set_adjustment_degrees_neg(6);
  right_motor.set_adjustment_degrees_pos(9.678);
  back_motor.set_adjustment_degrees_neg(7);
  back_motor.set_adjustment_degrees_pos(10);
}

