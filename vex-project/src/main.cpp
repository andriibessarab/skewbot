//----------------------------------------------------------------------------
//
//  Module:       main.cpp
//  Description:  The main entrypoint
//
//----------------------------------------------------------------------------

#include "vex.h"
#include "robot_config.h"
#include "state_converter.h"
#include "serial_io.h"
#include "solution_finder.h"
#include "misc_functions.h"
#include "sensor_validation.h"

using namespace vex;

int main()
{
    // Configure robot
    vexcode_init();

    // Open file
    FILE *file = fopen("SkewSolutions.bin", "rb");
    if (!file)
    {
        print_status("Couldn't open file. Aborting...");
        wait(10, seconds);
        Brain.programStop();
    }

    // Get state from serial
    print_status("Awaiting data...");
    std::string raw_state_string = get_state_from_serial();

    // Process state
    print_status("Processing...");
    std::string state_struct_stringified = find_normalized_stringified_struct(raw_state_string);

    wait(2, seconds);

    // Obtain solution
    print_status("Searching...");
    std::string solution = find_solution(state_struct_stringified, file);
    fclose(file);

    wait(2, seconds);

    // Use sensor to validate cube placement
    // loops until valid
    char expected_char = state_struct_stringified[SENSOR_SIDE_CENTER_INDEX];
    validate_cube_placement(expected_char, distance_sensor, optical_sensor, touch_led);

    wait(2, seconds);

    // Wait for touch to start solve
    print_status("Ready!");
    touch_led.setColor(white);
    while (!touch_led.pressing())
    {}
    while (touch_led.pressing())
    {}
    touch_led.setColor(blue);

    // Solve
    print_status("Solving...");

    for (int i = 0; i < solution.size(); ++i)
    {
        
        top_motor.stop();
        left_motor.stop();
        right_motor.stop();
        back_motor.stop();
        char move = solution[i];
        switch (move)
        {
        case 'U':
            perform_a_move(back_motor, true, false);
            break;
        case 'u':
            perform_a_move(back_motor, false, false);
            break;
        case 'L':
            perform_a_move(left_motor, true,false);
            break;
        case 'l':
            perform_a_move(left_motor, false,false);
            break;
        case 'R':
            perform_a_move(right_motor, true,false);
            break;
        case 'r':
            perform_a_move(right_motor, false,false);
            break;
        case 'F':
            perform_a_move(top_motor, true,true);
            top_motor.PrintPosition();
            break;
        case 'f':
            perform_a_move(top_motor, false,true);
            top_motor.PrintPosition();
            break;
        default:
            break;
        }
    }
    top_motor.stop();
    left_motor.stop();
    right_motor.stop();
    back_motor.stop();

    // End the program
    touch_led.setColor(green);
    print_status("Solved!");
    wait(20, seconds);
    Brain.programStop();
}
