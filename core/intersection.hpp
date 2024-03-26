#include <vector>
#include <cmath>
#include <algorithm>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <unordered_set>
#include "settings.h"

namespace std {
    template <>
    struct hash<std::pair<double, double>> {
        size_t operator()(const std::pair<double, double>& p) const {
            return std::hash<double>()(p.first) ^ std::hash<double>()(p.second);
        }
    };
}


bool isPointInsidePolygon(std::vector<sf::CircleShape>& points, const sf::CircleShape& c) {
    if (points.empty())
    {
        return false;
    }
    bool res = false;
    sf::CircleShape p1 = points[0];
    sf::CircleShape p2;
    for (int i = 1; i <= points.size(); i++) {
        p2 = points[i % points.size()];
        if (c.getPosition().y > std::min(p1.getPosition().y, p2.getPosition().y)) {
            if (c.getPosition().y <= std::max(p1.getPosition().y, p2.getPosition().y)) {
                if (c.getPosition().x <= std::max(p1.getPosition().x, p2.getPosition().x)) {
                    double x_intersection = (c.getPosition().y - p1.getPosition().y) * (p2.getPosition().x - p1.getPosition().x) / (p2.getPosition().y - p1.getPosition().y) + p1.getPosition().x;
                    if (p1.getPosition().x == p2.getPosition().x || c.getPosition().x <= x_intersection) {
                        res = !res;
                    }
                }
            }
        }

        p1 = p2;
    }
    return res;
}



std::pair<bool, sf::CircleShape> findIntersection(const sf::CircleShape& A, const sf::CircleShape& B, const sf::CircleShape& C, const sf::CircleShape& D) {

    sf::CircleShape ret_point;

    auto det = [](const sf::CircleShape& p1, const sf::CircleShape& p2) { return p1.getPosition().x * p2.getPosition().y - p1.getPosition().y * p2.getPosition().x; };
    auto isBetween = [](double a, double b, double c) { return std::min(a, b) <= c && c <= std::max(a, b); };

    sf::CircleShape AB;
    sf::CircleShape CD;
    CD.setPosition(D.getPosition().x - C.getPosition().x, D.getPosition().y - C.getPosition().y);
    AB.setPosition(B.getPosition().x - A.getPosition().x, B.getPosition().y - A.getPosition().y);

    double delta = det(AB, CD);

    if (std::fabs(delta) < 1e-9)
        return {false, ret_point}; // Нет пересечения

    sf::CircleShape AC;
    AC.setPosition(C.getPosition().x - A.getPosition().x, C.getPosition().y - A.getPosition().y);
    double t = det(AC, CD) / delta;
    double u = det(AC, AB) / delta;

    if (isBetween(0, 1, t) && isBetween(0, 1, u))
    {
        ret_point.setPosition(A.getPosition().x + t * AB.getPosition().x, A.getPosition().y + t * AB.getPosition().y);
        return {true, ret_point};
    }
    else
    {
        return {false, ret_point}; // Нет пересечения
    }
}


std::vector<sf::CircleShape> findIntersections(std::vector<sf::CircleShape>& polygon1, std::vector<sf::CircleShape>& polygon2) {
    std::vector<sf::CircleShape> intersections;
    std::unordered_set<std::pair<double, double>> intersectionSet;

    auto insertUnique = [&](const sf::CircleShape& point) {
        auto [x, y] = point.getPosition();
        intersectionSet.insert({x, y});
    };

    auto isDuplicate = [&](const sf::CircleShape& point) {
        auto [x, y] = point.getPosition();
        return intersectionSet.find({x, y}) != intersectionSet.end();
    };

    auto findAndInsertIntersection = [&](const sf::CircleShape& p1, const sf::CircleShape& p2, const sf::CircleShape& p3, const sf::CircleShape& p4) {
        auto result = findIntersection(p1, p2, p3, p4);
        if (result.first && !isDuplicate(result.second)) {
            intersections.push_back(result.second);
            insertUnique(result.second);
        }
    };

    for (size_t i = 0; i < polygon1.size(); ++i) {
        for (size_t j = 0; j < polygon2.size(); ++j) {
            findAndInsertIntersection(polygon1[i], polygon1[(i + 1) % polygon1.size()], polygon2[j], polygon2[(j + 1) % polygon2.size()]);
        }
    }

    for (const auto& vertex : polygon1) {
        if (isPointInsidePolygon(polygon2, vertex) && !isDuplicate(vertex)) {
            intersections.push_back(vertex);
            insertUnique(vertex);
        }
    }

    for (const auto& vertex : polygon2) {
        if (isPointInsidePolygon(polygon1, vertex) && !isDuplicate(vertex)) {
            intersections.push_back(vertex);
            insertUnique(vertex);
        }
    }

    return intersections;
}


bool comparePoints(sf::CircleShape a, sf::CircleShape b) {
    return (a.getPosition().x < b.getPosition().x) || (a.getPosition().x == b.getPosition().x && a.getPosition().y < b.getPosition().y);
}

int orientation(sf::CircleShape p, sf::CircleShape q, sf::CircleShape r) {
    int val = (q.getPosition().y - p.getPosition().y) * (r.getPosition().x - q.getPosition().x) - (q.getPosition().x - p.getPosition().x) * (r.getPosition().y - q.getPosition().y);
    if (val == 0) return 0;
    return (val > 0) ? 1 : 2;
}

void sortVertex(std::vector<sf::CircleShape>& points) {
    if (!points.empty())
    {
        std::sort(points.begin(), points.end(), comparePoints);

        sf::CircleShape p0 = points[0];
        std::sort(points.begin() + 1, points.end(), [p0](sf::CircleShape a, sf::CircleShape b) {
            int orient = orientation(p0, a, b);
            if (orient == 0) {
                return (a.getPosition().x + a.getPosition().y < b.getPosition().x + b.getPosition().y);
            }
            return (orient == 2);
        });
    }

}