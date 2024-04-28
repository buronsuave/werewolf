#ifndef SERVER_H
#define SERVER_H

#include "common.h"

class Server
{
    protected:
    void *get_in_addr(struct sockaddr *sa);
    int send_message(int i, char message[], SOCKET listener, fd_set master, int nbytes);
    int send_broadcast(int fdmax, char message[], SOCKET listener, fd_set master, int nbytes);
    SOCKET get_listener_socket(void);

    public:
    virtual void handle_recv(fd_set master, int fdmax, int listener, int i, char buf[], int nbytes) = 0;
    int main(void);
};

#endif