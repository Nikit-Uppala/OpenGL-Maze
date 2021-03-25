#include "player.h"

Player::Player(int r, int c, float health, glm::vec3 origin, glm::vec3 row_gap, glm::vec3 col_gap)
{
    this->row = r;
    this->col = c;
    this->health = health;
    this->orientation = 0;
    this->origin = origin;
    this->row_gap = row_gap;
    this->col_gap = col_gap;
}

void Player::move_row(int sign, bool canMove)
{
    if(!canMove) return;
    this->row += sign;
    if(sign == 1) this->orientation = 2;
    else this->orientation = 3;
}

void Player::move_col(int sign, bool canMove)
{
    if(!canMove) return;
    if(this->col + sign >= 0)
        this->col += sign;
    if(sign == 1) this->orientation = 0;
    else this->orientation = 1;
}

void Player::draw(unsigned int shaderProgram, unsigned int VAO)
{
    glm::mat4 model(1.0f);
    glm::vec3 position = this->origin - (float)this->row*this->row_gap + (float)this->col*this->col_gap;
    model = glm::translate(model, position);
    switch(this->orientation)
    {
        case 1: model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f)); break;
        case 2: model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f)); break;
        case 3: model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f)); break;
    }
    int location = glGetUniformLocation(shaderProgram, "model");
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(model));
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void Player::bindData(unsigned int& VAO, unsigned int& VBO, unsigned int& IBO)
{
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Player::vertices), Player::vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), 0);
    glEnableVertexAttribArray(0);
    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Player::indices), Player::indices, GL_STATIC_DRAW);
}