#include "GameServer.h"







void GameServer::handle_recv(fd_set master, int fdmax, int listener, int i, char buf[], int nbytes)
{
    write_formatted_log(GRAY "[SERVER LOG] Message received: %s" RESET "\n", buf);
    switch (stage)
    {
        case STAGE_NEW:
        {
            if (strstr(buf, GAME_EVENT_JOIN))
            {
                char response[DEFAULT_BUFLEN] = GAME_EVENT_MAINHOST;
                write_formatted_log(GRAY "[SERVER LOG] Join request detected" RESET "\n");
                send_message(i, response, listener, master, DEFAULT_BUFLEN);
                write_formatted_log(GRAY "[SERVER LOG] Changing state to LOBBY" RESET "\n");
                stage = STAGE_LOBBY;
                return;
            }
            else
            {
                char response[DEFAULT_BUFLEN] = GAME_EVENT_BADREQUEST;
                write_formatted_log(GRAY "[SERVER LOG] Invalid message recieved" RESET "\n");
                send_message(i, response, listener, master, DEFAULT_BUFLEN);
                return;
            }

            break;
        }

        case STAGE_LOBBY:
        {
            if (strstr(buf, GAME_EVENT_JOIN))
            {
                char response[DEFAULT_BUFLEN] = GAME_EVENT_LOBBY;
                write_formatted_log(GRAY "[SERVER LOG] Join request detected" RESET "\n");
                send_message(i, response, listener, master, DEFAULT_BUFLEN);
                return; 
            }
            else if (strstr(buf, GAME_EVENT_NAME))
            {
                // Create Player and assign name
                buf += strlen(GAME_EVENT_NAME);
                // Check if string is already in one player
                // if so...

                
                      if(players.size() != 0){

                        char response[DEFAULT_BUFLEN] = GAME_EVENT_LOBBY;
                        write_formatted_log(GRAY "[SERVER LOG] -----------------" RESET "\n");
                        send_message(i, response, listener, master, DEFAULT_BUFLEN);
                        return; 

                      }
            
                //      send game event lobby
                //      return
                Player player(i, buf);
                players.push_back(player);

                printf("Player with name %s has joinded", players[players.size()-1].getName());
                return;
            }
            else if (strstr(buf, GAME_EVENT_INIT)) // Needs to implement the check of current number of players
            {   
                
                // unsigned int Current_Players;
                // Current_Players = players.size();


                if (players.size() < GAME_MIN_PLAYERS || players.size() > GAME_MAX_PLAYERS)
                {
                    char response[DEFAULT_BUFLEN] = GAME_EVENT_MAINHOST;
                    write_formatted_log(GRAY "[SERVER LOG] Invalid number of players" RESET "\n");
                    send_message(i, response, listener, master, DEFAULT_BUFLEN);
                    return;
                }

                // nameAlexa
                // role3jugador1,jugador2,jugador3

                // REF: EXAMPLES
                // EXAMPLE: HOW TO SEND A MESSAGE TO ALL WEREWOLVES?

                // std::vector<Player> werewolves;
                // for (auto player:players)
                // {
                //     if (player.getRole() == ROLE_WEREWOLF && player.isAlive())
                //     {
                //         werewolves.push_back(player);
                //     }
                // }
                // // werewolves -> all players with role werewolve
                // for (auto werewolf:werewolves)
                // {
                //     send_message(werewolf.getFdId(), "a", listener, master, DEFAULT_BUFLEN);
                // }

                // EXAMPLE: HOW TO SEND A MESSAGE TO PLAYER WITH ROLE SEER?
                // for (auto player:players)
                // {
                //     if (player.getRole() == ROLE_SEER)
                //     {
                //         send_message(player.getFdId(), "a", listener, master, DEFAULT_BUFLEN);
                //         break;
                //     }
                // }


                // Create Player and assign name
                buf += strlen(GAME_EVENT_INIT);
                // Check if string is already in one player
                // if so...

                if(players.size() != 0){

                        char response[DEFAULT_BUFLEN] = GAME_EVENT_MAINHOST;
                        write_formatted_log(GRAY "[SERVER LOG] Players in the Mainhost" RESET "\n");
                        send_message(i, response, listener, master, DEFAULT_BUFLEN);
                        return; 

                      }


                //      send game event mainhost
                //      return
                Player player(i, buf);
                players.push_back(player);
                printf("Player with name %s has joinded", players[players.size()-1].getName());

                char response[DEFAULT_BUFLEN] = GAME_EVENT_START;
                write_formatted_log(GRAY "[SERVER LOG] Init request detected" RESET "\n");
                write_formatted_log(GRAY "[SERVER LOG] Current value of fdmax: %d" RESET "\n", fdmax);
                send_broadcast(fdmax, response, listener, master, DEFAULT_BUFLEN);

                // TODO: Generar roles
                return;

                write_formatted_log(GRAY "[SERVER LOG] Changing state to NIGHT" RESET "\n");
                stage = STAGE_NIGHT;
            }
            else
            {
                char response[DEFAULT_BUFLEN] = GAME_EVENT_BADREQUEST;
                write_formatted_log(GRAY "[SERVER LOG] Invalid message recieved" RESET "\n");
                send_message(i, response, listener, master, DEFAULT_BUFLEN);
            }

            break;
        }
    }
}