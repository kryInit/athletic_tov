#include "vec3.hpp"
#include "unit.hpp"
#include "utility.hpp"
#include <cmath>
#include <algorithm>

using namespace std;

bool containsP(Vec3 p[3], Vec3 x) {
    double s,t;
    Vec3 AB = p[1] - p[0], AC = p[2] - p[0], AP = x-p[0];
    if (abs(AB.x*AC.y - AB.y*AC.x) > allowableError) {
        double l = 1./(AB.x*AC.y - AB.y * AC.x);
        s = (AC.y*AP.x - AC.x*AP.y) * l;
        t = (AB.x*AP.y - AB.y*AP.x) * l;
//        if (abs(s*AB.z + t*AC.z - AP.z) > allowableError) return false;
    }
    else if (abs(AB.y*AC.z -  AB.z*AC.y) > allowableError) {
        double l = 1./(AB.y*AC.z - AB.z * AC.y);
        s = (AC.z*AP.y - AC.y*AP.z) * l;
        t = (AB.y*AP.z - AB.z*AP.y) * l;
//        if (abs(s*AB.z + t*AC.z - AP.z) > allowableError) return false;
    }
    else if (abs(AB.z*AC.x - AB.x*AC.z) > allowableError) {
        double l = 1./(AB.z*AC.x - AB.x * AC.z);
        s = (AC.x*AP.z - AC.z*AP.x) * l;
        t = (AB.z*AP.x - AB.x*AP.z) * l;
//        if (abs(s*AB.y + t*AC.y - AP.y) > allowableError) return false;
    } else {
        //三角形ではない
        return false;
    }
    return -allowableError <= s && s <= 1.0 + allowableError &&
           -allowableError <= t && t <= 1.0 + allowableError &&
           s + t <= 1.0 + allowableError;
}


void __usort(int depth, int l, int r, vector<Unit>& v);
void __serch(Vec3 C, double R, int depth, int l, int r, vector<Unit>& v, set<int> &ids);

void usort(vector<Unit>& v) {
    __usort(0, 0, v.size()-1, v);
}
void serch(Vec3 c, double r, vector<Unit>& v, set<int> &ids) {
    __serch(c, r, 0, 0, v.size()-1, v, ids);
}


inline bool check(Vec3 c, double r, Vec3 p) {
    return (c.x-r-allowableError <= p.x) && (p.x <= c.x+r+allowableError) &&
           (c.y-r-allowableError <= p.y) && (p.y <= c.y+r+allowableError) &&
           (c.z-r-allowableError <= p.z) && (p.z <= c.z+r+allowableError);
}

void __usort(int depth, int l, int r, vector<Unit>& v) {
    if (l >= r) return;
    if (depth == 0) {
        sort(v.begin()+l, v.begin()+r+1, [](const Unit& l, const Unit& r) { return l.p.x < r.p.x; });
        __usort(1, l, ((r-l)>>1)+l - 1, v);
        __usort(1, ((r-l)>>1)+l + 1, r, v);
    }
    if (depth == 1) {
        sort(v.begin()+l, v.begin()+r+1, [](const Unit& l, const Unit& r) { return l.p.y < r.p.y; });
        __usort(2, l, ((r-l)>>1)+l - 1, v);
        __usort(2, ((r-l)>>1)+l + 1, r, v);
    }
    if (depth == 2) {
        sort(v.begin()+l, v.begin()+r+1, [](const Unit& l, const Unit& r) { return l.p.z < r.p.z; });
        __usort(0, l, ((r-l)>>1)+l - 1, v);
        __usort(0, ((r-l)>>1)+l + 1, r, v);
    }
}

void __serch(Vec3 C, double R, int depth, int l, int r, vector<Unit>& v, set<int> &ids) {
    if (l > r) return;
    int c = ((r-l)>>1) + l;
    Unit tmp = v[c];

    if (check(C, R, tmp.p)) ids.insert(tmp.id);
    if (l == r) return;
    if (depth == 0) {
        if (C.x-R-allowableError <= tmp.p.x) __serch(C, R, 1, l, c-1, v, ids);
        if (tmp.p.x <= C.x+R+allowableError) __serch(C, R, 1, c+1, r, v, ids);
    }
    if (depth == 1) {
        if (C.y-R-allowableError <= tmp.p.y) __serch(C, R, 2, l, c-1, v, ids);
        if (tmp.p.y <= C.y+R+allowableError) __serch(C, R, 2, c+1, r, v, ids);
    }
    if (depth == 2) {
        if (C.z-R-allowableError <= tmp.p.z) __serch(C, R, 0, l, c-1, v, ids);
        if (tmp.p.z <= C.z+R+allowableError) __serch(C, R, 0, c+1, r, v, ids);
    }
}

