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

int orientation(const Point& p, const Point& q, const Point& r) {
    float val = (q.y - p.y) * (r.x - q.x) -
              (q.x - p.x) * (r.y - q.y);
    if (abs(val) < FLOAT_EPS) return 0;  // colinear
    return (val > FLOAT_EPS) ? 1 : 2; // clock or counterclock wise
}


Point p0;

int compare_point(const void *ptr1, const void *ptr2) {
    Point *p1 = (Point *)ptr1;
    Point *p2 = (Point *)ptr2;
    // Find orientation
    int tmp = orientation(p0, *p1, *p2);
    if (tmp == 0)
        return (p2->get_instance_square(p0) - p1->get_instance_square(p0) >= FLOAT_EPS) ? -1 : 1;
    return (tmp == 2)? -1: 1;
}

Point next_to_top(vector<Point>& stack) {
    Point top = stack.back();
    stack.pop_back();
    Point next = stack.back();
    stack.push_back(top);
    return next;
}

// GRAHAM SCAN CONVEX HULL
vector<Point> convex_hull(vector<Point>& points) {
    vector<Point> stack;
    if (points.empty())
        return stack;
    size_t min_index = 0;
    for (size_t i = 1; i < points.size(); ++i) {
        if (points[i].y < points[min_index].y ||
            ((abs(points[i].y - points[min_index].y) < FLOAT_EPS) &&
            (points[i].x < points[min_index].x)))
            min_index = i;
    }
    swap(points[0], points[min_index]);
    p0 = points[0];
    qsort(&points[1], points.size()-1, sizeof(Point), compare_point);
    size_t m = 1;
    // Keep removing i while angle of i and i+1 is same with respect to p0
    for (size_t i = 1; i < points.size(); ++i) {
        while (i < points.size() - 1 && orientation(p0, points[i], points[i+1]) == 0)
            i++;
        points[m++] = points[i];
    }
    if (m < 3) return stack;
    stack.push_back(points[0]);
    stack.push_back(points[1]);
    stack.push_back(points[2]);
    for (size_t i = 3; i < m; ++i) {
        while (orientation(next_to_top(stack), stack.back(), points[i]) != 2)
            stack.pop_back();
        stack.push_back(points[i]);
    }
    return stack;
}

int main() {
    vector<Point> points = {{0, 3}, {1, 1}, {2, 2}, {4, 4},
                      {0, 0}, {1, 2}, {3, 1}, {3, 3}};
    auto hull = convex_hull(points);
    for (auto& v : hull) {
        cout << "(" << v.x << "," << v.y << ")" << endl;
    }
}