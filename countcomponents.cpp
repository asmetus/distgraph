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

    set <long double> points;
    queue <long double> order;

    long long components = 0;
    while(1){
    	cout << "---------------------------" << endl;
    	long double x;
		cin >> x;
		if(order.size() == N){
			long double y = order.front();
			order.pop();
			auto it = points.find(y);
			auto pos = std::distance(points.begin(), it);
			long double yr = *(next(it, 1));
			long double yl = *(prev(it, 1));
			if((pos == 0 && yr - y > delta) || (pos == N-1 && y - yl > delta))	components--;
			else{
				if(yr - y > delta && y - yl > delta)							components--;
				else if(yr - y < delta && y - yl < delta && yr - yl > delta)	components++;
			}
			points.erase(y);
		}

		order.push(x);
		points.insert(x);
		auto it = points.find(x);
		auto pos = std::distance(points.begin(), it);
		long double xr = *(next(it, 1));
		long double xl = *(prev(it, 1));
		if(points.size() == 1){
			components++;
			continue;
		}
		if((pos == 0 && xr - x > delta) || (pos == points.size()-1 && x - xl > delta))	components++;
		else{
			if(xr - x > delta && x - xl > delta)							components++;
			else if(xr - x < delta && x - xl < delta && xr - xl > delta)	components--;
		}

		cout << components << endl;
		for (auto p : points) {
	        cout << p << " ";
	    }
	    cout << endl;
    }

    return 0;
}