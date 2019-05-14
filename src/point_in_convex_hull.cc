#include <vector>
#include <iostream>
#include<algorithm>
#include<cmath>

using namespace std;

#define FLOAT_EPS 1e-6

class Point {

public:
    Point(float x=0.0, float y=0.0): x(x), y(y) {}
    
    Point operator +(const Point& p) const {
        return Point(x + p.x, y + p.y);
    }

    Point operator -(const Point& p) const {
        return Point(x - p.x, y - p.y);
    }
    
    Point& operator =(const Point& p) {
        if (this == &p)
            return *this;
        x = p.x;
        y = p.y;
        return *this;
    }

    float get_instance_square(const Point& p) const {
        return (p.x - x) * (p.x - x) + (p.y -y) * (p.y -y);
    }

public:
    float x;
    float y;
};

float cross(const Point& p0, const Point& p1, const Point& p2) {
    return (p1.x - p0.x) * (p2.y - p0.y) - (p2.x - p0.x) * (p1.y - p0.y);
}
 
bool point_in_convex_hull(const std::vector<Point>& points, const Point& p) {
    size_t length = points.size();
    if(length < 3) return false;
 
    size_t i = 1;
    size_t j = length - 2;
    size_t mid;

    while(i <= j) {
        mid = (i + j) >> 1;
        double a1 = cross(points[0], points[mid], p);
        double a2 = cross(points[0], points[mid + 1], p);
        if(a1 > FLOAT_EPS && a2 < FLOAT_EPS) {
            if(cross(points[mid], points[mid+1], p) > FLOAT_EPS)
                return true;
            return false;
        }
        else if (a1 < FLOAT_EPS)
            j = mid -1;
        else
            i = mid + 1;
    }
    return false;
}

int main() {
    vector<Point> points;
    points.push_back(Point(0, 0));
    points.push_back(Point(3, 1));
    points.push_back(Point(4, 4));
    Point p(2, 2);
    cout << point_in_convex_hull(points, p) << endl;
    p = Point(-1, -1);
    cout << point_in_convex_hull(points, p) << endl;
    p = Point(0, 0);
    cout << point_in_convex_hull(points, p) << endl;
}