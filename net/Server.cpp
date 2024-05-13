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
        write_formatted_log(GRAY "[SERVER LOG] Current value of i: %d" RESET "\n", i);
        if (send(i, message, nbytes, 0) == SOCKET_ERROR)
        {
            write_formatted_log(GRAY "[SERVER LOG] send failed: %d" RESET "\n", WSAGetLastError());
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
    int i_result = getaddrinfo(NULL, PORT, &hints, &result);
    if (i_result != 0)
    {
        write_formatted_log(GRAY "[SERVER LOG] getaddrinfo failed: %d" RESET "\n", i_result);
        WSACleanup();
        exit(1);
    }

    // Create a SOCKET for the server to listen for client connections
    listener = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (listener == INVALID_SOCKET)
    {
        write_formatted_log(GRAY "[SERVER LOG] Error at socket(): %ld" RESET "\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        exit(1);
    }

    // Setup the TCP listening socket
    i_result = bind(listener, result->ai_addr, (int)result->ai_addrlen);
    if (i_result == SOCKET_ERROR)
    {
        write_formatted_log(GRAY "[SERVER LOG] bind failed: %d" RESET "\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(listener);
        WSACleanup();
        exit(1);
    }

    freeaddrinfo(result);

    i_result = listen(listener, SOMAXCONN);
    if (i_result == SOCKET_ERROR)
    {
        write_formatted_log(GRAY "[SERVER LOG] listen failed: %d" RESET "\n", WSAGetLastError());
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
        write_formatted_log(GRAY "[SERVER LOG] WSAStartup failed: %d" RESET "\n", iResult);
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
            write_formatted_log(GRAY "[SERVER LOG] select failed: %d" RESET "\n", WSAGetLastError());
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
                        write_formatted_log(GRAY "[SERVER LOG] accept failed: %d" RESET "\n", WSAGetLastError());
                    }
                    else
                    {
                        FD_SET(newfd, &master);
                        if (newfd > fdmax)
                        {
                            fdmax = newfd;
                        }
                        write_formatted_log(
                            GRAY "[SERVER LOG] New connection from %s on socket %d" RESET "\n", 
                            inet_ntoa(((struct sockaddr_in *)&remoteaddr)->sin_addr), 
                            newfd);
                    }
                }
                else
                {
                    // Handle data from a client
                    char buf[DEFAULT_BUFLEN];
                    int nbytes = recv(i, buf, sizeof(buf), 0);
                    if (nbytes <= 0)
                    {
                        // Connection closed
                        if (nbytes == 0)
                        {
                            write_formatted_log(GRAY "[SERVER LOG] Socket %d hung up" RESET "\n", i);
                        }
                        else
                        {
                            write_formatted_log(GRAY "[SERVER LOG] recv failed: %d" RESET "\n", WSAGetLastError());
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