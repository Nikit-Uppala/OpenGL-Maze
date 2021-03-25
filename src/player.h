#include "main.h"
#ifndef PLAYER_H
#define PLAYER_H
#endif

class Player
{
    int faces = 5;
    float vertices[5][12] = {
        {
            0.0f, 0.0f, 0.0f,
            0.0f, 0.25f, 0.0f,
            0.4f, 0.25f, 0.0f,
            0.4f, 0.0f, 0.0f
        },
        {
            -0.30f, -0.35f, 0.0f,
             0.30f, -0.35f, 0.0f,
             0.30f,  0.35f, 0.0f,
            -0.30f,  0.35f, 0.0f
        },
        {
            -0.30f, -0.35f, 0.0f,
            -0.30f, -0.50f, 0.0f,
            -0.20f, -0.50f, 0.0f,
            -0.20f, -0.35f, 0.0f
        },
        {
             0.30f, -0.35f, 0.0f,
             0.30f, -0.50f, 0.0f,
             0.20f, -0.50f, 0.0f,
             0.20f, -0.35f, 0.0f
        },
        {
            -0.30f, -0.25f, 0.0f,
            -0.30f,  0.25f, 0.0f,
            -0.40f,  0.25f, 0.0f,
            -0.40f, -0.25f, 0.0f
        }
    };
    float curved_face[114];
    
    public:
        Player() {}
        Player(int r, int c, float health, glm::vec3 origin, glm::vec3 row_gap, glm::vec3 col_gap, 
        glm::vec3 scaling, glm::vec3 color);
        void move_row(int sign, bool canMove);
        void move_col(int sign, bool canMove);
        void draw(unsigned int shaderProgram, unsigned int VAO[]);
        void bindData(unsigned int VAO[], unsigned int VBO[]);
        int row, col;
    private:
        glm::vec3 origin;
        glm::vec3 row_gap;
        glm::vec3 col_gap;
        glm::vec3 scaling;
        glm::vec3 color;
        float health;
        int orientation;
};