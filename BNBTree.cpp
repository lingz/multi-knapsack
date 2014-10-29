#include "BNBTree.h"
#include <functional>
#include <algorithm>
#include <iostream>
#include <cassert>
#include "knapsack.h"

using namespace std;

vector<bool> setIntersection(vector<bool> a, vector<bool> b);
vector<bool> setNegation(vector<bool> a);
vector<bool> setSubtraction(vector<bool> a, vector<bool> b);

Knapsack_sol::Knapsack_sol() {}
Knapsack_sol::Knapsack_sol(knapsack knapsack, int size)
  : knapsack_id(knapsack.index), size(size), remainingCapacity(knapsack.capacity),
  included(vector<bool>(size, false)), value(0)
{
};

int Knapsack_sol::getRemainingCapacity() { return remainingCapacity;}
int Knapsack_sol::getValue() {return value; }
vector<bool>& Knapsack_sol::getIncluded() { return included;}

void Knapsack_sol::addItem(item item) {
  assert(item.weight <= remainingCapacity);
  assert(!included[item.index]);
  remainingCapacity -= item.weight;
  value += item.value;
  included[item.index] = true;
}

void Knapsack_sol::removeItem(item item) {
  assert(included[item.index]);
  remainingCapacity += item.weight;
  value -= item.value;
  included[item.index] = false;
}

BnbTree::BnbTree( vector< int > value, vector< int > weight, vector< int > capacity) 
  : weight(weight), capacity(capacity), value(value)
{
  item new_item;
  knapsack new_knapsack;

  numItems = value.size();
  numKnapsacks = capacity.size();

  maxKnapsack = *max_element(capacity.begin(), capacity.end());
  minItem = *min_element(capacity.begin(), capacity.end());

  // problem simplification - remove too small bags, and too large bags
  for (int i = 0; i < capacity.size(); i++) {
    if (capacity[i] < minItem) {
      capacity.erase(capacity.begin() + i);
    }
  }

  for (int i = 0; i < weight.size(); i++) {
    if (weight[i] > maxKnapsack) {
      weight.erase(weight.begin() + i);
      value.erase(value.begin() + i);
    }
  }

  // Now we know the number of pruned items, initialize our vectors
  numItems = value.size();
  numKnapsacks = capacity.size();
  knapsacks.resize(numKnapsacks);
  items.resize(numItems);

  for (int i = 0; i < numKnapsacks; i++) {
    knapsacks[i].index = i;
    knapsacks[i].capacity = capacity[i];
  }

  for (int i = 0; i < numItems; i++) {
    items[i].index = i;
    items[i].value = value[i];
    items[i].weight = weight[i];
    items[i].vw_ratio = (float) items[i].value / items[i].weight;
  }

  sort(items.begin(), items.end());
  sort(knapsacks.begin(), knapsacks.end());
  reverse(knapsacks.begin(), knapsacks.end());
  reverse(items.begin(), items.end());

}

void BnbTree::solve() {
  score = 0;
  vector<bool> included = vector<bool>(numItems, false);
  sol = vector<vector<bool> >(numKnapsacks, vector<bool>(numItems, false));
  for (int i = 0; i < knapsacks.size(); i++) {
    knapsack considered_knapsack = knapsacks[i];
    Knapsack_sol considered_sol = Knapsack_sol(considered_knapsack, numItems);
    for (int j = 0; j < items.size(); j++) {
      if (included[j]) continue;
      item considered_item = items[j];
      if (considered_item.weight <= considered_sol.getRemainingCapacity()) {
        considered_sol.addItem(considered_item);
        included[j] = true;
        sol[i][j] = true;
      }
    }
    score += considered_sol.getValue();
  }
}

int BnbTree::getScore() {
  return score;
}

vector<vector<int> > BnbTree::getWitness() {
  vector<vector<int> > result;
  result.resize(numKnapsacks);
  int index;
  for (int i = 0; i < numKnapsacks; i++) {
    index = knapsacks[i].index;
    vector<int> resultRow;
    for (int j = 0; j < numItems; j++) {
      if (sol[i][j]) {
        resultRow.push_back(items[j].index);
      }
    }
    result[index] = resultRow;
  }
  return result;
}
