#include "core/settings.h"
#include "core/utils.hpp"
#include "core/intersection.hpp"
#include "SFML/Graphics.hpp"
#include <vector>
#include <iostream>
#include <sstream>


std::vector<std::vector<sf::CircleShape>> polygons;

int main() {
    // Initialize variables
    // -- Create window instance
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), TITLE);
    // -- Load icon file
    sf::Image app_icon;
    if (!app_icon.loadFromFile("../assets/images/icon.png"))
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
    enum mode {
        PolygonMaking,
        PolygonExtension,
        MovingPoint,
    };
    mode cur_mode = PolygonMaking;
    bool CURSOR_ON_POINT     = false;
    int  ON_POINT_PINDEX     = -1;
    int  ON_POINT_VINDEX     = -1;
    int  HELPER_POINT_PINDEX = -1;
    int  HELPER_POINT_VINDEX = -1;


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

        switch (cur_mode) {
            case PolygonExtension:
            {
                // Draw a line between cursor and root point
                sf::VertexArray line(sf::Lines, 2);

                line[0].position.x = polygons[HELPER_POINT_PINDEX][HELPER_POINT_VINDEX].getPosition().x;
                line[0].position.y = polygons[HELPER_POINT_PINDEX][HELPER_POINT_VINDEX].getPosition().y;

                line[1].position.x = sf::Mouse::getPosition(window).x;
                line[1].position.y = sf::Mouse::getPosition(window).y;

                window.draw(line);
                break;
            }
            case MovingPoint:
            {
                // Move on point to cursor cords
                polygons[ON_POINT_PINDEX][ON_POINT_VINDEX].setPosition(
                        sf::Mouse::getPosition(window).x,
                        sf::Mouse::getPosition(window).y);

            }
            default:
                ;
        }

        // Draw polygons
        CURSOR_ON_POINT = false;

        for (int polygon_index = 0; polygon_index < polygons.size(); polygon_index++){

            sf::ConvexShape convex;
            convex.setPointCount(polygons[polygon_index].size());
            sf::Vector2<int> mouse_pos  = sf::Mouse::getPosition(window);


            for (int point_index = 0; point_index < polygons[polygon_index].size(); point_index++) {
                sf::CircleShape    cur_point = polygons[polygon_index][point_index];
                sf::Vector2<float> point_pos = cur_point.getPosition();


                // Magnet to nearest point
                if (
                        squared_euclid_distance(mouse_pos.x, point_pos.x, mouse_pos.y, point_pos.y) <= GRID_SELECT_RADIUS*GRID_SELECT_RADIUS
                        and
                        !CURSOR_ON_POINT
                        )
                {
                    CURSOR_ON_POINT  = true;
                    ON_POINT_PINDEX  = polygon_index;
                    ON_POINT_VINDEX  = point_index;
                    cur_point.setFillColor(SELECTED_POINT_FILL_COLOR);
                }
                else
                {
                    if (point_index == 0 and HELPER_POINT_PINDEX == polygon_index)
                    {
                        modify_first_point(cur_point);
                    }
                    else
                    {
                        cur_point.setFillColor(POINT_FILL_COLOR);
                    }

                }

                convex.setPoint(point_index, cur_point.getPosition());
                cur_point.setPosition(cur_point.getPosition().x - POINT_RADIUS, cur_point.getPosition().y  - POINT_RADIUS);
                window.draw(cur_point);
            }
            convex.setFillColor(POLYGON_FILL_COLOR);
            convex.setOutlineThickness(POLYGON_THICKNESS);
            convex.setOutlineColor(POLYGON_OUTLINE_COLOR);
            if (polygon_index == HELPER_POINT_PINDEX) { continue;}
            window.draw(convex);

        }



        // Draw intersections
        if (cur_mode == PolygonMaking)
        {
            std::vector<std::vector<sf::CircleShape>> temp;
            for (int polygon_index = 0; polygon_index < polygons.size(); polygon_index++)
            {
                if (polygons[polygon_index].size() > 2 and polygon_index != HELPER_POINT_PINDEX)
                {
                    temp.push_back(polygons[polygon_index]);
                }
            }
            if (temp.size() > 1)
            {
                std::vector<sf::CircleShape> inter = findIntersections(temp[0], temp[1]);
                sortVertex(inter);
                for (int i = 2; i < temp.size(); i++){
                    inter = findIntersections(inter, temp[i]);
                    sortVertex(inter);
                }

                sf::ConvexShape intersection_convex;
                intersection_convex.setPointCount(inter.size());

                for (int i = 0; i < inter.size(); i++){
                    sf::CircleShape intersection_point = inter[i];
                    modify_active_point(intersection_point);
                    intersection_convex.setPoint(i, intersection_point.getPosition());
                    intersection_point.setPosition(
                            intersection_point.getPosition().x - POINT_RADIUS,
                            intersection_point.getPosition().y - POINT_RADIUS);
                    window.draw(intersection_point);

                }
                intersection_convex.setFillColor(CONVEX_FILL_COLOR);
                intersection_convex.setOutlineColor(CONVEX_OUTLINE_COLOR);
                intersection_convex.setOutlineThickness(CONVEX_THICKNESS);
                window.draw(intersection_convex);
            }

        }

        // Create a debug info
        if (DEBUG) {
            adjust_debug_text(debug_info, 5, FONT_SIZE*0, FONT_SIZE, font);
            std::stringstream string;
            string << "Mouse X: "                  << sf::Mouse::getPosition(window).x << '\n';
            string << "Mouse Y: "                  << sf::Mouse::getPosition(window).y << '\n';
            string << "Total number of polygons: " << polygons.size()                           << '\n';
            string << "Current mode is: "          << cur_mode                                  << '\n';
            if (CURSOR_ON_POINT)
            {
                string << "On point X: " << polygons[ON_POINT_PINDEX][ON_POINT_VINDEX].getPosition().x << '\n';
                string << "On point Y: " << polygons[ON_POINT_PINDEX][ON_POINT_VINDEX].getPosition().y << '\n';
            }
            debug_info.setString(string.str());
            string.clear();
            window.draw(debug_info);
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
                    // Case 1: Place point
                    if  ((cur_mode == PolygonMaking or cur_mode == PolygonExtension) and !CURSOR_ON_POINT) {
                        // Create new polygon
                        sf::CircleShape new_point(POINT_RADIUS);
                        modify_root_point(new_point);

                        sf::Vector2<int> mouse_pos = sf::Mouse::getPosition(window);
                        sf::Vector2<int> nearest_grid = link_to_grid(mouse_pos);

                        if (squared_euclid_distance(mouse_pos.x, nearest_grid.x, mouse_pos.y, nearest_grid.y) <=
                            GRID_LINKING_RADIUS * GRID_LINKING_RADIUS) {
                            new_point.setPosition(nearest_grid.x, nearest_grid.y);
                        } else {
                            new_point.setPosition(mouse_pos.x, mouse_pos.y);
                        }

                        if (cur_mode == PolygonMaking)
                        {
                            polygons.push_back({new_point});
                            HELPER_POINT_PINDEX = polygons.size() - 1;
                            HELPER_POINT_VINDEX = 0;
                        }
                        else if (cur_mode == PolygonExtension)
                        {
                            polygons[HELPER_POINT_PINDEX].push_back(new_point);
                            HELPER_POINT_VINDEX = polygons[HELPER_POINT_PINDEX].size() - 1;
                        }

                        cur_mode = PolygonExtension;
                    }
                    else if (cur_mode == PolygonExtension and CURSOR_ON_POINT)
                    {
                        if (HELPER_POINT_PINDEX == ON_POINT_PINDEX and ON_POINT_VINDEX == 0)
                        {
                            cur_mode = mode::PolygonMaking;
                            HELPER_POINT_PINDEX = -1;
                            HELPER_POINT_VINDEX = -1;
                        }
                    }
                    else if (cur_mode == MovingPoint)
                    {
                        cur_mode = PolygonMaking;
                    }
                    // Case 2: Removing point
                    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) && CURSOR_ON_POINT)
                    {
                        polygons[ON_POINT_PINDEX] = polygons.back();
                        polygons.pop_back();
                    }

                    // Case 3: Moving point
                    else if (CURSOR_ON_POINT)
                    {
                        cur_mode = MovingPoint;
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