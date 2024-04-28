#include "Server.h"

void* Server::get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET)
    {
        return &(((struct sockaddr_in *)sa)->sin_addr);
    }
    return &(((struct sockaddr_in6 *)sa)->sin6_addr);
}

int Server::send_message(int i, char message[], SOCKET listener, fd_set master, int nbytes)
{
    if (FD_ISSET(i, &master) && i != listener)
    {
        printf(GRAY "[SERVER LOG] Current value of i: %d" RESET "\n", i);
        if (send(i, message, nbytes, 0) == SOCKET_ERROR)
        {
            printf(GRAY "[SERVER LOG] send failed: %d" RESET "\n", WSAGetLastError());
            return 1;
        }
        else
        {
            return 0;
        }
    }
    else
    {
        return 1;
    }
}

int Server::send_broadcast(int fdmax, char message[], SOCKET listener, fd_set master, int nbytes)
{
    for (int i = 0; i <= fdmax; i++)
    {
        send_message(i, message, listener, master, nbytes);
    }
    return 0;
}

SOCKET Server::get_listener_socket(void)
{
    SOCKET listener = INVALID_SOCKET; // Listening socket descriptor
    struct addrinfo *result = NULL;
    struct addrinfo hints;

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    // Resolve the server address and port
    int iResult = getaddrinfo(NULL, PORT, &hints, &result);
    if (iResult != 0)
    {
        printf(GRAY "[SERVER LOG] getaddrinfo failed: %d" RESET "\n", iResult);
        WSACleanup();
        exit(1);
    }

    // Create a SOCKET for the server to listen for client connections
    listener = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (listener == INVALID_SOCKET)
    {
        printf(GRAY "[SERVER LOG] Error at socket(): %ld" RESET "\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        exit(1);
    }

    // Setup the TCP listening socket
    iResult = bind(listener, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR)
    {
        printf(GRAY "[SERVER LOG] bind failed: %d" RESET "\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(listener);
        WSACleanup();
        exit(1);
    }

    freeaddrinfo(result);

    iResult = listen(listener, SOMAXCONN);
    if (iResult == SOCKET_ERROR)
    {
        printf(GRAY "[SERVER LOG] listen failed: %d" RESET "\n", WSAGetLastError());
        closesocket(listener);
        WSACleanup();
        exit(1);
    }

    return listener;
}

int Server::main(void)
{
    WSADATA wsaData;
    SOCKET listener = INVALID_SOCKET;
    SOCKET clients[MAX_CLIENTS];
    fd_set master;
    fd_set read_fds;
    int fdmax;
    int i, rv;

    FD_ZERO(&master);
    FD_ZERO(&read_fds);

    // Initialize Winsock
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0)
    {
        printf(GRAY "[SERVER LOG] WSAStartup failed: %d" RESET "\n", iResult);
        return 1;
    }

    listener = get_listener_socket();
    FD_SET(listener, &master);
    fdmax = listener;

    // Main loop
    for (;;)
    {
        read_fds = master;
        rv = select(0, &read_fds, NULL, NULL, NULL);
        if (rv == SOCKET_ERROR)
        {
            printf(GRAY "[SERVER LOG] select failed: %d" RESET "\n", WSAGetLastError());
            break;
        }

        for (i = 0; i <= fdmax; i++)
        {
            if (FD_ISSET(i, &read_fds))
            {
                if (i == listener)
                {
                    // New connection
                    struct sockaddr_storage remoteaddr;
                    int addrlen = sizeof(remoteaddr);
                    SOCKET newfd = accept(listener, (struct sockaddr *)&remoteaddr, &addrlen);
                    if (newfd == INVALID_SOCKET)
                    {
                        printf(GRAY "[SERVER LOG] accept failed: %d" RESET "\n", WSAGetLastError());
                    }
                    else
                    {
                        FD_SET(newfd, &master);
                        if (newfd > fdmax)
                        {
                            fdmax = newfd;
                        }
                        printf(GRAY "[SERVER LOG] New connection from %s on socket %d" RESET "\n", inet_ntoa(((struct sockaddr_in *)&remoteaddr)->sin_addr), newfd);
                    }
                }
                else
                {
                    // Handle data from a client
                    char buf[256];
                    int nbytes = recv(i, buf, sizeof(buf), 0);
                    if (nbytes <= 0)
                    {
                        // Connection closed
                        if (nbytes == 0)
                        {
                            printf(GRAY "[SERVER LOG] Socket %d hung up" RESET "\n", i);
                        }
                        else
                        {
                            printf(GRAY "[SERVER LOG] recv failed: %d" RESET "\n", WSAGetLastError());
                        }
                        closesocket(i);
                        FD_CLR(i, &master);
                    }
                    else
                    {
                        // New data. Here something should be implemented
                        handle_recv(master, fdmax, listener, i, buf, nbytes);
                    }
                }
            }
        }
    }

    // Cleanup
    for (i = 0; i <= fdmax; i++)
    {
        if (FD_ISSET(i, &master))
        {
            closesocket(i);
        }
    }
    WSACleanup();

    return 0;
}