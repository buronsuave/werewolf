#ifndef COMMON_H
#define COMMON_H

#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fstream>
#include <ctime>
#include <vector>

#pragma comment(lib, "Ws2_32.lib")

// Default constants for testing
#define PORT "9034"
#define SERVER_IP "127.0.0.1" //"26.150.165.214" 
#define MAX_CLIENTS 100
#define DEFAULT_BUFLEN 512

// Color of debug logs in console
// #define GRAY "\033[90m"
// #define RESET "\033[0m"

// Colors for log files (not required)
#define GRAY ""
#define RESET ""

// Log functions
void write_log(const char*);
void write_formatted_log(const char* format, ...);

#endif