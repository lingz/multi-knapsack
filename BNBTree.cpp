#include "BNBTree.h"
#include <functional>
#include <algorithm>
#include <iostream>

using namespace std;


BnbTree::BnbTree( vector< int > value, vector< int > weight, vector< int > capacity) 
{
  item new_item;
  knapsack new_knapsack;

  knapsacks.resize(capacity.size());
  items.resize(value.size());
  
  for (int i = 0; i < capacity.size(); i++) {
    knapsacks[i].index = i;
    knapsacks[i].capacity = capacity[i];
  }

  for (int i = 0; i < value.size(); i++) {
    items[i].index = i;
    items[i].value = value[i];
    items[i].weight = weight[i];
    items[i].vw_ratio = (float) items[i].value / items[i].weight;
  }
  sort(items.begin(), items.end());
  sort(knapsacks.begin(), knapsacks.end());
  reverse(knapsacks.begin(), knapsacks.end());
  reverse(items.begin(), items.end());

  greedyLower();
}

void BnbTree::greedyLower() {

}

