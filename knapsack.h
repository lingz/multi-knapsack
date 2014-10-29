#ifndef KNAPSACK_
#define KNAPSACK_

#include<vector>
#include<numeric>

using namespace std;

class Knapsack{
private:
  vector< int > value, weight, answer, DP;
  vector< vector< bool > > isin;
  int capacity;
  int totalValue, totalWeight;
  int solution;

public:
  Knapsack( vector< int > value, vector< int > weight, int capacity, bool needWitness ){
    this->value = value;
    this->weight = weight;
    this->capacity = capacity;

    totalValue = accumulate(value.begin(), value.end(), 0);
    totalWeight = accumulate(weight.begin(), weight.end(), 0);

    this->answer.clear(); this->isin.clear(); this->DP.clear();

    if ( needWitness ){
      solveWithWitness_weight();
    }
    else{
      solveWithoutWitness_weight();
    }
  }

  void solveWithoutWitness_value(){

    DP.resize( totalValue + 1, totalWeight + 1 );
    DP[0] = 0;

    for ( int i = 0; i < value.size(); i++ ){
      for ( int v = totalValue; v >= value[i]; v-- ){
	if ( DP[v - value[i]] + weight[i]  < DP[v] ){
	  DP[v] = weight[i] + DP[v - value[i]];
	}
      }
    }

    for ( int v = totalValue; v >= 0; v-- ){
      if ( DP[v] <= capacity && DP[v] != totalWeight + 1){
	solution = v;
	break;
      }
    }
  }

  void solveWithWitness_value(){

    isin.resize( value.size() );
    for ( int i = 0; i < value.size(); i++ ){
      isin[i].resize( totalValue + 1, false );
    }

    DP.resize( totalValue + 1, totalWeight + 1);
    DP[0] = 0;

    for ( int i = 0; i < value.size(); i++ ){
      for ( int v = totalValue; v >= value[i]; v-- ){
	if ( DP[v - value[i]] + weight[i]  < DP[v] ){
	  DP[v] = weight[i] + DP[v - value[i]];
	  isin[ i ][ v ] = true;
	}
      }
    }

    for ( int v = totalValue; v >= 0; v-- ){
      if ( DP[v] <= capacity && DP[v] != totalWeight + 1){
	solution = v;
	break;
      }
    }

    int position = value.size() - 1;
    int v = solution;
    while ( position >= 0 ){
      if ( isin[ position ][ v ] ){
	answer.push_back(position);
	v -= value[position];
      }
      position--;
    }

  }

  void solveWithoutWitness_weight(){

    DP.resize( capacity + 1, 0);

    for ( int i = 0; i < value.size(); i++ ){
      for ( int w = capacity; w >= weight[i]; w-- ){
	if ( DP[w] < value[i] + DP[w - weight[i]] ){
	  DP[w] = value[i] + DP[w - weight[i]];
	}
      }
    }

    solution = DP[capacity];
  }

  void solveWithWitness_weight(){

    DP.resize( capacity + 1, 0);

    isin.resize( value.size() );
    for ( int i = 0; i < value.size(); i++){
      isin[i].resize(capacity + 1, false);
    }

    for ( int i = 0; i < value.size(); i++ ){
      for ( int w = capacity; w >= weight[i]; w-- ){
	if ( DP[w] < value[i] + DP[w - weight[i]] ){
	  DP[w] = value[i] + DP[w - weight[i]];
	  isin[ i ][ w ] = true;
	}
      }
    }

    solution = DP[capacity];

    int position = value.size()-1;
    int w = capacity;
    while ( position >= 0 ){
      if ( isin[ position][ w ] ){
	answer.push_back( position );
	w -= weight[position];
      }
      position--;
    }
  }

  vector< int > getWitness(){
    return this->answer;
  }

  int getSolution(){
    return solution;
  }

};

#endif
