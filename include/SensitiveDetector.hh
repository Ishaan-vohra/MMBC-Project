#ifndef SENSITIVEDETECTOR_HH
#define SENSITIVEDETECTOR_HH

#include "G4VSensitiveDetector.hh"
#include "TH1D.h"
#include "TFile.h"
#include "TTree.h"
#include <map>
#include "RunAction.hh"
#include "EventAction.hh"
#include "G4Step.hh"
#include "G4Geantino.hh"
#include "DetectorConstruction.hh"
#include "G4RunManager.hh"
#include "TTree.h"
#include "TH1F.h"
#include "TFile.h"
#include "TDirectory.h"
#include "TMath.h"
#include "GlobalVariables.hh"


namespace B1
{
    class SensitiveDetector : public G4VSensitiveDetector
    {
    public:
        SensitiveDetector(const G4String &name);

    private:
        std::vector<G4String> detector_names;

        G4String volumeName; // Name of detector volume

        std::map<G4String, TH1D *> hWeight_map;
        std::map<G4String, TH1D *> hEntryNumber_map;
        std::map<G4String, TH1D *> hZMomentum_map;
        std::map<G4String, TH1D *> hYMomentum_map;
        std::map<G4String, TH1D *> hXMomentum_map;
        std::map<G4String, TH1D *> hEnergy_map;
        virtual ~SensitiveDetector();

        virtual G4bool ProcessHits(G4Step *step, G4TouchableHistory *history) override;
    };
}


#endif // SENSITIVEDETECTOR_HH