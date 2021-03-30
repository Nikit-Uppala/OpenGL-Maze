#include "main.h"

#ifndef GAME_H
#define GAME_H
#endif

class Game
{
    float btn_vertices[12] = {
        -0.5f, -0.5f, -0.1f,
         0.5f, -0.5f, -0.1f,
         0.5f,  0.5f, -0.1f,
        -0.5f,  0.5f, -0.1f
    };
    float pow_obs_vertices[9] = {
        -0.5f, -0.5f, -0.1f,
         0.5f, -0.5f, -0.1f,
         0.0f,  0.5f, -0.1f
    };
    glm::vec3 vap_color = glm::vec3(1.0f, 0.0f, 1.0f);
    glm::vec3 pow_color = glm::vec3(0.0f, 1.0f, 1.0f);
    glm::vec3 pow_up_color = glm::vec3(0.0f, 0.391f, 0.0f);
    glm::vec3 obs_color = glm::vec3(0.391f, 0.0f, 0.0f);
    public:
        Game() {}
        Game(int rows, int cols);
        void decrease_timer();
        void bindData(unsigned int& VAO_btn, unsigned int& VAO_pow_obs);
        void draw(unsigned int shaderProgram, unsigned int VAO_btn, unsigned int VAO_pow_obs,
                glm::vec3 origin, glm::vec3 row_gap, glm::vec3 col_gap, int cols);
        void check_btn_press(int row, int col, int rows, int cols);
        int vap_r, vap_c;
        int pow_r, pow_c;
        bool imposter_alive;
        bool power_ups_released;
        bool lighting;
        bool pow_touched[2];
        bool obs_touched[2];
        int pow_cells[2];
        int obs_cells[2];
        int tasks_completed;
        int total_tasks;
        float time_left;
        int pows, obs;
    private:
        float total_time;
        void generate_pows_obs(int rows, int cols);
};