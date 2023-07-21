#ifndef GENERATOR_HH
#define GENERATOR_HH

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4Event.hh"

#include "G4ParticleGun.hh"
#include "G4SystemOfUnits.hh"
#include "G4ParticleTable.hh"
#include "globals.hh"
#include "G4PhysicalConstants.hh"
#include "Randomize.hh"
#include <iostream>
#include <fstream>
#include <string>

class MyPrimaryGenerator : public G4VUserPrimaryGeneratorAction{
    public:
        MyPrimaryGenerator();
        ~MyPrimaryGenerator();


        virtual void GeneratePrimaries(G4Event *anEvent);

    private:
        G4ParticleGun *fParticleGun;
        std::fstream newfile;
        G4int water;
        G4double x_Position;
        G4double y_Position;
        G4double z_Position;

};

#endif