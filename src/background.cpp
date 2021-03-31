#include "background.h"

Background::Background(glm::vec3 color, glm::vec3 scaling, glm::vec3 translation)
{
    this->color = color;
    this->scaling = scaling;
    this->translation = translation;
}

void Background::bindData(unsigned int& VAO_bg, unsigned int& VBO_bg)
{
    // Binding data (square transformed with appropriate model matrix in drawing)
    glGenBuffers(1, &VBO_bg);
    glGenVertexArrays(1, &VAO_bg);
    glBindVertexArray(VAO_bg);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_bg);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Background::vertices), Background::vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
    glEnableVertexAttribArray(0);
}

void Background::display(unsigned int shaderProgram, unsigned int VAO_bg)
{
    // Drawing background with depth -0.5
    int location = glGetUniformLocation(shaderProgram, "color");
    glUniform3f(location, this->color[0], this->color[1], this->color[2]);
    glm::mat4 model = glm::translate(glm::mat4(1.0f), this->translation);
    model = glm::scale(model, this->scaling);
    location = glGetUniformLocation(shaderProgram, "model");
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(model));
    glBindVertexArray(VAO_bg);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 6);
}