// Description:
//             This file contains the functions for parsing the command-line
// arguments and the main function.

#include "common.h"

// g_AcceptContext will hold all the global variables used across all
// the files.
AcceptContext g_AcceptContext;


/*
    This function converts a given address family into its corresponding string
    representation for display purposes.
*/
const char *AFImage(BYTE addressFamily)
{
    char *szRetVal;

    // return the printable string equivalent of the corresponding 
    // address family.
    switch (addressFamily)
    {
        case AF_UNSPEC : szRetVal = "AF_UNSPEC";
                         break;
        case AF_INET   : szRetVal = "AF_INET";
                         break;
        case AF_INET6  : szRetVal = "AF_INET6";
                         break;
        default        : szRetVal = "Unrecognized";
                         break;
    }
    
    return szRetVal;
}



/*
    This function converts a given type of accept into its corresponding string
    representation for display purposes.
*/
const char *AcceptTypeImage(BYTE typeOfAccept)
{
    char *szRetVal;

    // return a string equivalent of the accept type for displaying
    // to the user of his choices.
    switch (typeOfAccept)
    {
        case NON_BLOCKING_ACCEPT : szRetVal = "NON_BLOCKING_ACCEPT";
                                   break;
        case ASYNC_SELECT_ACCEPT : szRetVal = "ASYNC_SELECT_ACCEPT";
                                   break;
        default                  : szRetVal = "Unrecognized";
                                   break;
    }
    
    return szRetVal;
}



/*
    This function prints the available command-line options, the arguments
    expected by each of them and the valid input values and the default 
    values for each them.
*/
void PrintUsage(char *szProgramName)
{
    printf("\n\n"
           "Usage:\n"
           "------\n"
           "   %s <options> \n\n"
           "where <options> is one or more of the following: \n\n"
           "   -a <0|4|6>      Address Family: 0 for Either\n"
           "                                   4 for IPv4\n"
           "                                   6 for IPv6\n"
           "                   Default: %d\n\n"
           "   -i <interface>  Interface address\n"
           "                   Default: %s\n\n"
           "   -e <endpoint>   Port number\n"
           "                   Default: %s\n\n"
           "   -t <1|2>        Type of Accept: 1 for Non-blocking accept\n"
           "                                   2 for Accept with WSAAsyncSelect\n"
           "                   Default: %d\n\n"
           "\n",
           szProgramName,
           DEFAULT_ADDRESS_FAMILY,
          ( DEFAULT_INTERFACE == NULL ? "NULL" : DEFAULT_INTERFACE),
           DEFAULT_PORT,
           DEFAULT_TYPE_OF_ACCEPT
           );

    return;
}

/*
    This function loads the configuration in the g_AcceptContext
*/
BOOL LoadConfig()
{
    g_AcceptContext.addressFamily = DEFAULT_ADDRESS_FAMILY;
    g_AcceptContext.szInterface = DEFAULT_INTERFACE;
    g_AcceptContext.szPort = DEFAULT_PORT;
    g_AcceptContext.typeOfAccept = DEFAULT_TYPE_OF_ACCEPT;

    return TRUE;
}