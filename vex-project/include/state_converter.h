#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <array>

// --- Skewb State Representation ---
enum CENTER { W, G, O, B, R, Y };
enum CORNER { UFL, UBL, UBR, UFR, DFL, DBL, DBR, DFR };

struct skewb_state
{
    std::vector<CENTER> center_colors;
    std::vector<CORNER> corner_permutations;
    std::unordered_map<CORNER, int> corner_orientations;
};

// --- Constants ---
extern const std::vector<std::array<int, 3>> corners;
extern std::unordered_map<int, int> indexes_map;
extern std::unordered_map<char, CENTER> char_to_center;
extern std::unordered_map<std::string, CORNER> colors_to_corner;

// --- Function Prototypes ---
std::string normalize_state(std::string state);
skewb_state convert_state_to_struct(const std::string& state);
std::string skewb_state_to_string(const skewb_state& state);
int get_position_of_inertial_corner(const std::string& state);
int get_orientation_of_inertial_corner(const std::string& state, int pos);
std::string state_rotate_y(std::string state);
std::string state_rotate_x(std::string state);
std::string center_to_string(CENTER c);
std::string corner_to_string(CORNER c);
