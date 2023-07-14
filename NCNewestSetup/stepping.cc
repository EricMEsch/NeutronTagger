#include "stepping.hh"

MySteppingAction::MySteppingAction(MyEventAction *eventAction){
    fEventAction = eventAction;
}

MySteppingAction::~MySteppingAction()
{}


void MySteppingAction::UserSteppingAction(const G4Step *step){
    /*Gets the Volume that was entered at StepPoint*/
    G4LogicalVolume *volume = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume();

    G4Track *track = step->GetTrack();

    G4StepPoint *preStepPoint = step->GetPreStepPoint();
    const G4StepPoint* endPoint = step->GetPostStepPoint();
    G4VProcess* process = const_cast<G4VProcess*>(endPoint->GetProcessDefinedStep());

    const G4ParticleDefinition *particle = track->GetParticleDefinition();
    G4ParticleTable *particleTable = G4ParticleTable::GetParticleTable(); // Load the particle table with all stored properties
    G4String particleName ="neutron";
    G4ParticleDefinition *requiredParticle = particleTable->FindParticle(particleName);

    /*Gets the initialized detectorconstruction in here without exploding everything*/
    const MyDetectorConstruction *detectorConstruction = static_cast<const MyDetectorConstruction*> 
                                                                    (G4RunManager::GetRunManager()->GetUserDetectorConstruction());

    G4LogicalVolume *fScoringVolume = detectorConstruction->GetScoringVolume();

    if(volume == fScoringVolume)
    {
        track->SetTrackStatus(fStopAndKill);
        if(particle == requiredParticle)
            G4cout << "ALARM" << G4endl;
        return;
    }

    G4StepStatus stepStatus = endPoint->GetStepStatus();
    G4bool transmit = (stepStatus==fGeomBoundary || stepStatus==fWorldBoundary);
    if (transmit) return;
    
    G4AnalysisManager *man = G4AnalysisManager::Instance();

    if(particle == requiredParticle && step->GetTrack()->GetTrackStatus() == fStopAndKill && process->GetProcessName() == "nCapture"){
        G4HadronicProcess* hproc = dynamic_cast<G4HadronicProcess*>(process);
        const G4Isotope* target = NULL;
        if (hproc) target = hproc->GetTargetIsotope();
        G4String targetName = "XXXX";  
        if (target) {
            targetName = target->GetName();
            // G4cout << targetName << G4endl;
            man->FillNtupleSColumn(0,0,targetName);
            G4ThreeVector position = endPoint->GetPosition();
            G4double x_position = position.getX();
            G4double y_position = position.getY();
            G4double z_position = position.getZ();
            man->FillNtupleDColumn(0,1,x_position);
            man->FillNtupleDColumn(0,2,y_position);
            man->FillNtupleDColumn(0,3,z_position);
            G4double energy = preStepPoint->GetKineticEnergy();
            man->FillNtupleDColumn(0,4,energy);
            if(targetName.contains("Gd"))
            {
                fEventAction->AddnGd(1);
            }
            if(targetName.contains("H1") || targetName.contains("O16"))
            {
                fEventAction->AddnWat(1);
            }
        }
    }


}