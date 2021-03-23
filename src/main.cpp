#include "main.h"

const unsigned int width = 1280;
const unsigned int height = 720;

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
    std::cout << "Vertex Shader is " << vertexShader << "\n";
    std::cout << "Fragment shader is " << fragmentShader << "\n";
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

    unsigned int shaderProgram = createProgram();
    glUseProgram(shaderProgram);
    while(!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);


        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}