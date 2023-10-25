#include "generator.hh"

MyPrimaryGenerator::MyPrimaryGenerator(){
    fParticleGun = new G4ParticleGun(1);  //Number of particles per Event


    G4ParticleTable *particleTable = G4ParticleTable::GetParticleTable(); // Load the particle table with all stored properties
    G4String particleName ="neutron";
    G4ParticleDefinition *particle = particleTable->FindParticle(particleName); // Chose the particle

    G4ThreeVector pos(0.,0.,0.);                                 // Where to creat particle
    G4ThreeVector mom(0.,0.,1.);                                //Momentum of particle

    fParticleGun->SetParticlePosition(pos);
    fParticleGun->SetParticleMomentumDirection(mom); 
    fParticleGun->SetParticleMomentum(0.8*MeV);              // Energy of Particle
    fParticleGun->SetParticleDefinition(particle);            //Which particle to shoot
    newfile = new std::fstream;
    newfile->open("combined_all_muons.csv", std::ios::in);
}

MyPrimaryGenerator::~MyPrimaryGenerator(){
    delete fParticleGun;
    newfile->close();
    delete newfile;
}

void MyPrimaryGenerator::GeneratePrimaries(G4Event *anEvent){

    G4int eventID = anEvent->GetEventID();
    
    std::string delimiter = ",";
    if(newfile->is_open()){
        std::string str;
        if(getline(*newfile, str))
        {
            size_t pos = 0;
            pos =str.find(delimiter);
            int mu = std::stoi(str.substr(0, pos));
                
            str.erase(0,pos + delimiter.length());
            pos =str.find(delimiter);
            G4double xPos = std::stod(str.substr(0, pos))*m;

            str.erase(0,pos + delimiter.length());
            pos =str.find(delimiter);
            G4double yPos = std::stod(str.substr(0, pos))*m;

            str.erase(0,pos + delimiter.length());
            pos =str.find(delimiter);
            G4double zPos = std::stod(str.substr(0, pos))*m;

            str.erase(0,pos + delimiter.length());
            pos =str.find(delimiter);
            G4double xMom = std::stod(str.substr(0, pos));

            str.erase(0,pos + delimiter.length());
            pos =str.find(delimiter);
            G4double yMom = std::stod(str.substr(0, pos));

            str.erase(0,pos + delimiter.length());
            pos =str.find(delimiter);
            G4double zMom = std::stod(str.substr(0, pos));

            str.erase(0,pos + delimiter.length());
            pos =str.find(delimiter);
            G4double energy = std::stod(str.substr(0, pos))*eV;

            G4double radius = std::sqrt(xPos * xPos + yPos * yPos);
            G4ThreeVector position(xPos,yPos,zPos);                                 // Where to creat particle
            G4ThreeVector mom(xMom,yMom,zMom);                                //Momentum of particle

            //Do not create neutrons that are too far from detector setup
            if( radius > 4.2*m || std::fabs(zPos) > 6.4*m)
            {
                G4cout << "Out of bounds" << G4endl;
            }
            else
            {
                fParticleGun->SetParticlePosition(position);
                fParticleGun->SetParticleMomentumDirection(mom); 
                fParticleGun->SetParticleEnergy(energy); 
                fParticleGun->GeneratePrimaryVertex(anEvent);
            }
            
        }
    }
}
