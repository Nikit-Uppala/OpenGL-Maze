#include "main.h"

#ifndef TEXTRENDERER_H
#define TEXTRENDERER_H
#endif

class TextRenderer
{
    public:
    const char* vertexShaderSource = "#version 330 core\n"
                                    "layout (location=0) in vec4 vertex;\n"
                                    "out vec2 Texture;\n"
                                    "uniform mat4 projection;\n"
                                    "void main()\n"
                                    "{\n"
                                    "gl_Position = projection * vec4(vertex.xy, 0.5f, 1.0f);\n"
                                    "Texture = vertex.zw;\n"
                                    "}\n";
    const char* fragmentShaderSource = "#version 330 core\n"
                                    "in vec2 Texture;\n"
                                    "out vec4 textColor;\n"
                                    "uniform vec3 color;\n"
                                    "uniform sampler2D text;\n"
                                    "void main()\n"
                                    "{\n"
                                    "vec4 sampled = vec4(1.0f, 1.0f, 1.0f, texture(text, Texture).r);\n"
                                    "textColor = vec4(color, 1.0f) * sampled;\n"
                                    "}\n";
        TextRenderer() {}
        void renderText(unsigned int shaderProgram, char* string, float x, float y, float scaling, glm::vec3 color, 
                        unsigned int VAO, unsigned int VBO);
        void createMap();
        void bindData(unsigned int& VAO, unsigned int& VBO);
    private:
};