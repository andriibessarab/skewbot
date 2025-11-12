//----------------------------------------------------------------------------
//
//  Module:       main.cpp
//  Description:  The main entrypoint
//
//----------------------------------------------------------------------------

// --- Info ---
// SCANNED STATE
// Index = (FaceIndex * 5) + StickerIndex
// Faces: U=0, F=1, R=2, B=3, L=4, D=5
// Stickers: C=0, TL=1, TR=2, BR=3, BL=4

#include "vex.h"
#include "robot_config.h"
#include "state_converter.h"



using namespace vex;

// --- Constants ---
const int MAX_BUFFER_SIZE = 100;

// --- Local Functions Prototypes ---
void print_status(std::string status);
std::string get_state_from_serial();
void FindSolutions(std::string CubeState);

/////////////////////////////// MAIN PROGRAM ///////////////////////////////
int main()
{
    back_motor.spin(fwd, 20, percent);
    // Initializing Robot Configuration
    vexcodeInit();

    print_status("Skewb Solver");

    // wait(2, seconds);
    // print_status("Awaiting state...");

    // // Get state from serial
    // std::string raw_state_string = get_state_from_serial();

    // print_status("Processing...");

    // // Process state
    // std::string normalized_state_string = normalize_state(raw_state_string);
    // skewb_state state_struct = convert_state_to_struct(normalized_state_string);
    // std::string state_struct_stringified = skewb_state_to_string(state_struct);
    // printf("\n%s\n", state_struct_stringified.c_str());

    // wait(2, seconds);
    // print_status("Thinking...");

    // // Obtain solution

    // wait(2, seconds);
    // print_status("Ready!");

    // // Wait for touch to start solve

    // // Check sensory data

    // // Solve

    // // End the program
    // wait(10, seconds);
    // Brain.programStop();
}

/////////////////////////////// LOCAL FUNCTIONS ///////////////////////////////

// Print status updates to brain
void print_status(std::string status)
{
    Brain.Screen.clearScreen();
    Brain.Screen.setCursor(2, 1);
    Brain.Screen.print("---SKEWBOT---");
    Brain.Screen.setCursor(3, 1);
    Brain.Screen.print(status.c_str());
}

// Get state from serial
std::string get_state_from_serial()
{
    char buffer[MAX_BUFFER_SIZE];

    while (true)
    {
        if (fgets(buffer, MAX_BUFFER_SIZE, stdin) != NULL)
        {
            buffer[strcspn(buffer, "\n\r")] = '\0';
            if (strlen(buffer) > 0)
                return std::string(buffer);
        }
        wait(20, msec);
    }
}

bool CheckOrientation(const std::string &Orientation);
void FindSolutions(const std::string &CubeState){
    const int StateSize = 9;
    std::string State[StateSize];
    int PrevSubIndex = 0;
    int IndexToPush = 0;
    bool found = false;
    const char Centers[6] = {'W', 'G', 'O', 'B', 'R', 'Y'};
    std::string Solution;
    const std::array<std::string, 8> Corners = {"UFR", "UFL", "UBL", "UBR", "DFR", "DFL", "DBL", "DBR"};
    const char Moves[8] = {'L', 'l', 'R', 'r', 'U', 'u', 'F', 'f'};
    for (int i = 0; i < CubeState.length(); i++){
        if (CubeState[i] == ' '){
            State[IndexToPush] = CubeState.substr(PrevSubIndex, i-PrevSubIndex);
            PrevSubIndex = i+1;
            IndexToPush++;
            if (IndexToPush == 8){
                State[8] = CubeState.substr(PrevSubIndex);
                //Get out of for loop cz no break...
                i = CubeState.length();
            }
        }
    }
    FILE* file = fopen("SkewSolutions.bin","rb");
    if (!file){
        Brain.Screen.print("Failed To Open SkewSolutions.bin");
        return;
    }
    while (!found){
        uint64_t CurState = 0b0;
        std::string centers;
        size_t Read1 = fread(&CurState, sizeof(CurState), 1, file);
        uint8_t current = 0b0;
        bool IsEliminated = false;
        if (Read1 != 1){
            found = true;
        }else{
        for (int i = 0; i < 6; i++) {
            current = (CurState >> (40 + (i * 3))) & 0b111;
            if (Centers[current] != State[0][i]){
                fseek(file,32,SEEK_CUR);
                IsEliminated = true;
                //get out of for loop cz no break...
                i = 6;
            }
        }
        if(IsEliminated){
            continue;
        }
        for (int i = 0; i < 8; i++) {
            current = (CurState >> (16 + (i * 3))) & 0b111;
            if(Corners[current] != State[1+i]){
                fseek(file,32,SEEK_CUR);
                IsEliminated = true;
                //get out of for loop cz no break...
                i = 8;
            }
        }
        if (IsEliminated){
            continue;
        }
        std::string Orienation;
        for (int i = 0; i < 8; i++) {
            current = (CurState >> (2 * i)) & 0b11;
            //convert 0,1,2,3,4,5 -> '0','1'...etc
            Orienation += '0' + current;
        }
        if(CheckOrientation(Orienation)){
            found = true;
            uint32_t EncodedSolution = 0b0;
            size_t ReadSuccessful = fread(&EncodedSolution, sizeof(EncodedSolution), 1, file);
            if(ReadSuccessful == 1){
                for (int i = 0; i < 11; i++) {
                current = (EncodedSolution >> (i * 3)) & 0b111;
                Solution += Moves[current];
        }
        }
    }else{
        fseek(file,32,SEEK_CUR);
    }
    
    


   
}
}
}