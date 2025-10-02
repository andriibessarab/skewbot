#include <vector>
#include <queue>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>


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
                    corner_permutations{UFL, UBL, UBR, UFR, DFL, DBL, DBR, DFR},
                    corner_orientations{0, 0, 0, 0, 0, 0, 0, 0} {
    }

    void reset() {
        *this = skewb_state();
    }


    bool isEqual(const skewb_state state) const {
        return this->center_colors == state.center_colors && this->corner_permutations == state.corner_permutations && this->corner_orientations == state.corner_orientations;


    }
    void Right(int turn) {

    }
    void Left(int turn) {

    }
    void Back(int turn) {//inv

    }
    void Up(int turn) {

    }
};

std::queue<skewb_state> q;

void BFS(int depth, skewb_state CurrentState, std::vector<std::string> history) {




}

