#include "game.h"
#include<stdlib.h>
#include<time.h>

Game::Game(int rows, int cols)
{
    // inititalising parameters
    srand(time(0));
    this->total_time = 60.0f; // 2 min time
    this->time_left = this->total_time;
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
    this->total_tasks = 2;
    this->tasks_completed = 0;
    this->lighting = 1;
    this->pow_score = 15;
    this->game_over = 0;
    this->obs_score = 15;
}

void Game::decrease_timer()
{
    // time left = total_time - time elapsed
    this->time_left = this->total_time - glfwGetTime();
}

void Game::bindData(unsigned int& VAO_btn, unsigned int& VAO_pow_obs)
{
    //buttons square
    unsigned int VBO_btn;
    glGenBuffers(1, &VBO_btn);
    glGenVertexArrays(1, &VAO_btn);
    glBindVertexArray(VAO_btn);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_btn);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Game::btn_vertices), Game::btn_vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), 0);
    glEnableVertexAttribArray(0);
    // power ups and obstacles triangle
    unsigned int VBO_pow_obs;
    glGenBuffers(1, &VBO_pow_obs);
    glGenVertexArrays(1, &VAO_pow_obs);
    glBindVertexArray(VAO_pow_obs);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_pow_obs);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Game::pow_obs_vertices), Game::pow_obs_vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
    glEnableVertexAttribArray(0);
}

void Game::draw(unsigned int shaderProgram, unsigned int VAO_btn, unsigned int VAO_pow_obs, 
                glm::vec3 origin, glm::vec3 row_gap, glm::vec3 col_gap, int cols)
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
    if(this->power_ups_released) // if power ups and obstcales are released then draw them
    {
        glBindVertexArray(VAO_pow_obs);
        for(int i=0; i<this->pows; i++)
        {
            if(this->pow_touched[i]) continue; // if power up taken then continue(dont draw)
            int r = this->pow_cells[i]/cols;
            int c = this->pow_cells[i]%cols;
            glm::vec3 position = origin - (float)r*row_gap + (float)c*col_gap;
            model = glm::translate(glm::mat4(1.0f), position);
            location = glGetUniformLocation(shaderProgram, "model");
            glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(model));
            location = glGetUniformLocation(shaderProgram, "color");
            glUniform3f(location, Game::pow_up_color[0], Game::pow_up_color[1], Game::pow_up_color[2]);
            glDrawArrays(GL_TRIANGLES, 0, 3);
        }
        for(int i=0; i<this->obs; i++)
        {
            if(this->obs_touched[i]) continue; // if power up taken then continue(dont draw)
            int r = this->obs_cells[i]/cols;
            int c = this->obs_cells[i]%cols;
            glm::vec3 position = origin - (float)r*row_gap + (float)c*col_gap;
            model = glm::translate(glm::mat4(1.0f), position);
            location = glGetUniformLocation(shaderProgram, "model");
            glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(model));
            location = glGetUniformLocation(shaderProgram, "color");
            glUniform3f(location, Game::obs_color[0], Game::obs_color[1], Game::obs_color[2]);
            glDrawArrays(GL_TRIANGLES, 0, 3);
        }
    }
}

void Game::check_btn_press(int row, int col, int rows, int cols)
{
    // If player is in same cell as power up or obstcale then it gets activated
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
        generate_pows_obs(rows, cols);
    }
}

void Game::generate_pows_obs(int rows, int cols)
{
    // generating power ups and obstcales at random positions
    srand(time(0));
    int totalCells = rows*cols;
    this->pows = 1 + rand()%2;
    this->obs = 1 + rand()%2;
    for(int i=0; i<this->pows; i++)
    {
        this->pow_touched[i] = 0;
        bool match = 1;
        while(match)
        {
            match = 0;
            this->pow_cells[i] = rand()%totalCells;
            for(int j=i-1; j>=0; j--)
                if(this->pow_cells[i] == this->pow_cells[j]) match = 1; // if power ups overlap then generate random number
        }
    }
    for(int i=0; i<this->obs; i++)
    {
        this->obs_touched[i]=0;
        bool match = 1;
        while(match)
        {
            match = 0;
            this->obs_cells[i] = rand()%totalCells;
            for(int j=0; j<this->pows; j++)
                if(this->obs_cells[i] == this->pow_cells[j]) match = 1; // if obstacle overlaps power up then generate random number
            for(int j=i-1; j>=0; j--)
                if(this->obs_cells[i] == this->obs_cells[j]) match = 1; // if obstacles overlap then generate random number
        }
    }
}