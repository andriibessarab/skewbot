//----------------------------------------------------------------------------
//                                                                          
//  Module:       robot_config.cpp                                                    
//  Description:  Initializing sensors and default vex stuff                                                                                 
//                                                                          
//----------------------------------------------------------------------------

#include "robot_config.h"

brain Brain;
inertial BrainInertial = inertial();
motor top_motor = motor(TOP_MOTOR_PORT);
motor left_motor = motor(LEFT_MOTOR_PORT);
motor right_motor = motor(RIGHT_MOTOR_PORT);
motor back_motor = motor(BACK_MOTOR_PORT);
distance distance_sensor = distance(DISTANCE_SENSOR_PORT);
touchled touch_led = touchled(TOUCH_LED_PORT);
optical optical_sensor = optical(OPTICAL_SENSOR_PORT);

void initializeRandomSeed() {
  wait(100, msec);
  double xAxis = BrainInertial.acceleration(xaxis) * 1000;
  double yAxis = BrainInertial.acceleration(yaxis) * 1000;
  double zAxis = BrainInertial.acceleration(zaxis) * 1000;
  
  int seed = int(
    xAxis + yAxis + zAxis
  );
  
  srand(seed);
}

void vexcodeInit(void) {
  initializeRandomSeed();
}
