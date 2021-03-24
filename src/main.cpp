#include "main.h"
#include "line.h"

const unsigned int width = 1280;
const unsigned int height = 720;
const float ascpectRatio = (float)width/height;
int rows = 7;
int cols = 7;

const char *vertexShaderSource = "#version 330 core\n"
                                 "layout (location = 0) in vec3 position;\n"
                                 "uniform mat4 model\n;"
                                 "uniform mat4 view;\n"
                                 "uniform mat4 projection;\n"
                                 "void main()\n"
                                 "{\n"
                                 "gl_Position = projection * view * model * vec4(position, 1.0f);\n"
                                 "}\n";

const char *fragmentShaderSource = "#version 330 core\n"
                                   "out vec4 FragColor;\n"
                                   "void main()\n"
                                   "{\n"
                                   "FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);\n"
                                   "}\n";

float vertices_h[] = {
    -0.5f, 0.0f, 0.0f,
     0.5f, 0.0f, 0.0f
};
Line horizontal(vertices_h);

float vertices_v[] = {
    0.0f, -0.5f, 0.0f,
    0.0f,  0.5f, 0.0f
};
Line vertical(vertices_v);

GLFWwindow* createWindow()
{
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    #ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif
    GLFWwindow* window = glfwCreateWindow(width, height, "Maze",NULL, NULL);
    return window;
}

void resize_callback(GLFWwindow* window, int w, int h)
{
    glViewport(0, 0, w, h);
}

unsigned int createShader(const char* source, int type)
{
    unsigned int shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);
    int status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if(!status)
    {
        char errorInfo[512];
        glGetShaderInfoLog(shader, 512, NULL, errorInfo);
        std::cout << errorInfo << "\n";
    }
    return shader;
}

unsigned int createProgram()
{
    unsigned int vertexShader = createShader(vertexShaderSource, GL_VERTEX_SHADER);
    unsigned int fragmentShader = createShader(fragmentShaderSource, GL_FRAGMENT_SHADER);
    unsigned int program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glValidateProgram(program);
    glLinkProgram(program);
    int status;
    glGetProgramiv(program, GL_COMPILE_STATUS, &status);
    if(!status)
        std::cout << "Failed to compile program\n";
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    return program;
}

int main()
{
    if(!glfwInit())
    {
        std::cout << "Failed to initialize GLFW\n";
        return -1;
    }
    GLFWwindow* window = createWindow();
    if(!window)
    {
        glfwTerminate();
        std::cout << "Failed to create a window\n";
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        glfwTerminate();
        std::cout << "Failed to initialise glad\n";
        return -1;
    }
    glViewport(0, 0, width, height);
    glfwSetFramebufferSizeCallback(window, resize_callback);

    unsigned int VAO_h, VBO_h;
    horizontal.bindData(VAO_h, VBO_h);
    unsigned int VAO_v, VBO_v;
    vertical.bindData(VAO_v, VBO_v);

    unsigned int shaderProgram = createProgram();
    glUseProgram(shaderProgram);

    glm::mat4 projection = glm::ortho(-8.0f*2, 8.0f*2, -4.5f*2, 4.5f*2, -1.0f, 1.0f);
    int location = glGetUniformLocation(shaderProgram, "projection");
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(projection));
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -0.5f));
    location = glGetUniformLocation(shaderProgram, "view");
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(view));
    while(!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(VAO_h);
        for(int r=0; r < rows+1; r++)
        {
            glm::vec3 start = glm::vec3(-7.0f, 3+4.0f-r, 0.0f);
            for(int c=0; c < cols; c++)
            {
                glm::mat4 model = glm::translate(glm::mat4(1.0f), start + (float)c*glm::vec3(1.0f, 0.0f, 0.0f));
                location = glGetUniformLocation(shaderProgram, "model");
                glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(model));
                glDrawArrays(GL_LINES, 0 ,2);
            }
        }
        glBindVertexArray(VAO_v);
        for (int r = 0; r < rows; r++)
        {
            glm::vec3 start = glm::vec3(-7.5f, 3+3.5f - r, 0.0f);
            for (int c = 0; c < cols+1; c++)
            {
                glm::mat4 model = glm::translate(glm::mat4(1.0f), start + (float)c * glm::vec3(1.0f, 0.0f, 0.0f));
                location = glGetUniformLocation(shaderProgram, "model");
                glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(model));
                glDrawArrays(GL_LINES, 0, 2);
            }
        }
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}