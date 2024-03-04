#pragma once

#include <cstdint>
#include "SFML/Graphics.hpp"
#include "vector"

struct Polygon {
    uint32_t             num_of_dots;
    std::vector<sf::Vector2f> points;
};