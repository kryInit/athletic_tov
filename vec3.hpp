#ifndef VEC3_HPP
#define VEC3_HPP

#include <cmath>


struct Vec3 {
    double x,y,z;
    Vec3() = default;
    Vec3(double _x, double _y, double _z)
            : x(_x)
            , y(_y)
            , z(_z) {}

    inline Vec3 operator -() const
    {
        return{ -x, -y, -z };
    }
    inline Vec3 operator -(const Vec3& other) const
    {
        return{ x - other.x, y - other.y, z - other.z };
    }
    inline Vec3 operator *(double s) const
    {
        return{ x*s, y*s, z*s };
    }
    inline Vec3 operator /(double s) const
    {
        return{ x/s, y/s, z/s };
    }

    inline double length2() {
        return x*x + y*y + z*z;
    }
    inline double length() {
        return sqrt(length2());
    }
};

#endif
