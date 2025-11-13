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
bool CheckOrientation(const std::string &Orientation);
std::string FindSolutions(const std::string &CubeState, FILE* file);
void perform_a_move(custom_motor &m, bool inverted);

/////////////////////////////// MAIN PROGRAM ///////////////////////////////

int main()
{
    // temp
    std::string TestString = "WBROYG UFR DBL UBL UBR UFL DFL DFR DBR";
  
    // Configure robot
    vexcode_init();
  
    // Open file
    FILE* file = fopen("SkewSolutions.bin", "rb");
  
    Brain.Screen.setFont(mono12);  // move to config
  
    wait(2, seconds);
    

    // Get state from serial
    print_status("Awaiting data...");
    std::string raw_state_string = get_state_from_serial();

    // Process state
    print_status("Processing...");
    std::string normalized_state_string = normalize_state(raw_state_string);
    skewb_state state_struct = convert_state_to_struct(normalized_state_string);
    std::string state_struct_stringified = skewb_state_to_string(state_struct);

    wait(2, seconds);

    // Obtain solution
    print_status("Searching...");
    std::string solution = FindSolutions(TestString,file);
    fclose(file);
  
    wait(2, seconds);
  
    print_status("Ready!");
  
    wait (5, seconds);
    

    // Wait for touch to start solve

    // Check sensory data

    // Solve
    print_status("Solving...");
    for (int i = 0; i < solution.length(); ++i)
    {
        char move = solution[i];

        switch (move)
        {
            case 'U':
                perform_a_move(back_motor, false);
                break;
            case 'u':
                perform_a_move(back_motor, true);
                break;
            case 'L':
                perform_a_move(left_motor, false);
                break;
            case 'l':
                perform_a_move(left_motor, true);
                break;
            case 'R':
                perform_a_move(right_motor, false);
                break;
            case 'r':
                perform_a_move(right_motor, true);
                break;
            case 'F':
                perform_a_move(top_motor, false);
                break;
            case 'f':
                perform_a_move(top_motor, true);
                break;
            default:
                break;
        }
    }

    // End the program
    print_status("Solved!");
    wait(10, seconds);
    Brain.programStop();
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

struct State_Solution
{
    uint64_t CurState;
    uint64_t EncodedSolution;
};

std::string FindSolutions(const std::string &CubeState, FILE* file)
{
    const int8_t StateSize = 9;
    const int SizeOfBuffer = 300;
    State_Solution Buffer[SizeOfBuffer];
    std::string State[StateSize];
    int8_t PrevSubIndex = 0;
    int8_t IndexToPush = 0;
    bool found = false;
    const char Centers[6] = {'W', 'G', 'O', 'B', 'R', 'Y'};
    std::string Solution;
    const std::array<std::string, 8> Corners = {"UFR", "UFL", "UBL", "UBR", "DFR", "DFL", "DBL", "DBR"};
    const char Moves[9] = {' ','L', 'l', 'R', 'r', 'U', 'u', 'F', 'f'};
    int counter = 0;
    for (int8_t i = 0; i < CubeState.length(); i++){
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
    while (!found){

        std::string centers;
        size_t ReadCount = fread(Buffer,sizeof(State_Solution),SizeOfBuffer,file);
        if(ReadCount == 0){
            found = true;
            break;
        }
        
       
   
           
        
        for (size_t i = 0; i < ReadCount; i++){
            uint8_t current = 0b0;
            bool IsEliminated = false;
            for (int j = 0; j < 6; j++) {
                current = (Buffer[i].CurState >> (40 + (j * 3))) & 0b111;
                if (Centers[current] != State[0][j]){
                    IsEliminated = true;
                    //get out of for loop cz no break...
                    j = 6;
                }
            }
            if(IsEliminated){
                continue;
            }
            for (int j = 0; j < 8; j++) {
                current = (Buffer[i].CurState >> (16 + (j * 3))) & 0b111;
                if(Corners[current] != State[1+j]){
                    IsEliminated = true;
                    //get out of for loop cz no break...
                    j = 8;
                }
            }
            if (IsEliminated){
                continue;
            }

            std::string Orienation;
            for (int j = 0; j < 8; j++) {
                current = (Buffer[i].CurState >> (2 * j)) & 0b11;
                //convert 0,1,2,3,4,5 -> '0','1'...etc
                Orienation += '0' + current;
            }
            if(orientation_matches("WWWWBBOGOGYRRYWOBBRRROYOYGYGBG",Orienation)){
                Brain.Screen.setCursor(8,1);
                Brain.Screen.print(Orienation.c_str());
                found = true;
                for (int j = 0; j < 11; j++) {
                current = (Buffer[i].EncodedSolution >> (j * 4)) & 0b1111;
                if (current == 0){
                    break;
                }
                Solution += Moves[current];
            }
                return Solution;
            }else{
                counter++;
                Brain.Screen.setCursor(9,1);
                Brain.Screen.clearLine();
                Brain.Screen.print(std::to_string(counter).c_str());
                
            }
        }
        if (ReadCount != SizeOfBuffer){
            //Means read to the end of file
            // i.e. we successfully read 6 entries but size that we tried to read was 16
            found = true;
        }
        
        
}

    return Solution;
}

void perform_a_move(custom_motor &m, bool inverted)
{
    const double SINGLE_MOVE_ANGLE = 120.00;

    if(inverted)
        m.spin_degrees(-SINGLE_MOVE_ANGLE);
    else
        m.spin_degrees(SINGLE_MOVE_ANGLE);

    
}
