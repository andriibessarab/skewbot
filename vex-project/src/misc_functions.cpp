//----------------------------------------------------------------------------
//
//  Module:       misc_functions.cpp
//  Description:  Misc. functions used in main
//  Authors:      MTE 100 & 121 Group 15
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
