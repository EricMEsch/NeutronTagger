#include "construction.hh"

MyDetectorConstruction::MyDetectorConstruction() //Constructor
{
    fMessenger = new G4GenericMessenger(this, "/detector/", "Detector Construction");

    fMessenger->DeclareProperty("platehHeight", platehHeight, "half height of plates");
    fMessenger->DeclareProperty("platehDepth", platehDepth, "half depth of plates");
    fMessenger->DeclareProperty("platehThick", platehThick, "half Thickness of plates");
    fMessenger->DeclareProperty("plateNum", plateNum, "Number of Plates");
    fMessenger->DeclareProperty("plateDist", plateDist, "Distance from Cryostat");

    platehHeight = 325*cm;
    platehDepth = 5*cm;
    platehThick = 0.01*cm;
    plateNum = 750.;
    plateDist = 0.;

    defineMaterials();
}

MyDetectorConstruction::~MyDetectorConstruction() //Destructor
{
    delete fMessenger;
}

void MyDetectorConstruction::defineMaterials(){
    G4NistManager *nist = G4NistManager::Instance(); /*NistManager to use prebuild materials (instead you could create own materials)*/ 

    H2O = new G4Material("H2O", 1.000*g/cm3, 2);
    H2O->AddElement(nist->FindOrBuildElement("H"), 2); 
    H2O->AddElement(nist->FindOrBuildElement("O"), 1);


    /*Use a Material of the Nistmanager*/


    /*Define Gadolinium with natural abundances if enriched Gd is used*/
    
    /*
    G4Isotope *Gd154 = new G4Isotope("Gd154", 64, 154);
    G4Isotope *Gd155 = new G4Isotope("Gd155", 64, 155);
    G4Isotope *Gd156 = new G4Isotope("Gd156", 64, 156);
    G4Isotope *Gd157 = new G4Isotope("Gd157", 64, 157);
    G4Isotope *Gd158 = new G4Isotope("Gd158", 64, 158);
    G4Isotope *Gd160 = new G4Isotope("Gd160", 64, 160);

    elGd = new G4Element("Gadolinium", "Gd", 6);
    elGd->AddIsotope(Gd154, 2.18*perCent);
    elGd->AddIsotope(Gd155, 14.8*perCent);
    elGd->AddIsotope(Gd156, 20.57*perCent);
    elGd->AddIsotope(Gd157, 15.65*perCent);
    elGd->AddIsotope(Gd158, 24.94*perCent);
    elGd->AddIsotope(Gd160, 21.86*perCent);

    Gdmat = new G4Material("Gadoliniummat", 7.9*g/cm3, 1);
    Gdmat->AddElement(elGd, 1);
    */
    
    /* Or use with natural abundances*/
    elGd = nist->FindOrBuildElement("Gd");
    Gdmat = nist->FindOrBuildMaterial("G4_Gd");

    vac = nist->FindOrBuildMaterial("G4_Galactic");

    Gdsol = new G4Material("Gd_Solution", 1.000*g/cm3, 2);
    Gdsol->AddMaterial(H2O, 99.8*perCent);  /*Final Material consisting of above stuff and their %*/
    Gdsol->AddMaterial(Gdmat, 0.2*perCent);
    /*Liquid Argon inside cryostat*/
    lAr = new G4Material("liquidArgon", 18, 39.95*g/mole, 1.390*g/cm3);

    Steelmat = nist->FindOrBuildMaterial("G4_STAINLESS-STEEL");
    Airmat = nist->FindOrBuildMaterial("G4_AIR");

    /*Worldmaterial is Air*/
    worldMat = Airmat;

    /*add refractive index of Gamma in Water*/
    G4double energy[2] = {1.239841939*eV/0.5, 1.239841939*eV/0.3}; // Convert energie from wavelength to eV
    G4double rindexH2O[2] = {1.333,1.333};
    G4MaterialPropertiesTable *mptH2O = new G4MaterialPropertiesTable();
    mptH2O->AddProperty("RINDEX", energy, rindexH2O, 2);
    H2O->SetMaterialPropertiesTable(mptH2O);

}


