#include <cmath>
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

int main(int argc, char** argv){
    std::string delimiter = ",";
    std::ofstream outfile{ "run.txt" };
    std::ifstream newfile{ "output0_nt_Scoring.csv" };
    //newfile.open("output0_nt_Scoring.txt", std::ios::in);
    int counterW = 0;
    int counterElse = 0;
    int counterSteel = 0;
    int counterArg = 0;
    int counterGd = 0;
    if(newfile.is_open()){
        std::string s;
        newfile.ignore(1000,'\n');
        newfile.ignore(1000,'\n');
        newfile.ignore(1000,'\n');
        newfile.ignore(1000,'\n');
        newfile.ignore(1000,'\n');
        newfile.ignore(1000,'\n');
        newfile.ignore(1000,'\n');
        newfile.ignore(1000,'\n');
        newfile.ignore(1000,'\n');
        while(getline(newfile, s)){
            size_t pos = 0;
            std::string token;
            pos =s.find(delimiter);
            token = s.substr(0, pos);
            if(token == "H1" || token == "O16"){
                outfile << "1,";
                counterW = counterW + 1;
            }
            else if((token == "Gd154" || token == "Gd155") || (token == "Gd156" || token == "Gd158") || (token == "Gd160")){
                outfile << "0,";
                counterGd = counterGd + 1;
            }
            else if(token == "Gd157"){
                outfile << "3,";
                counterGd = counterGd + 1;
            }
            else if((token.find("Ar") != string::npos)){
                outfile << "4,";
                counterArg = counterArg + 1;
            }
            else if((token.find("Fe") != string::npos)  || (token.find("Cr") != string::npos) || (token.find("Ni") != string::npos) ){
                outfile << "4,";
                counterSteel = counterSteel + 1;
            }
            else {
                outfile << "4,";
                counterElse = counterElse + 1;
            }
            s.erase(0,pos + delimiter.length());
            pos =s.find(delimiter);
            token = s.substr(0, pos);
            //num = std::stod(token);
            outfile << token;
            s.erase(0,pos + delimiter.length());
            pos =s.find(delimiter);
            token = s.substr(0, pos);
            //num = std::stod(token);
            outfile << "," << token;
            s.erase(0,pos + delimiter.length());
            //num = std::stod(s);
            outfile << "," << s << std::endl;
        }
        newfile.close();
        outfile.close();
        std::cout << "Amount of Water Captures: " << counterW << std::endl;
        std::cout << "Amount of Gd Captures: " << counterGd << std::endl;
        std::cout << "Amount of Argon Captures: " << counterArg << std::endl;
        std::cout << "Amount of Steel Captures: " << counterSteel << std::endl;
        std::cout << "Amount of Else Captures: " << counterElse << std::endl;
        std::ofstream outfile{ "run.mac" };
        if(argc == 3)
        {
            int j = stoi(argv[2]);
            int i = stoi(argv[1]);
            outfile << "/detector/plateThick " << j*10 << endl;
            outfile << "/detector/plateDist " << i*10 << endl;
            outfile << "/run/reinitializeGeometry" << endl;
        }
        else
        {
            cout << "Wrong argument count noob" << endl;
        }
        outfile << "/run/beamOn " << counterW + counterGd +counterArg + counterSteel + counterElse << std::endl;
        outfile.close();
    }
    else{
        std::cout << "WTF" << std::endl;
    }
    return 0;
}