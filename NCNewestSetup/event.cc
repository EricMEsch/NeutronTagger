#include "event.hh"

MyEventAction::MyEventAction(MyRunAction* runAction){
    nGd = 0;
    nWat = 0;
    targetname = "XXXX";
    x_Position = 0.;
    y_Position = 0.;
    z_Position = 0.;
    energy = 0.;
    fRunAction = runAction;
}

MyEventAction::~MyEventAction(){
}

void MyEventAction::BeginOfEventAction(const G4Event*){
    nGd = 0;
    nWat = 0;
}

void MyEventAction::EndOfEventAction(const G4Event*){
    G4AnalysisManager *man = G4AnalysisManager::Instance();
    fRunAction->AddnGd(nGd);
    fRunAction->AddnWat(nWat);
    man->AddNtupleRow(0);
}