#include "main.h"

#ifndef GAME_H
#define GAME_H
#endif

class Game
{
    float btn_vertices[232];
    float pow_obs_vertices[9] = {
        -0.5f, -0.5f, -0.05f,
         0.5f, -0.5f, -0.05f,
         0.0f,  0.5f, -0.05f
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
        float health_gain_in_dark;
        float health_loss_imposter;
        int pow_r, pow_c;
        bool imposter_alive;
        bool power_ups_released;
        bool lighting;
        int game_over;
        bool pow_touched[2];
        bool obs_touched[2];
        int pow_cells[2];
        int obs_cells[2];
        int tasks_completed;
        int total_tasks;
        float pow_score;
        float obs_score;
        float time_left;
        int pows, obs;
    private:
        float total_time;
        void generate_pows_obs(int rows, int cols);
};