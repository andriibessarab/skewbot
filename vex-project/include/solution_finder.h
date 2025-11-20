//----------------------------------------------------------------------------
//
//  Module:       solution_finder.h
//  Description:  Header for module that finds solution on SD card
//  Authors:      MTE 100 & 121 Group 15
//
//----------------------------------------------------------------------------

#ifndef SOLUTION_FINDER_H
#define SOLUTION_FINDER_H

#pragma once

#include "vex.h"
#include "robot_config.h"

// --- Constants ---
const int CUBE_STATE_LEN = 38;
const int ORIENTATION_START_IDX = 40;
const int ORIENTATION_LEN = 8;
const int8_t STATE_ARRAY_SIZE = 9;
const int SOLUTION_BUFFER_SIZE = 256;
const int FILE_BUFFER_SIZE = 32768;
const int SCREEN_UPDATE_FREQ = 1000;
const int NUM_CENTERS = 6;
const int NUM_CORNERS = 8;
const int MAX_SOLUTION_STEPS = 11; // Loop limit for decoding
const int CENTER_BIT_OFFSET = 40;
const int CENTER_BIT_STEP = 3;
const int CORNER_BIT_OFFSET = 16;
const int CORNER_BIT_STEP = 3;
const int ORIENT_BIT_STEP = 2;
const int SOLUTION_BIT_STEP = 4;
const int MASK_3_BIT = 0b111;
const int MASK_2_BIT = 0b11;
const int MASK_4_BIT = 0b1111;
const char CENTER_COLORS[6] = {'W', 'G', 'O', 'B', 'R', 'Y'};
const std::array<std::string, 8> CORNER_NAMES = {"UFR", "UFL", "UBL", "UBR", "DFR", "DFL", "DBL", "DBR"};
const char MOVE_CHARS[9] = {' ', 'L', 'l', 'R', 'r', 'U', 'u', 'F', 'f'};

// ---Struct---
// Struct that holds the state and the solution encoding
struct state_solution
{
    uint64_t cur_state = 0;
    uint64_t encoded_solution = 0;
};

// Searches a binary file for a matching cube state and returns the solution move sequence.
std::string find_solution(std::string stringified_state_struct, FILE *file);

#endif // SOLUTION_FINDER_H