#include "player.h"

Player::Player(int r, int c, float health)
{
    this->row = r;
    this->col = c;
    this->health = health;
}

void Player::move_row(int sign, bool canMove)
{
    if(!canMove) return;
    this->row += sign;
}

void Player::move_col(int sign, bool canMove)
{
    if(!canMove) return;
    this->col += sign;
}