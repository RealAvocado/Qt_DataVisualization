#ifndef POINT_H
#define POINT_H


class Point
{
public:
    double x; //key
    double y; //value
    Point();
    Point(double x, double y);

    static bool sortByXCoordinate(Point &point_a, Point &point_b);
};

#endif // POINT_H
