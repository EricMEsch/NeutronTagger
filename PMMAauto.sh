#!/bin/bash
cd ../..
. software/geant4/geant4-v11.0.3-install/share/Geant4-11.0.3/geant4make/geant4make.sh
cd sim/GdDesignStudiesBuild
rm /NCNewestSetup/construction.cc
rm /NCNewestSetup/construction.hh
rm /GammaKaskades/construction.cc
rm /GammaKaskades/construction.hh
cp ../GdDesignStudies/PMMAauto/construction.cc NCNewestSetup/
cp ../GdDesignStudies/PMMAauto/construction.hh NCNewestSetup/
cp ../GdDesignStudies/PMMAauto/construction.cc GammaKaskades/
cp ../GdDesignStudies/PMMAauto/construction.hh GammaKaskades/
mkdir PMMAauto
cp ../GdDesignStudies/PMMAauto/main.cc PMMAauto/
cd NCNewestSetup/build
cmake ..
make -j4
cd ../..
cd GammaKaskades/build
cmake ..
make -j4
cd ../../PMMAauto
g++ -Wall -o main main.cc
for((i=0; i<=10; i++)); do
    for((j= 1; j <= 20; j++)); do
        ./main i j
        cp run.mac ../NCNewestSetup/build
        ../NCNewestSetup/build/sim run.mac
        path="../PMMAautoEva/PMMA"
        file = "${path}width${j}dist${i}.csv"
        mv ../NCNewestSetup/build/output0_nt_Scoring.csv "$file"
    done
done