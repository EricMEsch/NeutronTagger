#ifndef CONSTRUCTION_HH
#define CONSTRUCTION_HH

#include "G4VUserDetectorConstruction.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4OpticalSurface.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4PVPlacement.hh"
#include "G4NistManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4GenericMessenger.hh"
#include <cmath>

#include "detector.hh"

class MyDetectorConstruction : public G4VUserDetectorConstruction {
    public:
        MyDetectorConstruction();
        ~MyDetectorConstruction();

        G4double plateThick;
        G4double plateDist;


        G4LogicalVolume *GetScoringVolume() const{ return fScoringVolume;}

        virtual G4VPhysicalVolume *Construct();
    
    private:

        
        G4Tubs *solidDetector, *solidWatertank, *solidCryoborder, *solidCryo, *solidouterCryoborder, *solidVacGap, *solidPMMA, *solidInnerWatertank;
        G4Box *solidWorld;
        G4LogicalVolume *logicWorld;
        G4VPhysicalVolume *physWorld, *physDetector, *physWatertank, *physCryoborder, *physCryo, *physouterCryoborder, *physVacGap, *physPMMA;
        G4VPhysicalVolume *physInnerWatertank;

        G4LogicalVolume *fScoringVolume, *logicDetector, *logicWatertank, *logicCryoborder, *logicCryo, *logicouterCryoborder;
        G4LogicalVolume *logicVacGap, *logicPMMA, *logicInnerWatertank;
        G4GenericMessenger *fMessenger;

        void defineMaterials();
        G4Material *H2O, *Gdmat, *Steelmat, *Airmat, *worldMat, *lAr, *Gdsol, *vac, *PMMA, *loadedPMMA;
        G4Element *elGd, *C, *H, *O;
};

#endif