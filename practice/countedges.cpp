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

    long long edges = 0;
    while(1){
    	cout << "---------------------------" << endl;
    	long double x;
    	cout << "Enter a new point: ";
		cin >> x;
		if(order.size() == N){
			long double y = order.front();
			order.pop();
			auto pos = std::distance(points.begin(), points.find(y));
			auto yl = points.lower_bound(y - delta);
			auto yr = points.upper_bound(y + delta);
			int posl = std::distance(points.begin(), yl);
			int posr = std::distance(points.begin(), yr) - 1;
			edges -= posr - posl;
			auto it = points.find(y);
			points.erase(it);
		}
		order.push(x);
		auto xl = points.lower_bound(x - delta);
		auto xr = points.upper_bound(x + delta);
		if(xl == xr){
			points.insert(x);
		}else{
			xr--;
			int posl = std::distance(points.begin(), xl);
			int posr = std::distance(points.begin(), xr);
			edges += posr - posl + 1;
			points.insert(x);
		}
		cout << "Edges count: " << edges << endl;
		cout << "Array: ";
		for (auto p : points) {
	        cout << p << " ";
	    }
	    cout << endl;
    }
    return 0;
}