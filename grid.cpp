#include "grid.h"
#include <iomanip>

void initiateGrid(std::vector<Tile *>& newGrid, std::vector<Tile *>& oldGrid) {
    srand(std::chrono::system_clock::now().time_since_epoch().count());
    for (int i = 0; i < TILE_COUNT; i++) {
        newGrid.push_back(new Tile);
        // newGrid[i]->fuel = ((((float)rand())/((float)RAND_MAX))/ 2.f) + 0.5f;
        newGrid[i]->fuel = 0.5;
        newGrid[i]->onFire = false;
        newGrid[i]->tileType = TILE_GRASS;
        oldGrid.push_back(new Tile);
        oldGrid[i]->fuel = newGrid[i]->fuel;
        oldGrid[i]->onFire = newGrid[i]->onFire;
        oldGrid[i]->tileType = newGrid[i]->tileType;

    }
    newGrid.shrink_to_fit();
    oldGrid.shrink_to_fit();
}

void printTile(Tile *tile) {
    std::cout << "Fuel: " << tile->fuel << std::endl;
    std::cout << "On Fire: " << tile->onFire << std::endl;
    std::cout << "Tile Type: " << TILE_NAME_TABLE[TILE_GRASS] << std::endl;
}
void printGrid(std::vector<Tile *>& grid) {
    std::cout << std::fixed;
    std::cout << std::setprecision(3);
    std::cout << std::endl;
    for (auto it = grid.begin(); it != grid.end(); ++it) {
        if ((it - grid.begin()) % TILE_ROW_COUNT == 0) 
            std::cout << "\033[0m" << std::endl;
        
        if ((*it)->onFire)
            std::cout << "\033[41m" << " " << (*it)->fuel << " ";
        else 
            std::cout << "\033[42m" << " " << (*it)->fuel << " ";
        
        std::cout << "\033[0m" << " ";
    }   
    std::cout << std::endl;
}
void startFire(std::vector<Tile *>& grid, std::vector<int>& fireList) {
    int fire_index = TILE_ROW_COUNT/2+(TILE_ROW_COUNT/2)*TILE_ROW_COUNT;
    grid[fire_index]->onFire = true;
    fireList.push_back(fire_index);
}
void updateGrid(std::vector<Tile*>& newGrid, std::vector<Tile*>& oldGrid, std::vector<int>& updateList, std::vector<int>& fireList) {
    int fire_index;
    std::vector<int> newFireList = fireList; 
    for (auto it = fireList.begin(); it != fireList.end(); ++it) {
        fire_index = *it;
        int row = fire_index / TILE_ROW_COUNT;
        int col = fire_index % TILE_ROW_COUNT;

        newGrid[fire_index]->fuel = oldGrid[fire_index]->fuel - TILE_BURNRATE_TABLE[oldGrid[fire_index]->tileType];
        // Handle whether fire is burnt out yet
        if (newGrid[fire_index]->fuel <= 0) {
            newGrid[fire_index]->fuel = 0;
            oldGrid[fire_index]->onFire = false;
            newFireList.erase(it);
        }
        updateList.push_back(fire_index);
        // If the fire is not on the left most position in the screen
        if (col != 0 && !oldGrid[fire_index-1]->onFire && oldGrid[fire_index]->fuel > 0 && checkIfFireLit()) {
            newGrid[fire_index-1]->onFire = true;
            newFireList.push_back(fire_index-1);
            updateList.push_back(fire_index-1);
        }
        // update adjacent tiles. Remember to do any conditions with old grid and any updating to new grid. Also add to update list anything new. 
    }
    fireList.swap(newFireList);
    updateOldGrid(newGrid, oldGrid, updateList);
}
bool checkIfFireLit() {
    float odds = 1;
    float roll =  ((float)rand())/((float)RAND_MAX);
    if(roll < odds) 
        return true;
    else 
        return false;
}
// void indexToRow(int index) {
//     return 
// }
void updateOldGrid(std::vector<Tile*>& newGrid, std::vector<Tile*>& oldGrid, std::vector<int>& updateList) {
    for (auto it = updateList.begin(); it != updateList.end(); ++it) {
        int updateIndex = *it;
        printTile(newGrid[updateIndex]);
        oldGrid[updateIndex]->fuel = newGrid[updateIndex]->fuel;
        oldGrid[updateIndex]->onFire = newGrid[updateIndex]->onFire;
        oldGrid[updateIndex]->tileType = newGrid[updateIndex]->tileType;
    }
    while (!updateList.empty()) 
        updateList.pop_back();
    printGrid(oldGrid);
}