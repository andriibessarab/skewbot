//----------------------------------------------------------------------------
//
//  Module:       main.cpp
//  Description:  The main entrypoint
//  Authors:      MTE 100 & 121 Group 15
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
        print_status("Couldn't open file. Abort...");
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
    if(solution == "")
    {
        print_status("Couldn't find solution. Abort...");
        wait(10, seconds);
        Brain.programStop(); 
    }

    wait(2, seconds);

    // Use sensor to validate cube placement
    // loops until valid
    char expected_char = state_struct_stringified[SENSOR_SIDE_CENTER_INDEX];
    validate_cube_placement(expected_char, distance_sensor, optical_sensor, touch_led);

    wait(2, seconds);
    // std::string solution = "FlUfLuFlUfLuFlUfLuFlUfLuFlUfLuFlUfLuFlUfLu";

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
    Brain.Timer.reset(); // start timer
    for (int i = 0; i < solution.size(); ++i)
    {
        char move = solution[i];

        switch (move)
        {
            case 'U':
                back_motor.move_relative(TURN_ANGLE);
                break;
            case 'u':
                back_motor.move_relative(TURN_ANGLE);
                back_motor.move_relative(TURN_ANGLE);
                break;
            case 'L':
                left_motor.move_relative(TURN_ANGLE);
                break;
            case 'l':
                left_motor.move_relative(-TURN_ANGLE);
                break;
            case 'R':
                right_motor.move_relative(TURN_ANGLE);
                break;
            case 'r':
                right_motor.move_relative(-TURN_ANGLE);
                break;
            case 'F':
                top_motor.move_relative(TURN_ANGLE);
                break;
            case 'f': 
                top_motor.move_relative(TURN_ANGLE);
                top_motor.move_relative(TURN_ANGLE);
                break;
            default:
                break;
        }

        wait(50, msec);
    }

    // End the program
    const double time = Brain.Timer.value() / 1000;
    touch_led.setColor(green);
    std::string final_messsage = "Solved in " + std::to_string(static_cast<int>(time)) + " seconds";
    print_status(final_messsage);
    wait(20, seconds);
    Brain.programStop();
}
