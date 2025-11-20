//----------------------------------------------------------------------------
//
//  Module:       sensor_validation.cpp
//  Description:  Validate cube placement using sensors.
//  Authors:      MTE 100 & 121 Group 15
//
//----------------------------------------------------------------------------

#include "sensor_validation.h"

bool validate_cube_placement(char expected_char, distance& distance_sensor, optical& optical_sensor, touchled& touch_led)
{
    bool cube_validated = false; 

    while(!cube_validated)
    {
        print_status("Insert cube and press...");

        // Wait for button press
        touch_led.setColor(white);
        while (!touch_led.pressing()) { wait(20, msec); }
        while (touch_led.pressing())  { wait(20, msec); }
        touch_led.setColor(blue);
        
        // Read Sensors
        double dist_val = distance_sensor.objectDistance(mm);  // mm
        double hue = optical_sensor.hue();                     // 0 - 360
        double brightness = optical_sensor.brightness();       // 0 - 100
        
        char detected_char = '?';
        
        // --- Color Detection Logic ---
        if (brightness > MIN_BRIGHTNESS_WHITE) 
        {
            detected_char = 'W';
        }
        else 
        {
            if ((hue >= HUE_RED_MIN && hue < HUE_RED_MAX))
                detected_char = 'R';
            else if ((hue >= HUE_ORANGE_HIGH_MIN && hue <= 360) || (hue <= HUE_ORANGE_LOW_MAX))
                detected_char = 'O';
            
            else if (hue >= HUE_YELLOW_MIN && hue < HUE_YELLOW_MAX)
                detected_char = 'Y';
            
            else if (hue >= HUE_GREEN_MIN && hue < HUE_GREEN_MAX)
                detected_char = 'G';
            
            else if (hue >= HUE_BLUE_MIN && hue < HUE_BLUE_MAX)
                detected_char = 'B';
        }
        // Small delay to read screen
        wait(DELAY_DEBUG_PRINT_SEC, seconds);

        // Check Validation
        bool distance_ok = (dist_val >= CUBE_PRESENT_DIST_MIN && dist_val <= CUBE_PRESENT_DIST_MAX);
        bool colour_ok = (detected_char == expected_char);

        if (distance_ok && colour_ok)
        {
            touch_led.setColor(green);
            cube_validated = true;
            wait(DELAY_SUCCESS_SEC, seconds);
        }
        else
        {
            touch_led.setColor(red); 
            
            if(!distance_ok)      print_status("Error: No Cube");
            else if(!colour_ok)   print_status("Error: Wrong Placement");
            
            wait(DELAY_FAILURE_SEC, seconds);
        }

        wait(DELAY_LOOP_CYCLE_MSEC, msec);
    }
    
    return true; 
}