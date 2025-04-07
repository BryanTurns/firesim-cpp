#pragma once
// Includes
#include <stdlib.h>
#include <iostream>
#include <chrono>
#include <random>
#include <array>
#include <vector>
#include <string>
#include <unistd.h>
#include "settings.h"
#include <unordered_set>
// Typedefs
typedef unsigned short TileType;
// MUST CHANGE WHEN ADDING TILE TYPES
const int TILE_TYPE_COUNT = 1;
// Tile types
const TileType TILE_GRASS = 0;  
// Structs
typedef struct Tile
{
    bool onFire;
    float fuel;
    TileType tileType;
    Tile() : onFire(false), fuel(0.5), tileType(TILE_GRASS) {}
} Tile;
// Tile properties
const std::array<std::string, TILE_TYPE_COUNT> TILE_NAME_TABLE = {"grass"};
const std::array<float, TILE_TYPE_COUNT> TILE_BURNRATE_TABLE = {0.005};
// Functions
void initiateGrid(std::array<Tile, TILE_COUNT>& newGrid, std::array<Tile, TILE_COUNT>& oldGrid);
void startFire(std::array<Tile, TILE_COUNT>& grid, std::unordered_set<int>& fireList);
void updateGrid(std::array<Tile, TILE_COUNT>& newGrid, std::array<Tile, TILE_COUNT>& oldGrid, std::unordered_set<int>& updateList, std::unordered_set<int>& fireList);
void updateOldGrid(std::array<Tile, TILE_COUNT>& newGrid, std::array<Tile, TILE_COUNT>& oldGrid, std::unordered_set<int>& updateList);
bool checkIfFireLit();
void printTile(Tile *tile);
void printGrid(std::array<Tile, TILE_COUNT>& grid);