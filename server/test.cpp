#include "net/server.h"

BOOL init()
{
    printf("Entering init()\n");
    WSADATA wsaData;
    // Load global configuration
    if (LoadConfig() == FALSE)
    {
        // Error input. Return false statement.
        return FALSE;
    }

    // Xall WSAStartup before calling any of the Winsock API functions.
    int retVal = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (retVal != 0)
    {
        printf("WSAStartup failed. Error = %d\n", retVal);
        return FALSE;
    }

    return TRUE;
};

int main(int argc, char *argv[])
{
    printf("Entering main()\n");
    int retVal;

    if (init() == FALSE)
    {
        goto CLEANUP;
    }

    // Depending on the command-line options given, create one or more
    // listening sockets on the requested interface(s).
    CreateListeningSockets();

    // Depending on the type of accept requested, call the suitable function.
    switch(g_AcceptContext.typeOfAccept)
    {
        case NON_BLOCKING_ACCEPT:
            NonBlockingAcceptMain();
            break;

        default:
            // some error. return a non-zero error code.
            retVal = 1;
            break;
    }

    // we may not come here as per the current implementation since
    // the XXXAcceptMain functions themselves are waiting forever for
    // connections or data. But in case we add a timeout option in future
    // we might come here and so we'll cleanup everything properly.

    // Close all the listening sockets and remove them from the global
    // list. In case there are some accepted sockets still in the list,
    // (due to some error), they'll also be closed as well.
    DestroyListeningSockets();

    // Inform Winsock that we're done with all the Winsock APIs.
    WSACleanup();
  
CLEANUP:

    printf("Exiting main()\n");
    return retVal;
}