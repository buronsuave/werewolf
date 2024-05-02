#ifndef CLIENT_H
#define CLIENT_H

#include "common.h"

class Client
{
    protected:
    int send_message(SOCKET s, char message[], int nbytes);
    SOCKET get_connect_socket(void);

    public:
    virtual void handle_recv(char recvbuf[], SOCKET s) = 0;
    int main(void);
};

#endif


/*
class Shape
{
    virtual int computeArea() = 0;
};

class Triangle : public Shape
{
    int computeArea() { return b*h/2; }
}

class Circle : public Shape
{
    int computeArea() { return Pi*r^2; }
}


*/