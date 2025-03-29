#include "grid.h"

void initiateGrid(std::vector<Tile *>& grid) {
    srand(std::chrono::system_clock::now().time_since_epoch().count());
    for (int i = 0; i < TILE_COUNT; i++) {
        grid.push_back(new Tile);
        grid[i]->fuel = ((((float)rand())/((float)RAND_MAX))/ 2.f) + 0.5f;
        grid[i]->onFire = false;
        grid[i]->tileType = TILE_GRASS;
    }
    grid.shrink_to_fit();
}

void printTile(Tile *tile) {
    std::cout << "Fuel: " << tile->fuel << std::endl;
    std::cout << "On Fire: " << tile->onFire << std::endl;
    std::cout << "Tile Type: " << TILE_NAME_TABLE[TILE_GRASS] << std::endl;
}