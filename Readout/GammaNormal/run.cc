#include "run.hh"

MyRunAction::MyRunAction()
{
    G4AnalysisManager *man = G4AnalysisManager::Instance();
    man->CreateNtuple("Scoring", "Scoring");
    man->CreateNtupleDColumn("CherenkovPhotonsPerCapturePosition");
    man->FinishNtuple(0);

    man->CreateNtuple("Hits", "Hits");
    man->CreateNtupleDColumn("zPosition");
    man->FinishNtuple(1);
}

MyRunAction::~MyRunAction()
{}

void MyRunAction::BeginOfRunAction(const G4Run* run){
    G4AnalysisManager *man = G4AnalysisManager::Instance();

    G4int runID = run->GetRunID();

    std::stringstream strRunID;  // converts to string i guess?
    strRunID << runID;

    man->OpenFile("output"+strRunID.str()+".csv");
}

void MyRunAction::EndOfRunAction(const G4Run*){
    G4AnalysisManager *man = G4AnalysisManager::Instance();

    man->Write();
    man->CloseFile();
}