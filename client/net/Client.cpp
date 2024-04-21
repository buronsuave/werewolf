#include "client.h"
ClientContext g_ClientContext;

/*
    Prints the given socket address in a printable string format.
*/
void PrintAddressString(LPSOCKADDR pSockAddr, DWORD dwSockAddrLen)
{
    // INET6_ADDRSTRLEN is the maximum size of a valid IPv6 address 
    // including port,colons,NULL,etc.
    wchar_t buf[INET6_ADDRSTRLEN];
    DWORD dwBufSize = 0;    

    memset(buf,0,sizeof(buf));
    dwBufSize = sizeof(buf);

    // This function converts the pSockAddr to a printable format into buf.   
    if (WSAAddressToStringW(pSockAddr, 
                           dwSockAddrLen, 
                           NULL, 
                           buf, 
                           &dwBufSize) == SOCKET_ERROR)
    {
        printf("ERROR: WSAAddressToString failed %d \n", WSAGetLastError());
        goto CLEANUP;
    }

    wprintf(L"%ls\n", buf); // Use wprintf and %ls format specifier for wide strings

CLEANUP:
    return;
}

SOCKET CreateClientSocket()
{
    struct addrinfo hints;
    struct addrinfo *res;
    struct addrinfo *pAddr;
    SOCKET clientSock = INVALID_SOCKET;
    int i;
    int rc;

    printf("Entering CreateClientSocket()\n");

    memset(&hints, 0, sizeof(hints));
    
    hints.ai_family = g_ClientContext.addressFamily;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_socktype = SOCK_STREAM;

    // Get the local addresses that are suitable for connecting to the
    // given server address.
    if (getaddrinfo(g_ClientContext.szServer, 
                g_ClientContext.szPort,
                &hints, 
                &res) != NO_ERROR)
    {
        printf("getaddrinfo failed. Error = %d\n", WSAGetLastError());
        goto CLEANUP;
    }

    if (res == NULL)
    {
        printf("getaddrinfo returned res = NULL\n");
        goto CLEANUP;
    }

    printf("getaddrinfo successful.Enumerating the returned addresses ...\n\n");

    for (pAddr = res, i = 1; pAddr != NULL; pAddr = pAddr->ai_next, i++)
    {
        printf("Processing Address %d returned by getaddrinfo : ", i);
        PrintAddressString(pAddr->ai_addr, pAddr->ai_addrlen);
        clientSock = WSASocket(pAddr->ai_family, 
                            pAddr->ai_socktype,
                            pAddr->ai_protocol,
                            NULL,
                            NULL,
                            0);
        if (clientSock == INVALID_SOCKET)
        {
            printf("WSASocket failed. Error = %d\n", WSAGetLastError());
            printf("Ignoring this address and continuing with the next. \n\n");
            
            // Anyway, let's continue with other addresses.
            continue;
        }

        printf("Created client socket with handle = %d\n", clientSock);

        rc = connect(clientSock, pAddr->ai_addr, pAddr->ai_addrlen);
        if (rc == SOCKET_ERROR)
        {
            printf("connect failed. Error = %d\n", WSAGetLastError());
            closesocket(clientSock);
            clientSock = INVALID_SOCKET;
            continue;
        }

        printf("Connected successfully\n");
        break;
    }

   
CLEANUP:

    // If getaddrinfo succeeded, it would have allocated memory for the
    // res structure which we have to free.
    if (res)
    {
        freeaddrinfo(res);
        printf("Freed the memory allocated for res by getaddrinfo\n");
    }

    printf("Exiting CreateClientSocket()\n");
    return clientSock;
}

/*
    Allocate a buffer to use for sending data and initialize the buffer
    with suitable initial values.
*/
BOOL PrepareSendBuffer(const char* message)
{
    BOOL bSuccess = FALSE;
    
    printf("Entering PrepareSendBuffer()\n");
    
    g_ClientContext.pSendBuf = (char *) malloc(g_ClientContext.sendBufSize + 1);
    if (g_ClientContext.pSendBuf == NULL)
    {
        printf("malloc failed.\n");
        goto CLEANUP;
    }
    
    printf("Allocated Send Buffer: %p\n", g_ClientContext.pSendBuf);

    // fill up with message.
    strcpy(g_ClientContext.pSendBuf, message);  
    g_ClientContext.pSendBuf[g_ClientContext.sendBufSize] = '\0';
    g_ClientContext.nBytesRemainingToBeSent = g_ClientContext.sendBufSize;
    
    bSuccess = TRUE;

CLEANUP:    
    printf("Exiting PrepareSendBuffer()\n");    
    return bSuccess;
}


