#pragma once
// Includes
#include <stdlib.h>
#include <iostream>
#include <chrono>
#include <random>
#include <array>
#include <vector>
#include <string>
#include "settings.h"
// Typedefs
typedef unsigned short TileType;
// MUST CHANGE WHEN ADDING TILE TYPES
const int numTileTypes = 1;
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
const std::array<std::string, numTileTypes> TILE_NAME_TABLE = {"grass"};
const std::array<float, numTileTypes> TILE_BURNRATE_TABLE = {1};
// Functions
void initiateGrid(std::vector<Tile *>& grid);
void printTile(Tile *tile);
