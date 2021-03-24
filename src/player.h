#include "main.h"
#ifndef PLAYER_H
#define PLAYER_H
#endif

class Player
{
    public:
        Player() {}
        Player(int r, int c, float health);
        void move_row(int sign, bool canMove);
        void move_col(int sign, bool canMove);
    private:
        int row, col;
        float health;
};