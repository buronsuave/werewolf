#include "Player.h"

Player::Player(int fd_id, char* name)
{
    this->_fd_id = fd_id;
    this->_role = ROLE_UNASIGNED;
    this->_alive = true;
    this->_name = name;
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

void Player::setName(char* name)
{
    this->_name = name;
}