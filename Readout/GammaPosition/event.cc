#include "event.hh"

MyEventAction::MyEventAction(MyRunAction*){
    nG4Photons = 0;
    z_Position = 0;
}

MyEventAction::~MyEventAction(){
}

void MyEventAction::BeginOfEventAction(const G4Event*){
    nG4Photons = 0;
    z_Position = 0;
}

void MyEventAction::EndOfEventAction(const G4Event*){
    G4AnalysisManager *man = G4AnalysisManager::Instance();
    G4double result =nG4Photons/1.;
    man->FillNtupleDColumn(0,0,result);
    man->FillNtupleDColumn(0,1,z_Position);
    man->AddNtupleRow(0);
}