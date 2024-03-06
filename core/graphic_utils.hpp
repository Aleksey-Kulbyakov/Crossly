#pragma once
#include "SFML/Graphics.hpp"
#include "settings.h"
#include "cmath"

sf::Vector2<int> link_to_grid(sf::Vector2<int>& pos) {
    int x = std::round((float)pos.x / GRID_SIZE) * GRID_SIZE;
    int y = std::round((float)pos.y / GRID_SIZE) * GRID_SIZE;
    return sf::Vector2<int>{x, y};
}
