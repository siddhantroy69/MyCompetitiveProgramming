/*************************************************************************
 *                                                                       *
 *                    XVIII Olimpiada Informatyczna                      *
 *                                                                       *
 *   Zadanie:           Wykres                                           *
 *   Autor:             Filip Wolski                                     *
 *   Opis:              Rozwiazanie powolne                              *
 *                      kolo pokrywajace szesciennie,                    *
 *                      wyszukiwanie przyrostowe                         *
 *                                                                       *
 *************************************************************************/

#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdio>
#include <cstdlib>

#define RMIN 0
#define RMAX 1420000

#define MXN 100000
#define MXM 100000
#define EPS 1e-8

using namespace std;

typedef long double LD;

struct wsp_t {
	LD x, y;
	wsp_t(LD _x = 0., LD _y = 0.) : x(_x), y(_y) { }
};

inline LD sqr(LD x) { return x*x; }

wsp_t wsp[MXN];

bool fits(const wsp_t &p, LD x, LD y, LD r) {
	x -= p.x; y -= p.y;
	LD dist2 = x*x + y*y;
	return sqrtl(dist2) <= r + EPS;
}

bool can_hold(int pc, int kn, LD r, wsp_t &mid) {
	bool anyone = false;
	for (int i = pc; i <= kn; ++i) {
		for (int j = pc; j <= kn; ++j) {
			if (wsp[i].x != wsp[j].x || wsp[i].y != wsp[j].y) {
				bool ok = true;
				anyone = true;
				LD x = (wsp[i].x + wsp[j].x) / 2.;
				LD y = (wsp[i].y + wsp[j].y) / 2.;
				LD dx = (wsp[j].y - wsp[i].y) / 2.;
				LD dy = (wsp[i].x - wsp[j].x) / 2.;
				LD d = sqrtl(dx * dx + dy * dy);
				if (d > r) continue;
				LD h = sqrtl(r*r - d*d);
				x += dx * h / d;
				y += dy * h / d;
				for (int k = pc; k <= kn && ok; ++k) {
					if (!fits(wsp[k], x, y, r)) {
						ok = false;
					}
				}
				if (ok) {
					mid.x = x; mid.y = y;
					return true;
				}
			}
		}
	}
	if (!anyone) {
		mid = wsp[pc];
		return true;
	}
	return false;
}

int n;

void check(int &pos, LD r, wsp_t &mid) {
	int next = pos+1;
	mid.x = wsp[pos].x;
	mid.y = wsp[pos].y;
	while (next < n && can_hold(pos, next, r, mid)) {
		++next;
	}
	pos = next;
}

wsp_t mid[MXM], midsol[MXM];
int m, s, ssol;

int main() {
	LD rmin = RMIN;
	LD rmax = RMAX;
	scanf("%d%d", &n, &m);
	for (int i = 0; i < n; ++i) {
		scanf("%Lf%Lf", &wsp[i].x, &wsp[i].y);
	}
	ssol = -1;
	while (rmin + EPS < rmax) {
		LD r = (rmin + rmax) / 2.;
		int cnt = 0;
		for (s = 0; s < m && cnt < n; ++s) {
			check(cnt, r, mid[s]);
		}
		if (cnt == n) {
			ssol = s;
			copy(mid, mid+s, midsol);
			rmax = r;
		} else {
			rmin = r;
		}
	}
	assert(ssol != -1 && ssol <= m);
	printf("%.8Lf\n%d\n", rmax, ssol);
	for (int i = 0; i < ssol; ++i) {
		printf("%.8Lf %.8Lf\n", midsol[i].x, midsol[i].y);
	}
	return 0;
}
