//----------------------------------------------------------------------------
//
//  Module:       state_converter.h
//  Description:  Header for module that has everything needed to convert 
//                colour string to stringified struct in correct orientation.
//  Authors:      MTE 100 & 121 Group 15
//
//----------------------------------------------------------------------------

#ifndef STATE_CONVERTER_H
#define STATE_CONVERTER_H

#pragma once

#include "robot_config.h"

// --- Skewb State Representation ---
enum CENTER { W, G, O, B, R, Y };
enum CORNER { UFL, UBL, UBR, UFR, DFL, DBL, DBR, DFR };

// --- Structs ---
struct skewb_state
{
    std::vector<CENTER> center_colors;
    std::vector<CORNER> corner_permutations;
    std::unordered_map<CORNER, int> corner_orientations;
};

// --- Constants ---
const std::vector<std::array<int, 3>> corners = {
    {3, 7, 11},   // 0: UFR
    {4, 6, 22},   // 1: UFL
    {2, 16, 12},  // 2: UBR
    {1, 17, 21},  // 3: UBL
    {27, 8, 14},  // 4: DFR
    {26, 9, 23},  // 5: DFL
    {28, 19, 13}, // 6: DBR
    {29, 18, 24}  // 7: DBL
};
const std::unordered_map<int, int> indexes_map = {
    {0, 0},
    {1, 1},
    {2, 3},
    {3, 2},
    {4, 4},
    {5, 5},
    {6, 7},
    {7, 6}
};
// Helper maps for conversion b/w scanned state and needed cpp struct
const std::unordered_map<char, CENTER> char_to_center = {
    {'W', W}, {'G', G}, {'O', O}, {'B', B}, {'R', R}, {'Y', Y}
};
const std::unordered_map<std::string, CORNER> colors_to_corner = {
    {"GRW", UFR}, // W,R,G
    {"GOW", UFL}, // W,G,O
    {"BRW", UBR}, // W,B,R
    {"BOW", UBL}, // W,O,B
    {"GRY", DFR}, // Y,G,R
    {"GOY", DFL}, // Y,O,G
    {"BRY", DBR}, // Y,R,B
    {"BOY", DBL}  // Y,B,O
};

// --- Function Prototypes ---
std::string flip_orientation(std::string state);
std::string normalize_position(std::string state);
skewb_state convert_state_to_struct(const std::string& state);
std::string skewb_state_to_string(const skewb_state &state, bool include_orientations);
int get_position_of_inertial_corner(const std::string& state);
int get_orientation_of_inertial_corner(const std::string& state, int pos);
std::string state_rotate_y(std::string state);
std::string state_rotate_x(std::string state);
std::string center_to_string(CENTER c);
std::string corner_to_string(CORNER c);
std::string find_normalized_stringified_struct(std::string state_string);

#endif STATE_CONVERTER_H