/*
    Deallocate the buffer that was used for sending.
*/
void FreeSendBuffer()
{
    if (g_ClientContext.pSendBuf != NULL)
    {
        free(g_ClientContext.pSendBuf);
        printf("Freed Send Buffer : %p\n", g_ClientContext.pSendBuf);
        g_ClientContext.pSendBuf = NULL;
    }
}


/*
    Allocate a buffer for receiving data from the server.
    Currently since we are not using the received data, we'll just receive
    all the data in a big one-time buffer for all the receives and overwrite
    the same buffer each time. The count of received bytes is what we
    are presently interested in.
*/
BOOL PrepareRecvBuffer()
{
    BOOL bSuccess = FALSE;
    
    printf("Entering PrepareRecvBuffer()\n");
    
    g_ClientContext.pRecvBuf = (char *) malloc(g_ClientContext.recvBufSize + 1);
    if (g_ClientContext.pRecvBuf == NULL)
    {
        printf("malloc failed.\n");
        goto CLEANUP;
    }
    
    printf("Allocated Recv Buffer: %p\n", g_ClientContext.pRecvBuf);
    
    memset(g_ClientContext.pRecvBuf, 0, g_ClientContext.recvBufSize + 1);   
    g_ClientContext.nBytesRecd = 0;
    
    bSuccess = TRUE;

CLEANUP:    
    printf("Exiting PrepareRecvBuffer()\n");    
    return bSuccess;
}


/*
    Deallocate the buffer used for receiving.
*/
void FreeRecvBuffer()
{
    if (g_ClientContext.pRecvBuf != NULL)
    {
        free(g_ClientContext.pRecvBuf);
        printf("Freed Recv Buffer : %p\n", g_ClientContext.pRecvBuf);
        g_ClientContext.pRecvBuf = NULL;
    }
}

/*
    Try sending data to server once. Return the error, if any.
*/
int DoSendOnce()
{
    int nBytesSent;    
    int startPosition;
    int err = 0;
    
    printf("Entering DoSendOnce()\n");

    // send from the position where we left off last.
    startPosition = g_ClientContext.sendBufSize - 
                    g_ClientContext.nBytesRemainingToBeSent;
    nBytesSent = send(g_ClientContext.sock, 
                      g_ClientContext.pSendBuf + startPosition, 
                      g_ClientContext.nBytesRemainingToBeSent,
                      0);
    
    if (nBytesSent == SOCKET_ERROR)
    {
        err = WSAGetLastError();
        goto CLEANUP;
    }

    // remember where to begin send next time.
    g_ClientContext.nBytesRemainingToBeSent -= nBytesSent;
    printf("Sent %d bytes so far\n", startPosition + nBytesSent);

CLEANUP:
  
    printf("Exiting DoSendOnce()\n");
    return err;
}


/*
    Try receiving from the server. This function returns the number of bytes
    received (unlike DoSendOnce which returns the error code).
*/
int DoRecvOnce()
{
    int nBytesRecd;    
    
    printf("Entering DoRecvOnce()\n");

    // receive into the same global receive buffer, overwriting the previous
    // contents, as we are only interested in the number of bytes received
    // for verification.
    nBytesRecd = recv(g_ClientContext.sock, 
                      g_ClientContext.pRecvBuf, 
                      g_ClientContext.recvBufSize,
                      0);
    
    if (nBytesRecd == SOCKET_ERROR)
    {
        printf("recv returned: %d\n", WSAGetLastError());
        goto CLEANUP;
    }

    // update the stats.
    g_ClientContext.nBytesRecd += nBytesRecd;
    printf("Recd %d bytes so far\n", g_ClientContext.nBytesRecd);

CLEANUP:
  
    printf("Exiting DoRecvOnce()\n");
    return nBytesRecd;
}


/*
    Repeat sending the data to the server until all the data has been
    completely sent to the server.
*/
void DoSendUntilDone()
{
    int err;

    printf("Entering DoSendUntilDone()\n");    
    
    do
    {
        // Get the job done by callnig DoSendOnce.
        err = DoSendOnce();

        // handle error cases as appropriate.
        switch(err)
        {
            case 0:
                if (g_ClientContext.nBytesRemainingToBeSent == 0)
                {
                    printf("Send completed\n");
                    goto CLEANUP;
                }
                break;
               
            case WSAEWOULDBLOCK:
                printf("Got WSAEWOULDBLOCK from send.\n");
                printf("Waiting for 1 second before retrying send ...\n");
                Sleep(1000);
                break;

            default:
                // other errors.
                printf("ERROR: send failed: Error = %d\n", err);
                goto CLEANUP;
        }
    } while (TRUE);

CLEANUP:
    printf("Exiting DoSendUntilDone()\n");
    return;
}


