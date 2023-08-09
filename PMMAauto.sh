#!/bin/bash
cd ../..
. software/geant4/geant4-v11.0.3-install/share/Geant4-11.0.3/geant4make/geant4make.sh
cd sim/GdDesignStudiesBuild
cp ../GdDesignStudies/normalgenerator/NC/generator.cc NCNewestSetup/
cp ../GdDesignStudies/normalgenerator/NC/generator.hh NCNewestSetup/
cp ../GdDesignStudies/PMMAauto/construction.cc NCNewestSetup/
cp ../GdDesignStudies/PMMAauto/construction.hh NCNewestSetup/
cp ../GdDesignStudies/PMMAauto/construction.cc GammaKaskades/
cp ../GdDesignStudies/PMMAauto/construction.hh GammaKaskades/
mkdir PMMAauto
cp ../GdDesignStudies/PMMAauto/main.cc PMMAauto/
cp ../GdDesignStudies/PMMAauto/Realconverter.cc PMMAauto/
cd NCNewestSetup/build
cmake ..
make -j4
cd ../..
cd GammaKaskades/build
cmake ..
make -j4
cd ../../PMMAauto
g++ -Wall -o main main.cc
for((i=2; i<=2; i =i+2)); do
    for((j= 3; j <= 5; j=j+2)); do
        ./main ${i} ${j}
        cp run.mac ../NCNewestSetup/build
        cd ../NCNewestSetup/build
        ./sim run.mac
        cd ../../PMMAauto
        rm ./output0_nt_Scoring.csv
        cp ../NCNewestSetup/build/output0_nt_Scoring.csv ./
        g++ -Wall -o Converter Realconverter.cc
        ./Converter ${i} ${j}
        rm ../GammaKaskades/build/run.txt
        rm ../GammaKaskades/build/run.mac
        cp run.mac ../GammaKaskades/build/
        cp run.txt ../GammaKaskades/build/
        cd ../GammaKaskades/build
        ./sim run.mac
        cd ../../PMMAautoEva
        name="PMMA"
        file="${name}width${j}dist${i}.csv"
        cp ../GammaKaskades/build/output0_nt_Scoring.csv ./
        mv output0_nt_Scoring.csv "$file"
        cd ../PMMAauto
    done
done