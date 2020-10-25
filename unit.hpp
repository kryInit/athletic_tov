#ifndef UNIT_HPP
#define UNIT_HPP

#include "vec3.hpp"

struct Unit{
    Vec3 p;
    int id;
    Unit() = default;
    Unit(Vec3 _p, int _id)
            : p(_p)
            , id(_id) {}
};

#endif
