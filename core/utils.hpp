#pragma once
#include "SFML/Graphics.hpp"
#include "settings.h"
#include "cmath"

double squared_euclid_distance(int x1, int x2, int y1, int y2)
{
    return (x1-x2)*(x1-x2) + (y1-y2)*(y1-y2);
}

sf::Vector2<int> link_to_grid(sf::Vector2<int>& pos)
{
    int x = std::round((float)pos.x / GRID_SIZE) * GRID_SIZE;
    int y = std::round((float)pos.y / GRID_SIZE) * GRID_SIZE;
    return sf::Vector2<int>{x, y};
}

sf::RectangleShape create_cell()
{
    sf::RectangleShape cell  (sf::Vector2f(GRID_SIZE, GRID_SIZE));
    cell.setOutlineThickness (GRID_THICKNESS);
    cell.setOutlineColor     (GRID_OUTLINE_COLOR);
    cell.setFillColor        (GRID_FILL_COLOR);
    return cell;
}

sf::CircleShape create_point()
{
    sf::CircleShape point     (POINT_RADIUS);
    point.setOutlineThickness (POINT_THICKNESS);
    point.setOutlineColor     (POINT_OUTLINE_COLOR);
    point.setFillColor        (POINT_FILL_COLOR);
    return point;
}

void modify_root_point(sf::CircleShape& point)
{
    point.setOutlineThickness(POINT_THICKNESS);
    point.setOutlineColor(POINT_OUTLINE_COLOR);
    point.setFillColor(POINT_FILL_COLOR);
}

void modify_first_point(sf::CircleShape& point)
{
    point.setOutlineThickness(FIRST_POINT_THICKNESS);
    point.setOutlineColor(FIRST_POINT_OUTLINE_COLOR);
    point.setFillColor(FIRST_POINT_FILL_COLOR);
    point.setRadius(FIRST_POINT_RADIUS);
}

void modify_active_point(sf::CircleShape& point)
{
    point.setFillColor(ACTIVE_POINT_FILL_COLOR);
    point.setOutlineColor(ACTIVE_POINT_OUTLINE_COLOR);
    point.setOutlineThickness(ACTIVE_POINT_THICKNESS);
    point.setRadius(ACTIVE_POINT_RADIUS);
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