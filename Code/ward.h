#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <array>
#include <cmath>




using namespace std;

class Ward {
public:
    int ID;
    char region;
    double booze, gambling, hooker,crooked;

    Ward(){}

    Ward(int ID, char region, double booze, double gambling, double hooker, int crooked){
      this->ID = ID;
      this->region = region;
      this->booze = booze;
      this->gambling = gambling;
      this->hooker = hooker;
      this->crooked = crooked;
      }
};


class Bin{
public:
    vector<int> bin;
    double score, btotal, gtotal, htotal, ctotal;

};


class Calculator {
public:
    vector<Ward> wardlist;
    double boozetotal, gamblingtotal, hookertotal, crookedtotal, wardtotal, boozeCap, gamblingCap, hookerCap, crookedCap, wardCap;
    bool isBig;

    //scale 3 of the 4

    Calculator(bool isBig, string filename) {
      load_data(filename);
      this->isBig = isBig;

      double bt=0, gt=0, ht=0, ct=0;

      for(int i=0; i<wardlist.size(); i++){
        bt += wardlist[i].booze;
        gt += wardlist[i].gambling;
        ht += wardlist[i].hooker;
        ct += wardlist[i].crooked;
      }

      if(isBig){
        this->boozeCap = 0.6*bt;
        this->gamblingCap = 0.6*gt;
        this->hookerCap = 0.6*ht;
        this->crookedCap = 0.6*ct;
        this->wardCap = 0.6*(wardlist.size());
      } else {
        this->boozeCap = 0.4*bt;
        this->gamblingCap = 0.4*gt;
        this->hookerCap = 0.4*ht;
        this->crookedCap = 0.4*ct;
        this->wardCap = 0.4*(wardlist.size());
      }
    }

    // calculate score
    // return double
    double getscore(vector<int> bin){
      double b=0, g=0, h=0, c=0, score = 0;
      int w=0;
      for (int i=0; i < bin.size(); i++) {
        //compute error
        b += bin[i] * wardlist[i].booze;
        g += bin[i] * wardlist[i].gambling;
        h += bin[i] * wardlist[i].hooker;
        c += bin[i] * wardlist[i].crooked;
        w += bin[i];
      }

      // L1 norm of the errors
      score = abs((b-boozeCap)) + abs((g - gamblingCap)) + abs((h - hookerCap)) + abs((c - crookedCap)) + abs(w-wardCap);
      return score;
    }


    // load data into a vector
    void load_data(string filename) {
      ifstream infile;
      infile.open(filename.c_str());
      string line;
      double booze, gamble, hooker;
      char c, region;
      int ID, crooked;
      vector<Ward> data;

      if(infile.is_open()){
        getline(infile, line);
        int count = 0;
        while(getline(infile, line)) {
          count++;
          stringstream x(line);
          x >> region >> c >> ID >> c >> booze >> c>>gamble>> c >> hooker >> c >> crooked;
          Ward newward = Ward(ID, region, booze, gamble, hooker, crooked);
          data.push_back(newward);
        }
        infile.close();
      } else {
        cout << "cant open" << endl;
      }
      this->wardlist = data;
    }
 };
