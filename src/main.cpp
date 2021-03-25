#include "main.h"
#include "line.h"
#include "maze.h"
#include "player.h"

const unsigned int width = 1280;
const unsigned int height = 720;
const float ascpectRatio = (float)width/height;
int rows = 7;
int cols = 15;

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

glm::vec3 row_gap(0.0f, 1.0f, 0.0f);
glm::vec3 col_gap(1.0f, 0.0f, 0.0f);
glm::vec3 row_start(-14.0f, 5.5f, 0.0f);
glm::vec3 scaling(2.0f, 2.0f, 0.0f);
glm::vec3 col_start = row_start - scaling/2.0f*row_gap - scaling/2.0f*col_gap;
glm::vec3 origin(row_start[0], col_start[1], 0.0f);
Player player(0, 0, 100, origin, scaling*row_gap, scaling*col_gap);

Maze maze(rows, cols, row_start, col_start, row_gap, col_gap, scaling);

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

void input(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if(action == 1 || action == 2)
    {
        switch(key)
        {
            case 'W': player.move_row(-1, !maze.included[player.row][player.col][0]); break;
            case 'S': player.move_row( 1, !maze.included[player.row+1][player.col][0]); break;
            case 'A': player.move_col(-1, !maze.included[player.row][player.col][1]); break;
            case 'D': player.move_col( 1, !maze.included[player.row][player.col+1][1]); break;
        }
    }
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
    glfwSetKeyCallback(window, input);

    unsigned int VAO_h, VBO_h;
    horizontal.bindData(VAO_h, VBO_h);
    unsigned int VAO_v, VBO_v;
    vertical.bindData(VAO_v, VBO_v);
    maze.generate_maze();
    unsigned int VAO, VBO, IBO;
    player.bindData(VAO, VBO, IBO);

    unsigned int shaderProgram = createProgram();
    glUseProgram(shaderProgram);

    glm::mat4 projection = glm::ortho(-16.0f, 16.0f, -9.0f, 9.0f, -1.0f, 1.0f);
    int location = glGetUniformLocation(shaderProgram, "projection");
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(projection));
    glm::mat4 view = glm::mat4(1.0f);
    location = glGetUniformLocation(shaderProgram, "view");
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(view));
    glEnable(GL_DEPTH_TEST);
    while(!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        maze.draw(shaderProgram, VAO_h, VAO_v);
        player.draw(shaderProgram, VAO);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}