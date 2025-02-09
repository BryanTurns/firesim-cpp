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
 
typedef struct Vertex
{
    vec2 pos;
    vec3 col;
} Vertex;
 
static const Vertex vertices[6] =
{
    { { -0.6f, -0.4f}, { 1.f, 0.f, 0.f } },
    { {  0.6f, -0.4f}, { 0.f, 1.f, 0.f } },
    { {   0.f,  0.6f}, { 0.f, 0.f, 1.f } },
    { {  -1.f, -0.4f}, { 1.f, 0.f, 0.f } },
    { {  0.8f, -0.2f}, { 1.f, 1.f, 0.f } },
    { {   3.f,  0.6f}, { 1.f, 0.f, 1.f } }
};
 
static const char* vertex_shader_text =
"#version 330\n"
"uniform mat4 MVP;\n"
"in vec3 vCol;\n"
"in vec2 vPos;\n"
"out vec3 color;\n"
"void main()\n"
"{\n"
"    gl_Position = MVP * vec4(vPos, 0.0, 1.0);\n"
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

 
    // NOTE: OpenGL error checks have been omitted for brevity
 
    GLuint vertex_buffer;
    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
 
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
    
    const GLint mvp_location = glGetUniformLocation(program, "MVP");
    const GLint vpos_location = glGetAttribLocation(program, "vPos");
    const GLint vcol_location = glGetAttribLocation(program, "vCol");
 
    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
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
 
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);
 
        mat4x4 m, p, mvp;
        mat4x4_identity(m);
        mat4x4_rotate_X(m, m, (float) glfwGetTime());
        mat4x4_ortho(p, -ratio, ratio, -1.f, 1.f, 1.f, -1.f);
        mat4x4_mul(mvp, p, m);
 
        glUseProgram(program);
        glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*) &mvp);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
 
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
 
    glfwDestroyWindow(window);
 
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
 
int genGrid(int t_count, Vertex verticies[]) {
    double increment = 1.f/10.f;

    for (int i = 0; i < t_count; i++) {
        
    }
}