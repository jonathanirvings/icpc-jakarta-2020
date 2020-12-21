#include<stdio.h>
#include<algorithm>
#include<set>

using namespace std;

#define data _data

struct dt{
	int x,y;
};

dt data[1000010];

int main(){
	int n;

	scanf("%d", &n);
	for(int i=0; i<n; i++){
		scanf("%d %d", &data[i].x, &data[i].y);
	}

	int minx = 10000000;
	int miny = 10000000;
	int maxx = -10000000;
	int maxy = -10000000;
	for(int i=0; i<n; i++){
		maxx = max(maxx, data[i].x);
		maxy = max(maxy, data[i].y);

		minx = min(minx, data[i].x);
		miny = min(miny, data[i].y);
	}

	int ansx = (maxx - minx + 1) / 2;
	int ansy = (maxy - miny + 1) / 2;

	printf("%d %d\n", ansx, ansy);

	return 0;
}
