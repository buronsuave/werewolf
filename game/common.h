#ifndef GAME_COMMON_H
#define GAME_COMMON_H

#include "../net/Client.h"
#include "../net/Server.h"

enum STAGE
{
    STAGE_LOBBY = 0,
    STAGE_DAY = 1,
    STAGE_NIGHT = 2,
    STAGE_OVER = 3,
    STAGE_NEW = 4,
    STAGE_VIEWER= 5,
    STAGE_ROLE = 6    
};

enum ROLE
{
    ROLE_VILLAGER = 0,
    ROLE_WEREWOLF = 1,
    ROLE_WITCH = 2,
    ROLE_HUNTER = 3,
    ROLE_SEER = 4,
    ROLE_UNASIGNED = 5
};

#define GAME_EVENT_MAINHOST "mainhost"
#define GAME_EVENT_JOIN "join"
#define GAME_EVENT_INIT "init"
#define GAME_EVENT_LOBBY "lobby"
#define GAME_EVENT_START "start"
#define GAME_EVENT_BADREQUEST "badrequest"
#define GAME_EVENT_NAME "name"
#define GAME_EVENT_ROLE "role"

#define GAME_MAX_PLAYERS 20
#define GAME_MIN_PLAYERS 8

#endif