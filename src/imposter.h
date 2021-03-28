#include "main.h"
#include "player.h"
#include<vector>
#ifndef IMPOSTER_H
#define IMPOSTER_H
#endif

class Imposter: public Player
{
    private:
        // void shortest_distance(std::vector<int>graph[], int rows, int cols);
        void DFS(std::vector<int>graph[], int cell, int parent, bool visited[]);
        int parent[101];
        int dist[101];
        int seconds;
    public:
        Imposter(int r, int c, float health, glm::vec3 origin, glm::vec3 row_gap, glm::vec3 col_gap, 
        glm::vec3 scaling, glm::vec3 color): 
        Player(r, c, health, origin, row_gap, col_gap, scaling, color) { this->seconds = 0; }
        void move(std::vector<int>graph[], int rows, int cols, int row_t, int col_t);
};