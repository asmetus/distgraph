#include <iostream>
#include <random>
#include <ctime>
#include <set>
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
        return node ? node->sum : 0;
    }
    
    void update(Node* node) {
        if (node) {
            node->size = 1 + getSize(node->left) + getSize(node->right);
            node->sum = node->value + getSum(node->left) + getSum(node->right);
        }
    }
    
    void pushAdd(Node* node) {
        if (node && node->add != 0) {
            node->value += node->add;
            node->sum += node->add * node->size;
            
            if (node->left) node->left->add += node->add;
            if (node->right) node->right->add += node->add;
            
            node->add = 0;
        }
    }
    
    void split(Node* node, int key, Node*& left, Node*& right) {
        if (!node) {
            left = right = nullptr;
            return;
        }
        
        pushAdd(node);
        int curKey = getSize(node->left);
        
        if (key <= curKey) {
            split(node->left, key, left, node->left);
            right = node;
        } else {
            split(node->right, key - curKey - 1, node->right, right);
            left = node;
        }
        
        update(node);
    }
    
    Node* merge(Node* left, Node* right) {
        if (!left) return right;
        if (!right) return left;
        
        pushAdd(left);
        pushAdd(right);
        
        if (left->priority > right->priority) {
            left->right = merge(left->right, right);
            update(left);
            return left;
        } else {
            right->left = merge(left, right->left);
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
        
        delete mid;
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
            pushAdd(mid);
        }
        
        node = merge(merge(left, mid), right);
    }
    
    long long querySum(Node*& node, int l, int r) {
        Node* left = nullptr;
        Node* mid = nullptr;
        Node* right = nullptr;
        
        split(node, l, left, right);
        split(right, r - l + 1, mid, right);
        
        long long result = getSum(mid);
        
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

public:
    ImplicitTreap() : root(nullptr) {}
    
    ImplicitTreap(const vector<long long>& arr) {
        root = nullptr;
        build(root, arr, 0, arr.size() - 1);
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
    
    int size() {
        return getSize(root);
    }
    
    void printArray() {
        for (int i = 0; i < size(); i++) {
            cout << getSumOnRange(i, i) << " ";
        }
        cout << endl;
    }
};

int main(int argc, char* argv[]){
	int N = 4;
    long double delta = 0.25;
    if(argc > 1){
        N = stod(argv[1]);
        delta = stod(argv[2]);
    }
    ImplicitTreap treap_left;
    ImplicitTreap treap_right;
    // ImplicitTreap treap({1, 2, 3, 4, 5});
    // treap.addOnRange(2, 2, 100);
    // treap.printArray();
    // cout << "Сумма [1, 3]: " << treap.getSumOnRange(1, 3) << endl;
    
    // treap.addOnRange(1, 3, 10);
    // cout << "После прибавления 10 на [1,3]: ";
    // treap.printArray();
    
    // treap.erase(2);
    // cout << "После удаления позиции 2: ";
    // treap.printArray();
    
    // treap.insert(2, 100);
    // cout << "После вставки 100 на позицию 2: ";
    // treap.printArray();
    
    // cout << "Сумма [0, 4]: " << treap.getSumOnRange(0, 4) << endl; // 1+12+100+14+5 = 132

    set <long double> points;
    queue <long double> order;

    long long triangles = 0;
    while(1){
    	cout << "---------------------------" << endl;
    	long double x;
		cin >> x;
		if(order.size() == N){
			long double y = order.front();
			order.pop();
			auto pos = std::distance(points.begin(), points.find(y));
			auto yl = points.upper_bound(y - delta);
			auto yr = points.lower_bound(y + delta);
			int posl = std::distance(points.begin(), yl);
			int posr = std::distance(points.begin(), yr) - 1;
			cout << posl << ' ' << posr << endl;
			triangles -= treap_left.getSumOnRange(0, posr) - treap_right.getSumOnRange(0, posl-1) -
						 treap_left.getSumOnRange(pos, pos) - treap_right.getSumOnRange(pos, pos);
			points.erase(y);
			treap_left.addOnRange(pos+1, posr, -1);
			treap_right.addOnRange(posl, pos-1, -1);
			treap_left.erase(pos);
			treap_right.erase(pos);
		}
		cout << triangles << endl;
		for (auto p : points) {
	        cout << p << " ";
	    }
	    cout << endl;
		treap_left.printArray();
		treap_right.printArray();
		order.push(x);
		auto xl = points.upper_bound(x - delta);
		auto xr = points.lower_bound(x + delta);
		if(xl == xr){
			points.insert(x);
			auto pos = std::distance(points.begin(), points.find(x));
			treap_left.insert(pos, 0);
			treap_right.insert(pos, 0);
		}else{
			xr--;
			int posl = std::distance(points.begin(), xl);
			int posr = std::distance(points.begin(), xr);
			cout << posl << ' ' << posr << endl;
			triangles += treap_left.getSumOnRange(0, posr) - treap_right.getSumOnRange(0, posl-1);
			points.insert(x);
			auto pos = std::distance(points.begin(), points.find(x));
			treap_left.insert(pos, pos-posl);
			treap_right.insert(pos, posr-pos+1);
			treap_left.addOnRange(pos+1, posr+1, 1);
			treap_right.addOnRange(posl, pos-1, 1);
		}
		cout << triangles << endl;
		for (auto p : points) {
	        cout << p << " ";
	    }
	    cout << endl;
		treap_left.printArray();
		treap_right.printArray();
    }

    return 0;
}