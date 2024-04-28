#ifndef COMMON_H
#define COMMON_H

#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#pragma comment(lib, "Ws2_32.lib")

// Default constants for testing
#define PORT "9034"
#define SERVER_IP "127.0.0.1"
#define MAX_CLIENTS 10
#define DEFAULT_BUFLEN 512

// Color of debug logs in console
#define GRAY "\033[90m"
#define RESET "\033[0m"

enum STAGE
{
    STAGE_LOBBY = 0,
    STAGE_DAY = 1,
    STAGE_NIGHT = 2,
    STAGE_OVER = 3,
    STAGE_NEW = 4
};

#endif