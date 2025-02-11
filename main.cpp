#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
 
#include <linmath.h>
 
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>

#include <iostream>
#include <cmath>
#include <random>
#include <chrono>
 
typedef struct Vertex
{
    vec2 pos;
    vec3 col;
} Vertex;


static const Vertex vertices[6] =
{
    { { -0.5f, -.5f}, { 0.f, 0.f, 0.f } }, // Bottom left: 0
    { {  -.5f, .5f}, { 0.f, 0.f, 0.f } }, // Top left: 1
    { {   .5f,  -.5f}, { 0.f, 0.f, 0.f } }, // Bottom right: 2
    { {  -0.5f, 0.5f}, { 0.f, 0.f, 0.f } }, // Top left: 3
    { {  .5f, .5f}, { 0.f, 0.f, 0.f } }, // Top right: 4
    { {   0.5f,  -0.5f}, { 0.f, 0.f, 0.f } } // Bottom right: 5
};

unsigned int indices[] = {
    0, 1, 2,
    3, 4, 5
};
 
static const char* vertex_shader_text =
"#version 330\n"
"in vec3 vCol;\n"
"in vec2 vPos;\n"
"out vec3 color;\n"
"void main()\n"
"{\n"
"    gl_Position = vec4(vPos, 0.0, 1.0);\n"
"    color = vCol;\n"
"}\n";
 
static const char* fragment_shader_text =
"#version 330\n"
"in vec3 color;\n"
"out vec4 fragment;\n"
"void main()\n"
"{\n"
"    fragment = vec4(color, 1.0);\n"
"}\n";
 
Vertex *genGrid(int tile_count);
void checkGLError(const char *);
void updateGrid(Vertex *grid, int tile_count);

static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}
 
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}
 
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}  

int main(void)
{
    // Error checking
    int  success;
    char infoLog[512];
    glfwSetErrorCallback(error_callback);
 
    if (!glfwInit())
        exit(EXIT_FAILURE);
 
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
 
    GLFWwindow* window = glfwCreateWindow(640, 480, "OpenGL Triangle", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
 
    glfwSetKeyCallback(window, key_callback);
 
    glfwMakeContextCurrent(window);
    gladLoadGL(glfwGetProcAddress);
    glfwSwapInterval(1);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);  
   
    const GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_text, NULL);
    glCompileShader(vertex_shader);
    // Error checking
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(vertex_shader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
 
    const GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_text, NULL);
    glCompileShader(fragment_shader);
    // Error checking
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(fragment_shader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    
    // Build the shader program for the GPU
    const GLuint program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);
    // Error checking
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(program, 512, NULL, infoLog);
    }
    
    const GLint vpos_location = glGetAttribLocation(program, "vPos");
    const GLint vcol_location = glGetAttribLocation(program, "vCol");
    
    int tile_count = 100;
    int vertex_count = tile_count*tile_count*2*3;
    Vertex *grid = genGrid(tile_count);

    // GLuint VBO;
    GLuint VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    checkGLError("bind");
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*vertex_count, grid, GL_STATIC_DRAW);
    checkGLError("data");
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    // Locations of the vpos on GPU
    glEnableVertexAttribArray(vpos_location);
    // Tells the shader how to interpret the array of verticies. 
    glVertexAttribPointer(vpos_location, 2, GL_FLOAT, GL_FALSE,
                          sizeof(Vertex), (void*) offsetof(Vertex, pos));
    // Locations of the colors on GPU
    glEnableVertexAttribArray(vcol_location);
    glVertexAttribPointer(vcol_location, 3, GL_FLOAT, GL_FALSE,
                          sizeof(Vertex), (void*) offsetof(Vertex, col));

    
    
    while (!glfwWindowShouldClose(window))
    {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        const float ratio = width / (float) height;

        updateGrid(grid, tile_count);
 
        glViewport(0, 0, width, height);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
 
        glUseProgram(program);
        glBindVertexArray(VAO);
        checkGLError("bind VAO");
        glDrawArrays(GL_TRIANGLES, 0, vertex_count);


 
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
 
    glfwDestroyWindow(window);
    free(grid);
 
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
void startFire(int x, int y);

void updateGrid(Vertex *grid, int vertex_count) {
    for (int i = 0; i < vertex_count; i+= 6) {
        if (grid[i].col[0] == 0)
            continue;
        
        int SCALE_FACTOR = 1;
        if (i > 0) {
            
        }

    }
}
 
Vertex *genGrid(int tile_count) {
    float increment = 2.0/tile_count;
    Vertex *grid = (Vertex *) std::malloc(sizeof(Vertex)*2*3*tile_count*tile_count);
    // Generate fuel amount per tile
    srand( std::chrono::system_clock::now().time_since_epoch().count());
    for (int i = 0; i < tile_count; i++) {
        int curRow = i*tile_count*2*3;
        float base_y = i*increment-1;
        for (int j = 0; j < tile_count; j++) {
            float fuel = ((((float)rand())/((float)RAND_MAX))/ 2.f) + 0.5f;

            float base_x = j*increment-1;
            int start_index = curRow + j*6;
            // First Triangle
            grid[start_index] = {{base_x, base_y}, {0.f, fuel, 0.f}}; // Bottom left
            grid[start_index+1] = {{base_x+increment, base_y}, {0.f, fuel, 0.f}}; // Top left
            grid[start_index+2] = {{base_x, base_y+increment}, {0.f, fuel, 0.f}}; // Bottom right
            // Second Triangle
            grid[start_index+3] = {{base_x+increment, base_y+increment}, {0.f, fuel, 0.f}}; // Top right
            grid[start_index+4] = {{base_x+increment, base_y}, {0.f, fuel, 0.f}}; // Bottom right
            grid[start_index+5] = {{base_x, base_y+increment}, {0.f, fuel, 0.f}}; // Top left
        }    
    }
    return grid;
}

void checkGLError(const char *text) {
    GLenum err;
    
    while ((err = glGetError()) != GL_NO_ERROR) {
        std::cout << text << ": ";
        if (err == GL_INVALID_ENUM) 
            std::cout << "ENUM" << std::endl;
        else if (err == GL_INVALID_OPERATION) 
            std::cout << "OPERATION" << std::endl;
        else if (err == GL_INVALID_VALUE)
            std::cout << "VALUE" << std::endl;
        else if (err == GL_INVALID_FRAMEBUFFER_OPERATION)
            std::cout << "FRAME OPP" << std::endl;
        else if (err == GL_INVALID_OPERATION) 
            std::cout<< "OPP" << std::endl;
        else if (err == GL_OUT_OF_MEMORY)
            std::cout << "MEMORY" << std::endl;
    }
}