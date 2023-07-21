#include "generator.hh"

MyPrimaryGenerator::MyPrimaryGenerator(){
    fParticleGun = new G4ParticleGun(1);  //Number of particles per Event


    G4ParticleTable *particleTable = G4ParticleTable::GetParticleTable(); // Load the particle table with all stored properties
    G4String particleName ="gamma";
    G4ParticleDefinition *particle = particleTable->FindParticle(particleName); // Chose the particle

    /*These values will be overwritten later*/
    G4ThreeVector pos(0.,0.,0.);                                 // Where to creat particle
    G4ThreeVector mom(0.,0.,1.);                                //Momentum of particle

    /*Generic Messenger to either simulate water or Gd*/
    water = 1;

    /*These values will be overwritten later*/
    fParticleGun->SetParticlePosition(pos);
    fParticleGun->SetParticleMomentumDirection(mom); 
    fParticleGun->SetParticleMomentum(2.2*MeV);              // Energy of Particle
    fParticleGun->SetParticleDefinition(particle);            //Which particle to shoot
}

MyPrimaryGenerator::~MyPrimaryGenerator(){
    delete fParticleGun;
}

void MyPrimaryGenerator::GeneratePrimaries(G4Event *anEvent){
    /*Event Id corresponds to neutron capture event*/
    G4int eventID = anEvent->GetEventID();
    newfile.open("run.txt", std::ios::in);
    G4int average = 1;
    
    /*Takes run file and takes out position and if capture was on water*/
    
    if(newfile.is_open()){
            std::string str;
            int step = 0;
            std::string delimiter = ",";

            while(step != eventID){
                newfile.ignore(1000,'\n');
                step++;
            } 
            /*Reads out line*/
            if(getline(newfile, str) && step == eventID){
                size_t pos = 0;
                std::string token;
                pos =str.find(delimiter);
                token = str.substr(0, pos);
                if(token.empty() != true){
                    water = std::stoi(token);
                }                    
                str.erase(0,pos + delimiter.length());
                pos =str.find(delimiter);
                token = str.substr(0, pos);
                if(token.empty() != true){                            
                    x_Position = std::stod(token);       
                }
                str.erase(0,pos + delimiter.length());
                pos =str.find(delimiter);
                token = str.substr(0, pos);
                if(token.empty() != true){                            
                    y_Position = std::stod(token);       
                }
                str.erase(0,pos + delimiter.length());
                pos =str.find(delimiter);
                token = str.substr(0, pos);
                if(token.empty() != true){                            
                    z_Position = std::stod(token);       
                }
            }
            newfile.close();
        }
    
    G4ThreeVector posi(x_Position*mm,y_Position*mm,z_Position*mm); 
    
    fParticleGun->SetParticlePosition(posi);

    /*If Neutron capture was on water or Gd simulate respectively*/
    if(water == 1){
        fParticleGun->SetParticleEnergy(2.2*MeV);
        int step = 0;
        /*Simulate each neutron capture 1000 times*/
        while(step < average){
            /*Random generator to shot gamma isotropicaly (im not sure if this way is perfectly isotropic)*/
            G4double cosTheta = 2*G4UniformRand() - 1., phi = twopi*G4UniformRand();
            G4double sinTheta = std::sqrt(1. - cosTheta*cosTheta);
            G4double ux = sinTheta*std::cos(phi),
                     uy = sinTheta*std::sin(phi),
                     uz = cosTheta;
        
            fParticleGun->SetParticleMomentumDirection(G4ThreeVector(ux,uy,uz));
            fParticleGun->GeneratePrimaryVertex(anEvent);
            step++;
        }
    }
    
    /*Some unefficent implementation to read in Peters List*/
    else if(water == 0){
        newfile.open("156Gd-5keV-cascades.txt", std::ios::in);
        std::string delimiter = "  ";
        if(newfile.is_open()){
            std::string str;
            int step = 0;
            int ran = 0;
            int line = eventID + static_cast<int>(G4UniformRand()*1000000);
            while(ran != line){
                newfile.ignore(1000,'\n');
                ran++;
            } 
            /*Simulate each neutron capture 1000 times*/
            while(getline(newfile, str) && step < average){
                str.erase(0,22);
                size_t pos = 0;
                std::string token;
                while(true){
                    pos =str.find(delimiter);
                    if(pos != std::string::npos) {
                        token = str.substr(0, pos);
                        if(token.empty() != true){
                            int num = std::stoi(token);
                            if(num > 250)
                            {
                                fParticleGun->SetParticleEnergy(num*keV);
                                G4double cosTheta = 2*G4UniformRand() - 1., phi = twopi*G4UniformRand();
                                G4double sinTheta = std::sqrt(1. - cosTheta*cosTheta);
                                G4double ux = sinTheta*std::cos(phi),
                                         uy = sinTheta*std::sin(phi),
                                         uz = cosTheta;

                                fParticleGun->SetParticleMomentumDirection(G4ThreeVector(ux,uy,uz));
                                fParticleGun->GeneratePrimaryVertex(anEvent);
                            }
                            
                        }
                        str.erase(0,pos + delimiter.length());
                    }
                    else{
                        int num = std::stoi(str);
                        if(num > 250)
                        {
                            fParticleGun->SetParticleEnergy(num*keV);
                            G4double cosTheta = 2*G4UniformRand() - 1., phi = twopi*G4UniformRand();
                            G4double sinTheta = std::sqrt(1. - cosTheta*cosTheta);
                            G4double ux = sinTheta*std::cos(phi),
                                     uy = sinTheta*std::sin(phi),
                                     uz = cosTheta;

                            fParticleGun->SetParticleMomentumDirection(G4ThreeVector(ux,uy,uz));
                            fParticleGun->GeneratePrimaryVertex(anEvent);
                        }
                        break;
                    }
                }
                step++;

            }
            newfile.close();
        }
    }
    /*if water == 3 the capture was on Gd158*/
    else if(water == 3){
        newfile.open("158Gd-5keV-cascades.txt", std::ios::in);
        std::string delimiter = "  ";
        if(newfile.is_open()){
            std::string str;
            int step = 0;
            int ran = 0;
            int line = eventID + static_cast<int>(G4UniformRand()*1000000);
            while(ran != line){
                newfile.ignore(1000,'\n');
                ran++;
            } 
            /*Simulate each neutron capture 1000 times*/
            while(getline(newfile, str) && step < average){
                str.erase(0,22);
                size_t pos = 0;
                std::string token;
                while(true){
                    pos =str.find(delimiter);
                    if(pos != std::string::npos) {
                        token = str.substr(0, pos);
                        if(token.empty() != true){
                            int num = std::stoi(token);
                            if( num > 250)
                            {
                                fParticleGun->SetParticleEnergy(num*keV);
                                G4double cosTheta = 2*G4UniformRand() - 1., phi = twopi*G4UniformRand();
                                G4double sinTheta = std::sqrt(1. - cosTheta*cosTheta);
                                G4double ux = sinTheta*std::cos(phi),
                                         uy = sinTheta*std::sin(phi),
                                         uz = cosTheta;

                                fParticleGun->SetParticleMomentumDirection(G4ThreeVector(ux,uy,uz));
                                fParticleGun->GeneratePrimaryVertex(anEvent);
                            }
                        }
                        str.erase(0,pos + delimiter.length());
                    }
                    else{
                        int num = std::stoi(str);
                        if(num > 250)
                        {
                            fParticleGun->SetParticleEnergy(num*keV);
                            G4double cosTheta = 2*G4UniformRand() - 1., phi = twopi*G4UniformRand();
                            G4double sinTheta = std::sqrt(1. - cosTheta*cosTheta);
                            G4double ux = sinTheta*std::cos(phi),
                                     uy = sinTheta*std::sin(phi),
                                     uz = cosTheta;

                            fParticleGun->SetParticleMomentumDirection(G4ThreeVector(ux,uy,uz));
                            fParticleGun->GeneratePrimaryVertex(anEvent);
                        }
                        break;
                    }
                }
                step++;

            }
            newfile.close();
        }
    }
    else{
        G4cout << "No Particle" << G4endl;
    }
    
}
