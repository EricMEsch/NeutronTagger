#ifndef EVENT_HH
#define EVENT_HH

#include "G4UserEventAction.hh"
#include "G4Event.hh"

#include "G4AnalysisManager.hh"

#include "run.hh"

class MyEventAction : public G4UserEventAction {
    public:
        MyEventAction(MyRunAction*);
        ~MyEventAction();

        virtual void BeginOfEventAction(const G4Event*);
        virtual void EndOfEventAction(const G4Event*);
        G4String targetname;
        G4double x_Position;
        G4double y_Position;
        G4double z_Position;
        G4double energy;

        void AddnGd(G4int nPhot) {nGd += nPhot;}
        void AddnWat(G4int nPhot) {nWat += nPhot;}


    private:
    MyRunAction *fRunAction;
    G4int nGd;
    G4int nWat;
};

#endif