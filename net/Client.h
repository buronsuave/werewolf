#ifndef CLIENT_H
#define CLIENT_H

#include "common.h"

class Client
{
    protected:
    int send_message(SOCKET s, char message[], int nbytes);

    public:
    virtual void handle_recv(char recvbuf[], SOCKET s) = 0;
    int main(void);
};

#endif