#include "grid.h"
#include "graphics.h"
#include <iomanip>

void initiateGrid(std::array<Tile, TILE_COUNT>& newGrid, std::array<Tile, TILE_COUNT>& oldGrid) {
    srand(std::chrono::system_clock::now().time_since_epoch().count());
    for (int i = 0; i < TILE_COUNT; i++) {
        newGrid[i].fuel = ((((float)rand())/((float)RAND_MAX))/ 2.f) + 0.5f;
        // newGrid[i].fuel = 0.5;
        newGrid[i].onFire = false;
        newGrid[i].tileType = TILE_GRASS;
        oldGrid[i].fuel = newGrid[i].fuel;
        oldGrid[i].onFire = newGrid[i].onFire;
        oldGrid[i].tileType = newGrid[i].tileType;

    }
}

void printTile(Tile *tile) {
    std::cout << "Fuel: " << tile->fuel << std::endl;
    std::cout << "On Fire: " << tile->onFire << std::endl;
    std::cout << "Tile Type: " << TILE_NAME_TABLE[TILE_GRASS] << std::endl;
}
void printGrid(std::array<Tile, TILE_COUNT>& grid) {
    std::cout << std::fixed;
    std::cout << std::setprecision(3);
    std::cout << std::endl;
    for (auto it = grid.begin(); it != grid.end(); ++it) {
        if ((it - grid.begin()) % TILE_ROW_COUNT == 0) 
            std::cout << "\033[0m" << std::endl;
        
        if ((*it).onFire)
            std::cout << "\033[41m" << " " << (*it).fuel << " ";
        else 
            std::cout << "\033[42m" << " " << (*it).fuel << " ";
        
        std::cout << "\033[0m" << " ";
    }   
    std::cout << std::endl;
}
void startFire(std::array<Tile, TILE_COUNT>& grid, std::unordered_set<int>& fireList) {
    int fire_index = TILE_ROW_COUNT/2+(TILE_ROW_COUNT/2)*TILE_ROW_COUNT;
    grid[fire_index].onFire = true;
    fireList.insert(fire_index);
}
void updateGrid(std::array<Tile, TILE_COUNT>& newGrid, std::array<Tile, TILE_COUNT>& oldGrid, std::unordered_set<int>& updateList, std::unordered_set<int>& fireList) {
    int fire_index;
    std::unordered_set<int> newFireList = fireList; 
    for (auto it = fireList.begin(); it != fireList.end(); ++it) {
        fire_index = *it;
        int row = fire_index / TILE_ROW_COUNT;
        int col = fire_index % TILE_ROW_COUNT;

        newGrid[fire_index].fuel = oldGrid[fire_index].fuel - TILE_BURNRATE_TABLE[oldGrid[fire_index].tileType];
        // Handle whether fire is burnt out yet
        if (newGrid[fire_index].fuel <= 0) {
            newGrid[fire_index].fuel = 0;
            oldGrid[fire_index].onFire = false;
            newFireList.erase(*it);
        }
        updateList.insert(fire_index);
        // If the fire is not on the left most position in the screen
        int left_index = fire_index-1;
        if (col != 0 && !oldGrid[left_index].onFire && checkIfFireLit()) {
            newGrid[left_index].onFire = true;
            newFireList.insert(left_index);
            updateList.insert(left_index);
        }
        int up_index = fire_index - TILE_ROW_COUNT;
        if (row != 0 && !oldGrid[up_index].onFire && checkIfFireLit()) {
            newGrid[up_index].onFire = true;
            newFireList.insert(up_index);
            updateList.insert(up_index);
        }
        int right_index = fire_index + 1;
        if (col != TILE_ROW_COUNT - 1 && !oldGrid[right_index].onFire && checkIfFireLit()) {
            newGrid[right_index].onFire = true;
            newFireList.insert(right_index);
            updateList.insert(right_index);
        }
        int down_index = fire_index + TILE_ROW_COUNT ;
        if (row != TILE_ROW_COUNT - 1 && !oldGrid[right_index].onFire && checkIfFireLit()) {
            newGrid[down_index].onFire = true;
            newFireList.insert(down_index);
            updateList.insert(down_index);
        }
    }
    fireList = newFireList;
    updateOldGrid(newGrid, oldGrid, updateList);
}
bool checkIfFireLit() {
    float odds = 0.1;
    float roll =  ((float)rand())/((float)RAND_MAX);
    if(roll < odds) 
        return true;
    else 
        return false;
}
void updateOldGrid(std::array<Tile, TILE_COUNT>& newGrid, std::array<Tile, TILE_COUNT>& oldGrid, std::unordered_set<int>& updateList) {
    for (auto it = updateList.begin(); it != updateList.end(); ++it) {
        int updateIndex = *it;
        oldGrid[updateIndex].fuel = newGrid[updateIndex].fuel;
        oldGrid[updateIndex].onFire = newGrid[updateIndex].onFire;
        oldGrid[updateIndex].tileType = newGrid[updateIndex].tileType;
    }
}