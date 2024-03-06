#pragma once
#include "SFML/Graphics.hpp"
#include <sstream>

void adjust_debug_text(sf::Text& text_widget, int x, int y, int size, sf::Font& font) {
    text_widget.setPosition(x, y);
    text_widget.setFont(font);
    text_widget.setFillColor(sf::Color::Black);
    text_widget.setCharacterSize(size);
}