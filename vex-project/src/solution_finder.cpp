//----------------------------------------------------------------------------
//
//  Module:       solution_finder.cpp
//  Description:  Find solution on SD card
//  Authors:      MTE 100 & 121 Group 15
//
//----------------------------------------------------------------------------

#include "solution_finder.h"

std::string find_solution(std::string stringified_state_struct, FILE *file)
{
    const std::string cube_state = stringified_state_struct.substr(0, CUBE_STATE_LEN);
    const std::string needed_orientation = stringified_state_struct.substr(ORIENTATION_START_IDX, ORIENTATION_LEN);

    static char file_buffer_raw[FILE_BUFFER_SIZE] = {0};
    setvbuf(file, file_buffer_raw, _IOFBF, sizeof(file_buffer_raw));

    state_solution buffer[SOLUTION_BUFFER_SIZE] = {};
    std::string state[STATE_ARRAY_SIZE] = {};
    
    int8_t prev_sub_index = 0;
    int8_t index_to_push = 0;
    bool found = false;
    bool reading_complete = false;
    
    std::string solution = "";
    int counter = 0;

    // Parsing Input String
    for (int8_t i = 0; i < cube_state.length() && index_to_push < NUM_CORNERS; i++)
    {
        if (cube_state[i] == ' ')
        {
            state[index_to_push] = cube_state.substr(prev_sub_index, i - prev_sub_index);
            prev_sub_index = i + 1;
            index_to_push++;

            if (index_to_push == NUM_CORNERS)
            {
                state[NUM_CORNERS] = cube_state.substr(prev_sub_index);
            }
        }
    }

    // Main Search Loop
    while (!found && !reading_complete)
    {
        counter++;
        Brain.Screen.setCursor(9, 1);

        if (counter % SCREEN_UPDATE_FREQ == 0)
        {
            Brain.Screen.setCursor(8, 1);
            Brain.Screen.clearLine();
            Brain.Screen.print("%.2f%% checked", ((counter * SOLUTION_BUFFER_SIZE * 100) / 3149279.0));
            
        }

        size_t read_count = fread(buffer, sizeof(state_solution), SOLUTION_BUFFER_SIZE, file);

        if (read_count == 0)
        {
            reading_complete = true;
        }
        else
        {
            // Process the chunk we just read
            for (size_t i = 0; i < read_count && !found; i++)
            {
                bool is_eliminated = false;
                std::string centers_str = "";
                std::string corners_str = "";
                int current = 0;

                // Check Centers
                for (int j = 0; j < NUM_CENTERS && !is_eliminated; j++)
                {
                    current = (buffer[i].cur_state >> (CENTER_BIT_OFFSET + (j * CENTER_BIT_STEP))) & MASK_3_BIT;
                    centers_str += CENTER_COLORS[current];
                    
                    if (CENTER_COLORS[current] != state[0][j])
                    {
                        is_eliminated = true;
                    }
                }

                // Check Corners (Only runs if centers were valid)
                if (!is_eliminated)
                {
                    for (int j = 0; j < NUM_CORNERS && !is_eliminated; j++)
                    {
                        current = (buffer[i].cur_state >> (CORNER_BIT_OFFSET + (j * CORNER_BIT_STEP))) & MASK_3_BIT;
                        corners_str += CORNER_NAMES[current] + " ";
                        
                        if (CORNER_NAMES[current] != state[1 + j])
                        {
                            is_eliminated = true;
                        }
                    }
                }

                // Check Orientation and Decode (Only runs if corners were valid)
                if (!is_eliminated)
                {
                    std::string orientation = "";
                    for (int j = 0; j < NUM_CORNERS; j++)
                    {
                        current = (buffer[i].cur_state >> (ORIENT_BIT_STEP * j)) & MASK_2_BIT;
                        orientation += '0' + current;
                    }

                    if (orientation == needed_orientation)
                    {
                        Brain.Screen.setCursor(8, 1);
                        Brain.Screen.clearLine();
                        Brain.Screen.print("solution found");
                        wait(2, seconds);
                        Brain.Screen.clearLine();

                        found = true;
                        
                        bool decoding_done = false;
                        for (int j = 0; j < MAX_SOLUTION_STEPS && !decoding_done; j++)
                        {
                            current = (buffer[i].encoded_solution >> (j * SOLUTION_BIT_STEP)) & MASK_4_BIT;
                            if (current == 0)
                            {
                                decoding_done = true;
                            }
                            else 
                            {
                                solution += MOVE_CHARS[current];
                            }
                        }
                    }
                }
            }

            // Check if we finished the file without finding anything
            if (read_count != SOLUTION_BUFFER_SIZE && !found)
            {
                Brain.Screen.setCursor(8, 1);
                Brain.Screen.clearLine();
                Brain.Screen.print("solution not found");
                wait(2, seconds);
                Brain.Screen.clearLine();
                reading_complete = true;
            }
        }
    }

    return solution;
}
