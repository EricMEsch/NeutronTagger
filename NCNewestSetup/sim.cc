#include <iostream>

#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4VisManager.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"
#include "Shielding.hh"

#include "construction.hh"
#include "physics.hh"
#include "action.hh"

int main(int argc, char** argv)
{
    G4RunManager *runManager = new G4RunManager();
    /* Initialize all your implemented stuff*/
    runManager->SetUserInitialization(new MyDetectorConstruction()); //Uses your Detectorconstruction
    runManager->SetUserInitialization(new MyPhysicsList());     // Use your physics
    G4VModularPhysicsList *physics = new Shielding();
    runManager->SetUserInitialization(physics);
    runManager->SetUserInitialization(new MyActionInitialization()); // Particle gun

    

    runManager->Initialize();                                // To use the created geometry, needs physics list

    G4UIExecutive *ui = 0;     

    /*only generate grafic output if nothing else is asked*/
    if(argc == 1){
        ui = new G4UIExecutive(argc,argv);
    }
    

    G4VisManager *visManager = new G4VisExecutive();
    visManager->Initialize();

    G4UImanager *UImanager = G4UImanager::GetUIpointer();
    
    if(ui){
        /*Load visualization macro file*/
        UImanager->ApplyCommand("/control/execute vis.mac");
        ui->SessionStart();
    }
    else {
        G4String command = "/control/execute ";
        G4String fileName = argv[1];
        UImanager->ApplyCommand(command+fileName);
        if(argc == 3)
        {
            fileName = argv[2];
            UImanager->ApplyCommand(command+fileName);
        }
    }
    

    return 0;
}