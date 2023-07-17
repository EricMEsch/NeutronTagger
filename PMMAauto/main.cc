#include <cmath>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

int main(int argc, char** argv){
    std::ofstream outfile{ "run.mac" };
    if(argc == 3)
    {
        if(outfile.is_open())
        {
            int i = stoi(argv[1]);
            int j = stoi(argv[2]);
            outfile << "/detector/plateThick " << j*10 << endl;
            outfile << "/detector/plateDist " << i*10 << endl;
            outfile << "/run/reinitializeGeometry" << endl;
            outfile << "/run/beamOn 100000" << endl;
        }
    }
    else
    {
        cout << "Wrong argument count noob" << endl;
    }
    outfile.close();
       
    return 0;
}