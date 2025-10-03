#include <vector>
#include <queue>
#include <fstream>
#include <iostream>
#include <string>


struct skewb_state;


std::ofstream output("Output.txt", std::ios::app);
std::vector<std::string> history = {};
int depth = 0;


int main() {

    if (!output) {
        std::cout << "Error opening file" << std::endl;
        return 1;
    }
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
                    corner_permutations{UFR, UFL, UBL, UBR, DFR, DFL, DBL, DBR},
                    corner_orientations{0, 0, 0, 0, 0, 0, 0, 0} {
    }

    void reset() {
        *this = skewb_state();
    }

    [[nodiscard]] bool is_equal(const skewb_state& state) const {
        return this->center_colors == state.center_colors && this->corner_permutations == state.corner_permutations &&
               this->corner_orientations == state.corner_orientations;
    }

    void l_move(const bool inv) {
        if (inv)
            l_move(false);

        const CENTER temp = center_colors[1];
        center_colors[1] = center_colors[2];
        center_colors[2] = center_colors[5];
        center_colors[5] = temp;

        const CORNER temp_c = corner_permutations[1];
        corner_permutations[1] = corner_permutations[6];
        corner_permutations[6] = corner_permutations[4];
        corner_permutations[4] = temp_c;

        corner_orientations[1] -= 1;
        corner_orientations[4] -= 1;
        corner_orientations[5] -= 1;
        corner_orientations[6] -= 1;

        normalize_orientations();
    }

    void r_move(const bool inv) {
        if (inv)
            r_move(false);

        const CENTER temp = center_colors[3];
        center_colors[3] = center_colors[4];
        center_colors[4] = center_colors[5];
        center_colors[5] = temp;

        const CORNER temp_c = corner_permutations[3];
        corner_permutations[3] = corner_permutations[4];
        corner_permutations[4] = corner_permutations[6];
        corner_permutations[6] = temp_c;

        corner_orientations[3] -= 1;
        corner_orientations[4] -= 1;
        corner_orientations[6] -= 1;
        corner_orientations[7] -= 1;

        normalize_orientations();
    }

    void u_move(const bool inv) {
        if (inv)
            u_move(false);

        const CENTER temp = center_colors[0];
        center_colors[0] = center_colors[3];
        center_colors[3] = center_colors[2];
        center_colors[2] = temp;

        const CORNER temp_c = corner_permutations[1];
        corner_permutations[1] = corner_permutations[3];
        corner_permutations[3] = corner_permutations[6];
        corner_permutations[6] = temp_c;

        corner_orientations[1] -= 1;
        corner_orientations[2] -= 1;
        corner_orientations[3] -= 1;
        corner_orientations[6] -= 1;

        normalize_orientations();
    }

    void b_move(const bool inv) {
        if (inv)
            b_move(false);

        const CENTER temp = center_colors[2];
        center_colors[2] = center_colors[3];
        center_colors[3] = center_colors[6];
        center_colors[6] = temp;

        const CORNER temp_c = corner_permutations[2];
        corner_permutations[2] = corner_permutations[7];
        corner_permutations[7] = corner_permutations[5];
        corner_permutations[5] = temp_c;

        corner_orientations[2] -= 1;
        corner_orientations[5] -= 1;
        corner_orientations[6] -= 1;
        corner_orientations[7] -= 1;

        normalize_orientations();
    }

    void normalize_orientations() {
        for (int & corner_orientation : corner_orientations) {
            int orientation = corner_orientation;

            orientation %= 3;

            if (orientation < 0)
                orientation += 3;

            corner_orientation = orientation;
        }
    }
};


std::queue<skewb_state> q;
void BFS(int depth, skewb_state CurrentState, std::vector<std::string> history) {

}
