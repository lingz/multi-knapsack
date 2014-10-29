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

  greedyLower();
}

void BnbTree::greedyLower() {
  vector<knapsack> sorted_knapsacks;
  sorted_knapsacks.resize(knapsacks.size());
  vector<item> sorted_items;
  sorted_items.resize(items.size());

  copy(knapsacks.begin(), knapsacks.end(), sorted_knapsacks.begin());
  copy(items.begin(), items.end(), sorted_items.begin());



  sort(sorted_knapsacks.begin(), sorted_knapsacks.end());
  sort(sorted_items.begin(), sorted_items.end());
  reverse(sorted_knapsacks.begin(), sorted_knapsacks.end());
  reverse(sorted_items.begin(), sorted_items.end());

  for (int i = 0; i < sorted_knapsacks.size(); i++) {
    knapsack knapsack_s = sorted_knapsacks[i];
    cout << "K: " << knapsack_s.index << " " << knapsack_s.capacity << endl;
  }
  for (int i = 0; i < sorted_items.size(); i++) {
    item item_s = sorted_items[i];
    cout << "I: " << item_s.index << " " << item_s.vw_ratio << endl;
  }
}

