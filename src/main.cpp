#include "main.h"
#include "line.h"
#include "maze.h"
// #include "player.h"
#include "background.h"
#include<stdlib.h>
#include<time.h>
#include "imposter.h"
#include<vector>
#include "timer.h"
#include "game.h"
#include<string>
#include "textrenderer.h"
#include<stdio.h>

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
                                 "out vec3 FragPos;\n"
                                 "void main()\n"
                                 "{\n"
                                 "FragPos = vec3(model * vec4(position, 1.0f));\n"
                                 "gl_Position = projection * view * model * vec4(position, 1.0f);\n"
                                 "}\n";

const char *fragmentShaderSource = "#version 330 core\n"
                                "out vec4 FragColor;\n"
                                "uniform vec3 color;\n"
                                "void main()\n"
                                "{\n"
                                "FragColor = vec4(color, 1.0f);\n"
                                "}\n";

const char* lightingFragmentShaderSource = "#version 330 core\n"
                                        "out vec4 FragColor;\n"
                                        "in vec3 FragPos;\n"
                                        "uniform vec3 color;\n"
                                        "uniform vec3 lightPos;\n"
                                        "uniform vec3 viewPos;\n"
                                        "void main()\n"
                                        "{\n"
                                        "float specularStrength = 1.0f;\n"
                                        "vec3 lightDirection = normalize(lightPos-FragPos);\n"
                                        "vec3 reflectDir = reflect(-lightDirection, vec3(0.0f, 0.0f, 1.0f));\n"
                                        "vec3 viewDir = normalize(viewPos - FragPos);\n"
                                        "float intensity = pow(max(dot(viewDir, reflectDir), 0.0f), 2.4f);\n"
                                        "vec3 effect = specularStrength*intensity*vec3(1.0f, 1.0f, 1.0f);\n"
                                        "FragColor = vec4(effect * color, 1.0f);\n"
                                        "}\n";
// Vertices for horizontal line
float vertices_h[] = {
    -0.5f, 0.0f, 0.0f,
     0.5f, 0.0f, 0.0f
};
Line horizontal(vertices_h); // Object created
// Vertices for vertical line 
float vertices_v[] = {
    0.0f, -0.5f, 0.0f,
    0.0f,  0.5f, 0.0f
};
Line vertical(vertices_v); // object created

// Setting parameters for the start position of the maze, gap between rows and columns with appropirate scaling.
glm::vec3 row_gap(0.0f, 1.0f, 0.0f);
glm::vec3 col_gap(1.0f, 0.0f, 0.0f);
glm::vec3 row_start(-14.0f, 5.5f, 0.0f);
glm::vec3 scaling(2.0f, 2.0f, 0.0f);
glm::vec3 col_start = row_start - scaling/2.0f*row_gap - scaling/2.0f*col_gap;
glm::vec3 origin(row_start[0], col_start[1], 0.0f);
glm::vec3 character_scaling(1.3f, 1.3f, 0.0f);

Timer t(1/60.0f); // Timer so that the experience is same in every device
// Player starts in the cell (0, 0) with 100 health. 
float max_health = 100.0f;
Player player(0, 0, max_health, origin, scaling*row_gap, scaling*col_gap, character_scaling, row_gap);
// Imposter starts at a random cell in the maze
Imposter imposter;

Maze maze(rows, cols, row_start, col_start, row_gap, col_gap, scaling);
std::vector<int>graph[151]; // Storing the maze as a graph to find path from imposter to the player
Game game(rows, cols); // This object has all the information like time left, where are the task buttons, power ups, obstacles
// Maze is displayed with some background so that the lightting feature can be experienced
Background background(glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(cols*2.0f, rows*2.0f, 0.0f), glm::vec3(0.0f, -1.5f, -0.5f));
unsigned int currentShader, shaderProgram, lightProgram, textProgram;

// Text renderer object
TextRenderer render;

// This function creates a window and returns it
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

// This callback sets the viewport whenever the window is resized
void resize_callback(GLFWwindow* window, int w, int h)
{
    glViewport(0, 0, w, h);
}

