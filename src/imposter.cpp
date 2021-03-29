#include "imposter.h"
#include<queue>
#include<utility>
#define pi std::pair<int, int>
#define mp std::make_pair

void Imposter::DFS(std::vector<int>graph[], int cell, int parent, bool visited[])
{
    if(visited[cell]) return;
    visited[cell] = 1;
    this->parent[cell] = parent;
    for(auto i=graph[cell].begin(); i!=graph[cell].end(); i++)
        if(!visited[*i]) DFS(graph, *i, cell, visited);
}

void Imposter::move(std::vector<int>graph[], int rows, int cols, int row_t, int col_t)
{
    int target_cell = cols*row_t + col_t;
    int present_cell = this->row*cols + this->col;
    bool visited[rows*cols];
    for(int i=0; i<rows*cols; i++) visited[i] = 0;
    //Run DFS to get the path to the player
    DFS(graph, present_cell, present_cell, visited);
    while(this->parent[target_cell]!=present_cell)
        target_cell = this->parent[target_cell];
    int target_row = target_cell/cols;
    int target_col = target_cell%cols;
    if(this->row == target_row)
    {
        if(!this->moveCol) this->moveCol = target_col - this->col;
    }
    else 
    {
        if(!this->moveRow) this->moveRow = target_row - this->row;
    }
}