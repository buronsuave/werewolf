#include "GameServer.h"

void GameServer::handle_recv(fd_set master, int fdmax, int listener, int i, char buf[], int nbytes)
{
    printf(GRAY "[SERVER LOG] Message received: %s" RESET "\n", buf);
    switch (stage)
    {
        case STAGE_NEW:
        {
            if (strstr(buf, "join"))
            {
                char response[100] = "mainhost";
                printf(GRAY "[SERVER LOG] Join request detected" RESET "\n");
                send_message(i, response, listener, master, 101);
                printf(GRAY "[SERVER LOG] Changing state to LOBBY" RESET "\n");
                stage = STAGE_LOBBY;
            }
            else
            {
                char response[100] = "badrequest";
                printf(GRAY "[SERVER LOG] Invalid message recieved" RESET "\n");
                send_message(i, response, listener, master, 101);
            }

            break;
        }

        case STAGE_LOBBY:
        {
            if (strstr(buf, "join"))
            {
                char response[100] = "lobby";
                printf(GRAY "[SERVER LOG] Join request detected" RESET "\n");
                send_message(i, response, listener, master, 101);
            }
            else if (strstr(buf, "init")) // Needs to implement the check of current number of players
            {
                char response[100] = "start";
                printf(GRAY "[SERVER LOG] Init request detected" RESET "\n");

                // Create and send roles. Starting game with night
                printf(GRAY "[SERVER LOG] Current value of fdmax: %d" RESET "\n", fdmax);
                send_broadcast(fdmax, response, listener, master, 101);
                printf(GRAY "[SERVER LOG] Changing state to NIGHT" RESET "\n");
                stage = STAGE_NIGHT;
            }
            else
            {
                char response[100] = "badrequest";
                printf(GRAY "[SERVER LOG] Invalid message recieved" RESET "\n");
                send_message(i, response, listener, master, 101);
            }

            break;
        }
    }
}
