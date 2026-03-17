#include <iostream>
#include <random>
#include <ctime>
#include <vector>
#include <cassert>
#include <set>
#include <fstream>
#include <queue>
using namespace std;

mt19937 rnd(time(0));

struct Node {
    long long value;
    long long sum;
    long long add;
    int priority;
    int size;
    Node* left;
    Node* right;
    
    Node(long long val) : value(val), sum(val), add(0), 
                          priority(rnd()), size(1), 
                          left(nullptr), right(nullptr) {}
};

class ImplicitTreap {
private:
    Node* root;
    
    int getSize(Node* node) {
        return node ? node->size : 0;
    }
    
    long long getSum(Node* node) {
        if (!node) return 0;
        return node->sum + node->add * node->size;
    }
    
    void update(Node* node) {
        if (node) {
            node->size = 1 + getSize(node->left) + getSize(node->right);
            node->sum = node->value;
            if (node->left) {
                node->sum += getSum(node->left);
            }
            if (node->right) {
                node->sum += getSum(node->right);
            }
        }
    }
    
    void pushAdd(Node* node) {
        if (!node || node->add == 0) return;
        
        node->value += node->add;
        node->sum += node->add * node->size;
        
        if (node->left) {
            node->left->add += node->add;
        }
        if (node->right) {
            node->right->add += node->add;
        }
        
        node->add = 0;
    }
    
    void split(Node* node, int key, Node*& left, Node*& right, bool push = true) {
        if (!node) {
            left = right = nullptr;
            return;
        }
        
        if (push) pushAdd(node);
        int curKey = getSize(node->left);
        
        if (key <= curKey) {
            split(node->left, key, left, node->left, push);
            right = node;
        } else {
            split(node->right, key - curKey - 1, node->right, right, push);
            left = node;
        }
        
        update(node);
    }
    
    Node* merge(Node* left, Node* right, bool push = true) {
        if (!left) return right;
        if (!right) return left;
        
        if (push) {
            pushAdd(left);
            pushAdd(right);
        }
        
        if (left->priority > right->priority) {
            left->right = merge(left->right, right, push);
            update(left);
            return left;
        } else {
            right->left = merge(left, right->left, push);
            update(right);
            return right;
        }
    }
    
    void insert(Node*& node, int pos, long long value) {
        Node* left = nullptr;
        Node* right = nullptr;
        split(node, pos, left, right);
        
        Node* newNode = new Node(value);
        node = merge(merge(left, newNode), right);
    }
    
    void erase(Node*& node, int pos) {
        Node* left = nullptr;
        Node* mid = nullptr;
        Node* right = nullptr;
        
        split(node, pos, left, right);
        split(right, 1, mid, right);
        
        if (mid) {
            delete mid;
        }
        
        node = merge(left, right);
    }
    
    void addToRange(Node*& node, int l, int r, long long value) {
        Node* left = nullptr;
        Node* mid = nullptr;
        Node* right = nullptr;
        
        split(node, l, left, right);
        split(right, r - l + 1, mid, right);
        
        if (mid) {
            mid->add += value;
        }
        
        node = merge(merge(left, mid), right);
    }
    
    long long querySum(Node*& node, int l, int r) {
        Node* left = nullptr;
        Node* mid = nullptr;
        Node* right = nullptr;
        
        split(node, l, left, right);
        split(right, r - l + 1, mid, right);
        
        long long result = 0;
        if (mid) {
            result = getSum(mid);
        }
        
        node = merge(merge(left, mid), right);
        return result;
    }
    
    void build(Node*& node, const vector<long long>& arr, int l, int r) {
        if (l > r) {
            node = nullptr;
            return;
        }
        
        int mid = (l + r) / 2;
        node = new Node(arr[mid]);
        
        build(node->left, arr, l, mid - 1);
        build(node->right, arr, mid + 1, r);
        
        update(node);
    }
    
    void clear(Node* node) {
        if (!node) return;
        clear(node->left);
        clear(node->right);
        delete node;
    }

public:
    ImplicitTreap() : root(nullptr) {}
    
    ImplicitTreap(const vector<long long>& arr) {
        root = nullptr;
        build(root, arr, 0, arr.size() - 1);
    }
    
    ~ImplicitTreap() {
        clear(root);
    }
    
    void insert(int pos, long long value) {
        insert(root, pos, value);
    }
    
    void erase(int pos) {
        erase(root, pos);
    }
    
    void addOnRange(int l, int r, long long value) {
        addToRange(root, l, r, value);
    }
    
    long long getSumOnRange(int l, int r) {
        return querySum(root, l, r);
    }
};



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

    ImplicitTreap treap_left;
    ImplicitTreap treap_right;

    multiset <long double> points;
    queue <long double> order;
    
    long double squares = 0;
    long double mean = 0;
    long double std = 0;

    long long triangles = 0;
    int i = 0;
    long double x;
    while(in >> x){
        i++;
		if(order.size() == N){
			long double y = order.front();
			order.pop();
            auto it = points.find(y);
			auto pos = std::distance(points.begin(), it);
			auto yl = points.lower_bound(y - delta);
			auto yr = points.upper_bound(y + delta);
			int posl = std::distance(points.begin(), yl);
			int posr = std::distance(points.begin(), yr) - 1;
			triangles -= treap_left.getSumOnRange(0, posr) - treap_right.getSumOnRange(0, posl-1) -
						 treap_left.getSumOnRange(pos, pos) - treap_right.getSumOnRange(pos, pos);
			points.erase(it);
			treap_left.addOnRange(pos+1, posr, -1);
			treap_right.addOnRange(posl, pos-1, -1);
			treap_left.erase(pos);
			treap_right.erase(pos);
		}
		order.push(x);
		auto xl = points.lower_bound(x - delta);
		auto xr = points.upper_bound(x + delta);
		if(xl == xr){
			auto it = points.insert(x);
			auto pos = std::distance(points.begin(), it);
			treap_left.insert(pos, 0);
			treap_right.insert(pos, 0);
		}else{
			xr--;
			int posl = std::distance(points.begin(), xl);
			int posr = std::distance(points.begin(), xr);
			triangles += treap_left.getSumOnRange(0, posr) - treap_right.getSumOnRange(0, posl-1);
			auto it = points.insert(x);
			auto pos = std::distance(points.begin(), it);
			treap_left.insert(pos, pos-posl);
			treap_right.insert(pos, posr-pos+1);
			treap_left.addOnRange(pos+1, posr+1, 1);
			treap_right.addOnRange(posl, pos-1, 1);
		}
        long double tmp = mean;
        mean += (triangles - tmp) / i;
        squares += (triangles - mean) * (triangles - tmp);
        if(i-1) std = sqrt(squares * 1.0 / (i - 1));

        if(triangles < mean - 3 * std || triangles > mean + 3 * std)  cout << i << ' ';
    }
    cout << endl;
    return 0;
}