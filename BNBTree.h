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

class Knapsack_sol {
  public:
    int knapsack_id;
    Knapsack_sol();
    Knapsack_sol(knapsack knapsack, int size);
    int getRemainingCapacity();
    int getValue();
    vector<bool>& getIncluded();
    void addItem(item item);
    void removeItem(item item);
  private:
    vector<bool> included;
    int remainingCapacity;
    int size;
    int value;
};

class BnbTree{
  public:
    BnbTree( vector< int > value, vector< int > weight, vector< int > capacity);
    void solve();
  private:
    int e;
    vector<int> s;
    vector<vector<bool> > x;
    vector<vector<bool> > d;
    vector<int> k;
    vector< vector<int> > v;
    vector< vector<int> > v_bar;
    vector< vector<int> > b;
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
    vector<int> weight;
    vector<int> value;
    vector<int> capacity;
    int l;
    int numItems;
    int numKnapsacks;
    void greedyLower();
    void initialize();
    void recordWitnessVector(vector<int> witnessVector,
      int sortedKnapsackIndex);
    void computeVPrime();
    bool branch();
    void bound();
    void sortPhase();
    int maxKnapsack;
    int minItem;
    int u;
};




#endif
