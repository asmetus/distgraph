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

double lap(double mu, double b) {
    double u = uni(rng) * 1.0 - 0.5;
    return mu - b * (u < 0 ? -1.0 : 1.0) * logl(1.0 - 2.0 * abs(u));
}

void gen_uni(vector <long double> &v, long double a, long double b){
    for(int j=0;j<v.size();j++){
        v[j] = uni(rng) * (b-a) + a;
    }
    sort(v.begin(), v.end());
}

void gen_exp(vector <long double> &a, long double lambda, long double nothing){
    long double adbmal = 1.0 / lambda;
    for(int j=0;j<a.size();j++){
        a[j] = expon(rng) * adbmal;
    }
    sort(a.begin(), a.end());
}

void gen_norm(vector <long double> &a, long double mu, long double std){
    for(int j=0;j<a.size();j++){
        a[j] = norm(rng) * std + mu;
    }
    sort(a.begin(), a.end());
}

void gen_lap(vector <long double> &a, long double mu, long double b){
    for(int j=0;j<a.size();j++){
        a[j] = lap(mu, b);
    }
    sort(a.begin(), a.end());
}

void gen_gam(vector <long double> &v, long double a, long double b){
    std::gamma_distribution<long double> gam(a, b);
    for(int j=0;j<v.size();j++){
        v[j] = gam(rng);
    }
    sort(v.begin(), v.end());
}

void gen_dnorm(vector <long double> &v, long double mu, long double std){
    for(int j=0;j<v.size();j++){
        v[j] = norm(rng) * std + (uni(rng) > 0.5?mu:-mu);
    }
    sort(v.begin(), v.end());
}

long double choose(long double n, long double k){
    if(k > n)   return 0;
    long long result = 1;
    for(long long i=n-k+1;i<=n;i++)   result*=i;
    for(long long i=1;i<=k;i++)       result/=i;
    return result;
}

long long reps = 100000;
long long N = 100;
long double delta = 1e-2;
long long distrib = 0;
long long type = 0;
long long param = 3;
long double arg1;
bool flag1 = 0;
bool flag2 = 0;
long double arg2;
ofstream out("result.txt");

int main(int argc, char* argv[]){
    cout.precision(30);
    if(argc > 1){
        N = stod(argv[1]);
        delta = stod(argv[2]);
        reps = stod(argv[3]);

        if(argv[4][0] == 'u')    distrib = 0; // uni
        if(argv[4][0] == 'e')    distrib = 1; // exp
        if(argv[4][0] == 'n')    distrib = 2; // norm
        if(argv[4][0] == 'l')    distrib = 3; // lap
        if(argv[4][0] == 'g')    distrib = 4; // gam
        if(argv[4][0] == '2')    distrib = 5; // 2norm

        if(argv[5][1] == 'r'){  type=0;param=3;}// triang
        if(argv[5][1] == 'd'){  type=0;param=2;}// edges
        if(argv[5][1] == 'o')   type = 1;       // concomp
        if(argv[5][1] == 'n')   type = 2;       // indep
        if(argv[5][1] == 'h')   type = 3;       // chrom

        if(argc > 6){
            flag1 = 1;
            arg1 = stod(argv[6]);
        }
        if(argc > 7){
            flag2 = 1;
            arg2 = stod(argv[7]);
        }
    }
    vector <long double> results(reps);
    vector <long double> numbers(N);
    void (*gen)(vector <long double> &, long double, long double);
    switch (distrib){
    case 5:
            gen = &gen_dnorm;
            arg1 = (flag1?arg1:1);
            arg2 = (flag2?arg2:2);
            break;
        case 4:
            gen = &gen_gam;
            arg1 = (flag1?arg1:1);
            arg2 = (flag2?arg2:2);
            break;
        case 3:
            gen = &gen_lap;
            arg1 = (flag1?arg1:0);
            arg2 = (flag2?arg2:1);
            break;
        case 2:
            gen = &gen_norm;
            arg1 = (flag1?arg1:0);
            arg2 = (flag2?arg2:1);
            break;
        case 1:
            gen = &gen_exp;
            arg1 = (flag1?arg1:1);
            arg2 = (flag1?arg1:0);
            break;
        case 0:
            gen = &gen_uni;
            arg1 = (flag1?arg1:0);
            arg2 = (flag2?arg2:1);
            break;
    }
    long long step = reps/100;
    for(long long t=0;t<=reps;t++){
        if(t%step == 0) cout << t/step << endl;
        gen(numbers, arg1, arg2);
        long double res = 0;
        long long j = 0;
        switch (type){
            case 0:
                for(long long i=0;i<numbers.size();i++){
                    while(j < numbers.size() - 1 && numbers[j+1]*1.0 <= numbers[i] + delta)   j++;
                    res += choose(j-i, param - 1);
                }
                break;
            case 1:
                for(long long i=0;i<numbers.size();i++){
                    j = i;
                    while(j < numbers.size() - 1 && numbers[j+1]*1.0 <= numbers[j] + delta)   j++;
                    res += 1;
                    i = j;
                }
                break;
            case 2:
                for(long long i=0;i<numbers.size();i++){
                    res += 1;
                    while(j < numbers.size() - 1 && numbers[j+1]*1.0 <= numbers[i] + delta)   j++;
                    i = j;
                }
                break;
            case 3:
                for(long long i=0;i<numbers.size();i++){
                    auto xl = upper_bound(numbers.begin(), numbers.end(), numbers[i] - delta);
                    auto xr = lower_bound(numbers.begin(), numbers.end(), numbers[i] + delta);
                    long double click = distance(xl, xr-1);
                    res = max(res, click);
                }
                break;
        }
        results[t] = res;
    }
    long long x = 0;
    for(int i=0;i<reps;i++){
        if(results[i] > 10000 && results[i] < 1e18){
            x = results[i];
            out << x << ' ';
        }else   out << results[i] << ' ';
    }
}
