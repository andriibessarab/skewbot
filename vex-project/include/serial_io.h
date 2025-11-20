//----------------------------------------------------------------------------
//
//  Module:       serial_io.h
//  Description:  Header for module that reads colour string from serial
//  Authors:      MTE 100 & 121 Group 15
//
//----------------------------------------------------------------------------

#ifndef SERIAL_IO_H
#define SERIAL_IO_H

#pragma once

#include "vex.h"

// --- Constants ---
const int MAX_BUFFER_SIZE = 100;

// Reads a line of data from the standard input (serial port)
std::string get_state_from_serial();

#endif