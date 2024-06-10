#ifndef GAME_CLIENT_H
#define GAME_CLIENT_H

#include "common.h"

class GameClient : public Client
{
    private:
    STAGE stage = STAGE_LOBBY;

    public:
    ROLE role;
    void handle_recv(char buf[], SOCKET s);
    void print_active_players(char* players);
};

#endif