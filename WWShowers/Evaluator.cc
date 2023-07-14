#include <cmath>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

int main(){
    vector<int> result;
    std::string delimiter = ",";
    std::ofstream outfile{ "showers.txt" };
    std::ifstream infile{ "../GammaKaskades/build/output0_nt_Scoring.csv" };
    std::ifstream muonfile{ "../NCNewestSetup/build/neutrom_inbrot.txt" };
    if(infile.is_open() && muonfile.is_open())
    {
        int lastMuonNumber = 0;
        string s;
        string sm;
        infile.ignore(1000,'\n');
        infile.ignore(1000,'\n');
        infile.ignore(1000,'\n');
        infile.ignore(1000,'\n');
        infile.ignore(1000,'\n');
        while(getline(infile, s))
        {
            getline(muonfile, sm);
            size_t pos = 0;
            int currentMuonNumber = 0;
            int token = 0;
            pos =sm.find(delimiter);
            currentMuonNumber = stoi(sm.substr(0, pos));
            token = stoi(s.substr(0, pos));
            if(currentMuonNumber != lastMuonNumber)
            {
                result.push_back(token);
                lastMuonNumber = currentMuonNumber;
            }
            else
            {
                result[currentMuonNumber-1] = result[currentMuonNumber-1] + token;
            }
        }
        infile.close();
        muonfile.close();
    }
    else
    {
        cout << "Problem" << endl;
    }
    for(int n : result)
    {
        outfile << n << endl;
    }
    outfile.close();
       
    return 0;
}