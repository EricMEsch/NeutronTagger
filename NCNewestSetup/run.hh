#ifndef RUN_HH
#define RUN_HH

#include "G4UserRunAction.hh"
#include "G4Run.hh"

#include "G4AnalysisManager.hh"

class MyRunAction : public G4UserRunAction {
    public:
        MyRunAction();
        ~MyRunAction();

        virtual void BeginOfRunAction(const G4Run*);
        virtual void EndOfRunAction(const G4Run*);

        void AddnGd(G4int nPhot) {nGd += nPhot;}
        void AddnWat(G4int nPhot) {nWat += nPhot;}

    private:
    G4int nGd;
    G4int nWat;
};

#endif