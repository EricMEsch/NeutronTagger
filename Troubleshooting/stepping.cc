#include "stepping.hh"

MySteppingAction::MySteppingAction(MyEventAction *eventAction){
    fEventAction = eventAction;
}

MySteppingAction::~MySteppingAction()
{}




void MySteppingAction::UserSteppingAction(const G4Step *step){
    if(!step->GetPostStepPoint()->GetTouchableHandle()->GetVolume())
    {
        return;
    }
    /*Get Volume that the Particle enters*/
    const G4LogicalVolume *volume = step->GetPostStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume();
    /*Gets the initialized detectorconstruction in here without exploding everything to check that the particle reached Water Tank Wall (Scoring volume)*/
    const MyDetectorConstruction *detectorConstruction = static_cast<const MyDetectorConstruction*> 
                                                                    (G4RunManager::GetRunManager()->GetUserDetectorConstruction());
    const G4LogicalVolume *fScoringVolume = detectorConstruction->GetScoringVolume();
    G4StepPoint *endPoint = step->GetPostStepPoint();
    G4ThreeVector position = endPoint->GetPosition();
    G4double x_position = position.getX();
    G4double y_position = position.getY();
    G4double z_position = position.getZ();
    G4double radius = std::sqrt(std::pow(x_position, 2) + std::pow(y_position, 2));
    if(radius > 4051)
    {
        G4cout << "Hund: " << radius << "x: " << x_position << "y: " << y_position << "z: " << z_position << G4endl;
    }
    if(volume != fScoringVolume)
        return;

    /*Gets type of Particle to only count Optical Photons*/
    G4Track *track = step->GetTrack();
    const G4ParticleDefinition *particle = track->GetParticleDefinition();
    G4ParticleTable *particleTable = G4ParticleTable::GetParticleTable();
    if(particle == particleTable->FindParticle("opticalphoton"))
    {
        G4StepPoint *preStepPoint = step->GetPreStepPoint();
        G4AnalysisManager *man = G4AnalysisManager::Instance();
        if(preStepPoint->GetKineticEnergy() < 1.239841939*eV/0.3 && preStepPoint->GetKineticEnergy() > 1.239841939*eV/0.5){
            fEventAction->AddnG4Photons(1);
            G4cout << "x: " << x_position << "y: " << y_position << "z: " << z_position << G4endl;

            /*
            if(G4UniformRand()*100 <= 1)
            {
                G4ThreeVector position = endPoint->GetPosition();
                G4double z_position = position.getZ();
                man->FillNtupleDColumn(1,0,z_position);
                man->AddNtupleRow(1);
            }*/
            /*Kill Track so it wont be counted twice*/
        }
    }

}