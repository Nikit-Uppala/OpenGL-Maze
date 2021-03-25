#include "main.h"
#ifdef MAZE_H
#define MAZE_H
#endif
#include <vector>
#include <utility>
#define pi std::pair<int, int>
#define mp std::make_pair

class Maze
{
    public:
        Maze() {}
        Maze(int r, int c, glm::vec3 row_start, glm::vec3 col_start, glm::vec3 row_gap, glm::vec3 col_gap, glm::vec3 scale);
        void generate_maze(std::vector<int>graph[]);
        void draw(unsigned int shaderProgram, unsigned int VAO_h, unsigned VAO_v);
        bool included[50][50][2];
    private:
        int rows, cols;
        glm::vec3 row_start, col_start;
        glm::vec3 row_gap, col_gap;
        glm::vec3 scale;
        void kruskal(std::vector<std::pair<pi, int>>walls, std::vector<int>graph[]);
};