#ifndef GAME_CLIENT_H
#define GAME_CLIENT_H

#include "common.h"

class GameClient : public Client
{
    private:
    STAGE stage = STAGE_LOBBY;

    public:
    void handle_recv(char buf[], SOCKET s);
};

#endif