#ifndef CONSTRUCTION_HH
#define CONSTRUCTION_HH

#include "G4VUserDetectorConstruction.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
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

        G4double platehHeight,platehDepth,platehThick;
        G4double plateNum;
        G4double plateDist;


        G4LogicalVolume *GetScoringVolume() const{ return fScoringVolume;}

        virtual G4VPhysicalVolume *Construct();
    
    private:

        
        G4Tubs *solidDetector, *solidWatertank, *solidCryoborder, *solidCryo, *solidouterCryoborder, *solidVacGap;
        G4Box *solidWorld, *solidPlates, *solidtopPlates;
        G4LogicalVolume *logicWorld;
        G4VPhysicalVolume *physWorld, *physDetector, *physWatertank, *physPlates, *physCryoborder, *physCryo, *phystopPlates, *physbotPlates, *physouterCryoborder, *physVacGap;

        G4LogicalVolume *fScoringVolume, *logicDetector, *logicWatertank, *logicPlates, *logicCryoborder, *logicCryo, *logictopPlates, *logicouterCryoborder, *logicVacGap;

        G4GenericMessenger *fMessenger;

        void defineMaterials();
        G4Material *H2O, *Gdmat, *Steelmat, *Airmat, *worldMat, *lAr, *Gdsol, *vac;
        G4Element *elGd;
};

#endif