/*Creates Physical Volume, returns Pointer to said volume*/
G4VPhysicalVolume *MyDetectorConstruction::Construct(){


    /*World boundaries. Just have to be bigger than the Water tank*/
    G4double xWorld = 8.0*m;
    G4double yWorld = 8.0*m;
    G4double zWorld = 8.0*m;


    /*Parameters of the experiment Setup according to Warwick (Current status)*/
    G4double cryowall = 3.0*cm;
    G4double vacgap = 50.0*cm;
    G4double cryrad = 269*cm;
    G4double cryheight = 269*cm;
    G4double detectorrad = cryrad + cryowall + vacgap + cryowall + 80*cm;

    G4double tankrad = 550*cm;
    G4double tankheight = 650*cm;

    G4double dis = -1.*m;

    G4bool plates = false;


    /* Create world out of air as big block*/
    solidWorld = new G4Box("solidWorld",xWorld,yWorld,zWorld); /*create "box" with name,x,y,z where the dimensions are 2*x vs 2*y vs 2*z*/

    logicWorld = new G4LogicalVolume(solidWorld, worldMat, "logicWorld"); /*Put material in box*/

    /*Place box in world (rotation,position,box,name, Mother volume? (place inside another volume/touches?),
                            ?,copynumber, check for overlaps? (always true pls, might take lot of time))*/
    physWorld = new G4PVPlacement(0, G4ThreeVector(0.,0.,0.), logicWorld, "physWorld", 0, false, 0, true);

    
    

    /*(Measure all Cherenkov Photons that hit here, as photomultipliers will be here)*/
    solidDetector = new G4Tubs("soldDetector", 0, detectorrad + 20*cm, tankheight, 0, CLHEP::twopi);
    logicDetector = new G4LogicalVolume(solidDetector, Steelmat, "logicdetector");
    physDetector = new G4PVPlacement(0, G4ThreeVector(0.,0.,dis), logicDetector, "physDetector", logicWorld, false, 0, true);

    fScoringVolume = logicDetector;

    /*Water Tank around cryostat*/
    solidWatertank = new G4Tubs("solidWatertank", 0, detectorrad, tankheight - 1*cm, 0, CLHEP::twopi);
    logicWatertank = new G4LogicalVolume(solidWatertank, H2O, "logicWatertank");
    physWatertank = new G4PVPlacement(0, G4ThreeVector(0.,0.,0.), logicWatertank, "physWatertank", logicDetector, false, 0, true);

    if(plates)
    {
        solidPlates = new G4Box("solidPlates",platehDepth,platehThick,platehHeight);
        logicPlates = new G4LogicalVolume(solidPlates, Gdmat, "logicPlates"); 
        for(G4int i = 0; i < plateNum; i++)
        {
            G4Rotate3D rotZ(i* 360./plateNum *deg, G4ThreeVector(0,0,1));
            G4Translate3D transX(G4ThreeVector(cryrad + cryowall + vacgap + cryowall + platehDepth + plateDist,0*cm,0*cm));
            G4Transform3D transformPlates = (rotZ)*(transX);
            physPlates = new G4PVPlacement(transformPlates, logicPlates, "physPlates", logicWatertank, false, 0, true);
        }
        G4double platetophHeight = 5*cm;
        G4double platetophDepth = 325*cm;
        G4double platetophThick = 0.01*cm;
        G4double platetopNum = 300.;
        for(G4int i = 0; i < platetopNum; i++)
        {
            solidtopPlates = new G4Box("solidtopPlates",std::sqrt(std::pow(platetophDepth,2) - std::abs(std::pow(-320.*cm + (640.*i/platetopNum *cm),2))),platetophThick,platetophHeight);
            logictopPlates = new G4LogicalVolume(solidtopPlates, Gdmat, "logictopPlates"); 
            phystopPlates = new G4PVPlacement(0, G4ThreeVector(0.,-320.*cm + (640.*i/platetopNum *cm),cryheight + cryowall + vacgap + cryowall + platetophHeight), logictopPlates, "phystopPlates", logicWatertank, false, 0, true);
        }
        for(G4int i = 0; i < platetopNum; i++)
        {
            solidtopPlates = new G4Box("solidtopPlates",std::sqrt(std::pow(platetophDepth,2) - std::abs(std::pow(-320.*cm + (640.*i/platetopNum *cm),2))),platetophThick,platetophHeight);
            logictopPlates = new G4LogicalVolume(solidtopPlates, Gdmat, "logictopPlates"); 
            physbotPlates = new G4PVPlacement(0, G4ThreeVector(0.,-320.*cm + (640.*i/platetopNum *cm),-(cryheight + cryowall + vacgap + cryowall + platetophHeight)), logictopPlates, "physbotPlates", logicWatertank, false, 0, true);
        }
    }

    /*Outer Steel Border*/
    solidouterCryoborder = new G4Tubs("solidouterCryoborder", 0, cryrad + cryowall + vacgap + cryowall, cryheight + cryowall + vacgap + cryowall, 0, CLHEP::twopi);
    logicouterCryoborder = new G4LogicalVolume(solidouterCryoborder, Steelmat, "logicouterCryoborder");
    physouterCryoborder = new G4PVPlacement(0, G4ThreeVector(0.,0.,0.), logicouterCryoborder, "physouterCryoborder", logicWatertank, false, 0, true);

    /*Vacgap Steel Border*/
    solidVacGap = new G4Tubs("solidVacGap", 0, cryrad + cryowall + vacgap, cryheight + cryowall + vacgap, 0, CLHEP::twopi);
    logicVacGap = new G4LogicalVolume(solidVacGap, vac, "logicVacGap");
    physVacGap= new G4PVPlacement(0, G4ThreeVector(0.,0.,0.), logicVacGap, "physVacGap", logicouterCryoborder, false, 0, true);


    /*Inner Steel Border*/
    solidCryoborder = new G4Tubs("solidCryoborder", 0, cryrad + cryowall, cryheight + cryowall, 0, CLHEP::twopi);
    logicCryoborder = new G4LogicalVolume(solidCryoborder, Steelmat, "logicCryoborder");
    physCryoborder = new G4PVPlacement(0, G4ThreeVector(0.,0.,0.), logicCryoborder, "physCryoborder", logicVacGap, false, 0, true);

    /*Liquid argon inside cryostat*/
    solidCryo = new G4Tubs("solidCryo", 0, cryrad, cryheight, 0, CLHEP::twopi);
    logicCryo = new G4LogicalVolume(solidCryo, lAr, "logicCryo");
    physCryo = new G4PVPlacement(0, G4ThreeVector(0.,0.,0.), logicCryo, "physCryo", logicCryoborder, false, 0, true);
    


    return physWorld;

}