// This functions handles keyboard events in the game
void input(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if(action == 1) // If key is pressed
    {
        // WSAD for movement of the player
        // L for toggling the light
        switch(key)
        {
            case 'L': 
                game.lighting = !game.lighting;
                player.frames_in_dark = 0; 
                break;
            case 'W':
                if(!player.moveRow && !player.moveCol)
                    player.moveRow = -1;
                break;
            case 'S':
                if(!player.moveRow && !player.moveCol)
                    player.moveRow =  1;
                break;
            case 'A':
                if(!player.moveCol && !player.moveRow)
                    player.moveCol = -1;
                break;
            case 'D':
                if(!player.moveCol && !player.moveRow)
                    player.moveCol =  1;
                break;
        }
    }
}
// This fucntion creates a shader of type 'type' and returns it.
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
// This fucntion creates a shader program.
unsigned int createProgram(const char* vertexShaderSource, const char* fragmentShaderSource)
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

// This function checks whether player collides/being in same cell as power ups and obstacles
void handle_power_ups()
{
    if(game.power_ups_released)
    {
        int player_cell = player.row*cols + player.col;
        for(int i=0; i<game.pows; i++)
        {
            if(!game.pow_touched[i] && player_cell == game.pow_cells[i]) // If player is in same cell as power up
            {
                game.pow_touched[i] = 1;
                player.health = std::min(player.health + game.pow_score, max_health);
            }
        }
        for(int i=0; i<game.obs; i++)
        {
            if(!game.obs_touched[i] && player_cell == game.obs_cells[i]) // If player is in same cell as obstacle
            {
                game.obs_touched[i] = 1;
                player.health = std::max(player.health - game.obs_score, 0.0f);
                if(player.health <= 0) game.game_over = 2;
            }
        }
    }
}

void handle_player_movement()
{
    if(player.moveCol != 0)
        player.move_col(player.moveCol, !maze.included[player.row][player.col+(player.moveCol==1)][1]);
    else if(player.moveRow != 0)
        player.move_row(player.moveRow, !maze.included[player.row+(player.moveRow==1)][player.col][0]);
}

