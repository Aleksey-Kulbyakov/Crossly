#include "SFML/Graphics.hpp"


float intersectionArea(std::vector<sf::CircleShape>& intersection)
{
    float area = 0;
    if (intersection.empty())
    {
        return area;
    }
    for (int i = 1; i < intersection.size(); i++)
    {
        area += intersection[i-1].getPosition().x*intersection[i].getPosition().y - intersection[i-1].getPosition().y*intersection[i].getPosition().x;
    }
    area += intersection.back().getPosition().x*intersection[0].getPosition().y - intersection.back().getPosition().y*intersection[0].getPosition().x;
    return 0.5*std::abs(area);
}