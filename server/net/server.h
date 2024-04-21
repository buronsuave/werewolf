#ifndef SERVER_NET_H
#define SERVER_NET_H

#include "common.h"
extern AcceptContext g_AcceptContext;
const char *AFImage(BYTE addressFamily);
const char *AcceptTypeImage(BYTE typeOfAccept);
void PrintUsage(char *szProgramName);
BOOL LoadConfig();

#endif