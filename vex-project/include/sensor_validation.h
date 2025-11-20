//----------------------------------------------------------------------------
//
//  Module:       sensor_validation.h
//  Description:  Header for file that validates cube placement using sensors.
//  Authors:      MTE 100 & 121 Group 15
//
//----------------------------------------------------------------------------

#ifndef SENSOR_VALIDATION_H
#define SENSOR_VALIDATION_H

#pragma once

#include "vex.h"
#include "misc_functions.h"

using namespace vex;

// --- Tuning Constants ---

// Distance Sensor Thresholds
const double CUBE_PRESENT_DIST_MIN = 30.0;  // mm
const double CUBE_PRESENT_DIST_MAX = 40.0;  // mm

// Optical Sensor - Brightness Thresholds
const double MIN_BRIGHTNESS_WHITE = 25.0;   // If brightness is > this, it detects 'W'

// Optical Sensor - Hue Thresholds (0 - 360 degrees)
// 'R' (Red) Range
const double HUE_RED_MIN = 250.0;
const double HUE_RED_MAX = 350.0;

// 'O' (Orange) Range - Note: Orange wraps around 360/0
const double HUE_ORANGE_HIGH_MIN = 350.0;   // High end (350-360)
const double HUE_ORANGE_LOW_MAX  = 5.0;     // Low end (0-5)

// 'Y' (Yellow) Range
const double HUE_YELLOW_MIN = 50.0;
const double HUE_YELLOW_MAX = 80.0;

// 'G' (Green) Range
const double HUE_GREEN_MIN  = 80.0;
const double HUE_GREEN_MAX  = 160.0;

// 'B' (Blue) Range
const double HUE_BLUE_MIN   = 160.0;
const double HUE_BLUE_MAX   = 250.0;

// Timers & Delays
const double DELAY_DEBUG_PRINT_SEC = 0.2;   // Time to wait between screen updates (prevents flickering)
const double DELAY_SUCCESS_SEC     = 0.5;   // How long to show green light on match
const double DELAY_FAILURE_SEC     = 2.0;   // How long to show red light on error
const double DELAY_LOOP_CYCLE_MSEC = 50.0;  // Short wait at end of loop

// UI Coordinates
const int UI_TEXT_ROW = 1;
const int UI_TEXT_COL = 40;

// Constant used in main
const int SENSOR_SIDE_CENTER_INDEX = 2;


// --- Functions ---
bool validate_cube_placement(char expected_char, distance& distance_sensor, optical& optical_sensor, touchled& touch_led);

#endif