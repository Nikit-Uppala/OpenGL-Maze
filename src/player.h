#include "main.h"
#ifndef PLAYER_H
#define PLAYER_H
#endif

class Player
{
    float vertices[12] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.5f,  0.5f, 0.0f,
        -0.5f,  0.5f, 0.0f
    };

    unsigned int indices[6] = {
        0, 1, 2,
        0, 2, 3
    };
    public:
        Player() {}
        Player(int r, int c, float health, glm::vec3 origin, glm::vec3 row_gap, glm::vec3 col_gap);
        void move_row(int sign, bool canMove);
        void move_col(int sign, bool canMove);
        void draw(unsigned int shaderProgram, unsigned int VAO);
        void bindData(unsigned int &VAO, unsigned int &VBO, unsigned int &IBO);
        int row, col;
    private:
        glm::vec3 origin;
        glm::vec3 row_gap;
        glm::vec3 col_gap;
        float health;
        int orientation;
};