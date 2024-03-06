#include "core/settings.h"
#include "core/polygon.hpp"
#include "core/debug.hpp"
#include "SFML/Graphics.hpp"
#include <vector>
#include <iostream>



std::vector<Polygon> polygons;

int main() {
    // Initialize variables
    // -- Create window instance
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), TITLE);
    // -- Load icon file
    sf::Image app_icon;
    if (!app_icon.loadFromFile("../assets/icons/icon.png"))
    {
        std::cerr << "Failed to load an icon" << '\n';
    }
    window.setIcon(app_icon.getSize().x, app_icon.getSize().y, app_icon.getPixelsPtr());
    // -- Create grid primitive
    sf::RectangleShape cell(sf::Vector2f(GRID_SIZE, GRID_SIZE));
    cell.setOutlineThickness(GRID_THICKNESS);
    cell.setOutlineColor(GRID_OUTLINE_COLOR);
    cell.setFillColor(GRID_FILL_COLOR);
    //  -- Create point primitive
    sf::CircleShape point(POINT_RADIUS);
    point.setOutlineThickness(POINT_THICKNESS);
    point.setOutlineColor(POINT_OUTLINE_COLOR);
    point.setFillColor(POINT_FILL_COLOR);
    // -- Load main font for UI text
    sf::Font font;
    font.loadFromFile("../assets/fonts/Inter/InterDisplay-Medium.otf");
    // -- Debug info
    sf::Text debug_info;
    // End of initialization

    // Start main loop
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
//        point.setPosition(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y);
//        window.draw(point);

        // Debug info
        adjust_debug_text(debug_info,     0, FONT_SIZE*0, FONT_SIZE, font);

        std::stringstream string;
        string << "Mouse X: " << sf::Mouse::getPosition(window).x << '\n';
        string << "Mouse Y: " << sf::Mouse::getPosition(window).y << '\n';
        string << "Total number of polygons: " << polygons.size() << '\n';
        debug_info.setString(string.str());
        string.clear();

        window.draw(debug_info);

        // Draw polygons
        for (int polygon_index = 0; polygon_index < polygons.size(); polygon_index++) {
            for (int point_index = 0; point_index < polygons[polygon_index].num_of_dots; point_index++) {
                // std::cout << "Polygon: " << polygon_index << " Point: " << point_index <<  '\n';
                window.draw(polygons[polygon_index].points[point_index]);
            }
        }


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
                    // Create new polygon
                    sf::CircleShape root_point(POINT_RADIUS);
                    root_point.setOutlineThickness(POINT_THICKNESS);
                    root_point.setOutlineColor(POINT_OUTLINE_COLOR);
                    root_point.setFillColor(POINT_FILL_COLOR);
                    root_point.setPosition(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y);

                    std::vector<sf::CircleShape> points = {root_point};
                    Polygon new_polygon {1, points};
                    polygons.push_back(new_polygon);
                }
                default:
                    ;
            }
        }
        window.display();
    }

    return 0;
}