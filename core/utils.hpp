#pragma once
#include "SFML/Graphics.hpp"
#include "settings.h"
#include "cmath"

sf::Vector2<int> link_to_grid(sf::Vector2<int>& pos) {
    int x = std::round((float)pos.x / GRID_SIZE) * GRID_SIZE;
    int y = std::round((float)pos.y / GRID_SIZE) * GRID_SIZE;
    return sf::Vector2<int>{x, y};
}

sf::RectangleShape create_cell() {
    sf::RectangleShape cell  (sf::Vector2f(GRID_SIZE, GRID_SIZE));
    cell.setOutlineThickness (GRID_THICKNESS);
    cell.setOutlineColor     (GRID_OUTLINE_COLOR);
    cell.setFillColor        (GRID_FILL_COLOR);
    return cell;
}

sf::CircleShape create_point() {
    sf::CircleShape point     (POINT_RADIUS);
    point.setOutlineThickness (POINT_THICKNESS);
    point.setOutlineColor     (POINT_OUTLINE_COLOR);
    point.setFillColor        (POINT_FILL_COLOR);
    return point;
}

/*
 *
 *    DEBUG
 *
 * */

void adjust_debug_text(sf::Text& text_widget, int x, int y, int size, sf::Font& font) {
    text_widget.setPosition(x, y);
    text_widget.setFont(font);
    text_widget.setFillColor(sf::Color::Black);
    text_widget.setCharacterSize(size);
}