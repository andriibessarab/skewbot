#ifndef MISC_FUNCTIONS_H
#define MISC_FUNCTIONS_H

#include "vex.h"
#include "robot_config.h"

void print_status(std::string status);
void perform_a_move(custom_motor &m, bool inverted,bool IsTop);

#endif