#pragma GCC target("avx2")
#pragma GCC optimize("O3")
#pragma GCC optimize("unroll-loops")
#pragma GCC target("sse,sse2,sse3,ssse3,sse4,popcnt,abm,mmx,avx,tune=native")

#include <iostream>
#include <algorithm>
#include <queue>
#include <cmath>
#include <set>

#include "vec3.hpp"
#include "unit.hpp"
#include "utility.hpp"

//#undef DEBUG

using namespace std;

#define rep(i,n) for(int i=0;i<(int)(n);++i)


bool isKillable(double r, Vec3 v, Vec3 p[3]);

int ans[MAX_Q];
pair<pair<double, Vec3>, int> aq[MAX_Q];

int main() {
    clock_t start = clock(), prev = clock();
    int n,q;
    scanf("%d %d", &n, &q);

    vector<Unit> u(n*3), mu(n*3);
    {
        Unit tmp;
        Unit *up = &u[0], *mup = &mu[0];
        for(tmp.id = 0; tmp.id < n; ++tmp.id) {
            scanf("%lf %lf %lf", &tmp.p.x, &tmp.p.y, &tmp.p.z);
            *up = *mup = tmp; ++up; ++mup;
            scanf("%lf %lf %lf", &tmp.p.x, &tmp.p.y, &tmp.p.z);
            *up = *mup = tmp; ++up; ++mup;
            scanf("%lf %lf %lf", &tmp.p.x, &tmp.p.y, &tmp.p.z);
            *up = *mup = tmp; ++up; ++mup;
        }
    }
#ifdef DEBUG
    cerr << "in: " << (double)(clock() - start)/CLOCKS_PER_SEC*1000 << "[ms]\t" << (double)(clock() - prev)/CLOCKS_PER_SEC*1000 << "[ms]" << endl; prev = clock();
#endif

    usort(u);

#ifdef DEBUG
    cerr << "sort: " << (double)(clock() - start)/CLOCKS_PER_SEC*1000 << "[ms]\t" << (double)(clock() - prev)/CLOCKS_PER_SEC*1000 << "[ms]" << endl; prev = clock();
#endif
    {
        pair<pair<double, Vec3>, int> tmp;
        for(tmp.second = 0; tmp.second < q; ++tmp.second) {
            scanf("%lf %lf %lf %lf", &tmp.first.first, &tmp.first.second.x, &tmp.first.second.y, &tmp.first.second.z);
            aq[tmp.second] = tmp;
        }
    }
    sort(aq, aq+q, [](const pair<pair<double, Vec3>, int>& l, const pair<pair<double, Vec3>, int>& r) { return l.first.first < r.first.first; });

    clock_t cc = clock();
    int th, tmpans;
    double r, aprsum=0;
    bool useTree=true;
    Vec3 v, p[3]={};

    rep(I,q) {
        th = aq[I].second;
        r = aq[I].first.first;
        v = aq[I].first.second;
        tmpans = 0;

#ifdef DEBUG
        if ((I+1)%2000 == 0) cerr << (I+1)/2000 << "%" << endl;
#endif

        if ((clock()-start) >= (int)(9.5*CLOCKS_PER_SEC)) {
#ifdef DEBUG
            cerr << aprsum/I*r << endl;
#endif
            ans[th] = min(n, (int)lround(aprsum/(I+1)*r));
            aprsum += ans[th]/r;
            continue;
        }

        if (useTree) {
            set<int> s;
            double R = max(max(r,25.), (r+25)*0.71);
            serch(v,R,u,s);
            for(auto i : s) {
                int _i = i*3;
                p[0] = mu[_i].p;
                p[1] = mu[_i+1].p;
                p[2] = mu[_i+2].p;
                if (isKillable(r, v, p)) tmpans++;
            }
            if (clock()-cc >= (int)(0.1*CLOCKS_PER_SEC)) useTree = false;
            cc = clock();
        } else {
            rep(i,n) {
                int _i = i*3;
                p[0] = mu[_i].p;
                p[1] = mu[_i+1].p;
                p[2] = mu[_i+2].p;
                if (isKillable(r, v, p)) tmpans++;
            }
        }
        ans[th] = tmpans;
        aprsum = ans[th]/r;
    }
    rep(i,q) printf("%d\n", ans[i]);
#ifdef DEBUG
    cerr << "end: " << (double)(clock() - start)/CLOCKS_PER_SEC*1000 << "[ms]\t" << (double)(clock() - prev)/CLOCKS_PER_SEC*1000 << "[ms]" << endl; prev = clock();
#endif
}

bool isKillable(double r, Vec3 v, Vec3 p[3]) {
    double rr = 1./(r*r);
    double mind=1, maxd=-1;
    rep(i,3) {
        p[i] = p[i] - v;
        double d = p[i].length()-r;
        mind = min(mind, d);
        maxd = max(maxd, d);
    }
    if (mind <= allowableError && -allowableError <= maxd) return true;
    if (allowableError <= mind) {
        {
            Vec3 AB = p[1] - p[0], AC = p[2] - p[0];
            Vec3 n = crossProduct(AB, AC);
            Vec3 w = n * (dotProduct(n, p[0])/n.length2());
            if (dotProduct(w, w-p[0]) > allowableError) w = -w;
            if (containsP(p, w) && w.length2()*rr <= 1 + allowableError) return true;
        }
        rep(_,3) {
            Vec3 A = p[0];
            Vec3 n = p[1] - p[0];
            Vec3 w = A;

            if (n.x != 0) {
                w.x += -n.x*dotProduct(n,A) / n.length2();
                w.y += n.y*(w.x-A.x) / n.x;
                w.z += n.z*(w.x-A.x) / n.x;
            } else if (n.y != 0) {
                w.y += -n.y*dotProduct(n,A) / n.length2();
                w.x += n.x*(w.y-A.y) / n.y;
                w.z += n.z*(w.y-A.y) / n.y;
            } else {
                w.z += -n.z*dotProduct(n,A) / n.length2();
                w.y += n.y*(w.z-A.z) / n.z;
                w.x += n.x*(w.z-A.z) / n.z;
            }

            if (containsL(p, w) && w.length2()*rr <= 1 + allowableError) return true;

            swap(p[1], p[2]);
            swap(p[0], p[1]);
        }
    }
    return false;
}


