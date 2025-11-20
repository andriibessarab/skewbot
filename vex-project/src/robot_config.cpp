//----------------------------------------------------------------------------
//
//  Module:       robot_config.cpp
//  Description:  Initializing sensors and default vex stuff
//  Authors:      MTE 100 & 121 Group 15
//
//----------------------------------------------------------------------------

#include "robot_config.h"

brain Brain;
inertial BrainInertial = inertial();
distance distance_sensor = distance(DISTANCE_SENSOR_PORT);
touchled touch_led = touchled(TOUCH_LED_PORT);
optical optical_sensor = optical(OPTICAL_SENSOR_PORT);

skewb_motor top_motor(TOP_MOTOR_PORT, 1.9, 0.0, 0.0); 
skewb_motor left_motor(LEFT_MOTOR_PORT, 1.5, 0.00, 0.0); 
skewb_motor right_motor(RIGHT_MOTOR_PORT, 1.5, 0.0, 0.0);
skewb_motor back_motor(BACK_MOTOR_PORT, 1.6, 0.0, 0.0);

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

  // initialize sensors
  touch_led.setFade(slow);
  optical_sensor.setLightPower(100);
  optical_sensor.setLight(ledState::on);

  // make font smaller
  Brain.Screen.setFont(mono12);
}
