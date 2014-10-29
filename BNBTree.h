#ifndef BNBTree
#define BNBTree

#include<vector>
#include<numeric>

using namespace std;

struct item {
  int index;
  int value;
  int weight;
  double vw_ratio;
  bool operator < (const item& b) const {
    return vw_ratio < b.vw_ratio;
  }
};

struct knapsack {
  int index;
  int capacity;
  bool operator < (const knapsack& b) const {
    return capacity < b.capacity;
  }
};

class BnbTree{
  public:
    BnbTree( vector< int > value, vector< int > weight, vector< int > capacity);
  private:
    int e;
    int s_e;
    vector< vector<bool> > x;
    vector<bool> d_e;
    vector<int> k_e;
    vector< vector<int> > v_ei;
    vector< vector<int> > v_ei_bar;
    vector< vector<int> > b_ei;
    vector< vector<bool> > x_prime;
    int v_prime;
    vector<bool> n;
    vector<bool> m;
    vector<bool> f;
    vector< vector<bool> > a;
    vector<bool> j_1;
    int c;
    vector<item> items;
    vector<knapsack> knapsacks;
    int L;
    void greedyLower();
};




#endif
