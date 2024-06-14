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
                write_formatted_log(GRAY "[CLIENT LOG] Changing state to ROLE\n" RESET);
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
             if (strstr(buf,GAME_EVENT_ROLE))
             {
                int t;
                sscanf(buf + strlen(GAME_EVENT_ROLE), "%d", &t);
                this->role = ROLE(t);

           //printf("My role is %d", this->role); borrar al final
              
              //To send the rol in string format
        switch (this->role) {
            case 0:
                printf("You're a villager. Your objective is to eliminate the werewolves.\n");
                break;
            case 1:
                printf("You're a werewolf. Your objective is to eliminate the villagers without being caught.\n");
                break;
            case 2:
                printf("You're the witch. You can kill or save someone.\n");
                break;
            case 3:
                printf("You're the seer. You can ask for the role of someone during night.\n");
                break;
            case 4:
                 printf("You're the hunter. You can kill someone when you've been killed. Your objective is to eliminate the werewolves.\n");
                break;
            default:
                printf("Unknown role.\n");
                break;

              }

                write_formatted_log(GRAY "[CLIENT LOG] Changing state to NIGHT\n" RESET);   
                this->stage = STAGE_NIGHT;
                
             }
             else {//POR ESO SE IMPRIME FATAL
                printf("Fatal Error from STAGE ROLE\n");
                
             }
             break;


        }/*case STAGE_OVER:
        {
           

        }*/
        case STAGE_NIGHT:
        {
            if(strstr(buf, GAME_EVENT_ACTION_WEREWOLF))
          {
            //ESPERAR ENTRADA DE USUARIO (VOTO)
           // ENVIAR A SERVIDOR "VOTO LOBO" + VOTO
              printf("You are a wolf.\n");
              printf("You must vote for who will be killed during this night.\n\n");
              
              // Unwrap list of active players
              buf += strlen(GAME_EVENT_ACTION_WEREWOLF);
              print_active_players(buf);

              printf("Write the name of the person who will die: ");
              char response[DEFAULT_BUFLEN];
              char person_to_kill[DEFAULT_BUFLEN];
              strcpy(response, GAME_EVENT_ACTION_WEREWOLF);
              std::cin.getline(person_to_kill, DEFAULT_BUFLEN);
              strcat(response, person_to_kill);
              send_message(s, response, DEFAULT_BUFLEN);
              return; 
          }
          else if (strstr(buf, GAME_EVENT_ACTION_WITCH))
          {
              printf("You are the witch.\n");
              printf("You must choose someone and decide if will be saved or killed.\n\n");

              // Unwrap list of active players
              buf += strlen(GAME_EVENT_ACTION_WITCH);
              print_active_players(buf);

              printf("Write the action do you want to do: ");
              char save_kill[DEFAULT_BUFLEN]; 
             std::cin.getline(save_kill, DEFAULT_BUFLEN);
                if (strstr(save_kill,GAME_EVENT_ACTION_SAVE)){
                  char response[DEFAULT_BUFLEN];
                   strcpy(response, GAME_EVENT_ACTION_WITCH);
                   strcat(response, GAME_EVENT_ACTION_SAVE);
                   send_message(s, response, DEFAULT_BUFLEN);
                }else if(strstr(save_kill,GAME_EVENT_ACTION_KILL)){
                        char response[DEFAULT_BUFLEN];
                   strcpy(response, GAME_EVENT_ACTION_WITCH);
                   strcat(response, GAME_EVENT_ACTION_KILL);
                   send_message(s, response, DEFAULT_BUFLEN);
                  
                }else{
 
                    // Something else... (error while selecting option)
                }
            return;
          }else if(strstr(buf, GAME_EVENT_ACTION_SAVE)){

          char person_to_kill[DEFAULT_BUFLEN];
          char response[DEFAULT_BUFLEN];
                 printf("Write the name of the person who will be saved: ");
                 std::cin.getline(person_to_kill, DEFAULT_BUFLEN);

            strcpy(response, GAME_EVENT_ACTION_SAVE);
            strcat(response, person_to_kill);
            send_message(s, response, DEFAULT_BUFLEN);


return;
          }else if(strstr(buf, GAME_EVENT_ACTION_KILL)){

  char person_to_kill[DEFAULT_BUFLEN];
          char response[DEFAULT_BUFLEN];
                 printf("Write the name of the person who will be killed: ");
                 std::cin.getline(person_to_kill, DEFAULT_BUFLEN);

            strcpy(response, GAME_EVENT_ACTION_KILL);
            strcat(response, person_to_kill);
            send_message(s, response, DEFAULT_BUFLEN);
return;

          }
          else if (strstr(buf, GAME_EVENT_ACTION_SEER))
          {
              printf("You are the seer.\n");
              printf("You must choose someone and ask if they is a werewolf.\n\n");

              // Unwrap list of active players
              buf += strlen(GAME_EVENT_ACTION_SEER);
              print_active_players(buf);

              printf("Write the name of the person do you want to know: ");
              char response[DEFAULT_BUFLEN];
              char person_know[DEFAULT_BUFLEN];
              strcpy(response, GAME_EVENT_ACTION_SEER);
              std::cin.getline(person_know, DEFAULT_BUFLEN);
              strcat(response, person_know);
              send_message(s, response, DEFAULT_BUFLEN);
            return;
          }
          else if (strstr(buf, GAME_EVENT_SEER_CHECK_WOLF))
          {
              printf("The player you asked for is a werewolf.\n");
              char response[DEFAULT_BUFLEN];
              strcpy(response, GAME_EVENT_DAY);
              send_message(s, response, DEFAULT_BUFLEN); 
              this -> stage = STAGE_DAY;
              return;
             
          }
          else if (strstr(buf, GAME_EVENT_SEER_CHECK_NOWOLF))
          {
              printf("The player you asked for is NOT a werewolf.\n");
              char response[DEFAULT_BUFLEN];
              strcpy(response, GAME_EVENT_DAY);
              send_message(s, response, DEFAULT_BUFLEN);
              this -> stage = STAGE_DAY;
              return;
          }
          else if (strstr(buf, GAME_EVENT_ACTION_WAITING))
          {
            printf("Wait until all the other players are done with their actions.\n");
            return;
          }
          else {
            printf("Fatal Error from STAGE NIGHT.\n");
            write_formatted_log(GRAY "[CLIENT LOG] FATAL ERROR FROM STAGE NIGHT\n" RESET);
            
          }
          break;

    }
        case STAGE_DAY:
        {
        /* if(strstr(buf,GAME_EVENT_DAY))
         {
                printf("kill who you suspect to be the wolf: ");
                char response[DEFAULT_BUFLEN];
                char person_wolf[DEFAULT_BUFLEN];
                std::cin.getline(person_wolf, DEFAULT_BUFLEN);

  
                strcpy(response, GAME_EVENT_DAY);
                strcat(response, person_wolf);
                send_message(s, response, DEFAULT_BUFLEN);

         }else if(strstr(buf,GAME_EVENT_ACTION_HUNTER))
         {
           
             buf += strlen(GAME_EVENT_ACTION_HUNTER);
              print_active_players(buf);

             printf("who do u want to kill: ");
              char response[DEFAULT_BUFLEN];
              char person_kill[DEFAULT_BUFLEN];
              strcpy(response, GAME_EVENT_ACTION_HUNTER);
              std::cin.getline(person_kill, DEFAULT_BUFLEN);
              strcat(response, person_kill);
              send_message(s, response, DEFAULT_BUFLEN);
         
            return;
         }
         else if(strstr(buf,GAME_EVENT_OVER))
         {
            
            printf("Se ha acabdo la partida los lobos han ganado");
           //supongo que un break;
         }
         else
         {
            printf("Fatal Error from STAGE DAY.\n");
        } */

          if(strstr(buf,GAME_EVENT_ACTION_HUNTER)){
            printf("action hunter");
          }


    }

    }

}


void GameClient::print_active_players(char* players)
{
    printf("Active Players: \n");
    while (*players != '\0')
    {
        if (*players != ',') printf("%c", *players);
        else printf("\n");
        players++;
    }
    printf("\n\n");
}


void GameClient::print_dead_players(char* players)
{
    printf("Dead Players: \n");
    while (*players != '\0')
    {
        if (*players != ',') printf("%c", *players);
        else printf("\n");
        players++;
    }
    printf("\n\n");
}