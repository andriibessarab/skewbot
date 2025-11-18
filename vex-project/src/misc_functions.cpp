//----------------------------------------------------------------------------
//
//  Module:       misc_functions.cpp
//  Description:  Misc. functions used in main
//
//----------------------------------------------------------------------------


#include "vex.h"
#include "misc_functions.h"

// Print status updates to brain
void print_status(std::string status)
{
    Brain.Screen.clearScreen();
    Brain.Screen.setCursor(2, 1);
    Brain.Screen.print("---SKEWBOT---");
    Brain.Screen.setCursor(3, 1);
    Brain.Screen.print(status.c_str());
}

// Perform a move on the robot
void perform_a_move(custom_motor &m, bool inverted, bool is_top)
{
    if (inverted)
        m.spin_motor(100, true, is_top);
       
    else
        m.spin_motor(100, false, is_top);
}