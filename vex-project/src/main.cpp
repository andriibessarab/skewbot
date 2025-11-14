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
std::string FindSolutions(std::string stringified_state_struct, FILE* file);
void perform_a_move(custom_motor &m, bool inverted);

/////////////////////////////// MAIN PROGRAM ///////////////////////////////

int main()
{
    // Configure robot
    vexcode_init();
  
    // Open file
    FILE* file = fopen("SkewSolutions.bin", "rb");
    if (!file)
    {
        print_status("Couldn't open file. Aborting...");
        wait(10, seconds);
        Brain.programStop();
    }
  
    // Get state from serial
    print_status("Awaiting data...");
    std::string raw_state_string = get_state_from_serial();

    // Process state
    print_status("Processing...");
    std::string state_struct_stringified = find_normalized_stringified_struct(raw_state_string);

    wait(2, seconds);

    // Obtain solution
    print_status("Searching...");
    std::string solution = FindSolutions(state_struct_stringified, file);
    fclose(file);

    wait(2, seconds);
  
    print_status("Ready!");
    Brain.Screen.setCursor(8,1);
    Brain.Screen.clearLine();
    Brain.Screen.print("Solution: ");
    Brain.Screen.print(solution.c_str());
  
    wait (5, seconds);
    

    // Wait for touch to start solve

    // Check sensory data

    // Solve
    print_status("Solving...");
    // for (int i = 0; i < solution.length(); ++i)
    // {
    //     char move = solution[i];

    //     switch (move)
    //     {
    //         case 'U':
    //             perform_a_move(back_motor, false);
    //             break;
    //         case 'u':
    //             perform_a_move(back_motor, true);
    //             break;
    //         case 'L':
    //             perform_a_move(left_motor, false);
    //             break;
    //         case 'l':
    //             perform_a_move(left_motor, true);
    //             break;
    //         case 'R':
    //             perform_a_move(right_motor, false);
    //             break;
    //         case 'r':
    //             perform_a_move(right_motor, true);
    //             break;
    //         case 'F':
    //             perform_a_move(top_motor, false);
    //             break;
    //         case 'f':
    //             perform_a_move(top_motor, true);
    //             break;
    //         default:
    //             break;
    //     }
    // }

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
            // We got data, process it
            buffer[strcspn(buffer, "\n\r")] = '\0';
            if (strlen(buffer) > 0)
                return std::string(buffer); // Return the valid string
        }
        else
        {
            // --- THIS IS THE FIX ---
            // fgets returned NULL (EOF or error).
            // We must stop looping and return to main().
            
            // Return an empty string to signal an error.
            return ""; 
        }
        
        // This wait is only hit if fgets got an empty line
        wait(20, msec);
    }
}

struct State_Solution
{
    uint64_t CurState;
    uint64_t EncodedSolution;
};

std::string FindSolutions(std::string stringified_state_struct, FILE* file)
{
    // "XXXXXX XXX XXX XXX XXX XXX XXX XXX XXX  YYYYYY"
    const std::string CubeState = stringified_state_struct.substr(0, 37);
    const std::string ToPrint1 = stringified_state_struct.substr(0,19);
    const std::string ToPrint2 = stringified_state_struct.substr(19,18);

    const std::string needed_orientation = stringified_state_struct.substr(40, 8);

            Brain.Screen.setCursor(3,1);
            Brain.Screen.print("%s", ToPrint1.c_str());
            Brain.Screen.setCursor(4,1);
            Brain.Screen.print("%s", ToPrint2.c_str());
            Brain.Screen.setCursor(5,1);
            Brain.Screen.print("%s", needed_orientation.c_str());
            wait(20, seconds);

    static char file_buffer[8192];
    setvbuf(file, file_buffer, _IOFBF, sizeof(file_buffer));
    const int8_t StateSize = 9;
    const int SizeOfBuffer = 64;
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
        counter++;
        Brain.Screen.setCursor(9,1);
       
        if (counter % 1000 == 0){
            Brain.Screen.clearLine();
            Brain.Screen.print(std::to_string(counter).c_str());
        }
        

        std::string centers;
        std::string corners;
        bool IsEliminated = false;
        size_t ReadCount = fread(Buffer,sizeof(State_Solution),SizeOfBuffer,file);
        if(ReadCount == 0){
            found = true;
            break;
        }
        
       
        
        for (size_t i = 0; i < ReadCount; i++){
            uint8_t current = 0b0;
            for (int j = 0; j < 6; j++) {
                current = (Buffer[i].CurState >> (40 + (j * 3))) & 0b111;
                centers+=Centers[current];
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
                corners += Corners[current] + " ";
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
            

        
            Brain.Screen.setCursor(1,1);
            Brain.Screen.clearLine();
            Brain.Screen.print(centers.c_str());
            Brain.Screen.setCursor(2,1);
            Brain.Screen.clearLine();
            Brain.Screen.print(corners.c_str());
            Brain.Screen.setCursor(3,1);
            Brain.Screen.clearLine();
            Brain.Screen.print(Orienation.c_str());
        
            
            
           
            if(Orienation == needed_orientation){
                
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
