#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <random>
#include <fstream>
#include <cmath>

using namespace std;

mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
uniform_real_distribution <long double> uni(0, 1);
exponential_distribution <long double> expon(1);
normal_distribution <long double> norm(0, 1);

void gen_uni(vector <long double> &a){
    for(int j=0;j<a.size();j++){
        a[j] = uni(rng);
    }
    sort(a.begin(), a.end());
}

void gen_exp_log(vector <long double> &a){
    for(int j=0;j<a.size();j++){
        a[j] = -logl(uni(rng));
    }
    sort(a.begin(), a.end());
}

void gen_exp(vector <long double> &a){
    for(int j=0;j<a.size();j++){
        a[j] = expon(rng);
    }
    sort(a.begin(), a.end());
}

void gen_norm(vector <long double> &a){
    for(int j=0;j<a.size();j++){
        a[j] = norm(rng);
    }
    sort(a.begin(), a.end());
}

long double choose(long double n, long double k){
    if(k > n)   return 0;
    long double result = 1;
    for(long long i=n-k+1;i<=n;i++)   result*=1.0*i;
    for(long long i=2;i<=k;i++)       result/=1.0*i;
    return result;
}

long long reps = 10;
long long N = 1000;
long double delta = 1e-3;
long long distrib = 0;
long long click_size = 3;
ofstream out("result.txt");

int main(int argc, char* argv[]){ // N delta reps click distrib
    cout.precision(20);
    if(argc > 1){
        N = stod(argv[1]);
        delta = stod(argv[2]);
        reps = stod(argv[3]);
        click_size = stod(argv[4]);
        distrib = stod(argv[5]);
    }
    vector <long double> results(reps);
    vector <long double> numbers(N);
    long long step = max(reps/100, 1ll);
    for(long long t=1;t<=reps;t++){
        if(t%step == 0) cout << t/step << endl;
        switch (distrib){
            case 2:
                gen_norm(numbers);
                break;
            case 1:
                gen_exp(numbers);
                break;
            case 0:
                gen_uni(numbers);
                break;
        }
        long long res = 0;
        long long j = 0;
        for(long long i=0;i<numbers.size();i++){
            while(j < numbers.size() - 1 && numbers[j+1]*1.0 <= numbers[i] + delta)   j++;
            res += choose(j-i, click_size - 1);
        }
        //results[t] = res;
        out << res << ' ';
    }
    // long double sum = 0;
    // long double sq = 0;
    // for(long long i=0;i<=reps;i++) sum+=results[i];
    // for(long long i=0;i<=reps;i++) sq+=results[i]*results[i];
    // long double E = sum*1.0/reps;
    // long double D = sq*1.0/reps - E*E;
    //out << E << ' ' << D << endl;
}
