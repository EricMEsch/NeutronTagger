#!/bin/bash
cd ../..
. software/geant4/geant4-v11.0.3-install/share/Geant4-11.0.3/geant4make/geant4make.sh
cd sim/GdDesignStudies
rm /NCNewestSetup/construction.cc
rm /NCNewestSetup/construction.hh
rm /GammaKaskades/construction.cc
rm /GammaKaskades/construction.hh
cp Optics/construction.cc NCNewestSetup/
cp Optics/construction.hh NCNewestSetup/
cp Optics/construction.cc GammaKaskades/
cp Optics/construction.hh GammaKaskades/
cd NCNewestSetup/build
cmake ..
make -j4
./sim run.mac
rm ../../WWShowers/output0_nt_Scoring.csv
cp output0_nt_Scoring.csv ../../WWShowers
cd ../../WWShowers
./Converter
rm ../GammaKaskades/build/run.txt
rm ../GammaKaskades/build/run.mac
cp run.mac ../GammaKaskades/build/
cp run.txt ../GammaKaskades/build/
cd ../GammaKaskades/build/
cmake ..
make -j4
./sim run.mac