
#ifndef GAME_SERVER_H
#define GAME_SERVER_H

#include "common.h"
#include "Player.h"

class GameServer : public Server
{
    private:
    STAGE stage = STAGE_NEW;

    public:
    std::vector<Player> players;
    void handle_recv(fd_set master, int fdmax, int listener, int i, char buf[], int nbytes);
    void AssignROLE();

};

#endif