
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
    int conter=0;


    void handle_recv(fd_set master, int fdmax, int listener, int i, char buf[], int nbytes);
    void assign_role();
    void check_mainhost(fd_set master, int listener, int i, char buf[], int nbytes);
    void player_list(fd_set master, int listener, int fdmax, char buf[], int nbytes);
    void current_players_check(fd_set master, int listener, int fdmax, char buf[], int nbytes);


};

#endif