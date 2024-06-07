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

                //YA no se que pedo pero pues cualquier cosa xd
                 
                 
                //      send game event lobby
                //      return

            char playerName[DEFAULT_BUFLEN];
            strcpy(playerName, buf);




                Player player(i, buf);

       //Funcion boleana y meter al codigo

                players.push_back(player);

                printf("Player with name %s has joinded", players[players.size()-1].getName());
                int conter=0;

                bool nameExists = false;

   /*while(conter < players.size()){
        if (strcmp(players[conter].getName(), buf)==0 && players.size()-1 != conter )
        {
            printf("The name is already in use, try again with another name");
            write_formatted_log(GRAY "[SERVER LOG] ERROR: Name already in use" RESET "\n");

            char response[DEFAULT_BUFLEN] = GAME_EVENT_NAME;
            send_message(i, response, listener, master, DEFAULT_BUFLEN);
            players.pop_back();
            
        }
         conter++;
        }

*/        


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
                //         send_message(seer.getFdId(), "a", listener, master, DEFAULT_BUFLEN);
                //         break;
                //     }
                // }


                // Create Player and assign name
                buf += strlen(GAME_EVENT_INIT);
                // Check if string is already in one player
                // if so...
                    
                    
          
                //      send game event mainhost
                //      return
                Player player(i, buf);
                players.push_back(player);
                printf("Player with name %s has joinded", players[players.size()-1].getName());

                 
               

                char response[DEFAULT_BUFLEN] = GAME_EVENT_START;
                write_formatted_log(GRAY "[SERVER LOG] Init request detected" RESET "\n");
                write_formatted_log(GRAY "[SERVER LOG] Current value of fdmax: %d" RESET "\n", fdmax);
                write_formatted_log(GRAY "[SERVER LOG] si paso" RESET "\n");
                send_broadcast(fdmax, response, listener, master, DEFAULT_BUFLEN);
                  
                 
                assign_role();

        //SEND ROLE ONE TO ONE

               
                int j=0;
                for(auto &player:players)
                {
                
                    char response[DEFAULT_BUFLEN];
                    snprintf(response, DEFAULT_BUFLEN, "%s%d", GAME_EVENT_ROLE, player._role);
                    send_message(player._fd_id, response, listener, master, DEFAULT_BUFLEN);
                    write_formatted_log(GRAY "[SERVER LOG] Sending Role for client %d" RESET "\n", j);

                     j++;
                }

                //response = "role" + "0 o ROLE_VILLAGER"
                write_formatted_log(GRAY "[SERVER LOG] Changing state to NIGHT" RESET "\n");
                stage = STAGE_NIGHT;
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
        case STAGE_NIGHT :
        {
            if(strstr(buf, GAME_EVENT_ACTION_WEREWOLF)){

           //Mensjae contiene "voto lobo"-extrajer votos y agregar lista de votos de lobos y ver la eliminacion
        
            
          //si numero de votos = votos esperados-calcular vitima

                return;

            }else if (strstr(buf, GAME_EVENT_ACTION_SEER)){





                return;
            }else if(strstr(buf, GAME_EVENT_ACTION_WITCH)){





                return;
            }else{


            }
        

         //si hay vidente enviar--accion vidente a vidente-terminar
         //si hay bruja enviar accion bruja a brujar-terminar


         //"llamar a ambiar de dia" -terminar

         //terminar

         //si mensaje contiene "pregunta vidente"-extraer jugador desde el mensaje-calcular respues-enviar a vidente"respuesta vidente"+respuesta
         //si hay bruja "enviar accion a bruja terminar -terminar
         
         //llamar a cambiar de dia"

         //disque mensaje contiene "posion bruja" extraer jugadores desde el mensaje-extrar accion desde el mensaje-calcular accion- cambiar a dia. sino mostrar error terminar

         
      

        //Cantidad de jugadores vivos

              

        }
    }
}


void GameServer::assign_role( )
{
            srand((time(0)));
            int j = 0;
            //To the total amount of each role.
            int villager_role = 0, witch_role = 0, hunter_role = 0, seer_role = 0, wolf_role = 0;
            //The total of wolf per role.
            int total_wolf = (((players.size() - 3) / 5) * 2);//calculate the wolf
             
            int randomNumber = rand() % 5; 
            
            //So that the cycle is done for as long as it is smaller than the size of the players.
            while (j < players.size()) 
           {
             //Generate the random numbers
              randomNumber = rand() % 5; 

                switch (randomNumber) 
              {
                case 0:
                {
                    if (villager_role < (players.size() - total_wolf)) 
                    {
                       players[j]._role=ROLE_VILLAGER;
                        villager_role++;
                        j++;
                    }
                  break;

                }
                    
                case 1:
                {
                    if (wolf_role < total_wolf) 
                    {
                        players[j]._role = ROLE_WEREWOLF;
                        wolf_role++;
                        j++;
                    }
                  break;
                }
                 
                case 2:
                {
                    if (witch_role < 1) 
                    {
                        players[j]._role=ROLE_WITCH;
                        witch_role++;
                        j++;
                    }
                  break;
                }
                 
                case 3:
                {
                    if (hunter_role < 1) 
                    {
                       players[j]._role=ROLE_HUNTER;
                       hunter_role++;
                       j++;
                    }
                  break;
                }
               
                case 4:
                {
                    if (seer_role < 1) 
                    {
                        players[j]._role=ROLE_SEER;
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


 void GameServer::player_list(std::vector<Player> players,fd_set master, int listener, int fdmax, char buf[], int nbytes)   
 {
    //quitar aprametro player maybe no te creas a lo mejor no
              for(auto player:players)
                {
                 //dudillas a lo mejor esta mal por ahora checar mas tarde.
                int j=0;

                 if(players[j]._alive==true){
                   char response[DEFAULT_BUFLEN];
                    snprintf(response, DEFAULT_BUFLEN, "%s %s", GAME_EVENT_ROLE, players[j]._name);
                    send_broadcast(fdmax, response, listener, master, DEFAULT_BUFLEN);
                    write_formatted_log(GRAY "[SERVER LOG] Sending the list of players " RESET "\n");

                   j++;
                 }else
                 {

                    write_formatted_log(GRAY "[SERVER LOG] The player is not alive " RESET "\n");
                 }



                }
}

 void GameServer::current_players_check(std::vector<Player> players,fd_set master, int listener, int fdmax, char buf[], int nbytes){
unsigned int Current_Players=0;
 int j=0;
 //deberia quitar parametro player?
      for(auto player:players){
            if(players[j]._alive==true){
               
               Current_Players++;
               j++;
            }
        }

        //A lo mejor le falta algo para que muestre el enucunciado con lo de Total de players
         char response[DEFAULT_BUFLEN];
        snprintf(response, DEFAULT_BUFLEN, "%u", Current_Players); ///??????????????? PREGUNTAR COMO FUNCIONA ESTA MADRE
        send_broadcast(fdmax, response, listener, master, DEFAULT_BUFLEN);
        write_formatted_log(GRAY "[SERVER LOG] Sending the current players " RESET "\n");

 }
    