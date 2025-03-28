#pragma once
// Typedefs
typedef unsigned short TileType;
typedef struct Tile
{
    bool onFire;
    short int fuel;
    TileType tileType;
} Tile;
// Types of tiles
const TileType TILE_GRASS = 0;
// Tile properties
const TileType BURNRATE_TABLE[1] = {1};

