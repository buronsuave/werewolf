
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
    std::vector<Player> werewolves;
    void handle_recv(fd_set master, int fdmax, int listener, int i, char buf[], int nbytes);
    void assign_role(std::vector<Player> players, ROLE _role);
    void check_name(std::vector<Player> players, fd_set master, int listener, int i, char buf[], int nbytes);
    void check_mainhost(std::vector<Player> players, fd_set master, int listener, int i, char buf[], int nbytes);
};

#endif