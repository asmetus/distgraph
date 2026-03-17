#include <iostream>
#include <random>
#include <ctime>
#include <set>
#include <fstream>
#include <queue>
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

    long long edges = 0;
    int i = 0;
    long double x;
    while(in >> x){
    	i++;
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
		long double tmp = mean;
		mean += (edges - tmp) / i;
		squares += (edges - mean) * (edges - tmp);
		if(i-1) std = sqrt(squares * 1.0 / (i - 1));

		if(edges < mean - 3 * std || edges > mean + 3 * std)	cout << i << ' ';
    }
    cout << endl;
    return 0;
}