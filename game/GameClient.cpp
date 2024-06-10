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
                printf(" Eres un villager. Su objetivo es eliminar a los werewolves.\n");
                break;
            case 1:
                printf("Eres un werewolf. Su objetivo es eliminar a los villagers sin ser atrapado.\n");
                break;
            case 2:
                printf("Eres la witch. Puedes salvar a alguien o matarlo.\n");
                break;
            case 3:
                printf("Eres es un seer. Puede verificar el rol de un jugador cada noche.\n");
                break;
            case 4:
                 printf("Eres es un hunter. Su objetivo es eliminar a los werewolves.\n");
                break;
            default:
                printf("Rol desconocido para el jugador.\n");
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

              printf("You are the wolf\n");
              printf("They must choose who they want to kill \n");
              printf("write the name of the person who will die: ");
              char response[DEFAULT_BUFLEN];
              char person_to_kill[DEFAULT_BUFLEN];
              strcpy(response, GAME_EVENT_ACTION_WEREWOLF);
              std::cin.getline(person_to_kill, DEFAULT_BUFLEN);
              strcat(response, person_to_kill);
              send_message(s, response, DEFAULT_BUFLEN);
              return; 
          }
          else if (strstr(buf, GAME_EVENT_ACTION_SEER))
          {
              printf("You are the seer\n");
              printf("you must choose who want to know if he/she is a wolf \n");
              printf("write the name of the person do you want to know: ");
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
              printf("The player you see was a wolf\n");
              char response[DEFAULT_BUFLEN];
              strcpy(response, GAME_EVENT_DAY);
              send_message(s, response, DEFAULT_BUFLEN);
              this->stage = STAGE_DAY; 
              return;
          }
          else if (strstr(buf, GAME_EVENT_SEER_CHECK_NOWOLF))
          {
              printf("The player you see was NOT a wolf\n");
              char response[DEFAULT_BUFLEN];
              strcpy(response, GAME_EVENT_DAY);
              send_message(s, response, DEFAULT_BUFLEN);
              this->stage = STAGE_DAY;
              return;
          }
          else if (strstr(buf, GAME_EVENT_ACTION_WITCH))
          {
              printf("You are the witch\n");
              printf("you must choose who they want to kill or save \n");
              printf("write the action do you want to do: ");
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
 

                }

            return;
          }else if(strstr(buf, GAME_EVENT_ACTION_SAVE)){

          char person_to_kill[DEFAULT_BUFLEN];
          char response[DEFAULT_BUFLEN];
                 printf("write the name of the person who will save: ");
                 std::cin.getline(person_to_kill, DEFAULT_BUFLEN);

            strcpy(response, GAME_EVENT_ACTION_SAVE);
            strcat(response, person_to_kill);
            send_message(s, response, DEFAULT_BUFLEN);


return;
          }else if(strstr(buf, GAME_EVENT_ACTION_KILL)){

  char person_to_kill[DEFAULT_BUFLEN];
          char response[DEFAULT_BUFLEN];
                 printf("write the name of the person who will die: ");
                 std::cin.getline(person_to_kill, DEFAULT_BUFLEN);

            strcpy(response, GAME_EVENT_ACTION_KILL);
            strcat(response, person_to_kill);
            send_message(s, response, DEFAULT_BUFLEN);
return;

          }

          else if (strstr(buf, GAME_EVENT_ACTION_WAITING))
          {
            printf("The other players are doing their actions\n");
            return;
          }
          else if(strstr(buf,GAME_EVENT_DAY))
          {
             char response[DEFAULT_BUFLEN];
                   strcpy(response, GAME_EVENT_ACTION_WITCH);
                   strcat(response, GAME_EVENT_ACTION_KILL);
                   send_message(s, response, DEFAULT_BUFLEN);
            stage = STAGE_DAY;
            return;
          }
          // si no error 
          if (strstr(buf, GAME_EVENT_ALIVE_PLAYERS))
          {
            printf("estas vivo");
          }
          else {
            printf("Fatal Error from STAGE NIGHT");
          }
          break;

    }
        case STAGE_DAY:
        {
         if(strstr(buf,GAME_EVENT_DAY))
         {
                printf("Es de dia voten por una persona");
                return;
         }
         if(strstr(buf,GAME_EVENT_NEW))
         {
           // EXTRAER MUERTOS DESDE EL MENSAJE
           // MOSTRAR MUERTOS
           // SI ESTOY EN MUERTOS Y NO SOY CAZADOR
         //{
             stage = STAGE_VIEWER;
         //}
         }
         if(strstr(buf,GAME_EVENT_NEW))
         {
            //ESPERAR ENTRADA DE USUARIO (JUGADOR)
            //ENVIAR A SERVIDOR "VICTIMA CAZADOR 1" + JUGADOR 
            stage=STAGE_VIEWER;
            return;
         }
         if(strstr(buf,GAME_EVENT_NEW))
         {
           //ESPERAR ENTRADA DE USUARIO (JUGADOR)
           //ENVIAR A SERVIDOR "VICTIMA CAZADOR 2" + JUGADOR
           stage = STAGE_VIEWER;
            return;
         }
         if(strstr(buf,GAME_EVENT_VICTIM_HUNTER))
         {
              //EXTRAER VICTIMA DESDE MENSAJE
              //MOSTRAR VICTIMA
              //SI SOY VICTIMA
               stage = STAGE_VIEWER;
            return;
         }
         if(strstr(buf,GAME_EVENT_ACTION_EVERYONE))
         {
            //ESPERAR ENTRADA DE USUARIO (VOTO)
            //ENVIAR A SERVIDOR "VOTO JUGADOR" + VOTO
            return;
         }
         
         if(strstr(buf,GAME_EVENT_NIGHT))
         {
            stage = STAGE_NIGHT;
            return;
         }
         if(strstr(buf,GAME_EVENT_OVER))
         {
            //EXTRAER GANADOR DESDE EL MENSAJE
            //MOSTRAR GANADOR
            stage = STAGE_OVER;
            return; //fin del juego
         }
         else
         {
            printf("Fatal Error from STAGE DAY ");
        } 
    }

    }

}