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

        void AddnG4Photons(G4int nPhot) {nG4Photons += nPhot;}
        void setFirstPhotonTime(G4double time);
        void setLastPhotonTime(G4double time);

    private:
        G4int nG4Photons;
        G4double firstPhotonTime;
        G4double lastPhotonTime;
};

#endif