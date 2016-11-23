#include <iostream>
#include <vector>
#include <ctime>
#include "ward.h"

/*
Aim is to try solving the problem with
1. simulated annealing
2. tabu search
3. branch and bound with fixed bound??
4. genetic algorithm
*/
using namespace std;

#define ITER 50000000


// This method packs the '60%' knapsack using a greedy approach
vector<int> greedy(vector<int> init, Calculator calc){
	double ori=1000000;
	for (int i=0; i<0.6*calc.wardlist.size();i++){
		init[i]=1;
	}
	return init;
}

int changestate(int state){
	if(state==1) return 0;
	return 1;
}

// candidate generation - euclidean distance reduces with time
vector<int> generatecandidate(vector<int> current, int iter, double BETA){
	for (int i = 0; i < current.size(); i++){
		//randomly change the state of a ward
		if(rand() /(double)RAND_MAX < (double)((0.5*ITER)-iter)/ITER) {
			current[i]=changestate(current[i]);
			// look for alternate to change
			int j = rand() % current.size();
			while (!(j != i && current[j] == current[i])) {
				j = rand() % current.size();
			}
			current[j] = changestate(current[j]);
		}
	}
	return current;
}

// candidate generation - euclidean distance reduces with time
vector<int> generatecandidateV2(vector<int> current, int iter, double BETA){
	//randomly choose a thing to change
	int tochange = 30;
	tochange *= (1-(500000-iter)/500000);
	for (int i=0; i < tochange ; i++){
		//randomly change the state of a ward
		int j = rand() % 30;
		current[j]=changestate(current[j]);
	}
	return current;
}



int main(){
	srand(1);
	cout.precision(10);

	Calculator calc = Calculator(true, "data45.csv");
	vector<int> bin(45,0);
	double score;

	// greedy algorithm
	bin = greedy(bin, calc);
	cout << calc.getscore(bin) << endl;

	double BETA = 5;           //annealing starting beta = 1/T (temperature)
	double cooling_speed = 1.0001;  //annealing cooling speed
	double de;
	vector<int> newbin;

	time_t start = time(NULL);
	int counter = 0;
	double bestscore = 10000;

	// simulated annealing here
	int nmbIters = 50000;
	score = calc.getscore(bin);
	for(int i = 1; i < ITER; i++){
		newbin = generatecandidate(bin, i, BETA);
		de = calc.getscore(newbin) - score;

		if(de < 0 || exp(-BETA*de) > (double)rand()/RAND_MAX) {
			bin = newbin;
			score = calc.getscore(newbin);
			counter++;
		}

		if (score<bestscore) bestscore= score;

		BETA *= cooling_speed;
	}

	for (int j=0; j < bin.size(); j++) {
		if (bin[j]==1) {
			cout << j+1 << endl;
		}
	}
	cout << endl;

	cout <<score<<endl;
	cout << bestscore<<endl;

	int binsize=0;
	for(int i=0; i<bin.size() ;i++) binsize += bin[i];
	for(int i=0; i<bin.size() ;i++) cout << bin[i];
	cout << endl;
	cout << "bin size " << binsize << endl;
	cout << calc.wardCap << endl;


	time_t end = time(NULL);
	cout << "time: " << difftime(end,start) << " seconds" << endl;
	cout << (double)counter/ITER << endl;

  return 0;
}
