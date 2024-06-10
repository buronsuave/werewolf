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
                Player player(i, buf);
                players.push_back(player);
                printf("Player with name %s has joinded\n", players[players.size()-1].getName());

                int conter=0;
                while(conter < players.size() - 1)
                {
                  if (strstr(players[conter].getName(), buf))
                  {
                    char response[DEFAULT_BUFLEN] = GAME_EVENT_LOBBY;
                    printf("The name is already in use, try again with another name\n");
                    write_formatted_log(GRAY "[SERVER LOG] ERROR: Name already in use" RESET "\n");
                    send_message(i, response, listener, master, DEFAULT_BUFLEN);
                    players.pop_back();
                    return;
                  }
                  conter++;
                }
              return;
            }
            else if (strstr(buf, GAME_EVENT_INIT)) // Needs to implement the check of current number of players
            {   
               
                if (players.size() < GAME_MIN_PLAYERS || players.size() > GAME_MAX_PLAYERS)
                {
                    char response[DEFAULT_BUFLEN] = GAME_EVENT_MAINHOST;
                    write_formatted_log(GRAY "[SERVER LOG] Invalid number of players" RESET "\n");
                    send_message(i, response, listener, master, DEFAULT_BUFLEN);
                    return;
                }
              
                buf += strlen(GAME_EVENT_INIT);
                
                Player player(i, buf);
                players.push_back(player);
                printf("Player with name %s has joinded\n", players[players.size()-1].getName());

                char response[DEFAULT_BUFLEN] = GAME_EVENT_START;
                write_formatted_log(GRAY "[SERVER LOG] Init request detected" RESET "\n");
                write_formatted_log(GRAY "[SERVER LOG] Current value of fdmax: %d" RESET "\n", fdmax);
                write_formatted_log(GRAY "[SERVER LOG] si paso" RESET "\n");
                send_broadcast(fdmax, response, listener, master, DEFAULT_BUFLEN);
                  
                assign_role();

                for(auto &player:players)
                {
                    char response[DEFAULT_BUFLEN];
                    snprintf(response, DEFAULT_BUFLEN, "%s%d", GAME_EVENT_ROLE, player._role);
                    send_message(player._fd_id, response, listener, master, DEFAULT_BUFLEN);
                    write_formatted_log(GRAY "[SERVER LOG] Sending Role for client " RESET "\n");
                }

                   printf("The roles have already been assigned.\n");

                  write_formatted_log(GRAY "[SERVER LOG] Changing state to NIGHT" RESET "\n");

                //funcion werewolf_action
               
                werewolf_action(master, listener);

                printf("The wolves are voting.\n");
                write_formatted_log(GRAY "[SERVER LOG] The wolves are voting." RESET "\n");
                
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
        case STAGE_NIGHT :
        {

          if(strstr(buf, GAME_EVENT_ACTION_WEREWOLF))
          {

            int total_wolfs = (((players.size() - 3) / 5) * 2);//calculate the wolf
            int votes_wolfs = 0;
            int counter = 0;
            int kill_player=0;
            buf += strlen(GAME_EVENT_ACTION_WEREWOLF);
            for (auto &player:players)
            {
              if (strstr(player._name, buf))
              {
                player._vote++;
                votes_wolfs += player._vote;
              }
            }

            if (votes_wolfs == total_wolfs)
            {
              int max = 0;
              for (auto &player:players)
              {
                if (max <= player._vote)
                {
                  max = player._vote;
                  kill_player = counter;
                }
                counter++;
              }
              players[kill_player]._alive = false;
               char response[DEFAULT_BUFLEN];
              snprintf(response, DEFAULT_BUFLEN, "You have kill: %s", players[kill_player]._name);
              printf("player: %s has died", players[kill_player]._name);
              write_formatted_log(GRAY "[SERVER LOG] one player has died." RESET "\n");
             
            }

            if(votes_wolfs==total_wolfs){
                 witch_action(master, listener);
            }

              return;
            }
            else if (strstr(buf, GAME_EVENT_ACTION_WITCH))
            {
              
              //BRUJA MATA O PROTEGE A ALGUIEN
                buf += strlen(GAME_EVENT_ACTION_WITCH);
                if(strstr(buf, GAME_EVENT_ACTION_SAVE)){
                     char response[DEFAULT_BUFLEN] = GAME_EVENT_ACTION_SAVE;
                write_formatted_log(GRAY "[SERVER LOG]Action witch server decision" RESET "\n");//modificar el log
                send_message(i, response, listener, master, DEFAULT_BUFLEN);
                return;
                  }else if(strstr(buf, GAME_EVENT_ACTION_KILL)){
                         char response[DEFAULT_BUFLEN] = GAME_EVENT_ACTION_KILL;
                write_formatted_log(GRAY "[SERVER LOG]Action witch server decision" RESET "\n");
                send_message(i, response, listener, master, DEFAULT_BUFLEN);
                return;
                  }else{
                      return;
                  }
     
                return;
            }else if(strstr(buf, GAME_EVENT_ACTION_SAVE)){
               buf += strlen(GAME_EVENT_ACTION_SAVE);
                  for(auto &player:players){
                     if(strstr(player._name,buf)){
                        player._alive= true;
                        printf("player: %s has save ", player._name);
                        write_formatted_log(GRAY "[SERVER LOG] Sending the actions of the witch " RESET "\n");
                      
                    
                      } 

                  }
                  seer_action(master,listener);
                  return;
            }
           else if(strstr(buf, GAME_EVENT_ACTION_KILL)){
               buf += strlen(GAME_EVENT_ACTION_KILL);
               for(auto &player:players){
                     if(strstr(player._name,buf)){
                        player._alive= false;
                        printf("\nplayer: %s has died ", player._name);
                        write_formatted_log(GRAY "[SERVER LOG] Sending the action of the witch " RESET "\n");
                    
                    
                       
                      }       
                     } 
            seer_action(master,listener);
             return;
            }
            else if(strstr(buf, GAME_EVENT_ACTION_SEER))
            {
                   buf += strlen(GAME_EVENT_ACTION_SEER);



                 for(auto &player:players){
                  if(strstr(player._name,buf)){
                    if(player._role==1){
                      printf("\nplayer: %s is a wolf ", player._name);
                      write_formatted_log(GRAY "[SERVER LOG] Sending the action to the seer 1 " RESET "\n");
                      char response[DEFAULT_BUFLEN] = GAME_EVENT_SEER_CHECK_WOLF;
                      send_message(i, response, listener, master, nbytes);
                    }else{

                      printf("\nplayer: %s is not a wolf ", player._name);
                      write_formatted_log(GRAY "[SERVER LOG] Sending the action to the seer 2 " RESET "\n");
                      char response[DEFAULT_BUFLEN] = GAME_EVENT_SEER_CHECK_NOWOLF;
                      send_message(i, response, listener, master, nbytes);
                    }
                  }
                 }

                char response[DEFAULT_BUFLEN] = GAME_EVENT_DAY;
                write_formatted_log(GRAY "[SERVER LOG] Changing to day " RESET "\n");
                for (auto &player:players)
                {
                  send_broadcast(player._fd_id, response, listener, master, DEFAULT_BUFLEN);
                }
              
                stage=STAGE_DAY;
                return;
            }
        }
      }
        case STAGE_DAY:
          {
            if(strstr(buf,GAME_EVENT_DAY))
            {
              buf += strlen(GAME_EVENT_DAY);
              printf("Estamos en el dia");
    
            }
          }  
    }//switch
}//clase



