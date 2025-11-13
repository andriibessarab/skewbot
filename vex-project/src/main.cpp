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


/////////////////////////////// MAIN PROGRAM ///////////////////////////////
int main()
{   
    FILE* file = fopen("SkewSolutions.bin","rb");
    
    //back_motor.spin(fwd, 20, percent);
    
    // Initializing Robot Configuration
    vexcodeInit();

    Brain.Screen.setFont(mono12);
    print_status("Skewb Solver \n");
    

    if(!file){
        Brain.Screen.setCursor(4,1);
        Brain.Screen.print("Failed to open file");
        Brain.Screen.setCursor(6,1);
        Brain.Screen.print(" errno = %d (%s)\n", errno, strerror(errno));
        return 1;
    }
    Brain.Screen.setCursor(5,1);
    Brain.Screen.print("Opened Successfully");
    std::string TestString = "WBROYG UFR DBL UBL UBR UFL DFL DFR DBR";
    std::string output = FindSolutions(TestString,file);
    Brain.Screen.setCursor(6,1);
    Brain.Screen.print("Done: ");
    Brain.Screen.print(output.c_str());

    fclose(file);
    wait(5,seconds);
    Brain.programStop();
    


    return 0;


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

bool CheckOrientation(const std::string &Orientation){
    return 0;
}
struct State_Solution{
    uint64_t CurState;
    uint32_t EncodedSolution;
};

std::string FindSolutions(const std::string &CubeState, FILE* file){
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
    const char Moves[8] = {'L', 'l', 'R', 'r', 'U', 'u', 'F', 'f'};
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
                found = true;
                for (int j = 0; j < 11; j++) {
                current = (Buffer[i].EncodedSolution >> (j * 3)) & 0b111;
                Solution += Moves[current];
            }
       
                return Solution;
            }else{
                counter++;
                Brain.Screen.setCursor(8,1);
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