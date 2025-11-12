#include <iostream>
#include <array>
#include <fstream>
#include <cstring>
using namespace std;


void TransformDataIntoBin() {
    char Centers[6] = {'W', 'G', 'O', 'B', 'R', 'Y'};
    array<string, 8> Corners = {"UFR", "UFL", "UBL", "UBR", "DFR", "DFL", "DBL", "DBR"};
    char Moves[8] = {'L', 'l', 'R', 'r', 'U', 'u', 'F', 'f'};

    ifstream Data("Output.txt");
    if (!Data.is_open()) {
        cerr << "Error opening output file" << endl;
    }
    ofstream DataOut("SkewSolutions.bin", ios::binary);
    string centers;
    string cornerorientation;
    string history;
    string solution;

    array<string, 8> corners;

    while (Data >> centers) {
        uint32_t EncodedSolution = 0b0;
        uint64_t state = 0b0;
        for (int i = 0; i < 8; i++) {
            Data >> corners[i];
        }
        Data >> cornerorientation;

        if (cornerorientation == "00000000" && centers == "WGOBRY" && corners == Corners) {
            continue;
        } else {
            Data >> history;
            Data >> solution;
        }
        for (int i = 0; i < centers.size(); i++) {
            for (int j = 0; j < 6; j++) {
                if (centers[i] == Centers[j]) {
                    state |= (static_cast<uint64_t>(j) << (40 + (i * 3)));
                    break;
                }
            }
        }
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                if (corners[i] == Corners[j]) {
                    state |= (static_cast<uint64_t>(j) << (16 + (i * 3)));
                    break;
                }
            }
        }
        for (int i = 0; i < cornerorientation.size(); i++) {
            state |= (static_cast<uint32_t>(cornerorientation[i] - '0') << 2 * i);
        }
        cout << solution;
        for (int i = 0; i < solution.size(); i++) {
            for (int j = 0; j < 8; j++) {
                if (solution[i] == Moves[j]) {
                    EncodedSolution |= (static_cast<uint32_t>(j) << (i * 3));
                    break;
                }
            }
        }
        DataOut.write(reinterpret_cast<const char *>(&state), sizeof(state));
        DataOut.write(reinterpret_cast<const char *>(&EncodedSolution), sizeof(EncodedSolution));
    }
    DataOut.close();
}

int main() {
    TransformDataIntoBin();
    return 0;
}