int main()
{
    srand(time(0));
    imposter = Imposter(rand()%rows, rand()%cols, 100, origin, scaling*row_gap, scaling*col_gap, character_scaling, col_gap);
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
    // Setting callbacks for window resize and keyboard events
    glfwSetFramebufferSizeCallback(window, resize_callback);
    glfwSetKeyCallback(window, input);

    // Storing all the VAOs for different objects - horizontal line, vertical line, background, player, power ups and buttons
    unsigned int VAO_h, VBO_h;
    horizontal.bindData(VAO_h, VBO_h);
    unsigned int VAO_v, VBO_v;
    vertical.bindData(VAO_v, VBO_v);
    maze.generate_maze(graph);
    unsigned int VAO[6], VBO[6];
    player.bindData(VAO, VBO);
    unsigned int VAO_bg, VBO_bg;
    background.bindData(VAO_bg, VBO_bg);
    unsigned int VAO_btn, VAO_pow_obs;
    game.bindData(VAO_btn, VAO_pow_obs);

    glEnable(GL_DEPTH_TEST);
    // Enabling blending for text rendering
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    shaderProgram = createProgram(vertexShaderSource, fragmentShaderSource);
    lightProgram = createProgram(vertexShaderSource, lightingFragmentShaderSource);
    textProgram = createProgram(render.vertexShaderSource, render.fragmentShaderSource);
    
    // Using orthographic projection for 16:9 aspect ratio
    glm::mat4 proj = glm::ortho(0.0f, (float)width, 0.0f, (float)height);
    glUseProgram(textProgram);
    glUniformMatrix4fv(glGetUniformLocation(textProgram, "projection"), 1, GL_FALSE, glm::value_ptr(proj));
    
    currentShader = shaderProgram;
    glUseProgram(shaderProgram);

    glm::mat4 projection = glm::ortho(-16.0f, 16.0f, -9.0f, 9.0f, -1.0f, 1.0f);
    int location = glGetUniformLocation(shaderProgram, "projection");
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(projection));
    glm::mat4 view = glm::mat4(1.0f);
    location = glGetUniformLocation(shaderProgram, "view");
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(view));
    
    glUseProgram(lightProgram);
    location = glGetUniformLocation(lightProgram, "projection");
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(projection));
    location = glGetUniformLocation(lightProgram, "view");
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(view));
    
    unsigned int VAO_text, VBO_text;
    render.bindData(VAO_text, VBO_text);
    render.createMap();

    float prev_move = 0;
    float time_gap = 0.4f;
    int same_frames = 0;
    char HUD[200], message[200]; // HUD display and after game message
    while(!glfwWindowShouldClose(window))
    {
        if(!t.process_tick()) continue;
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        if(game.game_over != 0)
        {
            glm::vec3 color;
            if(game.game_over == 2) color = glm::vec3(0.0f, 0.0f, 1.0f), sprintf(message, "Game Over");
            else if(game.game_over == 1) color = glm::vec3(1.0f, 0.0f, 0.0f), sprintf(message, "You lost! :(");
            else color = glm::vec3(0.0f, 1.0f, 0.0f), sprintf(message, "You Won! :)");
            render.renderText(textProgram, message, 512, 360, 1.5f, color, VAO_text, VBO_text);
            glfwSwapBuffers(window);
            glfwPollEvents();
            continue;
        }
        game.decrease_timer();
        if(game.time_left <= 0) game.game_over = 2; // game ends when time is up
        glUseProgram(currentShader); // Use shader based on lighting on or off
        if(game.lighting)
        {
            player.frames_in_dark = 0;
            currentShader = shaderProgram;
        }
        else
        {
            currentShader = lightProgram;
            // increase player health when light is off
            player.frames_in_dark++;
            player.health = std::min(player.health + game.health_gain_in_dark, max_health);
            // Assigning viewing and light source positions
            location = glGetUniformLocation(lightProgram, "viewPos");
            glm::vec3 position = origin - (float)player.row*row_gap*scaling + (float)player.col*col_gap*scaling;
            glUniform3f(location, position[0], position[1], 2.0f);
            location = glGetUniformLocation(lightProgram, "lightPos");
            glUniform3f(location, position[0], position[1], 1.0f);
        }
        // Displaying objects - background, maze, player, imposter, etc 
        background.display(currentShader, VAO_bg);
        maze.draw(currentShader, VAO_h, VAO_v);
        game.draw(currentShader, VAO_btn, VAO_pow_obs, origin, row_gap*scaling, scaling*col_gap, cols);
        player.draw(currentShader, VAO);
        // Handling player movement
        handle_player_movement();
        game.check_btn_press(player.row, player.col, rows, cols);
        if(player.col == cols) game.game_over = 3;
        handle_power_ups();
        if(game.imposter_alive)
        {
            imposter.draw(currentShader, VAO);
            // Move imposter
            if(imposter.moveCol != 0)
                imposter.move_col(imposter.moveCol, 1);
            else if(imposter.moveRow != 0)
                imposter.move_row(imposter.moveRow, 1);
            if(glfwGetTime()-prev_move > time_gap) // Moving imposter at a frequncy of once in 0.4 seconds
            {
                prev_move += time_gap;
                imposter.move(graph, rows, cols, player.row, player.col);
            }
            if(player.row == imposter.row && player.col == imposter.col) // if position of player and imposter is the same
            {
                player.health -= game.health_loss_imposter; // depending on time health is decreased.
                if(player.health <= 0)
                    game.game_over = 1, player.health = 0;
            }
        }
        if(game.tasks_completed == game.total_tasks) maze.open_exit(); // exit opens when the user completes 2 tasks
        sprintf(HUD, "Time Left: %d sec, Health: %d, Tasks completed: %d", (int)game.time_left, (int)player.health, game.tasks_completed);
        render.renderText(textProgram, HUD, 100.0f, 656.0f, 1.0f, glm::vec3(1.0f, 1.0f, 1.0f), VAO_text, VBO_text);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}