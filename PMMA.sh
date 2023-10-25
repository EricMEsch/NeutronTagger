#!/bin/bash
cd ../..
. software/geant4/geant4-v11.0.3-install/share/Geant4-11.0.3/geant4make/geant4make.sh
cd sim/GdDesignStudiesBuild
cp ../GdDesignStudies/Randomgenerator/NC/generator.cc NCNewestSetup/
cp ../GdDesignStudies/Randomgenerator/NC/generator.hh NCNewestSetup/
cp ../GdDesignStudies/PMMA/construction.cc NCNewestSetup/
cp ../GdDesignStudies/PMMA/construction.hh NCNewestSetup/
cp ../GdDesignStudies/PMMA/construction.cc GammaKaskades/
cp ../GdDesignStudies/PMMA/construction.hh GammaKaskades/
cp ../GdDesignStudies/WWShowers/Realconverter.cc WWShowers/
cp ../GdDesignStudies/Optics/Evaluator.cc Optics/
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
cp output0_nt_Scoring.csv ../../Optics
cd ../../Optics
mv output0_nt_Scoring.csv PMMAPlatenodist0.txt
g++ -Wall -o ev Evaluator.cc