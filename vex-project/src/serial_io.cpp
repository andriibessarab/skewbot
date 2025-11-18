//----------------------------------------------------------------------------
//
//  Module:       serial_io.cpp
//  Description:  Read colour string from serial
//
//----------------------------------------------------------------------------

#include "serial_io.h"
#include "vex.h"

using namespace vex;

std::string get_state_from_serial() {
    char buffer[MAX_BUFFER_SIZE] = {};
    std::string result_state = "";
    bool state_found = false;

    // Loop as long as a valid state hasn't been found
    while (!state_found)
    {
        // Attempt to read data from the serial port (simulated via stdin)
        if (fgets(buffer, MAX_BUFFER_SIZE, stdin) != NULL)
        {
            // Data was received successfully
            
            // Remove trailing newline/carriage return characters
            buffer[strcspn(buffer, "\n\r")] = '\0';
            
            // Check if the received string is not empty
            if (strlen(buffer) > 0)
            {
                // We found a valid statee! Store it and set the flag to exit the loop.
                result_state = std::string(buffer);
                state_found = true;
            }
        }
        
        // Only wait if a state wasn't found in the current iteration
        if (!state_found)
        {
            wait(20, msec); // Wait for the specified time
        }
    }

    // Return the found state once the loop terminates
    return result_state;
}
