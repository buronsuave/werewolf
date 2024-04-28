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
    STAGE_NEW = 4
};

#define GAME_EVENT_MAINHOST "mainhost"
#define GAME_EVENT_JOIN "join"
#define GAME_EVENT_INIT "init"
#define GAME_EVENT_LOBBY "lobby"
#define GAME_EVENT_START "start"
#define GAME_EVENT_BADREQUEST "badrequest"

#endif