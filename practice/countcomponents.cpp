#include <iostream>
#include <random>
#include <ctime>
#include <set>
#include <queue>
using namespace std;

int main(int argc, char* argv[]){
	int N = 4;
    long double delta = 0.25;
    if(argc > 1){
        N = stod(argv[1]);
        delta = stod(argv[2]);
    }

    multiset <long double> points;
    queue <long double> order;

    long long components = 0;
    while(1){
    	cout << "---------------------------" << endl;
    	long double x;
    	cout << "Enter a new point: ";
		cin >> x;
		if(order.size() == N){
			long double y = order.front();
			order.pop();
			auto it = points.find(y);
			auto pos = std::distance(points.begin(), it);
			if(pos == 0){
				long double yr = *(next(it, 1));
				if(yr - y > delta)	components--;
			}
			if(pos == N-1){
				long double yl = *(prev(it, 1));
				if(y - yl > delta)	components--;
			}
			if(pos != 0 && pos != N-1){
				long double yr = *(next(it, 1));
				long double yl = *(prev(it, 1));
				if(yr - y > delta && y - yl > delta)							components--;
				else if(yr - y <= delta && y - yl <= delta && yr - yl > delta)	components++;
			}
			points.erase(it);
		}
		order.push(x);
		points.insert(x);
		if(points.size() == 1){
			components++;
			continue;
		}
		auto it = points.find(x);
		auto pos = std::distance(points.begin(), it);
		if(pos == 0){
				long double xr = *(next(it, 1));
				if(xr - x > delta)	components++;
			}
			if(pos == N-1){
				long double xl = *(prev(it, 1));
				if(x - xl > delta)	components++;
			}
			if(pos != 0 && pos != N-1){
				long double xr = *(next(it, 1));
				long double xl = *(prev(it, 1));
				if(xr - x > delta && x - xl > delta)							components++;
				else if(xr - x <= delta && x - xl <= delta && xr - xl > delta)	components--;
			}
		cout << "Components count: " << components << endl;
		cout << "Array: ";
		for (auto p : points) {
	        cout << p << " ";
	    }
	    cout << endl;
    }

    return 0;
}