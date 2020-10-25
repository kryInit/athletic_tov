#ifndef UTILITY_HPP
#define UTILITY_HPP

#include <vector>
#include <set>
#include <cmath>
#include "vec3.hpp"
#include "unit.hpp"

const int MIN_N = 1;
const int MAX_N = 200000;
const int MIN_Q = 1;
const int MAX_Q = 200000;
const double MIN_COORD = -10000;
const double MAX_COORD = 10000;
const double MIN_R = 0.1;
const float MAX_R = 500;
const double allowableError = 1e-9;

void usort(std::vector<Unit>& v);
void serch(Vec3 c, double r, std::vector<Unit>& v, std::set<int> &ids);

bool containsP(Vec3 p[3], Vec3 x);

inline double dotProduct(Vec3 v1, Vec3 v2) {
    return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
}
inline Vec3 crossProduct(Vec3 v1, Vec3 v2) {
    return {(v1.y*v2.z - v1.z*v2.y), (v1.z*v2.x - v1.x*v2.z), (v1.x*v2.y - v1.y*v2.x)};
}
inline bool containsL(Vec3 p[2], Vec3 x) {
    return (std::min(p[0].x, p[1].x) -allowableError <= x.x && x.x <= std::max(p[0].x, p[1].x) +allowableError &&
            std::min(p[0].y, p[1].y) -allowableError <= x.y && x.y <= std::max(p[0].y, p[1].y) +allowableError &&
            std::min(p[0].z, p[1].z) -allowableError <= x.z && x.z <= std::max(p[0].z, p[1].z) +allowableError);
}

#endif
