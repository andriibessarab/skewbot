#ifndef SERIAL_IO_H
#define SERIAL_IO_H

#include <string>

// --- Constants ---
const int MAX_BUFFER_SIZE = 100;

// Reads a line of data from the standard input (serial port)
std::string get_state_from_serial();

#endif