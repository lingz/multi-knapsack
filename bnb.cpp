#include<iostream>
#include "knapsack.h"
#include "BNBTree.h"

using namespace std;

int TYPE;
vector< int > value, weight;
vector< int > knapsacks;

int get_input(){
  int sz, index, val, wt, knaps;
  int capacity;
  cin >> TYPE;

  if ( TYPE == 2 ){
    cin >> sz >> knaps;
    for ( int i = 0; i < knaps; i++ ){
      cin >> capacity;
      knapsacks.push_back(capacity);
    }
    value.resize(sz); weight.resize(sz);
    for ( int i = 0; i < sz; i++ ){
      cin >> index >> val >> wt;
      index--;
      value[index] = val;
      weight[index] = wt;
    }
  }
}

int main() {
  get_input();
  BnbTree(value, weight, knapsacks);
}