void GameServer::assign_role()
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
                    if (villager_role < (players.size() - total_wolf)) //cambiar formula
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

void GameServer::check_mainhost(fd_set master, int listener, int i, char buf[], int nbytes)
{

        for (auto &player:players)
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

 void GameServer::player_list(fd_set master, int listener, int fdmax)   
 {
    for(auto &player:players)
      {

        if(player._alive==true)
        {
           printf("Player: %s esta vivo", player.getName());           
           char response[DEFAULT_BUFLEN] = GAME_EVENT_ALIVE_PLAYERS;
           send_broadcast(fdmax, response, listener, master, DEFAULT_BUFLEN);
           write_formatted_log(GRAY "[SERVER LOG] Sending the list of players " RESET "\n");
        }
        else
         {
            write_formatted_log(GRAY "[SERVER LOG] The player is not alive " RESET "\n");
         }

      }
}

 void GameServer::current_players_check(fd_set master, int listener, int fdmax){
 unsigned int Current_Players=0;
                
 
                   for(auto &player:players)
                   {
                   if(player._alive==true){   
                    //char response[DEFAULT_BUFLEN];
                    //send_broadcast(fdmax, response, listener, master, DEFAULT_BUFLEN); 
                    Current_Players++;
                    }
                  }

                        char response[DEFAULT_BUFLEN];
                         write_formatted_log(GRAY "[SERVER LOG] Sending the current players " RESET "\n");
                         printf("The currents players letf %u \n",Current_Players);
                         snprintf(response, DEFAULT_BUFLEN, "%s%u",GAME_EVENT_OVER, Current_Players); 
                         send_broadcast(fdmax, response, listener, master, DEFAULT_BUFLEN);
                         

                         //A lo mejor aqui se hace tambien la logica si cuantos lobos quedan vs aldeanos
 }
  
 void GameServer::decision_won_lost(fd_set master, int listener, int fdmax)
 {
    unsigned int Bad_guy=0,Good_guy=0;
 
    for(auto &player:players)
    {
       if(player._alive==true)
        {
          if(player._role==0)//Villager
          {
            Good_guy++;
          }
          else if(player._role==1)//Werewolf
          {
            Bad_guy++;
          }
          else if(player._role==2)//witch
          {
            Good_guy++;                       
          }
          else if(player._role==3)//hunter
          {
            Good_guy++;
          }
          else if (player._role == 4)//seer
          {
            Good_guy++;
          }
                  
        }
      }
      if(Bad_guy>=Good_guy)
      {
        char response[DEFAULT_BUFLEN];
        write_formatted_log(GRAY "[SERVER LOG] Sending the current players " RESET "\n");
        //snprintf(response, DEFAULT_BUFLEN, "%s%u",GAME_EVENT_OVER, Current_Players); 
        send_broadcast(fdmax, response, listener, master, DEFAULT_BUFLEN);
        //send the game is over the werewolf win
      }
      else
      {
        //the game continue
      }
 }

 void GameServer::player_list_dead(fd_set master, int listener, int fdmax, char buf[], int nbytes)   
 {
    for(auto &player:players)
      {

        if(player._alive==false)
        {
           char response[DEFAULT_BUFLEN];
           snprintf(response, DEFAULT_BUFLEN, "%s", player._name);
           send_broadcast(fdmax, response, listener, master, DEFAULT_BUFLEN);
           write_formatted_log(GRAY "[SERVER LOG] Sending the list of players " RESET "\n");
           printf("Player: %s esta vivo", player.getName());           
        }
        else
         {
            write_formatted_log(GRAY "[SERVER LOG] The player is not alive " RESET "\n");
         }

      }
}

void GameServer::witch_action(fd_set master, int listener) 
{

  //no envia a brujas el evento
  
  for (auto &player:players)
  {
    if (player._role == 2)//si es bruja
    {
      char response[DEFAULT_BUFLEN] = GAME_EVENT_ACTION_WITCH;
      write_formatted_log(GRAY "[SERVER LOG] Brujas deciden" RESET "\n");
      send_message(player._fd_id, response, listener, master, DEFAULT_BUFLEN);
    }
    else 
    {
      char response[DEFAULT_BUFLEN] = GAME_EVENT_ACTION_WAITING;
      write_formatted_log(GRAY "[SERVER LOG] brujas decideen" RESET "\n");
      send_message(player._fd_id, response, listener, master, DEFAULT_BUFLEN);
    }
  }
}

void GameServer::werewolf_action(fd_set master, int listener){

     for(auto &player:players)
                {
                  if(player._role==1)//si es werewolf
                  {
                    char response[DEFAULT_BUFLEN] = GAME_EVENT_ACTION_WEREWOLF;
                    write_formatted_log(GRAY "[SERVER LOG] the wolves are watching who to kill " RESET "\n");
                    send_message(player._fd_id, response, listener, master, DEFAULT_BUFLEN);
                    
                  }
                  else
                  {
                    char response[DEFAULT_BUFLEN] = GAME_EVENT_ACTION_WAITING;
                    write_formatted_log(GRAY "[SERVER LOG] the players are waiting " RESET "\n");
                    send_message(player._fd_id, response, listener, master, DEFAULT_BUFLEN);
                  }
                }
} 


void GameServer::seer_action(fd_set master, int listener){

 for(auto &player:players)
                {
                  if(player._role==3)//si es seer
                  {
                    char response[DEFAULT_BUFLEN] = GAME_EVENT_ACTION_SEER;
                    write_formatted_log(GRAY "[SERVER LOG] the seer is watching who is the wolf " RESET "\n");
                    send_message(player._fd_id, response, listener, master, DEFAULT_BUFLEN);
                  }
                  else
                  {
                    char response[DEFAULT_BUFLEN] = GAME_EVENT_ACTION_WAITING;
                    write_formatted_log(GRAY "[SERVER LOG] the players are waiting " RESET "\n");
                    send_message(player._fd_id, response, listener, master, DEFAULT_BUFLEN);
                  }
                }
}