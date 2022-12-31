#include "point.h"

Point::Point()
{

}

Point::Point(double x, double y)
{
    this->x=x;
    this->y=y;
}

bool Point::sortByXCoordinate(Point &point_a, Point &point_b)
{
    return point_a.x<point_b.x;
}
