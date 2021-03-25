#include "maze.h"
#include<algorithm>
#include<random>
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
    included[0][0][1] = 0;
    included[rows-1][cols][1] = 0;
}

int root(int cell, int parent[])
{
    int node = cell;
    while(node!=parent[node])
        node = parent[node];
    parent[cell] = node;
    return node;
}
void make_union(int p1, int p2, int parent[], int Size[])
{
    if(Size[p1]>=Size[p2])
    {
        Size[p1] += Size[parent[p2]];
        parent[p2] = parent[p1];
    }
    else
    {
        Size[p2] += Size[parent[p1]];
        parent[p1] = parent[p2];
    }
}

void Maze::kruskal(std::vector<std::pair<pi, int>>walls)
{
    int totalCells = this->rows*this->cols;
    int parent[totalCells], Size[totalCells];
    for(int i=0; i<totalCells; i++) parent[i]=i, Size[i]=1;
    for(auto i=walls.begin(); i!=walls.end(); i+=1)
    {
        if((*i).second == 0)
        {
            pi wall = (*i).first;
            int cell1 = this->cols*(wall.first-1)+wall.second;
            int cell2 = cell1+this->cols;
            int root1 = root(cell1, parent);
            int root2 = root(cell2, parent);
            if(root1 != root2)
            {
                this->included[wall.first][wall.second][0] = 0;
                make_union(root1, root2, parent, Size);
            }
        }
        else
        {
            pi wall = (*i).first;
            int cell1 = this->cols*wall.first + wall.second-1;
            int cell2 = cell1 + 1;
            int root1 = root(cell1, parent);
            int root2 = root(cell2, parent);
            if (root1 != root2)
            {
                this->included[wall.first][wall.second][1] = 0;
                make_union(root1, root2, parent, Size);
            }
        }
    }
}

void Maze::generate_maze()
{
    std::vector<std::pair<pi, int>> walls;
    for(int r = 1; r < this->rows; r++)
        for(int c = 0; c < this->cols; c++)
            walls.push_back(mp(mp(r, c), 0));
    for(int r=0; r < this->rows; r++)
        for(int c=1; c<this->cols; c++)
            walls.push_back(mp(mp(r, c), 1));
    std::shuffle(walls.begin(), walls.end(), std::default_random_engine(time(0)));
    kruskal(walls);
}

void Maze::draw(unsigned int shaderProgram, unsigned int VAO_h, unsigned int VAO_v)
{
    int location = glGetUniformLocation(shaderProgram, "color");
    glUniform3f(location, 1.0f, 1.0f, 1.0f);
    glBindVertexArray(VAO_h);
    for (int r = 0; r < rows + 1; r++)
    {
        glm::vec3 start = this->row_start - (float)r*this->scale * this->row_gap;
        for (int c = 0; c < cols; c++)
        {
            if(!this->included[r][c][0]) continue;
            glm::mat4 model = glm::translate(glm::mat4(1.0f), start + (float)c*this->col_gap*this->scale);
            model = glm::scale(model, this->scale);
            location = glGetUniformLocation(shaderProgram, "model");
            glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(model));
            glDrawArrays(GL_LINES, 0, 2);
        }
    }
    glBindVertexArray(VAO_v);
    for (int r = 0; r < rows; r++)
    {
        glm::vec3 start = this->col_start - (float)r*this->scale*this->row_gap;
        for (int c = 0; c < cols + 1; c++)
        {
            if(!this->included[r][c][1]) continue;
            glm::mat4 model = glm::translate(glm::mat4(1.0f), start + (float)c*this->col_gap*this->scale);
            model = glm::scale(model, this->scale);
            int location = glGetUniformLocation(shaderProgram, "model");
            glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(model));
            glDrawArrays(GL_LINES, 0, 2);
        }
    }
}