#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include "ward.h"

/*
Aim is to try solving the problem with Genetic Algorithm
*/
using namespace std;


#define CROSSOVER_RATE            0.7 //30wopt 0.7
#define MUTATION_RATE             0.05
#define POP_SIZE                  400 //must be an even number (30ward optimal is 200) (45wopt 400)
//#define POP_SIZE                  400 //must be an even number (30ward optimal is 200) (45wopt 400)
#define CHROMO_LENGTH             45
#define MAX_ALLOWABLE_GENERATIONS 4000 //30wopt 1000  45wopt 3000
//#define MAX_ALLOWABLE_GENERATIONS 10000 //30wopt 1000  45wopt 3000
#define HARSHNESS                 0.15 // vary this

//global Calculator
Calculator calc = Calculator(true, "data45.csv");

//----------------------------------------------------------------------------------------
//  define a data structure which will define a chromosome
//----------------------------------------------------------------------------------------
struct chromo {
    //the binary bit string is held in a std::string
  string    bits;

    float     fitness;

    chromo(): bits(""), fitness(0.0f){};
    chromo(string bts, float ftns): bits(bts), fitness(ftns){}
};


// Prototypes
string GetRandomBits();
string NaturalSelection(chromo* pop);
string Mutate(string chromo);
void Crossover(string &g1, string &g2);
double Score(string gene);
void Mergesort(chromo pop[], int low, int high);
void Merge(chromo pop[], int low, int mid, int high);
void GenerateSeedPopulation(vector<string>& seedpop, string seed, int counter);



int main(){
  int seed=1;
  double meanscore =0;
  for (; seed < 11; seed++) {
    srand(seed);
    cout.precision(10);
    time_t start = time(NULL);
    double bestscore = 10000;
    string bestbin;
    chromo Population[POP_SIZE];

    bool isSeed = false;
    int numberSeeded = 0;
    vector<string> seedpop;

    if (isSeed) {
      string seed;
      switch (CHROMO_LENGTH) {
        case 30:
        seed = "100111x01011101001xxx1x0011111";
        break;
        case 40:
        seed = "001011010101100x1110xx111x10010110110101";
        break;
        case 45:
        seed = "11x010111110011100011100xx0001111111110001x01";
        break;
      }

      //generate recursively and push to vector
      GenerateSeedPopulation(seedpop, seed, 0);
      cout << seedpop.size() <<endl;

      for (int i=0; i<seedpop.size() ; i++) {
        Population[i].bits = seedpop[i];
        Population[i].fitness = Score(Population[i].bits);
        numberSeeded++;
      }
    }

    for (int i=numberSeeded; i < POP_SIZE - numberSeeded; i++) {
      Population[i].bits = GetRandomBits();
      Population[i].fitness = Score(Population[i].bits);
      //cout << Population[i].fitness <<endl;
    }

    // sort
    Mergesort(Population, 0, POP_SIZE-1);

    for (int i=0; i < MAX_ALLOWABLE_GENERATIONS ; i++) {
      int currentpopsize=0;
      chromo temppop[POP_SIZE];

      while (currentpopsize < POP_SIZE) {
        string g1, g2;
        //selection
        g1 = NaturalSelection(Population);
        g2 = NaturalSelection(Population);

        // mutate
        g1 = Mutate(g1);
        g2 = Mutate(g2);

        //crossover
        Crossover(g1, g2);

        // replace pop
        temppop[currentpopsize++] = chromo(g1, Score(g1));
        temppop[currentpopsize++] = chromo(g2, Score(g2));
      }

      for (int j=0; j<POP_SIZE; j++) {
        Population[j]=temppop[j];
      }

      Mergesort(Population, 0, POP_SIZE-1);

      if (bestscore > Population[0].fitness) {
        bestscore = Population[0].fitness;
        bestbin = Population[0].bits;
      }

      //if (i % 1000 == 0) cout<< i << " bestscore is " << bestscore << endl;
    }


    cout << bestbin << endl;
    cout << bestscore << endl;

    time_t end = time(NULL);
  	cout << "time: " << difftime(end,start) << " seconds" << endl;
    meanscore += bestscore;
  }
  cout << meanscore/10.0 << endl;
}

//--------------------------------------------------------------------
// FUNCTIONS
//--------------------------------------------------------------------

// generates random string
string GetRandomBits() {
    string gene;
    for (int j=0; j < CHROMO_LENGTH; j++) {
      if((double)rand()/RAND_MAX < 0.5) {
        gene += "1";
      } else {
        gene += "0";
      }
    }
  return gene;
}

double Score(string gene) {
  double score;
  vector<int> converted;
  for (int i=0; i< gene.length(); i++) {
    if (gene[i] == '1') {
      converted.push_back(1);
    } else {
      converted.push_back(0);
    }
  }
  score = calc.getscore(converted);
  return score;
}

void Mergesort(chromo pop[], int low, int high) {
  if (low < high) {
    int mid = (low + high) /2;

    Mergesort(pop, low, mid);
    Mergesort(pop, mid+1, high);
    Merge(pop, low, mid, high);
  }
}

void Merge(chromo pop[], int low, int mid, int high){
  int n = high-low + 1;
  chromo* b = new chromo[n];
  int left = low, right = mid + 1, bIdx = 0;

  while (left <= mid && right <= high) {
    if(pop[left].fitness <= pop[right].fitness) {
      b[bIdx++] = pop[left++];
    } else {
      b[bIdx++] = pop[right++];
    }
  }

  while (left<=mid) b[bIdx++] = pop[left++];
  while (right<=high) b[bIdx++] = pop[right++];

  for(int k=0; k<n ; k++) pop[low+k] = b[k];

  delete [] b;
}

string NaturalSelection(chromo* pop) {
  double total_fitness=0;
  for (int i=0; i < POP_SIZE ; i++) total_fitness += pop[i].fitness;

  double slice = (double)rand()/RAND_MAX * HARSHNESS * total_fitness;
  double fitness = 0.0;

  for (int j=0; j<POP_SIZE ; j++) {
    fitness += pop[j].fitness;
    if (fitness >= slice) {
      return pop[j].bits;
    }
  }
  return "";
}

string Mutate(string gene) {
  for(int i=0; i < gene.length(); i++){
    if((float)rand()/RAND_MAX < MUTATION_RATE) {
      if(gene[i] == '1') {
        gene[i] = '0';
      } else {
        gene[i] = '1';
      }
    }
  }
  return gene;
}

void Crossover(string &g1, string &g2) {
  if ((float)rand()/RAND_MAX < CROSSOVER_RATE) {
    int point = rand()/RAND_MAX * CHROMO_LENGTH;

    string t1 = g1.substr(0, point) + g2.substr(point, CHROMO_LENGTH);
    string t2 = g2.substr(0, point) + g1.substr(point, CHROMO_LENGTH);
    g1 = t1;
    g2 = t2;
  }
}


void GenerateSeedPopulation(vector<string>& seedpop, string seed, int counter) {
  if (counter == seed.length()-1) {
    seedpop.push_back(seed);
  } else {
    int i = counter;
    while (seed[i] != 'x' && i != seed.length()-1) {
      i++;
    }
    GenerateSeedPopulation(seedpop, seed.replace(i, 1, "1"), i);
    GenerateSeedPopulation(seedpop, seed.replace(i, 1, "0"), i);
  }
  return;
}
