#include "physics.hh"

MyPhysicsList::MyPhysicsList(){
    RegisterPhysics(new G4EmStandardPhysics()); /* Adds electromagnetic interaction*/
    RegisterPhysics(new G4OpticalPhysics());
}

MyPhysicsList::~MyPhysicsList()
{}