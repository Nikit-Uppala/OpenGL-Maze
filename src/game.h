#include "main.h"

#ifndef GAME_H
#define GAME_H
#endif

class Game
{
    float vertices[12] = {
        -0.5f, -0.5f, -0.1f,
         0.5f, -0.5f, -0.1f,
         0.5f,  0.5f, -0.1f,
        -0.5f,  0.5f, -0.1f
    };
    glm::vec3 vap_color = glm::vec3(1.0f, 0.0f, 1.0f);
    glm::vec3 pow_color = glm::vec3(0.0f, 1.0f, 1.0f);
    public:
        Game() {}
        Game(int rows, int cols);
        void decrease_timer();
        void bindData(unsigned int& VAO_btn);
        bool game_over;
        void draw(unsigned int shaderProgram, unsigned int VAO_btn, glm::vec3 origin, glm::vec3 row_gap, glm::vec3 col_gap);
        void check_btn_press(int row, int col, int rows, int cols);
        int vap_r, vap_c;
        int pow_r, pow_c;
        bool imposter_alive;
        bool power_ups_released;
        bool lighting;
        int tasks_completed;
        int total_tasks;
    private:
        float time_left;
};