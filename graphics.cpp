#include "graphics.h"
// Multiply by 6????
void gridToGraphics(std::array<Vertex, VERTEX_COUNT>& graphicsBuffer, std::array<Tile, TILE_COUNT>& grid, std::unordered_set<int>& updateList) {
    for (auto it = updateList.begin(); it != updateList.end(); ++it) {
        int graphicsIndex = 6 * (*it);
        int gridIndex = (*it);
        if (grid[gridIndex].onFire == true) {
            for (int i = 0; i < 6; i++) {
                graphicsBuffer[graphicsIndex + i].col[0] = grid[gridIndex].fuel;
                graphicsBuffer[graphicsIndex + i].col[1] = 0;
                graphicsBuffer[graphicsIndex + i].col[2] = 0;
            }
        }
        else {
            for (int i = 0; i < 6; i++) {
                graphicsBuffer[graphicsIndex + i].col[0] = 0;
                graphicsBuffer[graphicsIndex + i].col[1] = grid[gridIndex].fuel;
                graphicsBuffer[graphicsIndex + i].col[2] = 0;
            }
        }
    }
}
void setGraphicsPosition(std::array<Vertex, VERTEX_COUNT>& graphicsBuffer, std::array<Tile, TILE_COUNT>& grid) {
    float increment = 2.0f/TILE_ROW_COUNT;
    int i = 0;
    for (auto it = grid.begin(); it != grid.end(); ++it, i++) {
        int gridIndex = i*6;
        int col = i % TILE_ROW_COUNT;
        int row = i / TILE_ROW_COUNT;
        float base_x = col*increment-1;
        float base_y = row*increment-1;
    
        // First triangle
        graphicsBuffer[gridIndex].pos[0] = base_x;  // Bottom left
        graphicsBuffer[gridIndex].pos[1] = base_y;
        graphicsBuffer[gridIndex+1].pos[0] = base_x; // Top left
        graphicsBuffer[gridIndex+1].pos[1] = base_y+increment;
        graphicsBuffer[gridIndex+2].pos[0] = base_x+increment; // Bottom right
        graphicsBuffer[gridIndex+2].pos[1] = base_y;
        // Second triangle
        graphicsBuffer[gridIndex+3].pos[0] = base_x + increment; // Top right
        graphicsBuffer[gridIndex+3].pos[1] = base_y + increment;
        graphicsBuffer[gridIndex+4].pos[0] = base_x + increment; // Bottom right
        graphicsBuffer[gridIndex+4].pos[1] = base_y;
        graphicsBuffer[gridIndex+5].pos[0] = base_x; // Top left
        graphicsBuffer[gridIndex+5].pos[1] = base_y + increment;
    }
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
void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}