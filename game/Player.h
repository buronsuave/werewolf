#include "common.h"

class Player
{
    private:
    int _fd_id;
    ROLE _role;
    bool _alive;
    char* _name;

    public:
    // constructor
    Player(int fd_id, char* name);

    // getters
    int getFdId();
    ROLE getRole();
    bool isAlive();
    char* getName();
    
    // setters
    void setFdId(int fd_id);
    void setRole(ROLE role);
    void setAlive(bool alive);
    void setName(char* name);
};