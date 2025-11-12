#include <array>
#include <iostream>
#include <fstream>
using namespace std;


void BinDecoder() {
    char Centers[6] = {'W', 'G', 'O', 'B', 'R', 'Y'};
    array<string, 8> Corners = {"UFR", "UFL", "UBL", "UBR", "DFR", "DFL", "DBL", "DBR"};
    char Moves[8] = {'L', 'l', 'R', 'r', 'U', 'u', 'F', 'f'};
    ifstream DataIn("SkewSolutions.bin");
    int HOWMUCHYOUWANNAREAD = 10;
    for (int i = 0; i < HOWMUCHYOUWANNAREAD; i++) {
        uint64_t State = 0b0;
        uint32_t EncodedSolution = 0b0;
        string centers;
        DataIn.read(reinterpret_cast<char *>(&State), sizeof(State));
        DataIn.read(reinterpret_cast<char *>(&EncodedSolution), sizeof(EncodedSolution));
        uint8_t current = 0b0;
        for (int i = 0; i < 6; i++) {
            current = (State >> (40 + (i * 3))) & 0b111;
            cout << Centers[current];
        }
        cout << " ";
        for (int i = 0; i < 8; i++) {
            current = (State >> (16 + (i * 3))) & 0b111;
            cout << Corners[current] << " ";
        }
        cout << " ";
        for (int i = 0; i < 8; i++) {
            current = (State >> (2 * i)) & 0b11;
            cout << static_cast<int>(current);
        }
        cout << " ";
        for (int i = 0; i < 11; i++) {
            current = (EncodedSolution >> (i * 3)) & 0b111;
            cout << Moves[current];
        }
        cout << endl;
    }
    DataIn.close();
}
int main() {
    BinDecoder();
    return 0;
}