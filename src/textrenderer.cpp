#include "textrenderer.h"
#include<cstring>
#include<map>
#include<ft2build.h>
#include FT_FREETYPE_H
// This struct store info about every character
typedef struct Character
{
    unsigned int textureID;
    glm::ivec2 size;
    glm::ivec2 bearing;
    unsigned int advance;
}Character;
std::map<char, Character>characters;

void TextRenderer::createMap()
{
    FT_Library lib;
    if(FT_Init_FreeType(&lib))
    {
        std::cout << "Error! Could not initialize freetype\n";
        return;
    }
    FT_Face face;
    if(FT_New_Face(lib, "../assets/test.ttf", 0, &face))
    {
        std::cout <<"Error! Could not create a face\n";
        return;
    }
    FT_Set_Pixel_Sizes(face, 0, 48);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    for(int i=0; i<256; i++)
    {
        if(FT_Load_Char(face, (char)i, FT_LOAD_RENDER))
        {
            std::cout << "Error! Could not load Glyph\n";
            continue;
        }
        // Generating textures for each character
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows, 
                    0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, 0);
        // Storing the data for redendering the text on the screen
        Character newChar = {
            texture, glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            (unsigned int)face->glyph->advance.x
        };
        characters.insert(std::pair<char, Character>((char)i, newChar));
    }
    FT_Done_Face(face);
    FT_Done_FreeType(lib);
}

void TextRenderer::bindData(unsigned int& VAO, unsigned int& VBO)
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindVertexArray(VAO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*6*4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void TextRenderer::renderText(unsigned int shaderProgram, char* string, float x, float y, float scaling, glm::vec3 color, 
                            unsigned int VAO, unsigned int VBO)
{
    // Using text shader Program
    glUseProgram(shaderProgram);
    glActiveTexture(GL_TEXTURE0);
    int location = glGetUniformLocation(shaderProgram, "color");
    glUniform3f(location, color[0], color[1], color[2]);
    glBindVertexArray(VAO);
    int len = strlen(string);
    for(int i=0; i<len; i++)
    {
        Character c = characters[string[i]];
        float X = x + c.bearing.x * scaling;
        float Y = y - scaling * (c.size.y - c.bearing.y);
        float h = c.size.y * scaling;
        float w = c.size.x * scaling;
        float vertices[6][4] = {
            {X, Y+h, 0.0f, 0.0f},
            {X, Y, 0.0f, 1.0f},
            {X+w, Y, 1.0f, 1.0f},
            {X, Y+h, 0.0f, 0.0f},
            {X+w, Y, 1.0f, 1.0f},
            {X+w, Y+h, 1.0f, 0.0f}
        };
        glBindTexture(GL_TEXTURE_2D, c.textureID); // binding the texture of the character
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        x += (c.advance>>6) * scaling; 
    }
    glBindTexture(GL_TEXTURE_2D, 0);
}