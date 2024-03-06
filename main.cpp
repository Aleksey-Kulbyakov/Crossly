#include "core/settings.h"
#include "core/polygon.hpp"
#include "SFML/Graphics.hpp"
#include <vector>
#include <iostream>
#include <sstream>


std::vector<Polygon> polygons;

int main() {
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), TITLE);

    // Grid
    sf::RectangleShape cell(sf::Vector2f(GRID_SIZE, GRID_SIZE));
    cell.setOutlineThickness(GRID_THICKNESS);
    cell.setOutlineColor(GRID_OUTLINE_COLOR);
    cell.setFillColor(GRID_FILL_COLOR);
    // Point
    sf::CircleShape point(POINT_RADIUS);
    point.setOutlineThickness(POINT_THICKNESS);
    point.setOutlineColor(POINT_OUTLINE_COLOR);
    point.setFillColor(POINT_FILL_COLOR);




    while (window.isOpen())
    {
        window.clear();
        sf::Event event{};

        // Draw a grid
        for (int x = 0; x < window.getSize().x; x += GRID_SIZE)
        {
            for (int y = 0; y < window.getSize().y; y += GRID_SIZE)
            {
                cell.setPosition(sf::Vector2f(x, y));
                window.draw(cell);
            }
        }
        // Draw a pointer under cursor
        cell.setPosition(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y);
        window.draw(point);

        // Debug info
        sf::Text debug_mouse_pos_x, debug_mouse_pos_y;
        sf::Font font;
        font.loadFromFile("../fonts/Inter/InterDisplay-Medium.otf");



        debug_mouse_pos_x.setPosition(0, 0);
        debug_mouse_pos_y.setPosition(0, 24);
        debug_mouse_pos_x.setFont(font);
        debug_mouse_pos_y.setFont(font);


        std::stringstream debug_text;

        debug_text << "Mouse X: " << sf::Mouse::getPosition(window).x << '\n';
        debug_mouse_pos_x.setString(debug_text.str());
        debug_text.clear();

        debug_text << "Mouse Y: " << sf::Mouse::getPosition(window).y << '\n';
        debug_mouse_pos_x.setString(debug_text.str());
        debug_text.clear();

        debug_mouse_pos_x.setCharacterSize(24);
        debug_mouse_pos_y.setCharacterSize(24);
        debug_mouse_pos_x.setFillColor(sf::Color::Black);
        debug_mouse_pos_y.setFillColor(sf::Color::Black);

        window.draw(debug_mouse_pos_x);
        window.draw(debug_mouse_pos_y);

        while (window.pollEvent(event))
        {
            switch (event.type) {
                case sf::Event::Closed:
                {
                    window.close();
                    return 0;
                }
                case sf::Event::MouseButtonPressed:
                {
                    point.setPosition(event.mouseButton.x,event.mouseButton.y);
                    std::cout << "Point on " << event.mouseButton.x << " " << event.mouseButton.y << '\n';
                    window.draw(point);
                }
                default:
                    ;
            }
        }






        window.display();
    }

    return 0;
}