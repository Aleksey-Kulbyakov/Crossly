#include <vector>
#include <cmath>
#include <algorithm>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "settings.h"


bool isPointInsidePolygon(std::vector<sf::CircleShape>& points, sf::CircleShape& c) {
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
    double a1 = B.getPosition().y - A.getPosition().y;
    double b1 = A.getPosition().x - B.getPosition().x;
    double c1 = a1 * A.getPosition().x + b1 * A.getPosition().y;

    double a2 = D.getPosition().y - C.getPosition().y;
    double b2 = C.getPosition().x - D.getPosition().x;
    double c2 = a2 * C.getPosition().x + b2 * C.getPosition().y;

    double delta = a1 * b2 - a2 * b1;
    if (std::fabs(delta) < 1e-9) {
        return {false, sf::CircleShape()};
    }

    double x = (b2 * c1 - b1 * c2) / delta;
    double y = (a1 * c2 - a2 * c1) / delta;

    if (x < std::min(A.getPosition().x, B.getPosition().x) || x > std::max(A.getPosition().x, B.getPosition().x) || x < std::min(C.getPosition().x, D.getPosition().x) || x > std::max(C.getPosition().x, D.getPosition().x) ||
        y < std::min(A.getPosition().y, B.getPosition().y) || y > std::max(A.getPosition().y, B.getPosition().y) || y < std::min(C.getPosition().y, D.getPosition().y) || y > std::max(C.getPosition().y, D.getPosition().y)) {
        return {false, sf::CircleShape()};
    }

    sf::CircleShape ret_point(ACTIVE_POINT_RADIUS);
    ret_point.setPosition(x, y);
    return {true, ret_point};
}

std::vector<sf::CircleShape> findIntersections(std::vector<sf::CircleShape>& polygon1, std::vector<sf::CircleShape>& polygon2) {
    std::vector<sf::CircleShape> intersections;

    for (int i = 0; i < polygon1.size(); ++i) {
        for (int j = 0; j < polygon2.size(); ++j) {
            auto result = findIntersection(polygon1[i], polygon1[(i + 1) % polygon1.size()], polygon2[j], polygon2[(j + 1) % polygon2.size()]);
            if (result.first) {
                bool duplicate = false;
                for (const auto& point : intersections) {
                    if (std::fabs(point.getPosition().x - result.second.getPosition().x) < 1e-9 && std::fabs(point.getPosition().y - result.second.getPosition().y) < 1e-9) {
                        duplicate = true;
                        break;
                    }
                }
                if (!duplicate)
                    intersections.push_back(result.second);
            }
        }
    }

    for (auto& vertex : polygon1) {
        if (isPointInsidePolygon(polygon2, vertex)) {
            bool duplicate = false;
            for (const auto& point : intersections) {
                if (std::fabs(point.getPosition().x - vertex.getPosition().x) < 1e-9 && std::fabs(point.getPosition().y - vertex.getPosition().y) < 1e-9) {
                    duplicate = true;
                    break;
                }
            }
            if (!duplicate)
                intersections.push_back(vertex);
        }
    }
    for (auto& vertex : polygon2) {
        if (isPointInsidePolygon(polygon1, vertex)) {
            bool duplicate = false;
            for (const auto& point : intersections) {
                if (std::fabs(point.getPosition().x - vertex.getPosition().x) < 1e-9 && std::fabs(point.getPosition().y - vertex.getPosition().y) < 1e-9) {
                    duplicate = true;
                    break;
                }
            }
            if (!duplicate)
                intersections.push_back(vertex);
        }
    }
    return intersections;
}


sf::CircleShape Centroid(const std::vector<sf::CircleShape>& points) {
    sf::CircleShape centroid;
    double sumX = 0, sumY = 0;
    for (const auto& p : points) {
        sumX += p.getPosition().x;
        sumY += p.getPosition().y;
    }
    centroid.setPosition(sumX / points.size(), sumY / points.size());
    return centroid;
}

double calculateAngle(const sf::CircleShape& centroid, const sf::CircleShape& point) {
    return atan2(point.getPosition().y - centroid.getPosition().y, point.getPosition().x - centroid.getPosition().x);
}

void sortPoints(std::vector<sf::CircleShape>& points) {
    sf::CircleShape centroid = Centroid(points);
    std::sort(points.begin(), points.end(), [&centroid](const sf::CircleShape& a, const sf::CircleShape& b) {
        return calculateAngle(centroid, a) < calculateAngle(centroid, b);
    });
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

//void general_inter() {
//    std::vector<std::vector<sf::CircleShape>> polygons = {
//            {{-4,-4}, {-3, 6}, {5, 6}},
//            {{-2, 3}, {4, 9}, {4, 2}},
//            {{1, -3}, {1, 7}, {7, 4},{7,0},
//                    {1, -3}, {1, 7}, {7, 4},{7,0}}
//    };
//
//    std::vector<sf::CircleShape> inter = findIntersections(polygons[0], polygons[1]);
//    sortVertex(inter);
//    for (int i = 2; i < polygons.size(); i++){
//        inter = findIntersections(inter, polygons[i]);
//        sortVertex(inter);
//    }
//    for (int i = 0; i < inter.size(); i++){
//        std::cout << inter[i].getPosition().x << ' ' << inter[i].getPosition().y << std::endl;
//    }
//}
//
//int main() {
//    general_inter();
//    return 0;
//}
