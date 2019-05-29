/* The file is implementation of detecting convex hull collision by SAT.
   reference: https://en.wikipedia.org/wiki/Hyperplane_separation_theorem
*/

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

    float get_distance(const Point& p) const {
        return sqrt(get_instance_square(p));
    }

public:
    float x;
    float y;
};

vector<Point> get_axes(const vector<Point>& convex_hull)
{
    vector<Point> axes;
    for (size_t i = 0; i < convex_hull.size(); ++i) {
        size_t j = i + 1;
        if (j >= convex_hull.size())
            j = 0;
        const Point& p1 = convex_hull[i];
        const Point& p2 = convex_hull[j];
        float distance = p1.get_distance(p2);
        axes.push_back(Point((p2.y - p1.y)/distance, -(p2.x - p1.x)/distance));
    }
    return axes;
}

pair<float, float> get_projection(const vector<Point>& convex_hull, const Point& axe) {
    float min = convex_hull[0].x * axe.x + convex_hull[0].y * axe.y;
    float max = min;
    float projection;
    for (auto& p : convex_hull) {
        projection = p.x * axe.x + axe.y * p.y;
        if (projection < min)
            min = projection;
        else if (projection > max)
            max = projection;
    }
    return make_pair(min, max);
}

bool check_projection_overlap(const vector<Point>& A, const vector<Point>& B, const vector<Point>& axes) {
    for (auto& a : axes) {
        auto p_A = get_projection(A, a);
        auto p_B = get_projection(B, a);
        if (p_A.second < p_B.first || p_B.second < p_B.first)
            return false;
    }
    return true;
}

bool check_collision_of_convex_hull(const vector<Point>& A, const vector<Point>& B) {
    vector<Point> axesA = get_axes(A);
    vector<Point> axesB = get_axes(B);
    bool result;
    result = check_projection_overlap(A, B, axesA);
    if (!result)
        return false;
    result = check_projection_overlap(A, B, axesB);
    if (!result)
        return false;
    return true;
}

int main() {
    vector<Point> vec1;
    vec1.push_back(Point(0, 0));
    vec1.push_back(Point(1, 0));
    vec1.push_back(Point(1, 1));
    cout << "result:" << check_collision_of_convex_hull(vec1, vec1) << endl; // collision
    vector<Point> vec2;
    vec2.push_back(Point(0, 0));
    vec2.push_back(Point(2, 0));
    vec2.push_back(Point(2, 2));
    cout << "result:" << check_collision_of_convex_hull(vec1, vec2) << endl; // collision
    vec2.clear();
    vec2.push_back(Point(0, 0));
    vec2.push_back(Point(-1, 0));
    vec2.push_back(Point(-1, -1));
    cout << "result:" << check_collision_of_convex_hull(vec1, vec2) << endl; // collision
    vec2.clear();
    vec2.push_back(Point(-2, 0));
    vec2.push_back(Point(-1, 0));
    vec2.push_back(Point(-1, -1));
    cout << "result:" << check_collision_of_convex_hull(vec1, vec2) << endl; // no collision
    return 0;
}