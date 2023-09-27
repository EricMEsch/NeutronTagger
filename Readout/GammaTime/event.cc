#include "event.hh"

MyEventAction::MyEventAction(MyRunAction*){
    nG4Photons = 0;
    firstPhotonTime = 0.;
    lastPhotonTime = 0.;
}

MyEventAction::~MyEventAction(){
}

void MyEventAction::BeginOfEventAction(const G4Event*){
    nG4Photons = 0;
    firstPhotonTime = 0.;
    lastPhotonTime = 0.;
}


void MyEventAction::setFirstPhotonTime(G4double time)
{
    if(firstPhotonTime == 0.)
    {
        firstPhotonTime = time;
    }
    else if (time < firstPhotonTime)
    {
        firstPhotonTime = time;
    }
    
}

void MyEventAction::setLastPhotonTime(G4double time)
{
    if(lastPhotonTime == 0.)
    {
        lastPhotonTime = time;
    }
    else if (time > lastPhotonTime)
    {
        lastPhotonTime = time;
    }
    
}


void MyEventAction::EndOfEventAction(const G4Event*){
    G4AnalysisManager *man = G4AnalysisManager::Instance();
    G4double result =nG4Photons/1.;
    man->FillNtupleDColumn(0,0,result);
    man->FillNtupleDColumn(0,1,firstPhotonTime);
    man->FillNtupleDColumn(0,2,lastPhotonTime);

    man->AddNtupleRow(0);
}