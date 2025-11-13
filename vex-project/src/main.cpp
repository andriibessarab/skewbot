//----------------------------------------------------------------------------
//                                                                          
//  Module:       main.cpp                                                    
//  Description:  The main entrypoint                                                                                   
//                                                                          
//----------------------------------------------------------------------------

// --- Info ---
// SCANNED STATE
// Index = (FaceIndex * 5) + StickerIndex
// Faces: U=0, F=1, R=2, B=3, L=4, D=5
// Stickers: C=0, TL=1, TR=2, BR=3, BL=4

#include "vex.h"
#include "robot_config.h"
#include "state_converter.h"

using namespace vex;

// --- Constants ---
const int MAX_BUFFER_SIZE = 100;

// --- Local Functions Prototypes ---
void print_status(std::string status);
std::string get_state_from_serial();
void perform_a_move(custom_motor &m, bool inverted);

/////////////////////////////// MAIN PROGRAM ///////////////////////////////
int main()
{
    // configure robot
    vexcode_init();

    print_status("Skewb Solver");

    // wait(2, seconds);
    // print_status("Awaiting state...");

    // // Get state from serial
    // std::string raw_state_string = get_state_from_serial();

    // print_status("Processing...");

    // // Process state
    // std::string normalized_state_string = normalize_state(raw_state_string);
    // skewb_state state_struct = convert_state_to_struct(normalized_state_string);
    // std::string state_struct_stringified = skewb_state_to_string(state_struct);
    // printf("\n%s\n", state_struct_stringified.c_str());

    // wait(2, seconds);
    // print_status("Thinking...");

    // // Obtain solution
    std::string solution = "";


    // wait(2, seconds);
    // print_status("Ready!");

    // // Wait for touch to start solve

    // // Check sensory data

    // // Solve
    for (int i = 0; i < solution.length(); ++i)
    {
        char move = solution[i];

        switch (move)
        {
            case 'U':
                perform_a_move(back_motor, false);
                break;
            case 'u':
                perform_a_move(back_motor, true);
                break;
            case 'L':
                perform_a_move(left_motor, false);
                break;
            case 'l':
                perform_a_move(left_motor, true);
                break;
            case 'R':
                perform_a_move(right_motor, false);
                break;
            case 'r':
                perform_a_move(right_motor, true);
                break;
            case 'F':
                perform_a_move(top_motor, false);
                break;
            case 'f':
                perform_a_move(top_motor, true);
                break;
            default:
                break;
        }
    }

    // // End the program
    // wait(10, seconds);
    // Brain.programStop();
}

/////////////////////////////// LOCAL FUNCTIONS ///////////////////////////////

// Print status updates to brain
void print_status(std::string status)
{
    Brain.Screen.clearScreen();
    Brain.Screen.setCursor(2, 1);
    Brain.Screen.print("---SKEWBOT---");
    Brain.Screen.setCursor(3, 1);
    Brain.Screen.print(status.c_str());
}

// Get state from serial
std::string get_state_from_serial()
{
    char buffer[MAX_BUFFER_SIZE];

    while (true)
    {
        if (fgets(buffer, MAX_BUFFER_SIZE, stdin) != NULL)
        {
            buffer[strcspn(buffer, "\n\r")] = '\0';
            if (strlen(buffer) > 0)
                return std::string(buffer);
        }
        wait(20, msec);
    }
}

void perform_a_move(custom_motor &m, bool inverted)
{
    const double SINGLE_MOVE_ANGLE = 120.00;

    if(inverted)
        m.spin_degrees(-SINGLE_MOVE_ANGLE);
    else
        m.spin_degrees(SINGLE_MOVE_ANGLE);

    
}
