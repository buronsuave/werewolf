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
                printf("Type your name, and when you get ready press enter to start the play.\n");
                char response[DEFAULT_BUFLEN];
                char name[DEFAULT_BUFLEN];
                strcpy(response, GAME_EVENT_INIT);
                std::cin.getline(name, DEFAULT_BUFLEN);
                strcat(response, name);

                send_message(s, response, DEFAULT_BUFLEN);
                return;
            }
            // TODO: lobby,jugador1,jugador2,jugador3
            else if (strstr(buf, GAME_EVENT_LOBBY)) // TODO: Checar si GAME_EVENT_LOBBY esta contenida en el buf
            {
                // TODO: MOSTRAR LOS JUGADORES QUE SE HAN UNIDO
                printf("You've just jumped to the lobby.\nWaiting for main host to start.\n");
                printf("Please enter your name: ");
                char response[DEFAULT_BUFLEN];
                char name[DEFAULT_BUFLEN];
                strcpy(response, GAME_EVENT_NAME);
                std::cin.getline(name, DEFAULT_BUFLEN);

                strcat(response, name);
                send_message(s, response, DEFAULT_BUFLEN);
                return;
            }
            else if (strstr(buf, GAME_EVENT_START))
            {
                printf("Main host started the play!\n");
                write_formatted_log(GRAY "[CLIENT LOG] Changing state to NIGHT\n" RESET);
                stage = STAGE_ROLE;
                return;
            }
            // Add case for loading card information
            else
            {
                write_formatted_log(GRAY "[CLIENT LOG] Unable to handle message\n" RESET);
                return;
            }

            break;
        }

        case STAGE_ROLE:
        {
             if (strstr(buf,GAME_EVENT_ROLE ))
             {
                
             }
             else {
                printf("Error fatal");
             }


        }
        case STAGE_NIGHT:
        {
  
        }
        case STAGE_DAY:
        {

        } 
    }
}

