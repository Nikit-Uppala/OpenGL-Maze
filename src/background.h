#include "main.h"
#ifndef BACKGROUND_H
#define BACKGROUND_H
#endif

class Background
{
    float vertices[12] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.5f,  0.5f, 0.0f,
        -0.5f,  0.5f, 0.0f
    };
    public:
        Background() {}
        Background(glm::vec3 color, glm::vec3 scaling, glm::vec3 translation);
        void bindData(unsigned int& VAO_bg, unsigned int& VBO_bg);
        void display(unsigned int shaderProgram, unsigned int VAO_bg);
    private:
        glm::vec3 translation;
        glm::vec3 color;
        glm::vec3 scaling;
};