/*
    Receive data on the socket until recv returns 0 or error.
*/
void DoRecvUntilDone()
{
    int err;

    printf("Entering DoRecvUntilDone()\n");
    do
    {
        // get the job done by calling DoRecvOnce.
        switch(DoRecvOnce())
        {
            case 0:
                printf("Recv returned 0. Remote socket must have been "
                       "gracefully closed.\n");
                goto CLEANUP;
              
            case SOCKET_ERROR:
                err = WSAGetLastError();
                if (err == WSAEWOULDBLOCK)
                {
                    printf("Got WSAEWOULDBLOCK from recv.\n");
                    printf("Waiting for 1 second before retrying send ...\n");
                    Sleep(1000);
                }
                else
                {
                    printf("ERROR: recv returned : %d\n", err);
                    goto CLEANUP;
                }
                break;

            default:
                // > 0 bytes read. let's continue.
                break;
        }
    } while (1);

CLEANUP:
    printf("Exiting DoRecvUntilDone()\n");
    return;
}


/*
    This functions shuts down the socket before closing it so that
    it would send a FIN packet on the TCP connection to the remote side
    to indicate that there won't be any more data sent on this socket.
    Only if this side shuts down, the remote side will get a 0 for a
    recv and conclude that the client is done with sending data.
*/
void DoShutDown()
{
    if (shutdown(g_ClientContext.sock, SD_SEND) == SOCKET_ERROR)
        printf("shutdown failed. err = %d\n", WSAGetLastError());
    else
        printf("shutdown successful\n");
}

/*
    This function implements the scenario where all the sends are done first
    and all the recvs next.
*/
void DoSendThenRecv()
{
    DoSendUntilDone();
    DoShutDown();
    DoRecvUntilDone();
}

/*
    This function implements the scenario where the client only sends but
    doesn't recv the echoed data back. This helps demonstrate possible
    deadlocks in the server side for huge sized data buffer.
*/
void DoSendNoRecv()
{
    DoSendUntilDone();
    DoShutDown();    
}

/*
    This function implements the scenario where the client waits for a certain
    time before doing the recv thereby allowing the server code to get
    a WSAEWOULDBLOCK on a send, especially when the data buffer is huge.
*/
void DoSendWaitRecv()
{
    DoSendUntilDone();
    DoShutDown();      
    printf("Waiting before doing recv ...\n");
    Sleep(g_ClientContext.delay);
    DoRecvUntilDone();
}

/*
    This function implements the scenario to test the server to get
    a FD_WRITE event before an FD_READ event.
*/
void DoWaitSendRecv()
{
    printf("Waiting before doing initial send ...\n");
    Sleep(g_ClientContext.delay);
    DoSendUntilDone();
    DoShutDown();        
    DoRecvUntilDone();
}

/*
    This function is supplied as the call back function for CreateThread.
    Although it is doing nothing other than calling DoRecvUntilDone, it
    needs to have this particular signature to be passed as a valid 
    call back function for CreateThread.
*/
DWORD WINAPI ReceiverThread(LPVOID pv)
{
    pv = NULL;

    DoRecvUntilDone();
    return 0;
}

/*
    This function illustrates the ideal send and recv scenario where
    one thread does the recv independently until it gets a 0 or an error
    and the other thread (in this case, the current thread) does sends
    until all data is sent. This is the truly asynchronous implementation
    of the client unlike the earlier scenarios which were mostly meant
    for testing specific sequence of events in the server and understanding
    the need for different parts of the server code.
*/
void DoIdealSendRecv()
{
    HANDLE hThread;

    // start the receiver thread.
    hThread = CreateThread(NULL, 0, ReceiverThread, NULL, 0, NULL);
    if (hThread == NULL)
    {
        printf("ERROR: CreateThread failed. Error = %d\n", GetLastError());
        goto CLEANUP;
    }

    // In this thread, send all data until done.
    DoSendUntilDone();
    DoShutDown();

    // wait till recv thread is done.
    if (WaitForSingleObject(hThread, INFINITE) != WAIT_OBJECT_0)
    {
        printf("ERROR: WaitForSingleObject failed: %d\n", GetLastError());
        goto CLEANUP;
    }
    printf("Recv Thread done.\n");

CLEANUP:
    
    return;
}

/*
    This function loads the values for g_ClientContext
*/
BOOL LoadConfig()
{
    // Fill up the default arguments and let the user options override these.
    g_ClientContext.addressFamily = DEFAULT_ADDRESS_FAMILY;
    g_ClientContext.szServer = DEFAULT_SERVER;
    g_ClientContext.szPort = DEFAULT_PORT;
    g_ClientContext.sendBufSize = DEFAULT_SEND_BUFFER_SIZE;
    g_ClientContext.recvBufSize = DEFAULT_RECV_BUFFER_SIZE;    
    g_ClientContext.delay = DEFAULT_DELAY;    
    return TRUE;
}