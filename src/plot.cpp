
#include "plot.hpp"
#include <cmath>
#include <format>
#include <queue>
#include <string>

namespace plot
{

class Point
{
        Point(int r, int angle)
        {
                this->x = r * std::cos(angle);
                this->y = r * std::sin(angle);
        }

        int x;
        int y;

        std::string stringify() { return std::format("{} {}\r\n", this->x, this->y); }
};

void start(std::queue<Point> points) {}

} // namespace plot
