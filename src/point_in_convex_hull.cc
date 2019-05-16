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

bool onsegment(const Point& pi, const Point& pj, const Point& pk) {
    if (min(pi.x, pj.x) <= pk.x && max(pi.x, pj.x) >= pk.x && min(pi.y, pj.y) <= pk.y && max(pi.y, pj.y) >= pk.y)
        return true;
    return false;
}

// check point in convex hull
bool point_in_convex_hull(const std::vector<Point>& points, const Point& p) {
    size_t length = points.size();
    if(length < 3) return false;
 
    size_t i = 1;
    size_t j = length - 2;
    size_t mid;

    while(i <= j) {
        mid = (i + j) >> 1;
        float a1 = cross(points[0], points[mid], p);
        float a2 = cross(points[0], points[mid + 1], p);
        if(a1 > FLOAT_EPS && a2 < -FLOAT_EPS) {
            float a = cross(points[mid], points[mid+1], p);
            if(a > FLOAT_EPS || (abs(a) < FLOAT_EPS && onsegment(points[mid], points[mid + 1], p)))
                return true;
            return false;
        }
        else if (abs(a1) < FLOAT_EPS && mid == 1 && onsegment(points[0], points[mid], p))
            return true;
        else if (abs(a2) < FLOAT_EPS && mid + 1 == length - 1 && onsegment(points[0], points[mid + 1], p))
            return true;
        else if (a1 < -FLOAT_EPS)
            j = mid -1;
        else
            i = mid + 1;
    }
    return false;
}

int main() {
    vector<Point> points;
    points.push_back(Point(0, 0));
    points.push_back(Point(4, 0));
    points.push_back(Point(4, 4));
    Point p(2, 2);
    cout << point_in_convex_hull(points, p) << endl; // true
    p = Point(-1, -1);
    cout << point_in_convex_hull(points, p) << endl; // false
    p = Point(4, 2);
    cout << point_in_convex_hull(points, p) << endl; // true
    p = Point(0, 0);
    cout << point_in_convex_hull(points, p) << endl; // true
}