#include <linmath.h>
#include <stdlib.h>
#include <chrono>
#include <iostream>
#include <thread>
#include <csignal>

using namespace std::chrono_literals;


typedef struct Vertex
{
    vec2 pos;
    vec3 col;
} Vertex;


void startFire(Vertex *grid, int tile_count);
int updateGrid(Vertex *grid, int tile_count);
inline int getGridIndex(int i, int j, int tile_count);
Vertex *genGrid(int tile_count);
void interruptHandler(int signum);

int max_us = 0;
int total_us = 0;
int counter = 0;
int max_fire_count = 0;

int main() {
    std::signal(SIGINT, interruptHandler);

    int tile_count = 1000;
    Vertex *grid = genGrid(tile_count);
    startFire(grid, tile_count);

    
    while (true) {
        auto start = std::chrono::high_resolution_clock::now();
        int fire_count = updateGrid(grid, tile_count);
        if (fire_count > max_fire_count)
            max_fire_count = fire_count;
        auto end =std::chrono::high_resolution_clock::now();
        auto duration_us = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        auto duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(duration_us);
        if (counter % 60 == 0)
            std::cout << "(" << (float)fire_count/1000000.f << ")\t" << duration_ms.count()  << "ms\t" << duration_us.count() << "us" << std::endl;
        
        if (duration_us.count() > max_us) 
            max_us = duration_us.count();
        total_us += duration_us.count();
        counter++;
        if (fire_count == 0)
            raise(SIGINT);

    }
}


void startFire(Vertex *grid, int tile_count) {
    int start_index = getGridIndex(tile_count / 2, tile_count/2, tile_count);
    // int start_index = vertex_count - 6;
    for (int i = 0; i < 6; i++) {
        grid[start_index+i].col[1] = 0;
        grid[start_index+i].col[0] = 1;
    }
}

int updateGrid(Vertex *grid, int tile_count) {
    float fire_source_fuel;
    float SCALE_FACTOR = 1.f/5.f;
    int fire_count = 0;
    for (int i = 0; i < tile_count; i++) {
        int curRow = i*tile_count*2*3;
        for (int j = 0; j < tile_count; j++) {
            float odds;
            int src_index = curRow + j*6;
            if ((fire_source_fuel = grid[src_index].col[0]) == 0)
                continue;
            fire_count++;
            // Left
            if (j > 0) {
                int target_index = getGridIndex(i, j-1, tile_count);
                odds = SCALE_FACTOR;
                // std::cout << "(" << i << ", " << j-1 << "): " << odds << std::endl;
                if (((float)rand())/((float)RAND_MAX) < odds) {
                    for (int v = 0; v < 6; v++) {
                        int vertex_index = target_index + v;
                        grid[vertex_index].col[0] = grid[vertex_index].col[1];
                        grid[vertex_index].col[1] = 0;
                    }
                }
            }
            // Right
            if (j < tile_count -1) {
                int target_index = getGridIndex(i, j+1, tile_count);
                odds = SCALE_FACTOR;
                if (((float)rand())/((float)RAND_MAX) < odds) {
                    for (int v = 0; v < 6; v++) {
                        int vertex_index = target_index + v;
                        grid[vertex_index].col[0] = grid[vertex_index].col[1];
                        grid[vertex_index].col[1] = 0;
                    }
                }
            }
            // Down
            if (i > 0) {
                int target_index = getGridIndex(i-1, j, tile_count);
                odds = SCALE_FACTOR;
                if (((float)rand())/((float)RAND_MAX) < odds) {
                    for (int v = 0; v < 6; v++) {
                        int vertex_index = target_index + v;
                        grid[vertex_index].col[0] = grid[vertex_index].col[1];
                        grid[vertex_index].col[1] = 0;
                    }
                }
            }
            // Up
            if (i < tile_count-1) {
                int target_index = getGridIndex(i+1, j, tile_count);
                odds = SCALE_FACTOR;
                if (((float)rand())/((float)RAND_MAX) < odds) {
                    for (int v = 0; v < 6; v++) {
                        int vertex_index = target_index + v;
                        grid[vertex_index].col[0] = grid[vertex_index].col[1];
                        grid[vertex_index].col[1] = 0;
                    }
                }
            }
            for (int v = 0; v < 6; v++) {
                grid[src_index+v].col[0] -= 0.005;
                if (grid[src_index].col[0] < 0) {
                    grid[src_index+v].col[0] = 0;
                }
            }
            
            
            

        }

    }
    return fire_count;
}
inline int getGridIndex(int i, int j, int tile_count) {
    return i*tile_count*2*3+ j*6;
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

void interruptHandler(int signum) {
    int average_us = total_us / counter;
    int average_ms = average_us / 1000;
    std::cout << "\nAv: " << average_ms << "ms\t" << average_us << "us" << std::endl;
    std::cout << "Max: " << max_us/1000 << "ms\t" << max_us << "us" << std::endl;
    std::cout << "FCount: " << max_fire_count << std::endl;
    exit(signum);
}