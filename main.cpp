#include <vector>
#include <queue>
#include <fstream>
#include <iostream>
#include <string>


std::ofstream output("Output.txt", std::ios::app);


std::string history = "";

int depth = 0;




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




    skewb_state copy() const {
        skewb_state result;
        result.center_colors = this->center_colors;
        result.corner_permutations = this->corner_permutations;
        result.corner_orientations = this->corner_orientations;
        return result;
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


std::string center_to_string(CENTER c) {
    switch (c) {
        case W: return "W";
        case G: return "G";
        case O: return "O";
        case B: return "B";
        case R: return "R";
        case Y: return "Y";
    }
    return "?";
}

std::string corner_to_string(CORNER c) {
    switch (c) {
        case UFL: return "UFL";
        case UBL: return "UBL";
        case UBR: return "UBR";
        case UFR: return "UFR";
        case DFL: return "DFL";
        case DBL: return "DBL";
        case DBR: return "DBR";
        case DFR: return "DFR";
    }
    return "?";
}

std::queue<std::tuple<skewb_state, char, std::string> > q;


void BFS(int depth, skewb_state CurrentState, std::string solution, char LastSolutionMove) {
    if (depth > 11) {
        return;
    }
    //NOTE: The moves are purposefully inverted as the inverse of the scramble should be the solution
    //and the inverse of the solution should be the actual scramble
    if (LastSolutionMove == 'U') {
        CurrentState.r_move(true);
    } else if (LastSolutionMove == 'u') {
        CurrentState.r_move(false);
    } else if (LastSolutionMove == 'L') {
        CurrentState.l_move(true);
    } else if (LastSolutionMove == 'l') {
        CurrentState.l_move(false);
    } else if (LastSolutionMove == 'R') {
        CurrentState.r_move(true);
    } else if (LastSolutionMove == 'r') {
        CurrentState.r_move(false);
    } else if (LastSolutionMove == 'B') {
        CurrentState.b_move(true);
    } else if (LastSolutionMove == 'b') {
        CurrentState.b_move(false);
    }
    for (int i = 0; i < CurrentState.center_colors.size(); i++) {
        output << center_to_string(CurrentState.center_colors[i]);
    }
    output << " ";
    for (int i = 0; i < CurrentState.corner_permutations.size(); i++) {
        output << corner_to_string(CurrentState.corner_permutations[i]);
    }
    output << " ";
    for (int i = 0; i < CurrentState.corner_orientations.size(); i++) {
        output << CurrentState.corner_orientations[i];
    }
    output << " ";
    output << solution << std::endl;
    if (LastSolutionMove != 'R' && LastSolutionMove != 'r') {
        q.push(std::make_tuple(CurrentState.copy(), 'R', solution + 'R'));
        q.push(std::make_tuple(CurrentState.copy(), 'r', solution + 'r'));
    }
    if (LastSolutionMove != 'L' && LastSolutionMove != 'l') {
        q.push(std::make_tuple(CurrentState.copy(), 'L', solution + 'L'));
        q.push(std::make_tuple(CurrentState.copy(), 'l', solution + 'l'));
    }
    if (LastSolutionMove != 'U' && LastSolutionMove != 'u') {
        q.push(std::make_tuple(CurrentState.copy(), 'U', solution + 'U'));
        q.push(std::make_tuple(CurrentState.copy(), 'u', solution + 'u'));
    }
    if (LastSolutionMove != 'B' && LastSolutionMove != 'b') {
        q.push(std::make_tuple(CurrentState.copy(), 'B', solution + 'B'));
        q.push(std::make_tuple(CurrentState.copy(), 'b', solution + 'b'));
    }
    if (!q.empty()) {
        std::tuple<skewb_state, char, std::string> LastQueue = q.front();
        q.pop();
        BFS(std::get<2>(LastQueue).size(), std::get<0>(LastQueue), std::get<2>(LastQueue), std::get<1>(LastQueue));
    }

}


int main() {
    if (!output) {
        std::cout << "Error opening file" << std::endl;
        return 1;
    }

    skewb_state InitialState = skewb_state();
    BFS(0, InitialState, "", ' ');

    return 0;
}

