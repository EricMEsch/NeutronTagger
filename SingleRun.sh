#!/bin/bash
cd ../..
. software/geant4/geant4-v11.0.3-install/share/Geant4-11.0.3/geant4make/geant4make.sh
cd sim/GdDesignStudiesBuild
cp ../GdDesignStudies/Optics/construction.cc NCNewestSetup/
cp ../GdDesignStudies/Optics/construction.hh NCNewestSetup/
cp ../GdDesignStudies/Optics/construction.cc GammaKaskades/
cp ../GdDesignStudies/Optics/construction.hh GammaKaskades/
cp ../GdDesignStudies/WWShowers/Realconverter.cc WWShowers/
cd NCNewestSetup/build
cmake ..
make -j4
./sim run.mac
rm ../../WWShowers/output0_nt_Scoring.csv
cp output0_nt_Scoring.csv ../../WWShowers
cd ../../WWShowers
g++ -Wall -o Converter Realconverter.cc
./Converter
rm ../GammaKaskades/build/run.txt
rm ../GammaKaskades/build/run.mac
cp run.mac ../GammaKaskades/build/
cp run.txt ../GammaKaskades/build/
cd ../GammaKaskades/build/
cmake ..
make -j4
./sim run.mac