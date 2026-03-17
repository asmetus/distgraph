#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <random>
#include <fstream>
#include <cmath>

using namespace std;

mt19937 rng;
uniform_real_distribution <long double> uni(0, 1);
exponential_distribution <long double> expon(1);
normal_distribution <long double> norm(0, 1);

long double lap(long double b_1, long double mu) {
    long double e1 = expon(rng), e2 = expon(rng);
    return mu - b_1 * (e1 - e2);
}

void gen_uni(vector <long double> &v, long double a, long double b){
    for(int j=0;j<v.size();j++){
        v[j] = uni(rng) * (b-a) + a;
    }
}

void gen_exp(vector <long double> &a, long double lambda, long double nothing){
    long double adbmal = 1.0 / lambda;
    for(int j=0;j<a.size();j++){
        a[j] = expon(rng) * adbmal;
    }
}

void gen_norm(vector <long double> &a, long double mu, long double std){
    for(int j=0;j<a.size();j++){
        a[j] = norm(rng) * std + mu;
    }
}

void gen_lap(vector <long double> &a, long double b, long double mu){
    long double b_1 = 1.0 / b;
    for(int j=0;j<a.size();j++){
        a[j] = lap(b_1, mu);
    }
}

void gen_gam(vector <long double> &v, long double a, long double b){
    std::gamma_distribution<long double> gam(a, b);
    for(int j=0;j<v.size();j++){
        v[j] = gam(rng);
    }
}

void gen_dnorm(vector <long double> &v, long double mu, long double std){
    for(int j=0;j<v.size();j++){
        v[j] = norm(rng) * std + (uni(rng) > 0.5?mu:-mu);
    }
}

long long N = 100;
long long distrib = 0;
long double arg1;
bool flag1 = 0;
bool flag2 = 0;
long double arg2;
string file_name = "numbers.txt";
int seed = 42;

int main(int argc, char* argv[]){
    cout.precision(30);
    if(argc > 1){
        N = stod(argv[1]);

        if(argv[2][0] == 'u')    distrib = 0; // uni
        if(argv[2][0] == 'e')    distrib = 1; // exp
        if(argv[2][0] == 'n')    distrib = 2; // norm
        if(argv[2][0] == 'l')    distrib = 3; // lap
        if(argv[2][0] == 'g')    distrib = 4; // gam
        if(argv[2][0] == '2')    distrib = 5; // 2norm

        file_name = argv[3];

        seed = stod(argv[4]);

        if(argc > 5){
            flag1 = 1;
            arg1 = stod(argv[5]);
        }
        if(argc > 6){
            flag2 = 1;
            arg2 = stod(argv[6]);
        }
    }

    rng.seed(seed);
    ofstream out("numbers/" + file_name);

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
    gen(numbers, arg1, arg2);
    for(int i=0;i<N;i++){
        out << numbers[i] << ' ';
    }
}
