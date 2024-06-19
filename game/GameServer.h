
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
    void assign_role();
    void check_mainhost(fd_set master, int listener, int i, char buf[], int nbytes);
    void current_players_check(fd_set master, int listener, int fdmax);
    void decision_won_lost(fd_set master, int listener, int fdmax);
    void player_list_dead(char *dead_players);
    void witch_action(fd_set master, int listener);
    void werewolf_action(fd_set master, int listener);
    void seer_action(fd_set master, int listener);
    void get_active_players(char* active_players);
    void hunter_action(fd_set master, int listener,int fdmax);
    void vote_max(fd_set master, int listener, int fdmax, char buf[]);
    void change_to_day(fd_set master, int listener);

}; 

#endif