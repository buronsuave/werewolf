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
    STAGE_ROLE = 6,  
    STAGE_GAME_OVER = 7  
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
#define GAME_EVENT_ACTION_WORELOF "worelof"
#define GAME_EVENT_ACTION_WITCH "witch"
#define GAME_EVENT_ACTION_SEER "seer"
#define GAME_EVENT_VICTIM_HUNTER "cazador"
#define GAME_EVENT_ACTION_HUNTER2 "hunter2"
#define GAME_EVENT_ACTION_HUNTER1 "hunter1"
#define GAME_EVENT_ACTION_EVERYONE "hunter1"
#define GAME_EVENT_NIGHT "nigth"
#define GAME_EVENT_DAY "day"
#define GAME_EVENT_NEW "new"
#define GAME_EVENT_GAMEOVER "gameover"

#define GAME_MAX_PLAYERS 20
#define GAME_MIN_PLAYERS 8

#endif