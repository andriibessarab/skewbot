#include <fstream>
#include <iostream>
#include <queue>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

std::ofstream output("Output.txt");

std::string history = "";
std::unordered_map<std::string, std::string> history_map;
long TotalReduced = 0;

enum CENTER { W, G, O, B, R, Y };

enum CORNER { UFL, UBL, UBR, UFR, DFL, DBL, DBR, DFR };

struct skewb_state {
    std::vector<CENTER> center_colors;

    std::vector<CORNER> corner_permutations;

    std::unordered_map<CORNER, int> corner_orientations;

    skewb_state()
        : center_colors{W, G, O, B, R, Y},
          corner_permutations{UFR, UFL, UBL, UBR, DFR, DFL, DBL, DBR},
          corner_orientations{
              {UFR, 0}, {UFL, 0}, {UBL, 0}, {UBR, 0},
              {DFR, 0}, {DFL, 0}, {DBL, 0}, {DBR, 0}
          } {
    }

    void reset() { *this = skewb_state(); }

    skewb_state copy() const {
        skewb_state result;
        result.center_colors = this->center_colors;
        result.corner_permutations = this->corner_permutations;
        result.corner_orientations = this->corner_orientations;
        return result;
    }

    [[nodiscard]] bool is_equal(const skewb_state &state) const {
        return this->center_colors == state.center_colors &&
               this->corner_permutations == state.corner_permutations &&
               this->corner_orientations == state.corner_orientations;
    }

    void l_move(const bool inv) {
        if (inv) l_move(false);

        const CENTER temp = center_colors[1];
        center_colors[1] = center_colors[2];
        center_colors[2] = center_colors[5];
        center_colors[5] = temp;

        const CORNER temp_c = corner_permutations[1];
        corner_permutations[1] = corner_permutations[6];
        corner_permutations[6] = corner_permutations[4];
        corner_permutations[4] = temp_c;

        corner_orientations[corner_permutations[1]] += 2;
        corner_orientations[corner_permutations[4]] += 2;
        corner_orientations[corner_permutations[5]] += 2;
        corner_orientations[corner_permutations[6]] += 2;

        normalize_orientations();
    }

    void r_move(const bool inv) {
        if (inv) r_move(false);

        const CENTER temp = center_colors[3];
        center_colors[3] = center_colors[4];
        center_colors[4] = center_colors[5];
        center_colors[5] = temp;

        const CORNER temp_c = corner_permutations[3];
        corner_permutations[3] = corner_permutations[4];
        corner_permutations[4] = corner_permutations[6];
        corner_permutations[6] = temp_c;

        corner_orientations[corner_permutations[3]] += 2;
        corner_orientations[corner_permutations[4]] += 2;
        corner_orientations[corner_permutations[6]] += 2;
        corner_orientations[corner_permutations[7]] += 2;

        normalize_orientations();
    }

    void u_move(const bool inv) {
        if (inv) u_move(false);

        const CENTER temp = center_colors[0];
        center_colors[0] = center_colors[3];
        center_colors[3] = center_colors[2];
        center_colors[2] = temp;

        const CORNER temp_c = corner_permutations[1];
        corner_permutations[1] = corner_permutations[3];
        corner_permutations[3] = corner_permutations[6];
        corner_permutations[6] = temp_c;

        corner_orientations[corner_permutations[1]] += 2;
        corner_orientations[corner_permutations[2]] += 2;
        corner_orientations[corner_permutations[3]] += 2;
        corner_orientations[corner_permutations[6]] += 2;

        normalize_orientations();
    }

    void f_move(const bool inv) {
        if (inv) f_move(false);

        const CENTER temp = center_colors[0];
        center_colors[0] = center_colors[1];
        center_colors[1] = center_colors[4];
        center_colors[4] = temp;

        const CORNER temp_c = corner_permutations[1];
        corner_permutations[1] = corner_permutations[4];
        corner_permutations[4] = corner_permutations[3];
        corner_permutations[3] = temp_c;


        corner_orientations[corner_permutations[0]] += 2;
        corner_orientations[corner_permutations[1]] += 2;
        corner_orientations[corner_permutations[3]] += 2;
        corner_orientations[corner_permutations[4]] += 2;

        normalize_orientations();
    }

