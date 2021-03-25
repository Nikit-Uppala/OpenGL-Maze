#include "main.h"
#include "player.h"
#include<vector>
#ifndef IMPOSTER_H
#define IMPOSTER_H
#endif

class Imposter: public Player
{
    public:
        // Imposter() {}
        Imposter(int r, int c, float health, glm::vec3 origin, glm::vec3 row_gap, glm::vec3 col_gap, 
        glm::vec3 scaling, glm::vec3 color): 
        Player(r, c, health, origin, row_gap, col_gap, scaling, color) {}
        void shortest_distance(std::vector<int>graph[]);
    private:
        int parent[101];
        int dist[101];
        int seconds;
};