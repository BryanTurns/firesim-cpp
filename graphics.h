#pragma once

#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <cmath>
#include "settings.h"
#include "grid.h"

#include <linmath.h>
typedef struct Vertex
{
    vec2 pos;
    vec3 col;
} Vertex;

void setGraphicsPosition(std::array<Vertex, VERTEX_COUNT>& graphicsBuffer, std::vector<Tile*>& grid);
void gridToGraphics(std::array<Vertex, VERTEX_COUNT>& graphicsBuffer, std::vector<Tile*>& grid, std::unordered_set<int>& updateList);
void checkGLError(const char *);
void error_callback(int error, const char* description);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);