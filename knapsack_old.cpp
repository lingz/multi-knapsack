#include<iostream>
#include<vector>
#include<algorithm>

#include "knapsack.h"

using namespace std;

int TYPE;

vector< int > value, weight;
int capacity;
vector< int > knapsacks;
vector< vector< int > > gain;

int get_input(){
  int sz, index, val, wt, knaps;
  cin >> TYPE;
  if ( TYPE == 1 ){
    cin >> sz;
    cin >> capacity;
    value.resize(sz); weight.resize(sz);
    for ( int i = 0; i < sz; i++ ){
      cin >> index >> val >> wt;
      index--;
      value[index] = val;
      weight[index] = wt;
    }
  }
  else if ( TYPE == 2 ){
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
  else if ( TYPE == 3 ){
    cin >> sz;
    cin >> capacity;
    for ( int i = 0; i < sz; i++ ){
      cin >> wt;
      weight.push_back(wt);
    }
    gain.resize(sz);
    for ( int i = 0; i < sz; i++ ){
      gain[i].resize(sz);
    }
    for ( int i = 0; i < sz; i++ ){
      cin >> val;
      gain[i][i] = val;
      value.push_back( val );
    }
    for ( int i = 0; i < sz - 1; i++ ){
      for ( int j = i+1; j < sz; j++ ){
	cin >> val;
	gain[i][j] = val;
	gain[j][i] = val;
      }
    }
  }
}

bool comp ( int i, int j){
  return value[i]*weight[j] > value[j]*weight[i];
}

vector< int > solve(vector< int > value, vector< int > weight, int capacity){

  // sort the numbers by increasing order of pi/wi
  vector< int > ind;
  for ( int i = 0; i < value.size(); i++ ){
    ind.push_back(i);
  }
  sort( ind.begin(), ind.end(), comp );

  vector< int > newv, neww;
  for ( int i = 0; i < value.size(); i++ ){
    newv.push_back(value[ind[i]]);
    neww.push_back(weight[ind[i]]);
  }

  // find critical item
  int tot = 0;
  int crit = 0;
  while ( tot < capacity && crit < newv.size()){
    tot += neww[crit];
    crit++;
  }

  int newcapacity = capacity;
  vector< int > answer;
  int pos = 0;
  while ( pos < crit/3 ){
    answer.push_back(pos);
    newcapacity -= neww[pos];
    pos++;
  }

  vector< int > tempv, tempw;
  while ( pos < (crit+2*(value.size()-1))/3 ){
    tempv.push_back(newv[pos]);
    tempw.push_back(neww[pos]);
    pos++;
  }

  Knapsack core(tempv, tempw, newcapacity, true);

  vector< int > coreanswer = core.getWitness();

  int already = answer.size();

  for ( int i = 0; i < coreanswer.size(); i++ ){
    answer.push_back(already+coreanswer[i]);
  }

  vector< int > sln;
  for ( int i = 0; i < answer.size(); i++ ){
    sln.push_back(ind[answer[i]]);
  }
  
  sort( sln.begin(), sln.end() );
  
  return sln;
}

int main(){
  get_input();
  vector< int > solution = solve(value, weight, capacity);
  for ( int i = 0; i < solution.size(); i++ ){
    cout << solution[i] << endl;
  }
}
