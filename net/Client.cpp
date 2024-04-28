#include "Client.h"

int Client::send_message(SOCKET s, char message[], int nbytes)
{
    if (send(s, message, nbytes, 0) == SOCKET_ERROR)
    {
        printf(GRAY "[CLIENT LOG] send failed: %d\n" RESET, WSAGetLastError());
        return 1;
    }
    else
    {
        return 0;
    }
}

int Client::main(void)
{
    WSADATA wsaData;
    SOCKET ConnectSocket = INVALID_SOCKET;
    struct addrinfo *result = NULL, *ptr = NULL, hints;
    char recvbuf[DEFAULT_BUFLEN];
    int recvbuflen = DEFAULT_BUFLEN;

    int iResult;

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0)
    {
        printf(GRAY "[CLIENT LOG] WSAStartup failed: %d\n" RESET, iResult);
        return 1;
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    // Resolve the server address and port
    iResult = getaddrinfo(SERVER_IP, PORT, &hints, &result);
    if (iResult != 0)
    {
        printf(GRAY "[CLIENT LOG] getaddrinfo failed: %d\n" RESET, iResult);
        WSACleanup();
        return 1;
    }

    // Attempt to connect to an address until one succeeds
    for (ptr = result; ptr != NULL; ptr = ptr->ai_next)
    {
        // Create a SOCKET for connecting to server
        ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
        if (ConnectSocket == INVALID_SOCKET)
        {
            printf(GRAY "[CLIENT LOG] socket failed: %d\n" RESET, WSAGetLastError());
            WSACleanup();
            return 1;
        }

        // Connect to server
        iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (iResult == SOCKET_ERROR)
        {
            closesocket(ConnectSocket);
            ConnectSocket = INVALID_SOCKET;
            continue;
        }
        break;
    }

    freeaddrinfo(result);

    if (ConnectSocket == INVALID_SOCKET)
    {
        printf(GRAY "[CLIENT LOG] Unable to connect to server!\n" RESET);
        WSACleanup();
        return 1;
    }

    // Ask for join
    const char *sendbuf = "join";
    iResult = send(ConnectSocket, sendbuf, (int)strlen(sendbuf), 0);
    if (iResult == SOCKET_ERROR)
    {
        printf(GRAY "[CLIENT LOG] send failed: %d\n" RESET, WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
        return 1;
    }

    // Main execution of the client
    for (;;)
    {
        iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
        if (iResult > 0)
        {
            printf(GRAY "[CLIENT LOG] Bytes received: %d\n" RESET, iResult);
            printf(GRAY "[CLIENT LOG] Received: %s\n" RESET, recvbuf);

            // Handle received message
            handle_recv(recvbuf, ConnectSocket);
        }
        else if (iResult == 0)
        {
            printf(GRAY "[CLIENT LOG] Connection closed\n" RESET);
        }
        else
        {
            printf(GRAY "[CLIENT LOG] recv failed: %d\n" RESET, WSAGetLastError());
            closesocket(ConnectSocket);
            WSACleanup();
            return 1;
        }
    }

    // Cleanup
    closesocket(ConnectSocket);
    WSACleanup();
    return 0;
}