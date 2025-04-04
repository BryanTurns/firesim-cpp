#include "grid.h"

int main() {
    // Initiate grids
    std::vector<Tile *> newGrid, oldGrid;
    std::vector<int> updateList, fireList;
    initiateGrid(newGrid, oldGrid);

    startFire(newGrid, fireList);
    updateList = fireList;
    updateOldGrid(newGrid, oldGrid, updateList);

    std::cout << "New Grid: ";
    printGrid(newGrid);
    std::cout << "Old Grid: ";
    printGrid(oldGrid);


    while (true) {
        updateGrid(newGrid, oldGrid, updateList, fireList);
        std::cout << "New Grid: ";
        printGrid(newGrid);
        std::cout << "Old Grid: ";
        printGrid(oldGrid);
        std::cout << "number of fires: " << fireList.size() << std::endl;
        sleep(3);
    }
    // printTile(newGrid[TILE_ROW_COUNT/2+(TILE_ROW_COUNT/2)*TILE_ROW_COUNT]);
    // printTile(newGrid[TILE_ROW_COUNT/2+(TILE_ROW_COUNT/2)*TILE_ROW_COUNT-1]);
    
    // delete &oldGrid;
    return 0;
}   