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
                        write_formatted_log(GRAY "[SERVER LOG] Its one player in join" RESET "\n");
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
                        write_formatted_log(GRAY "[SERVER LOG] Its one player waiting" RESET "\n");
                        //MISS 
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

                 
                 AssignROLE();

        //SEND ROLE ONE TO ONE
                 for(auto player:players)
                 {
                 char response[DEFAULT_BUFLEN];
                 strcpy(response,GAME_EVENT_ROLE);
                 strcat(response,(player.role)+"");
                 send_message(player.getFdId, response, listener, master, DEFAULT_BUFLEN);
                 }

    
                // TODO: Generar roles
            
                write_formatted_log(GRAY "[SERVER LOG] Changing state to NIGHT" RESET "\n");
                stage = STAGE_NIGHT;
                return;
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


void GameServer::AssignROLE()
{
         srand(static_cast<unsigned>(time(0)));
            int j = 0;
            int villager = 0, witch = 0, hunter = 0, seer = 0, lobo = 0;
            int wolf = ((players.size() - 3) / 5) + 1;//calculate the wolf

     //so that the cycle is done for as long as it is smaller than the size of the players.
      while (j <= players.size()) 
      {
        //generate the random numbers
        int randomNumber = rand() % 5;

    
            switch (randomNumber) 
            {
                //villagers
                case ROLE_VILLAGER:
                    if (villager < (players.size() - wolf - 3)) {
                        player[j].role = ROLE_VILLAGER;
                        villager++;
                        j++;
                    }
                    break;
                    //for te total of player-3/5+1 that is for the total of wolf
                case ROLE_WEREWOLF:
                    if (lobo < wolf) {
                        player[j].role = ROLE_WEREWOLF;
                        lobo++;
                        j++;
                    }
                    break;
                  //for one witch cycle
                case ROLE_WITCH:
                    if (witch < 1) {
                        player[j].role = ROLE_WITCH;
                        witch++;
                        j++;
                    }
                    break;
                 //for one hunter cycle
                case ROLE_HUNTER:
                    if (hunter < 1) {
                       player[j].role= ROLE_HUNTER;
                        hunter++;
                        j++;
                    }
                    break;
               //for one seer cycle
                case ROLE_SEER:
                    if (seer < 1) {
                        player[j].role = ROLE_SEER;
                        seer++;
                        j++;
                    }
                    break;

                default:
                    break;
            
        }
    }
 
}