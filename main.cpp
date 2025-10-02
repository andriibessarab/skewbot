#include <vector>

int main() {
    return 0;
}

enum CENTER {
    W,
    G,
    O,
    B,
    R,
    Y
};

enum CORNER {
    UFL,
    UBL,
    UBR,
    UFR,
    DFL,
    DBL,
    DBR,
    DFR
};

struct skewb_state {
    std::vector<CENTER> center_colors;

    std::vector<CORNER> corner_permutations;

    std::vector<int> corner_orientations;

    skewb_state() : center_colors{W, G, O, B, R, Y},
                    corner_permutations{UFL, UBL, UBR, UFR, DFL, DBL, DBR, DFR},
                    corner_orientations{0, 0, 0, 0, 0, 0, 0, 0} {
    }

    void reset() {
        *this = skewb_state();
    }
};
