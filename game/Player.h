#include "common.h"

class Player
{
    public:
    int _fd_id;
    ROLE _role;
    bool _alive;
    char _name[DEFAULT_BUFLEN]; 
    int _vote = 0;

    public:
    Player(int fd_id, const char* name);

    int getFdId();
    ROLE getRole();
    bool isAlive();
    char* getName();
    
    void setFdId(int fd_id);
    void setRole(ROLE role);
    void setAlive(bool alive);
    void setName(const char* name);
};
