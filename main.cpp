#include "core/settings.h"
#include "core/polygon.hpp"
#include "core/utils.hpp"
#include "SFML/Graphics.hpp"
#include <vector>
#include <iostream>
#include <sstream>


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
    // -- Create primitives
    auto cell = create_cell();
    auto point   = create_point();
    // -- Load main font for UI text
    sf::Font font; font.loadFromFile("../assets/fonts/Inter/InterDisplay-Medium.otf");
    // -- Debug info
    sf::Text debug_info;
    // -- Modes
    /* mode:
     * 0 - Create new polygon
     * 1 - Move point
     * 2 - Remove point
     * 3 - Line mode
     * */
    int mode = 0;
    sf::CircleShape POINT_HELPER;


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
                cell.setPosition(sf::Vector2f(x + 2, y + 2));
                window.draw(cell);
            }
        }

        switch (mode) {
            case 1:
            {
                // Move on point to cursor cords
                POINT_HELPER.setPosition(
                        sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y
                );
                std::cout << "Move point" << '\n';

            }
            case 3:
            {
                // Draw a line between cursor and root point
                sf::VertexArray line(sf::Lines, 2);
                line[0].position.x = POINT_HELPER.getPosition().x + POINT_RADIUS;
                line[0].position.y = POINT_HELPER.getPosition().y + POINT_RADIUS;


                line[1].position.x = sf::Mouse::getPosition(window).x;
                line[1].position.y = sf::Mouse::getPosition(window).y;

                window.draw(line);
            }
            default:
                ;
        }

        // Create a debug info
        adjust_debug_text(debug_info, 5, FONT_SIZE*0, FONT_SIZE, font);
        std::stringstream string;
        string << "Mouse X: "                  << sf::Mouse::getPosition(window).x << '\n';
        string << "Mouse Y: "                  << sf::Mouse::getPosition(window).y << '\n';
        string << "Total number of polygons: " << polygons.size()                           << '\n';
        string << "Current mode is: "          << mode                                      << '\n';
        debug_info.setString(string.str());
        string.clear();
        window.draw(debug_info);

        // Draw polygons
        bool CURSOR_ON_POINT  = false;
        int  ON_POLYGON_INDEX;
        int  ON_POINT_INDEX;

        for (int polygon_index = 0; polygon_index < polygons.size(); polygon_index++) {
            sf::ConvexShape convex;
            convex.setPointCount(polygons[polygon_index].num_of_dots);
            sf::Vector2<int> mouse_pos  = sf::Mouse::getPosition(window);

            for (int point_index = 0; point_index < polygons[polygon_index].num_of_dots; point_index++) {
                sf::CircleShape    cur_point = polygons[polygon_index].points[point_index];
                sf::Vector2<float> point_pos = cur_point.getPosition();

                if (
                        ((mouse_pos.x - point_pos.x)*(mouse_pos.x- point_pos.x)
                        +
                        (mouse_pos.y - point_pos.y)*(mouse_pos.y- point_pos.y)
                        <=
                        GRID_SELECT_RADIUS*GRID_SELECT_RADIUS )
                        && !CURSOR_ON_POINT) {
                    CURSOR_ON_POINT  = true;
                    ON_POINT_INDEX   = point_index;
                    ON_POLYGON_INDEX = polygon_index;
                    cur_point.setFillColor(SELECTED_POINT_FILL_COLOR);
                }
                else {
                    cur_point.setFillColor(POINT_FILL_COLOR);
                }

                convex.setPoint(point_index, cur_point.getPosition());
                window.draw(cur_point);
            }
            convex.setFillColor(CONVEX_FILL_COLOR);
            convex.setOutlineThickness(CONVEX_THICKNESS);
            convex.setOutlineColor(CONVEX_OUTLINE_COLOR);
            window.draw(convex);

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
                    // Case 1: Shift combo
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) && CURSOR_ON_POINT)
                    {
                        polygons[ON_POLYGON_INDEX].points[ON_POINT_INDEX] = polygons[ON_POLYGON_INDEX].points.back();
                        polygons[ON_POLYGON_INDEX].points.pop_back();
                        polygons[ON_POLYGON_INDEX].num_of_dots -= 1;
                        if (polygons[ON_POLYGON_INDEX].num_of_dots == 0) {
                            polygons[ON_POLYGON_INDEX] = polygons.back();
                            polygons.pop_back();
                        }
                        mode = 0;
                    }
                    // Case 2: Change on_point position
                    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) && CURSOR_ON_POINT)
                    {
                        mode = 1;
                        POINT_HELPER = polygons[ON_POLYGON_INDEX].points[ON_POINT_INDEX];
                    }
                    // Case 2: Draw a line
                    else if (CURSOR_ON_POINT)
                    {
                        mode = 3;
                        POINT_HELPER = polygons[ON_POLYGON_INDEX].points[ON_POINT_INDEX];
                    }


                    // Case 3: Adding a new dot
                    else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
                    {

                        // Create new polygon
                        sf::CircleShape root_point(POINT_RADIUS);
                        root_point.setOutlineThickness(POINT_THICKNESS);
                        root_point.setOutlineColor(POINT_OUTLINE_COLOR);
                        root_point.setFillColor(POINT_FILL_COLOR);

                        sf::Vector2<int> mouse_pos    = sf::Mouse::getPosition(window);
                        sf::Vector2<int> nearest_grid = link_to_grid(mouse_pos);

                        if (
                                (mouse_pos.x - nearest_grid.x)*(mouse_pos.x - nearest_grid.x)
                                +
                                (mouse_pos.y - nearest_grid.y)*(mouse_pos.y - nearest_grid.y)
                                <=
                                GRID_LINKING_RADIUS*GRID_LINKING_RADIUS
                                )
                        {
                            root_point.setPosition(nearest_grid.x - POINT_RADIUS, nearest_grid.y - POINT_RADIUS);
                        }
                        else {
                            root_point.setPosition(mouse_pos.x - POINT_RADIUS, mouse_pos.y - POINT_RADIUS);
                        }

                        std::vector<sf::CircleShape> points = {root_point};

                        if (mode == 3)
                        {
                            polygons[ON_POLYGON_INDEX].points.push_back(root_point);
                            polygons[ON_POLYGON_INDEX].num_of_dots += 1;
                        }
                        else
                        {
                            Polygon new_polygon {1, points};
                            polygons.push_back(new_polygon);
                            mode = 3;

                        }
                        POINT_HELPER = root_point;

                    }
                }
                case sf::Event::KeyPressed:
                {
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
                    {
                        mode = 0;
                    }
                }
                default:
                    ;
            }
        }
        window.display();
    }

    return 0;
}