#include "construction.hh"

MyDetectorConstruction::MyDetectorConstruction() //Constructor
{
    defineMaterials();
}

MyDetectorConstruction::~MyDetectorConstruction() //Destructor
{
}

void MyDetectorConstruction::defineMaterials(){
    G4NistManager *nist = G4NistManager::Instance(); /*NistManager to use prebuild materials (instead you could create own materials)*/ 

    H2O = new G4Material("H2O", 1.000*g/cm3, 2);
    H2O->AddElement(nist->FindOrBuildElement("H"), 2); 
    H2O->AddElement(nist->FindOrBuildElement("O"), 1);
    
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
    G4double absH2O[2] = {80*m,100*m};
    G4MaterialPropertiesTable *mptH2O = new G4MaterialPropertiesTable();
    mptH2O->AddProperty("RINDEX", energy, rindexH2O, 2);
    mptH2O->AddProperty("ABSLENGTH", energy, absH2O, 2);
    H2O->SetMaterialPropertiesTable(mptH2O);
    Gdmat->SetMaterialPropertiesTable(mptH2O);

}

void MyDetectorConstruction::addOpticalBorders()
{
    G4OpticalSurface *OpSurface = new G4OpticalSurface("VM");

    G4OpticalSurface *Invis = new G4OpticalSurface("Invisible");

    G4LogicalBorderSurface *Surface = new G4LogicalBorderSurface("SteelSurface", physInnerWatertank, physouterCryoborder , OpSurface);
    G4LogicalBorderSurface *Surface2 = new G4LogicalBorderSurface("SteelSurface", physWatertank, physDetector , OpSurface);

    new G4LogicalBorderSurface("Invisible", physCurtain, physWatertank , Invis);
    new G4LogicalBorderSurface("Invisible", physCurtain, physInnerWatertank , Invis);
    new G4LogicalBorderSurface("Invisible", physWatertank, physCurtain , Invis);
    new G4LogicalBorderSurface("Invisible", physInnerWatertank, physCurtain , Invis);


    OpSurface->SetType(dielectric_dielectric);
    OpSurface->SetModel(unified);
    OpSurface->SetFinish(groundfrontpainted);

    G4MaterialPropertiesTable *SMPT = new G4MaterialPropertiesTable();
    G4double energy[2] = {1.239841939*eV/0.5, 1.239841939*eV/0.3};
    G4double reflectivity[2] = {0.9,0.9};
    G4double transmission[2] = {0.,0.};
    SMPT->AddProperty("REFLECTIVITY", energy, reflectivity, 2);
    SMPT->AddProperty("TRANSMITTANCE", energy, transmission, 2);
    OpSurface->SetMaterialPropertiesTable(SMPT);

    
    G4MaterialPropertiesTable *IMPT = new G4MaterialPropertiesTable();
    G4double energyI[2] = {1.239841939*eV/0.5, 1.239841939*eV/0.3};
    G4double reflectivityI[2] = {0.,0.};
    G4double transmissionI[2] = {1.,1.};
    IMPT->AddProperty("REFLECTIVITY", energyI, reflectivityI, 2);
    IMPT->AddProperty("TRANSMITTANCE", energyI, transmissionI, 2);
    Invis->SetMaterialPropertiesTable(IMPT);

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
    G4double outercryrad = cryrad + cryowall + vacgap + cryowall;
    G4double outercryheight = cryheight + cryowall + vacgap + cryowall;

    G4double tankrad = 550*cm;
    G4double tankheight = 650*cm;

    G4double dis = -1.*m;



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

    solidCurtain = new G4Tubs("solidCurtain", 0, outercryrad + 10*cm + 10*angstrom, outercryheight + 10*cm + 10*angstrom, 0, CLHEP::twopi);
    logicCurtain = new G4LogicalVolume(solidCurtain, Gdmat, "logicCurtain");
    physCurtain = new G4PVPlacement(0, G4ThreeVector(0.,0.,0.), logicCurtain, "physCurtain", logicWatertank, false, 0, true);

    solidInnerWatertank = new G4Tubs("solidInnerWatertank", 0, outercryrad + 10*cm, outercryheight + 10*cm, 0, CLHEP::twopi);
    logicInnerWatertank = new G4LogicalVolume(solidInnerWatertank, H2O, "logicInnerWatertank");
    physInnerWatertank = new G4PVPlacement(0, G4ThreeVector(0.,0.,0.), logicInnerWatertank, "physInnerWatertank", logicCurtain, false, 0, true);


    /*Outer Steel Border*/
    solidouterCryoborder = new G4Tubs("solidouterCryoborder", 0, cryrad + cryowall + vacgap + cryowall, cryheight + cryowall + vacgap + cryowall, 0, CLHEP::twopi);
    logicouterCryoborder = new G4LogicalVolume(solidouterCryoborder, Steelmat, "logicouterCryoborder");
    physouterCryoborder = new G4PVPlacement(0, G4ThreeVector(0.,0.,0.), logicouterCryoborder, "physouterCryoborder", logicInnerWatertank, false, 0, true);

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

    addOpticalBorders();



    return physWorld;

}