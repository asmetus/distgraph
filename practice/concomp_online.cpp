#include <iostream>
#include <random>
#include <ctime>
#include <set>
#include <queue>
#include <fstream>
using namespace std;


int main(int argc, char* argv[]){
	int N = 4;
    long double delta = 0.25;
    string filename = "numbers.txt";
    if(argc > 1){
        N = stod(argv[1]);
        delta = stod(argv[2]);
        filename = argv[3];
    }
    ifstream in("numbers/" + filename);

    multiset <long double> points;
    queue <long double> order;

    long double squares = 0;
    long double mean = 0;
    long double std = 0;

    long long components = 0;
    int i = 0;
    long double x;
    while(in >> x){
		i++;
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
		long double tmp = mean;
		mean += (components - tmp) / i;
		squares += (components - mean) * (components - tmp);
		if(i-1) std = sqrt(squares * 1.0 / (i - 1));

		if(components < mean - 3 * std || components > mean + 3 * std)	cout << i << ' ';
    }
    cout << endl;
    return 0;
}