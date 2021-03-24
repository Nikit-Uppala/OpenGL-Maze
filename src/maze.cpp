#include "maze.h"
#include<algorithm>
#include<time.h>

Maze::Maze(int r, int c, glm::vec3 row_start, glm::vec3 col_start, glm::vec3 row_gap, glm::vec3 col_gap, glm::vec3 scale)
{
    this -> rows = r;
    this -> cols = c;
    this -> row_start = row_start;
    this -> col_start = col_start;
    this -> row_gap = row_gap;
    this -> col_gap = col_gap;
    this -> scale = scale;
    for(int i=0; i<=rows; i++)
        for(int j=0; j<=cols; j++)
            included[i][j][0] = 1, included[i][j][1] = 1;
}

void root(int r, int c, int *parent[])
{ 
}

void Maze::kruskal(std::vector<std::pair<pi, int>>walls)
{
    int parent[this->rows][this->cols], count_size[this->rows][this->cols], start=0;
    for(int r=0; r<this->rows; r++)
        for(int c=0; c<this->cols; c++) parent[r][c] = start++, count_size[r][c] = 1;
    
}

void Maze::generate_maze()
{
    // srand(time(0));
    // std::vector<std::pair<pi, int>> walls;
    // for(int r=1; r<this->rows; r++)
    //     for(int c=0; c<this->cols; c++)
    //         walls.std::push(mp(mp(r, c), 0));
    // for(int r = 1; r < this->rows; r++)
    //     for(int c = 0; c < this->cols; c++)
    //         walls.std::push(mp(mp(r, c), 0));
    // for(int r=0; r < this->rows; r++)
    //     for(int c=1; c<this->cols; c++)
    //         walls.std::push(mp(mp(r, c), 1));
    // std::shuffle(walls.std::begin(), walls.std::end(), default_random_engine(time(0)));
    // kruskal(walls);
}

void Maze::draw(unsigned int shaderProgram, unsigned int VAO_h, unsigned int VAO_v)
{
    glBindVertexArray(VAO_h);
    for (int r = 0; r < rows + 1; r++)
    {
        glm::vec3 start = glm::vec3(-7.0f, 3 + 4.0f - r*this->scale[0], 0.0f);
        for (int c = 0; c < cols; c++)
        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), start + (float)c*this->row_gap*this->scale);
            model = glm::scale(model, this->scale);
            int location = glGetUniformLocation(shaderProgram, "model");
            glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(model));
            glDrawArrays(GL_LINES, 0, 2);
        }
    }
    glBindVertexArray(VAO_v);
    for (int r = 0; r < rows; r++)
    {
        glm::vec3 start = glm::vec3(-8.0f, 3 + 3.0f - r*this->scale[0], 0.0f);
        for (int c = 0; c < cols + 1; c++)
        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), start + (float)c*this->row_gap*this->scale);
            model = glm::scale(model, this->scale);
            int location = glGetUniformLocation(shaderProgram, "model");
            glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(model));
            glDrawArrays(GL_LINES, 0, 2);
        }
    }
}