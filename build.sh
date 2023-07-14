#!/bin/bash
cd ..
mkdir GdDesignStudiesBuild
cp -n GdDesignStudies/Gammakaskades GdDesignStudiesBuild
cp -n GdDesignStudies/NCNewestSetup GdDesignStudiesBuild
cp -n GdDesignStudies/Optics GdDesignStudiesBuild
cp -n GdDesignStudies/WWShowers GdDesignStudiesBuild
cd GdDesignStudiesBuild
if [ ! -d Plots ]; then
    mkdir Plots
fi