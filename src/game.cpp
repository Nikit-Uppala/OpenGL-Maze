#include "game.h"
#include<stdlib.h>
#include<time.h>

Game::Game(int rows, int cols)
{
    srand(time(0));
    this->time_left = 120.0f;
    this->imposter_alive = 1;
    int cell_vap = rand()%(rows*cols);
    int cell_pow = rand()%(rows*cols);
    // Ensuring both the buttons are in different cells
    while(cell_pow == cell_vap) cell_vap = rand()%(rows*cols);
    this->vap_r = cell_vap/cols;
    this->vap_c = cell_vap%cols;
    this->pow_r = cell_pow/cols;
    this->pow_c = cell_pow%cols;
    this->power_ups_released = 0;
    this->game_over = 0;
    this->total_tasks = 2;
    this->tasks_completed = 0;
    this->lighting = 1;
}

void Game::decrease_timer()
{
    this->time_left -= glfwGetTime();
    if(time_left <= 0) this->game_over = 1;
}

void Game::bindData(unsigned int& VAO_btn)
{
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO_btn);
    glBindVertexArray(VAO_btn);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Game::vertices), Game::vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), 0);
    glEnableVertexAttribArray(0);
}

void Game::draw(unsigned int shaderProgram, unsigned int VAO_btn, glm::vec3 origin, glm::vec3 row_gap, glm::vec3 col_gap)
{ 
    glm::vec3 vap_pos = origin - (float)this->vap_r*row_gap + (float)this->vap_c*col_gap;
    glm::mat4 model = glm::translate(glm::mat4(1.0f), vap_pos);
    int location = glGetUniformLocation(shaderProgram, "model");
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(model));
    location = glGetUniformLocation(shaderProgram, "color");
    glUniform3f(location, Game::vap_color[0], Game::vap_color[1], Game::vap_color[2]);
    glBindVertexArray(VAO_btn);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 6);
    glm::vec3 pow_pos = origin - (float)this->pow_r*row_gap + (float)this->pow_c*col_gap;
    model = glm::translate(glm::mat4(1.0f), pow_pos);
    location = glGetUniformLocation(shaderProgram, "model");
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(model));
    location = glGetUniformLocation(shaderProgram, "color");
    glUniform3f(location, Game::pow_color[0], Game::pow_color[1], Game::pow_color[2]);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 6);
}

void Game::check_btn_press(int row, int col, int rows, int cols)
{
    int player_cell = row*cols+col;
    int vap_cell = this->vap_r*cols + this->vap_c;
    int pow_cell = this->pow_r*cols + this->pow_c;
    if(player_cell == vap_cell && this->imposter_alive)
    {
        this->imposter_alive = 0;
        this->tasks_completed += 1;
    }
    else if(player_cell == pow_cell && !this->power_ups_released)
    {
        this->tasks_completed += 1;
        this->power_ups_released = 1;
    }
}