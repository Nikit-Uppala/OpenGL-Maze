#include "main.h"
#include "line.h"

Line::Line(float* vertices)
{
    this -> vertices = vertices;
}

void Line::bindData(unsigned int& VAO, unsigned int& VBO)
{
    // binding data
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, 6*sizeof(float), this -> vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), NULL);
    glEnableVertexAttribArray(0);
}