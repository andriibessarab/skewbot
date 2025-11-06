#include "robot_config.h"

brain Brain;
inertial BrainInertial = inertial();

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