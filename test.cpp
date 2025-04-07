#include "grid.h"
#include "graphics.h"
#include "settings.h"
#include "shaders.h"

#include <chrono>
#include <thread>
using namespace std::chrono_literals;


int main() {
    // Initiate grids
    static std::array<Tile, TILE_COUNT> newGrid, oldGrid;
    static std::unordered_set<int> updateList, fireList;
    static std::array<Vertex, VERTEX_COUNT> graphicsBuffer;

    // Initiates the actual game logic objects
    initiateGrid(newGrid, oldGrid);
    startFire(newGrid, fireList);
    updateList = fireList;
    updateOldGrid(newGrid, oldGrid, updateList);

    // Initialize the graphics buffer
    for (int i = 0; i < TILE_COUNT; i++) 
        updateList.insert(i);
    gridToGraphics(graphicsBuffer, newGrid, updateList);
    setGraphicsPosition(graphicsBuffer, newGrid);
    updateList.clear();

    // Error checking
    int  success;
    char infoLog[512];
    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
        exit(EXIT_FAILURE);
    // Setup OpenGL window
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(640, 480, "Fire-Sim", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);
    gladLoadGL(glfwGetProcAddress);
    glfwSwapInterval(1);
    // Callbacks
    glfwSetKeyCallback(window, key_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    // Vertex shader
    const GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_text, NULL);
    glCompileShader(vertex_shader);
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(vertex_shader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // Fragment shader
    const GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_text, NULL);
    glCompileShader(fragment_shader);
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
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(program, 512, NULL, infoLog);
    }
    // Setting memory location of pos and col
    const GLint vpos_location = glGetAttribLocation(program, "vPos");
    const GLint vcol_location = glGetAttribLocation(program, "vCol");
    // 
    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    checkGLError("bind");
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*VERTEX_COUNT, (void *) &graphicsBuffer, GL_DYNAMIC_DRAW);
    checkGLError("data");
    glEnableVertexAttribArray(vpos_location);
    glVertexAttribPointer(vpos_location, 2, GL_FLOAT, GL_FALSE,
                          sizeof(Vertex), (void*) offsetof(Vertex, pos));
    glEnableVertexAttribArray(vcol_location);
    glVertexAttribPointer(vcol_location, 3, GL_FLOAT, GL_FALSE,
                          sizeof(Vertex), (void*) offsetof(Vertex, col));

    while (!glfwWindowShouldClose(window)) {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        const float ratio = width / (float) height;

        // Update game logic
        updateGrid(newGrid, oldGrid, updateList, fireList);
        gridToGraphics(graphicsBuffer, newGrid, updateList);
        updateList.clear();

        glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*VERTEX_COUNT, (void *) &graphicsBuffer, GL_DYNAMIC_DRAW);
        
        glViewport(0, 0, width, height);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(program);
        glBindVertexArray(VAO);
        checkGLError("bind VAO");
        glDrawArrays(GL_TRIANGLES, 0, VERTEX_COUNT);

        glfwSwapBuffers(window);
        glfwPollEvents();
        std::this_thread::sleep_for(17ms);
        
        if (fireList.empty()) {
            sleep(10);
            break;
        }
    }
    return 0;
}   