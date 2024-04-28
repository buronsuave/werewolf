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

SOCKET Client::get_connect_socket(void)
{
    SOCKET connect_socket = INVALID_SOCKET; // Connect socket descriptior
    struct addrinfo *result = NULL;
    struct addrinfo *ptr = NULL; 
    struct addrinfo hints;

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    // Resolve the server address and port
    int iResult = getaddrinfo(SERVER_IP, PORT, &hints, &result);
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
        connect_socket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
        if (connect_socket == INVALID_SOCKET)
        {
            printf(GRAY "[CLIENT LOG] socket failed: %d\n" RESET, WSAGetLastError());
            WSACleanup();
            return 1;
        }

        // Connect to server
        iResult = connect(connect_socket, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (iResult == SOCKET_ERROR)
        {
            closesocket(connect_socket);
            connect_socket = INVALID_SOCKET;
            continue;
        }
        break;
    }

    freeaddrinfo(result);

    return connect_socket;
}

int Client::main(void)
{
    WSADATA wsa_data;
    SOCKET connect_socket = INVALID_SOCKET;
    struct addrinfo *result = NULL, *ptr = NULL, hints;
    char recvbuf[DEFAULT_BUFLEN];
    int recvbuflen = DEFAULT_BUFLEN;

    // Initialize Winsock
    int i_result = WSAStartup(MAKEWORD(2, 2), &wsa_data);
    if (i_result != 0)
    {
        printf(GRAY "[CLIENT LOG] WSAStartup failed: %d\n" RESET, i_result);
        return 1;
    }

    connect_socket = get_connect_socket();
    if (connect_socket == INVALID_SOCKET)
    {
        printf(GRAY "[CLIENT LOG] Unable to connect to server!\n" RESET);
        WSACleanup();
        return 1;
    }

    // Ask for join
    const char *sendbuf = "join";
    i_result = send(connect_socket, sendbuf, (int)strlen(sendbuf), 0);
    if (i_result == SOCKET_ERROR)
    {
        printf(GRAY "[CLIENT LOG] send failed: %d\n" RESET, WSAGetLastError());
        closesocket(connect_socket);
        WSACleanup();
        return 1;
    }

    // Main execution of the client
    for (;;)
    {
        i_result = recv(connect_socket, recvbuf, recvbuflen, 0);
        if (i_result > 0)
        {
            printf(GRAY "[CLIENT LOG] Bytes received: %d\n" RESET, i_result);
            printf(GRAY "[CLIENT LOG] Received: %s\n" RESET, recvbuf);

            // Handle received message
            handle_recv(recvbuf, connect_socket);
        }
        else if (i_result == 0)
        {
            printf(GRAY "[CLIENT LOG] Connection closed\n" RESET);
        }
        else
        {
            printf(GRAY "[CLIENT LOG] recv failed: %d\n" RESET, WSAGetLastError());
            closesocket(connect_socket);
            WSACleanup();
            return 1;
        }
    }

    // Cleanup
    closesocket(connect_socket);
    WSACleanup();
    return 0;
}