#include "net/client.h"
#include <iostream>

/*
    This function checks the WS2 driver and set the configuration for 
    the client socket.
*/
BOOL init()
{
    printf("Entering init()\n");
    BOOL bStartupSuccessful = FALSE;
    WSAData wsaData;
    // Load global configuration
    if (LoadConfig() == FALSE)
    {
        // Error input. Return false statement
        return FALSE;
    }

    // Call WSAStartup to initialize Winsock before calling any of its APIs.
    int retVal = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (retVal != 0)
    {
        printf("WSAStartup failed. Error = %d\n", retVal);
        return FALSE;        
    }

    // Remember that the WSAStartup was successful so as to call WSACleanup 
    // once, correspondingly.
    bStartupSuccessful = TRUE;

    return bStartupSuccessful;
};

int main(int argc, char *argv[])
{
    printf("Entering main\n");
    char message_send[DEFAULT_SEND_BUFFER_SIZE+1];
    char message_recv[DEFAULT_RECV_BUFFER_SIZE+1];

    // Init socket.
    BOOL bStartupSuccessful = init();
    if (bStartupSuccessful == FALSE)
    {
        goto CLEANUP;
    }

    // Create a socket that's connected to the server.
    g_ClientContext.sock = CreateClientSocket();
    // If it's an invalid socket, CLEANUP.
    if (g_ClientContext.sock == INVALID_SOCKET)
        goto CLEANUP;

    std::cout << "Type a message: ";
    std::cin.getline(message_send, DEFAULT_SEND_BUFFER_SIZE);

    // Init Buffer for sending data.
    if (PrepareSendBuffer(message_send) == FALSE)
        goto CLOSE_SOCKET;

    // Init Buffer for receiving data.
    if (PrepareRecvBuffer() == FALSE)
        goto CLOSE_SOCKET;

    // Perform operation
    DoIdealSendRecv();

CLOSE_SOCKET:
    // Free the send and recv buffers.
    FreeSendBuffer();    
    FreeRecvBuffer();

    // Close the client socket.
    closesocket(g_ClientContext.sock);  
    printf("Closed socket %d. "
        "Total Bytes Recd = %d, "
        "Total Bytes Sent = %d\n",
        g_ClientContext.sock, 
        g_ClientContext.nBytesRecd,
        g_ClientContext.sendBufSize - 
        g_ClientContext.nBytesRemainingToBeSent);

 
CLEANUP:
    // Call WSACleanup only if WSAStartup was successful.
    if (bStartupSuccessful)       
    {
        // Inform Winsock that we are done with all the APIs.
        WSACleanup();
    }

    printf("Exiting main()\n");
    system("pause");
    return 0;
}
