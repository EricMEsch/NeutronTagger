#!/bin/bash
cd ../..
. software/geant4/geant4-v11.0.3-install/share/Geant4-11.0.3/geant4make/geant4make.sh
cd sim/GdDesignStudiesBuild
cp ../GdDesignStudies/normalgenerator/NC/generator.cc NCNewestSetup/
cp ../GdDesignStudies/normalgenerator/NC/generator.hh NCNewestSetup/
cp ../GdDesignStudies/PMMA/construction.cc NCNewestSetup/
cp ../GdDesignStudies/PMMA/construction.hh NCNewestSetup/
cp ../GdDesignStudies/PMMA/construction.cc GammaKaskades/
cp ../GdDesignStudies/PMMA/construction.hh GammaKaskades/
cd NCNewestSetup/build
cmake ..
make -j4
cd ../..
cd GammaKaskades/build
cmake ..
make -j4
cd ../../PMMAauto
g++ -Wall -o main main.cc
for((i=0; i<=0; i =i+2)); do
    for((j= 1; j <= 15; j=j+2)); do
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
        cd ../../PMMAautoEva0
        name="PMMA"
        file="${name}width${j}dist${i}.csv"
        cp ../GammaKaskades/build/output0_nt_Scoring.csv ./
        mv output0_nt_Scoring.csv "$file"
        cd ../PMMAauto
    done
done