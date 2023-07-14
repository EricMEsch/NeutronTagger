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
    newfile->open("neutrom_inbrot.txt", std::ios::in);
}

MyPrimaryGenerator::~MyPrimaryGenerator(){
    delete fParticleGun;
    newfile->close();
    delete newfile;
}

void MyPrimaryGenerator::GeneratePrimaries(G4Event *anEvent){

    G4int eventID = anEvent->GetEventID();
    
    std::string delimiter = " ";
    if(newfile->is_open()){
        std::string str;
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(1, 800000);
        int lineNumber = dis(gen);
        newfile->seekg(std::ios::beg);
        for(int i = 1; i < lineNumber; i++)
        {
            std::getline(*newfile, str);
        }
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


            G4ThreeVector position(xPos,yPos,zPos);                                 // Where to creat particle
            G4ThreeVector mom(xMom,yMom,zMom);                                //Momentum of particle

            fParticleGun->SetParticlePosition(position);
            fParticleGun->SetParticleMomentumDirection(mom); 
            fParticleGun->SetParticleEnergy(energy); 
            G4cout << "Mu: " << mu << G4endl; 
            fParticleGun->GeneratePrimaryVertex(anEvent);
        }
    }
}
