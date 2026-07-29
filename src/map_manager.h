#pragma once
#include <SFML/Graphics.hpp>

// Kích thước mỗi ô tile (pixel trên màn hình)
constexpr int TILE_SIZE   = 32;
// Kích thước map tính theo số ô
constexpr int MAP_WIDTH   = 40;
constexpr int MAP_HEIGHT  = 23;

// Dữ liệu map — 0 = không khí, 1 = snow_dirt_01, 2 = snow_dirt_02, ...
extern int mapData[MAP_HEIGHT][MAP_WIDTH];

void loadTilesets();
void drawMap(sf::RenderWindow& window);