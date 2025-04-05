#include "grid.h"

int main() {
    // Initiate grids
    std::vector<Tile *> newGrid, oldGrid;
    std::unordered_set<int> updateList, fireList;
    initiateGrid(newGrid, oldGrid);

    startFire(newGrid, fireList);
    updateList = fireList;
    updateOldGrid(newGrid, oldGrid, updateList);

    std::cout << "New Grid: ";
    printGrid(newGrid);


    while (true) {
        updateGrid(newGrid, oldGrid, updateList, fireList);
        printGrid(newGrid);
        sleep(1);
    }
    // printTile(newGrid[TILE_ROW_COUNT/2+(TILE_ROW_COUNT/2)*TILE_ROW_COUNT]);
    // printTile(newGrid[TILE_ROW_COUNT/2+(TILE_ROW_COUNT/2)*TILE_ROW_COUNT-1]);
    
    // delete &oldGrid;
    return 0;
}   