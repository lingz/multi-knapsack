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
    vector<vector<int> > getWitness();
    int getScore();
  private:
    vector<item> items;
    vector<knapsack> knapsacks;
    vector<int> weight;
    vector<int> value;
    vector<int> capacity;
    vector<vector<bool> > sol;
    void round(bool canSkip);
    int blocked;
    int score;
    int maxKnapsack;
    int minItem;
    int numItems;
    int numKnapsacks;
    bool hasSkipped;
};




#endif
