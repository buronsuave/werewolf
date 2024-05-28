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

                
                 void check_mainhost(std::vector<Player> players, fd_set master, int listener, int i, char buf[], int nbytes);
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

                // 
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
                    
                void check_name(std::vector<Player> players, fd_set master, int listener, int i, char buf[], int nbytes);  
                 
          
                //      send game event mainhost
                //      return
                Player player(i, buf);
                players.push_back(player);
                printf("Player with name %s has joinded", players[players.size()-1].getName());

                char response[DEFAULT_BUFLEN] = GAME_EVENT_START;
                write_formatted_log(GRAY "[SERVER LOG] Init request detected" RESET "\n");
                write_formatted_log(GRAY "[SERVER LOG] Current value of fdmax: %d" RESET "\n", fdmax);
                send_broadcast(fdmax, response, listener, master, DEFAULT_BUFLEN);

                
                void assign_role(std::vector<Player> players, ROLE _role);

        //SEND ROLE ONE TO ONE
                for(auto player:players)
                {

                char response[DEFAULT_BUFLEN];
                strcpy(response,GAME_EVENT_ROLE);
                strcat(response,(player._role) + "");
                send_message(player._fd_id, response, listener, master, DEFAULT_BUFLEN);
                write_formatted_log(GRAY "[SERVER LOG] Sending Role for all " RESET "\n");

                }


                  //response = "role" + "0 o ROLE_VILLAGER"
    
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
        case STAGE_NIGHT :
        {
     
         

            









        }
    }
}


void GameServer::assign_role(std::vector<Player> players, ROLE _role)
{
            srand(static_cast<unsigned>(time(0)));
            int j = 0;
            //To the total amount of each role.
            int villager_role = 0, witch_role = 0, hunter_role = 0, seer_role = 0, wolf_role = 0;
            //The total of wolf per role.
            int total_wolf = (((players.size() - 3) / 5) * 2);//calculate the wolf

            //So that the cycle is done for as long as it is smaller than the size of the players.
            while (j <= players.size()) 
           {
                //Generate the random numbers
                int randomNumber = rand() % 5;

    
                switch (randomNumber) 
              {

                
                case ROLE_VILLAGER:
                {
                    if (villager_role < (players.size() - total_wolf - 3)) 
                    {
                        players[j]._role = ROLE_VILLAGER;
                        villager_role++;
                        j++;
                    }
                  break;

                }
                    
                case ROLE_WEREWOLF:
                {
                    if (wolf_role < total_wolf) 
                    {
                        players[j]._role = ROLE_WEREWOLF;
                        wolf_role++;
                        j++;
                    }
                  break;
                }
                 
                case ROLE_WITCH:
                {
                    if (witch_role < 1) 
                    {
                        players[j]._role = ROLE_WITCH;
                        witch_role++;
                        j++;
                    }
                  break;
                }
                 
                case ROLE_HUNTER:
                {
                    if (hunter_role < 1) 
                    {
                       players[j]._role= ROLE_HUNTER;
                       hunter_role++;
                       j++;
                    }
                  break;
                }
               
                case ROLE_SEER:
                {
                    if (seer_role < 1) 
                    {
                        players[j]._role = ROLE_SEER;
                        seer_role++;
                        j++;
                    }
                  break;
                }

                default:
                    break;
            
              }
            }
 
}


void GameServer::check_name(std::vector<Player> players, fd_set master, int listener, int i, char buf[], int nbytes)
{

        for(auto player:players)
        {
           //TO check if the players have the same name
            if(strstr(players[i]._name, buf))
            {

            printf("The name is alredy in use, try again with other name");
            write_formatted_log(GRAY "[SERVER LOG] ERROR try again with other name player" RESET "\n");
                        

            char response[DEFAULT_BUFLEN] = GAME_EVENT_NAME;
            send_message(i, response, listener, master, DEFAULT_BUFLEN);
            stage = STAGE_LOBBY;
            return; 

            }else
            {  

            write_formatted_log(GRAY "[SERVER LOG] One more player its in the lobby" RESET "\n");
                        

            char response[DEFAULT_BUFLEN] = GAME_EVENT_INIT;
            send_message(i, response, listener, master, DEFAULT_BUFLEN);
            return;
            }
        }
}

void GameServer::check_mainhost(std::vector<Player> players, fd_set master, int listener, int i, char buf[], int nbytes)
{

        for (auto player:players)
        {
                    
            if(strstr(buf, GAME_EVENT_MAINHOST))
            {

            if(strstr(players[i]._name, buf)) 
            {
                         
            char response[DEFAULT_BUFLEN] = GAME_EVENT_LOBBY;
            write_formatted_log(GRAY "[SERVER LOG] Its the mainhost in the game" RESET "\n");
            send_message(i, response, listener, master, DEFAULT_BUFLEN);
            stage=STAGE_LOBBY;
            return; 
            }
                            
            }else
            {
            char response[DEFAULT_BUFLEN] = GAME_EVENT_NEW;
            write_formatted_log(GRAY "[SERVER LOG] ERROR no mainhost " RESET "\n");
            send_message(i, response, listener, master, DEFAULT_BUFLEN);
            stage=STAGE_NEW;
            return;
            }
        }
}