    void normalize_orientations() {
        for (auto &element: corner_orientations) {
            int &orientation_value = element.second;
            orientation_value %= 3;
        }
    }
};

std::string center_to_string(CENTER c) {
    switch (c) {
        case W:
            return "W";
        case G:
            return "G";
        case O:
            return "O";
        case B:
            return "B";
        case R:
            return "R";
        case Y:
            return "Y";
    }
    return "?";
}

std::string corner_to_string(CORNER c) {
    switch (c) {
        case UFL:
            return "UFL";
        case UBL:
            return "UBL";
        case UBR:
            return "UBR";
        case UFR:
            return "UFR";
        case DFL:
            return "DFL";
        case DBL:
            return "DBL";
        case DBR:
            return "DBR";
        case DFR:
            return "DFR";
    }
    return "?";
}

std::string get_solution(std::string history) {
    std::string solution = "";
    for (int i = history.size() - 1; i >= 0; i--) {
        // makes char Uppercase if lower, and lower if upper (inverted moves)
        std::isupper(history[i])
            ? solution += std::tolower(history[i])
            : solution += std::toupper(history[i]);
    }
    return solution;
}

std::queue<std::tuple<skewb_state, char, std::string, int> > q;

void BFS() {
    while (!q.empty()) {
        auto [CurrentState, lastmove, history, depth] = q.front();
        q.pop();
        if (depth > 11) {
            continue;
        }
        std::string solut;
        std::string scramble;

        if (lastmove == 'U') {
            CurrentState.u_move(false);
        } else if (lastmove == 'u') {
            CurrentState.u_move(true);
        } else if (lastmove == 'L') {
            CurrentState.l_move(false);
        } else if (lastmove == 'l') {
            CurrentState.l_move(true);
        } else if (lastmove == 'R') {
            CurrentState.r_move(false);
        } else if (lastmove == 'r') {
            CurrentState.r_move(true);
        } else if (lastmove == 'F') {
            CurrentState.f_move(false);
        } else if (lastmove == 'f') {
            CurrentState.f_move(true);
        }
        for (int i = 0; i < CurrentState.center_colors.size(); i++) {
            scramble += center_to_string(CurrentState.center_colors[i]);
        }
        scramble += " ";
        for (int i = 0; i < CurrentState.corner_permutations.size(); i++) {
            scramble += corner_to_string(CurrentState.corner_permutations[i]) + " ";
        }

        scramble += " ";

        for (int i = 0; i < CurrentState.corner_orientations.size(); i++) {
            scramble += std::to_string(
                CurrentState
                .corner_orientations[CurrentState.corner_permutations[i]]);
        }

        solut += history + " " + get_solution(history);
        if (history_map.find(scramble) == history_map.end()) {
            history_map[scramble] = solut;
        } else {
            TotalReduced++;
            continue;
        }

        for (char i: {'R', 'L', 'U', 'F'}) {
            if (i != static_cast<char>(std::toupper(lastmove))) {
                q.push(std::make_tuple(CurrentState.copy(), i, history + i,
                                       depth + 1));
                q.push(std::make_tuple(
                    CurrentState.copy(), static_cast<char>(std::tolower(i)),
                    history + static_cast<char>(std::tolower(i)), depth + 1));
            }
        }
    }
}

int main() {
    if (!output) {
        std::cout << "Error opening file" << std::endl;
        return 1;
    }

    skewb_state InitialState = skewb_state();
    q.push(std::make_tuple(InitialState, ' ', "", 0));
    BFS();
    int total = 0;
    for (auto i = history_map.begin(); i != history_map.end(); i++) {
        output << i->first << " " << i->second << std::endl;
        total++;
    }
    std::cout << "Total Amount Reduced: " << TotalReduced << std::endl;
    std::cout << "Total States(should be 3149280):  " << total<< std::endl;
    output.close();

    return 0;
}
