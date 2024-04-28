#include "GameClient.h"

void GameClient::handle_recv(char buf[], SOCKET s)
{
    switch (stage)
    {
        case STAGE_LOBBY:
        {
            if (strstr(buf, GAME_EVENT_MAINHOST))
            {
                printf("You're the main host\n");
                printf("You have the power to start the play!\n");
                system("pause");

                char response[DEFAULT_BUFLEN];
                strcpy(response, GAME_EVENT_INIT);
                send_message(s, response, DEFAULT_BUFLEN);
                return;
            }
            else if (strstr(buf, "lobby"))
            {
                printf("You've just jumped to the lobby.\nWaiting for main host to start.\n");
                return;
            }
            else if (strstr(buf, GAME_EVENT_START))
            {
                printf("Main host started the play!\n");
                printf(GRAY "[CLIENT LOG] Changing state to NIGHT\n" RESET);
                stage = STAGE_NIGHT;
                return;
            }
            // Add case for loading card information
            else
            {
                printf(GRAY "[CLIENT LOG] Unable to handle message\n" RESET);
                return;
            }

            break;
        }
    }
}

