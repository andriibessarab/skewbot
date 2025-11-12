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
}\

void vexcode_init(void)
{
  initalize_random_seed();
}

