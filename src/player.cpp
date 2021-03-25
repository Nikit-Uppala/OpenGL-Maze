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
    float a = 0.30f;
    float b = 0.15f;
    float angle = 5.0f;
    Player::curved_face[0] = 0.0f;
    Player::curved_face[1] = 0.35f;
    Player::curved_face[2] = 0.0f;
    for(int i=1; i <=37; i++)
    {
        Player::curved_face[3*i] = a*glm::cos(glm::radians((i-1)*angle));
        Player::curved_face[3*i+1] = Player::curved_face[1] + b*glm::sin(glm::radians((i-1)*angle));
        Player::curved_face[3*i+2] = 0.0f;
    }
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

void Player::draw(unsigned int shaderProgram, unsigned int VAO[])
{
    glm::mat4 model(1.0f);
    glm::vec3 position = this->origin - (float)this->row*this->row_gap + (float)this->col*this->col_gap;
    model = glm::translate(model, position);
    model = glm::scale(model, glm::vec3(1.2f, 1.2f, 0.0f));
    switch(this->orientation)
    {
        case 1: model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f)); break;
        case 2: model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f)); break;
        case 3: model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f)); break;
    }
    int location = glGetUniformLocation(shaderProgram, "model");
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(model));
    for(int i=0; i<Player::faces; i++)
    {
        location = glGetUniformLocation(shaderProgram, "color");
        if(i == 0) glUniform3f(location, 0.3f, 0.3f, 0.6f);
        else glUniform3f(location, 0.0f, 1.0f, 0.0f);
        glBindVertexArray(VAO[i]);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 6);
    }
    glBindVertexArray(VAO[5]);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 38);
}

void Player::bindData(unsigned int VAO[], unsigned int VBO[])
{
    for(int i=0; i<Player::faces; i++)
    {
        glGenBuffers(1, &VBO[i]);
        glGenVertexArrays(1, &VAO[i]);
        glBindVertexArray(VAO[i]);
        glBindBuffer(GL_ARRAY_BUFFER, VBO[i]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Player::vertices[i]), Player::vertices[i], GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), 0);
        glEnableVertexAttribArray(0);
    }
    glGenBuffers(1, &VBO[5]);
    glGenVertexArrays(1, &VAO[5]);
    glBindVertexArray(VAO[5]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[5]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Player::curved_face), Player::curved_face, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
    glEnableVertexAttribArray(0);
}