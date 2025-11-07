#include "state_converter.h"
#include <algorithm>
#include <cstring>

// --- Constants ---
const int MAX_BUFFER_SIZE = 100;

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

std::unordered_map<int, int> indexes_map = {
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
std::unordered_map<char, CENTER> char_to_center = {
    {'W', W}, {'G', G}, {'O', O}, {'B', B}, {'R', R}, {'Y', Y}
};

std::unordered_map<std::string, CORNER> colors_to_corner = {
    {"GRW", UFR}, // W,R,G
    {"GOW", UFL}, // W,G,O
    {"BRW", UBR}, // W,B,R
    {"BOW", UBL}, // W,O,B
    {"GRY", DFR}, // Y,G,R
    {"GOY", DFL}, // Y,O,G
    {"BRY", DBR}, // Y,R,B
    {"BOY", DBL}  // Y,B,O
};

// --- Functions ---
// Find pos. of WGR corner on scanned config
int get_position_of_inertial_corner(const std::string& state)
{
    // Check all 8 corners
    for (int i = 0; i < corners.size(); ++i)
    {
        // Sort them alphabetically and check if
        // the corner has all three colours.
        const std::array<int, 3>& corner_indices = corners[i];

        // Access the std::string using the [] operator,
        // which works just like accessing a char array.
        char c1 = state[corner_indices[0]];
        char c2 = state[corner_indices[1]];
        char c3 = state[corner_indices[2]];

        std::array<char, 3> chars = {c1, c2, c3};
        std::sort(chars.begin(), chars.end());

        if (chars[0] == 'G' && chars[1] == 'R' && chars[2] == 'W')
            return i;
    }
    return -1;
}

// Find orient. of WGR corner on scanned config
int get_orientation_of_inertial_corner(const std::string& state, int pos)
{
    const std::array<int, 3>& indices = corners[pos];

    // Check which of the 3 stickers is W
    if (state[indices[0]] == 'W')
        return 0;   // Up
    else if (state[indices[1]] == 'W')
        return 1;   // Front
    else
        return 2;   // Right
}

// State Y rotation
// F -> R -> B -> L -> F
std::string state_rotate_y(std::string state)
{
    std::string new_state = "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";

    new_state[0] = state[0];
    new_state[1] = state[2];
    new_state[2] = state[3];
    new_state[3] = state[4];
    new_state[4] = state[1];
    new_state[25] = state[25];
    new_state[26] = state[29];
    new_state[27] = state[26];
    new_state[28] = state[27];
    new_state[29] = state[28];

    for(int i=0; i<5; ++i)
    {
        new_state[5+i] = state[20+i];
        new_state[10+i] = state[5+i];
        new_state[15+i] = state[10+i];
        new_state[20+i] = state[15+i];
    }
    return new_state;
}

// State X rotation
// U -> F -> D -> B -> U
std::string state_rotate_x(std::string state)
{
    std::string new_state = "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";

    new_state[0] = state[15];
    new_state[1] = state[18];
    new_state[2] = state[19];
    new_state[3] = state[16];
    new_state[4] = state[17];
    new_state[10] = state[10];
    new_state[11] = state[12];
    new_state[12] = state[13];
    new_state[13] = state[14];
    new_state[14] = state[11];
    new_state[15] = state[25];
    new_state[16] = state[28];
    new_state[17] = state[29];
    new_state[18] = state[26];
    new_state[19] = state[27];
    new_state[20] = state[20];
    new_state[21] = state[24];
    new_state[22] = state[21];
    new_state[23] = state[22];
    new_state[24] = state[23];

    for(int i=0; i<5; ++i)
    {
        new_state[5+i] = state[0+i];
        new_state[25+i] = state[5+i];
    }
    return new_state;
}

// Normalize scanned state
std::string normalize_state(std::string state)
{
    int inert_corner_pos = get_position_of_inertial_corner(state);
    // if inert corner at the bottom, put it up top
    if (inert_corner_pos >= 4)
    {
        state = state_rotate_x(state_rotate_x(state));

        // update position
        inert_corner_pos = get_position_of_inertial_corner(state);
    }

    // move inert corner to the FTR spot
    if (inert_corner_pos == 1)
        state = state_rotate_y(state_rotate_y(state_rotate_y(state)));
    else if (inert_corner_pos == 2)
        state = state_rotate_y(state);
    else if (inert_corner_pos == 3)
        state = state_rotate_y(state_rotate_y(state));
    
    // it's now in correct pos, but could be facing wrogng way
    // get orientation and rotate accordingly
    int inert_corner_orient = get_orientation_of_inertial_corner(state, 0);
    if(inert_corner_orient == 1)
        state = state_rotate_x(state_rotate_y(state_rotate_x(state_rotate_y(state))));
    else if (inert_corner_orient == 2)
        state = state_rotate_x(state_rotate_y(state));
    
    return state;
}

// Convert normalized scanned state to cpp struct
skewb_state convert_state_to_struct(const std::string& state)
{
    skewb_state new_skewb_state;
    new_skewb_state.corner_permutations.resize(8);

    // centers
    // U, F, L, B, R, D
    new_skewb_state.center_colors.push_back(char_to_center.at(state[0]));
    new_skewb_state.center_colors.push_back(char_to_center.at(state[5]));
    new_skewb_state.center_colors.push_back(char_to_center.at(state[20]));
    new_skewb_state.center_colors.push_back(char_to_center.at(state[15]));
    new_skewb_state.center_colors.push_back(char_to_center.at(state[10]));
    new_skewb_state.center_colors.push_back(char_to_center.at(state[25]));

    // corners
    for (int i = 0; i < 8; ++i)
    {
        // indexes map needed due to unconvential order
        const auto& indices = corners[indexes_map[i]];

        // three colors of the given corner
        char c1 = state[indices[0]];
        char c2 = state[indices[1]];
        char c3 = state[indices[2]];

        // find which piece
        // order alphabetically and use map
        std::array<char, 3> chars = {c1, c2, c3};
        std::sort(chars.begin(), chars.end());
        std::string color_key = "";
        color_key += chars[0];
        color_key += chars[1];
        color_key += chars[2];
        CORNER piece = colors_to_corner.at(color_key);
    
        new_skewb_state.corner_permutations[i] = piece;

        // find orientation
        // 0 = W/Y on U/D, 1 = W/Y on F/B, 2 = W/Y on L/R
        for (int j = 0; j < 3; ++j)
        {
            char sticker_color = state[indices[j]];
            if (sticker_color == 'W' || sticker_color == 'Y')
                new_skewb_state.corner_orientations[piece] = j;
        }
    }

    return new_skewb_state;
}

// --- String Conversion Helpers ---

std::string center_to_string(CENTER c) {
    switch (c)
    {
        case W: return "W";
        case G: return "G";
        case O: return "O";
        case B: return "B";
        case R: return "R";
        case Y: return "Y";
    }
    return "?";
}

std::string corner_to_string(CORNER c)
{
    switch (c)
    {
        case UFR: return "UFR";
        case UFL: return "UFL";
        case UBL: return "UBL";
        case UBR: return "UBR";
        case DFR: return "DFR";
        case DFL: return "DFL";
        case DBL: return "DBL";
        case DBR: return "DBR";
    }
    return "?";
}

std::string skewb_state_to_string(const skewb_state& state)
{
    std::string state_key = "";

    // centers
    for (int i = 0; i < state.center_colors.size(); i++)
    {
        state_key += center_to_string(state.center_colors[i]);
    }

    state_key += " ";

    // permutations
    for (int i = 0; i < state.corner_permutations.size(); i++)
    {
        state_key += corner_to_string(state.corner_permutations[i]) + " ";
    }

    state_key += " ";

    // orientations
    for (int i = 0; i < state.corner_permutations.size(); i++)
    {
        CORNER piece_at_position_i = state.corner_permutations[i];
        state_key += std::to_string(state.corner_orientations.at(piece_at_position_i));
    }

    return state_key;
}
