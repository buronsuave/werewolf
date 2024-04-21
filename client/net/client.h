#ifndef CLIENT_NET_H
#define CLIENT_NET_H

#include <winsock2.h>   // for Winsock API.
#include <windows.h>    // for Win32 APIs and types.
#include <stdio.h>      // for printing to stdout.
#include <wspiapi.h>    // for IPv6 support

// Default values for command-line options
#define DEFAULT_ADDRESS_FAMILY      AF_INET
#define DEFAULT_SERVER              "localhost"
#define DEFAULT_PORT                "7243"
#define DEFAULT_SEND_BUFFER_SIZE    1024
#define DEFAULT_RECV_BUFFER_SIZE    10240
#define DEFAULT_DELAY               3000

typedef struct _ClientContext
{
    BYTE    addressFamily;
    char    *szServer;
    char    *szPort;
    LONG    sendBufSize;
    LONG    recvBufSize;
    LONG    delay;
    char    *pSendBuf;
    char    *pRecvBuf;    
    int     nBytesRemainingToBeSent;
    int     nBytesRecd;
    SOCKET  sock;
    
} ClientContext;

extern ClientContext g_ClientContext;

BOOL LoadConfig();
SOCKET CreateClientSocket();
BOOL PrepareSendBuffer(const char* message);
void FreeSendBuffer();
BOOL PrepareRecvBuffer();
void FreeRecvBuffer();
int DoSendOnce();
int DoRecvOnce();
void DoSendUntilDone();
void DoRecvUntilDone();
void DoShutDown();
void DoSendThenRecv();
void DoSendNoRecv();
void DoSendWaitRecv();
void DoWaitSendRecv();
DWORD WINAPI ReceiverThread(LPVOID pv);
void DoIdealSendRecv();

#endif