#include "Player.h"
#include <cstring> // Asegúrate de incluir cstring para strncpy

Player::Player(int fd_id, const char* name)
{
    this->_fd_id = fd_id;
    this->_role = ROLE_UNASIGNED;
    this->_alive = true;
    strncpy(this->_name, name, sizeof(this->_name) - 1);
    this->_name[sizeof(this->_name) - 1] = '\0'; // Asegura que _name esté null-terminated
    this->_vote = _vote;
}

int Player::getFdId()
{
    return this->_fd_id;
}

ROLE Player::getRole()
{
    return this->_role;
}

bool Player::isAlive()
{
    return this->_alive;
}

char* Player::getName()
{
    return this->_name;
}

void Player::setFdId(int fd_id)
{
    this->_fd_id = fd_id;
}

void Player::setRole(ROLE role)
{
    this->_role = role;
}

void Player::setAlive(bool alive)
{
    this->_alive = alive;
}

void Player::setName(const char* name)
{
    strncpy(this->_name, name, sizeof(this->_name) - 1);
    this->_name[sizeof(this->_name) - 1] = '\0'; // Asegura que _name esté null-terminated
}
