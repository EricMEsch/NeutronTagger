#include "run.hh"

MyRunAction::MyRunAction()
{
    nGd = 0;
    nWat = 0;
    G4AnalysisManager *man = G4AnalysisManager::Instance();
    man->CreateNtuple("Scoring", "Scoring");
    man->CreateNtupleSColumn("IsotopeName");
    man->CreateNtupleDColumn("X_Position(mm)");
    man->CreateNtupleDColumn("Y_Position(mm)");
    man->CreateNtupleDColumn("Z_Position(mm)");
    man->CreateNtupleDColumn("Energy");
    man->FinishNtuple(0);
}

MyRunAction::~MyRunAction()
{}

void MyRunAction::BeginOfRunAction(const G4Run* run){
    G4AnalysisManager *man = G4AnalysisManager::Instance();

    G4int runID = run->GetRunID();

    std::stringstream strRunID;  // converts to string i guess?
    strRunID << runID;
    nGd = 0;
    nWat = 0;
    man->OpenFile("output"+strRunID.str()+".csv");
}

void MyRunAction::EndOfRunAction(const G4Run*){
    G4AnalysisManager *man = G4AnalysisManager::Instance();

    man->Write();
    man->CloseFile();
    G4cout << "Gd Captures: " << nGd << G4endl;
    G4cout << "Water Captures: " << nWat << G4endl;
}