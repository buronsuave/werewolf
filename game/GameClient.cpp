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
            else if (strstr(buf, GAME_EVENT_LOBBY))
            {
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
            else
            {
                write_formatted_log(GRAY "[CLIENT LOG] Unable to handle message\n" RESET);
                return;
            }
            break;
        }

        case STAGE_ROLE: {
    if (strstr(buf, GAME_EVENT_ROLE)) {
            char buf[DEFAULT_BUFLEN];
            recv(s, buf, WINDOW_BUFFER_SIZE_EVENT - 1, 0);
             char roles[DEFAULT_BUFLEN];
             strcpy(roles,buf);
           
              for (int i = 0; i < strlen(roles); ++i) {
        switch (roles[i]) {
            case '0':
                std::cout << "Jugador " << i + 1 << " es un villager. Su objetivo es eliminar a los werewolves.\n";
                break;
            case '1':
                std::cout << "Jugador " << i + 1 << " es un werewolf. Su objetivo es eliminar a los villagers sin ser atrapado.\n";
                break;
            case '2':
                std::cout << "Jugador " << i + 1 << " es un seer. Puede verificar el rol de un jugador cada noche.\n";
                break;
            case '3':
                std::cout << "Jugador " << i + 1 << " es un witch. Puede verificar el rol de un jugador cada noche.\n";
                break;
            case '4':
                std::cout << "Jugador " << i + 1 << " es un hunter. Su objetivo es eliminar a los werewolves.\n";
                break;
            default:
                std::cout << "Rol desconocido para el jugador " << i + 1 << ".\n";
                break;
        }
    }
    } else {
        printf("Unexpected message received in ROLE stage\n");
        return;
    }
    break;
}


        case STAGE_NIGHT:
        {
            if (strstr(buf, GAME_EVENT_ACTION_WORELOF))
            {  
                 //ESPERAR ENTRADA DE USUARIO (VOTO)
                char response[DEFAULT_BUFLEN];
                char VOTO[DEFAULT_BUFLEN] ;
                std::cin.getline(response, DEFAULT_BUFLEN);
                //ENVIAR A SERVIDOR "VOTO LOBO" + VOTO
                strcpy(response, GAME_EVENT_VOTO_LOBO );
                strcat(response,VOTO);
              
               send_message(s , response,DEFAULT_BUFLEN);
               printf("tu voto selecionado es : %s",response);

                return;
            }
            if (strstr(buf, GAME_EVENT_ACTION_SEER))
            {
                //ESPERAR ENTRADA DE USUARIO (JUGADOR)
                char response[DEFAULT_BUFLEN];
                char VOTO[DEFAULT_BUFLEN] ;
                std::cin.getline(response, DEFAULT_BUFLEN);
                //ENVIAR A SERVIDOR "PREGUNTA VIDENTE" + JUGADOR
                 strcpy(response, GAME_EVENT_CUESTION_SEER );
                strcat(response,VOTO);
                send_message(s , response,DEFAULT_BUFLEN);
               printf("tu voto selecionado es : %s",response);
                return;
            }
            if (strstr(buf,GAME_EVENT_RESPONSE_SEER)){
                //EXTRAER RESPUESTA DESDE EL MENSAJE
                char buf[DEFAULT_BUFLEN];
            recv(s, buf, WINDOW_BUFFER_SIZE_EVENT - 1, 0);
             char   respuesta[DEFAULT_BUFLEN];
             strcpy(respuesta,buf);
                //MOSTRAR RESPUESTA
                printf("la respuesta a la pregunta del seer es : %s",respuesta);
                return;
            }
           if (strstr(buf, GAME_EVENT_ACTION_WITCH)) {
        char jugador[DEFAULT_BUFLEN];
        char accion[DEFAULT_BUFLEN];
        char response[DEFAULT_BUFLEN];
           //ESPERAR ENTRADA DE USUARIO (JUGADOR, ACCION)
        printf("Ingrese el nombre del jugador: ");
        std::cin.getline(jugador, DEFAULT_BUFLEN);

        printf("Ingrese la acción de la bruja: ");
        std::cin.getline(accion, DEFAULT_BUFLEN);

          // Crear el mensaje para el servidor
        snprintf(response, DEFAULT_BUFLEN, "%s: %s %s", GAME_EVENT_ACTION_WITCH, jugador, accion);
          //ENVIAR A SERVIDOR "POCION BRUJA" + (JUGADOR, ACCION)
        send(s, response, strlen(response), 0);
    }
            if (strstr(buf, GAME_EVENT_DAY))
            {
                stage = STAGE_DAY;
                return;
            }
            else
            {
                printf("Unexpected message received in NIGHT stage\n");
                return;
            }
            break;
        }

        case STAGE_DAY:
        {
            if (strstr(buf, GAME_EVENT_NEW))
            {

                // EXTRAER MUERTOS DESDE EL MENSAJE
                    char buf[DEFAULT_BUFLEN];
                   recv(s, buf,  - 1, 0);
   
               buf[DEFAULT_BUFLEN] = '\0'; // Añadir terminador de cadena

    // Mostrar la lista de muertos recibida
    std::cout << "La lista de muertos es: " << buf << std::endl;
               
                return;
            }
        
            if (strstr(buf, GAME_EVENT_ACTION_HUNTER1))
            {

                // ESPERAR ENTRADA DE USUARIO (JUGADOR)
                // ENVIAR A SERVIDOR "VICTIMA CAZADOR 1" + JUGADOR 
                stage = STAGE_VIEWER;
                return;
            }
            if (strstr(buf, GAME_EVENT_ACTION_HUNTER2))
            {
                // ESPERAR ENTRADA DE USUARIO (JUGADOR)
                // ENVIAR A SERVIDOR "VICTIMA CAZADOR 2" + JUGADOR
                stage = STAGE_VIEWER;
                return;
            }
            if (strstr(buf, GAME_EVENT_VICTIM_HUNTER))
            {
                // EXTRAER VICTIMA DESDE MENSAJE
                // MOSTRAR VICTIMA
                // SI SOY VICTIMA
                stage = STAGE_VIEWER;
                return;
            }
            if (strstr(buf, GAME_EVENT_ACTION_EVERYONE))
            {
                // ESPERAR ENTRADA DE USUARIO (VOTO)
                // ENVIAR A SERVIDOR "VOTO JUGADOR" + VOTO
                return;
            }
            if (strstr(buf, GAME_EVENT_NIGHT))
            {
                stage = STAGE_NIGHT;
                return;
            }
            if (strstr(buf, GAME_EVENT_GAMEOVER))
            {
                // EXTRAER GANADOR DESDE EL MENSAJE
                // MOSTRAR GANADOR
                stage = STAGE_GAME_OVER;
                return; // Fin del juego
            }
            else
            {
                printf("Fatal error\n");
                return;
            }
            break;
        }

        default:
        {
            printf("Unexpected stage\n");
            return;
        }
    
    }
}
