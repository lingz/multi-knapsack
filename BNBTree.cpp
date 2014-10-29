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

  // and the empty knapsack
  capacity.push_back(0);

  // Now we know the number of pruned items, initialize our vectors
  numItems = value.size();
  numKnapsacks = capacity.size();
  knapsacks.resize(numKnapsacks);
  items.resize(numItems);
  x = vector<vector<bool> >(numKnapsacks, vector<bool>(numItems, false));
  x_prime = vector<vector<bool> >(numKnapsacks, vector<bool>(numItems, false));
  f = vector<bool>(numItems, false);

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

void BnbTree::computeVPrime() {
  v_prime = 0;
  for (int i = 0; i < numKnapsacks; i++) {
    for (int j = 0; j < numItems; j++) {
      v_prime += x_prime[i][j];
    }
  }
}

void BnbTree::greedyLower() {
  l = 0;
  vector<bool> included = vector<bool>(numItems, false);
  for (int i = 0; i < knapsacks.size(); i++) {
    knapsack considered_knapsack = knapsacks[i];
    Knapsack_sol considered_sol = Knapsack_sol(considered_knapsack, numItems);
    for (int j = 0; j < items.size(); j++) {
      if (included[j]) continue;
      item considered_item = items[j];
      if (considered_item.weight <= considered_sol.getRemainingCapacity()) {
        considered_sol.addItem(considered_item);
        included[j] = true;
      }
    }
    l += considered_sol.getValue();
    vector<bool> assignedRow = considered_sol.getIncluded();
    vector<bool> &solRow = x_prime[considered_knapsack.index];
    copy(assignedRow.begin(), assignedRow.end(), solRow.begin());
    computeVPrime();
  }
  cout << l << endl;
}

void BnbTree::initialize() {
  greedyLower();
  j_1 = vector<bool>(numItems, false);

  v = vector<vector<int> >(numItems, vector<int>(numKnapsacks + 1, 0));
  v_bar = vector<vector<int> >(numItems, vector<int>(numKnapsacks + 1, 0));
  b = vector<vector<int> >(numItems, vector<int>(numKnapsacks + 1, 0));

  for (int i = 0; i < numKnapsacks; i++) {
    Knapsack sol = Knapsack(value, weight, capacity[i], true);
    recordWitnessVector(sol.getWitness(), i);
    v[0][i] = sol.getSolution();
  }
  // initialize f and a
  n = vector<bool>(numItems, true);
  f = n;
  a = vector<vector<bool> >(numKnapsacks, n);

  // initialize available capacity as total capacity
  c = accumulate(capacity.begin(), capacity.end(), 0);

  k = vector<int>(numItems, 0);
  b[0][0] = accumulate(v[0].begin(), v[0].end(), 0);
  u = b[0][0];
  cout << b[0][0] << endl;
  e = 1;
  d.resize(numItems);
  s.resize(numItems);
}

void BnbTree::recordWitnessVector(vector<int> witnessVector, int sortedKnapsackIndex) {
  vector<bool>& solRow = x[sortedKnapsackIndex];
  // zero the current vector in x
  for (int i = 0; i < solRow.size(); i++) {
    solRow[i] = false;
  }
  // for each vector returned by witness flip to true
  for (int i = 0; i < witnessVector.size(); i++) {
    solRow[witnessVector[i]] = true;
  }
}

bool BnbTree::branch() {
  cout << "Started Branch" << endl;
  int timesSeen;
  // search for a viable branch candidate
  for (int i = 0; i < numItems; i++) {
    timesSeen = 0;
    vector<bool> d_e = vector<bool>(numKnapsacks, false);
    for (int j = 0; j < numKnapsacks; j++) {
      if (x[j][i]) {
        timesSeen++;
        d_e[j] = true;
      }
    }
    if (timesSeen > 1) {
      s[e] = i;
      d[e] = d_e;
      cout << "End Branch" << endl;
      return true;
    }
  }
  cout << "End Branch" << endl;
  return false;
}

void BnbTree::bound() {
  cout << "Started Bound" << endl;
  int w = c;
  vector<bool> j_1_neg = setNegation(j_1);
  vector<bool> f_neg = setNegation(f);
  vector<bool> subtractingSet = setIntersection(
    f_neg,
    j_1_neg
  );
  for (int i = 0; i < numItems; i++) {
    if (subtractingSet[i]) {
      w -= weight[i];
    }
  }

  vector<bool> f_bar = f;

  for (int i = 0; i < numItems; i++) {
    if (j_1_neg[i] && weight[i] > w) {
      f_bar[i] = false;
    }
  }

  f_bar[s[e]] = false;

  for (int i = 0; i < numKnapsacks; i++) {
    if (!d[e][i]) {
      continue;
    }
    vector<bool> searchable = setIntersection(f_bar, a[i]);
    vector<int> newWeights = weight;

    cout << "1" << endl;
    for (int j = 0; j < numItems; j++) {
      if (!searchable[j]) {
        newWeights[j] = maxKnapsack + 1;
      }
    }

    Knapsack sol = Knapsack(value, newWeights, capacity[i], true);
    recordWitnessVector(sol.getWitness(), i);
    v[e][i] = sol.getSolution();
    int z = v[e][i];

    cout << "2" << endl;
    vector<bool> newSolVec = vector<bool>(numItems, false);
    for (int j = 0; j < numItems; j++) {
      if (f_neg[j] && x[i][j]) {
        newSolVec[j] = true;
        z += value[j];
      }
    }
    cout << "3" << endl;

    copy(newSolVec.begin(), newSolVec.end(), x[i].begin());

    cout << "5" << endl;

    v_bar[e][i] = z;
    cout << "6" << endl;
  }
  cout << "4" << endl;

  for (int h = 0; h < numKnapsacks; h++) {
    if (!d[e][h]) {
      continue;
    }
    b[e][h] = 0;
    for (int i = 0; i < numKnapsacks; i++) {
      vector<bool> searchable = setNegation(d[e]);
      searchable[h] = true;
      if (searchable[i]) {
        b[e][h] += v[e - 1][i];
      }
    }
    for (int i = 0; i < numKnapsacks; i++) {
      vector<bool> searchable = d[e];
      searchable[h] = false;
      if (searchable[i]) {
        b[e][h] += v_bar[e][i];
      }
    }
  }
  cout << "End Bound" << endl;
}

void BnbTree::sortPhase() {
  cout << "Started Sort" << endl;
  
  cout << "End Sort" << endl;
}




vector<bool> setIntersection(vector<bool> a, vector<bool> b) {
  vector<bool> result = vector<bool>(a.size(), false);
  for (int i = 0; i < a.size(); i++) {
    result[i] = a[i] && b[i];
  }
  return result;
}

vector<bool> setSubtraction(vector<bool> a, vector<bool> b) {
  vector<bool> notAAndB = setNegation(setIntersection(a,b));
  return setIntersection(a, notAAndB);
}

vector<bool> setNegation(vector<bool> a) {
  vector<bool> result = vector<bool>(a.size(), false);
  for (int i = 0; i < a.size(); i++) {
    result[i] = !a[i];
  }
  return result;
}

void BnbTree::solve() {
  initialize();
  bool success = branch();
  if (success) {
    bound();
    sortPhase();
  }
}

