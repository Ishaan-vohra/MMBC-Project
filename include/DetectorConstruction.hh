#ifndef DETECTORCONSTRUCTION_HH
#define DETECTORCONSTRUCTION_HH

#include "G4VUserDetectorConstruction.hh"
#include "EventAction.hh"
#include "globals.hh"
#include "G4Material.hh"
#include "SensitiveDetector.hh"
#include "G4RunManager.hh"
#include "G4SDManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Trd.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "GlobalVariables.hh"


namespace B1
{

  class DetectorConstruction : public G4VUserDetectorConstruction
  {
  public:
    DetectorConstruction();
    virtual ~DetectorConstruction();

    virtual G4VPhysicalVolume *Construct();
    virtual void ConstructSDandField();

    G4double GetHydrogenRadius() const;

  private:

    std::vector<G4String> detector_names;


    G4Material *fHydrogenGasMaterial;

    std::map<G4String, G4LogicalVolume *> HydrogenSphere_Logicals;

    std::vector<G4ThreeVector> positions = {
        G4ThreeVector(0 * CLHEP::m, 0 * CLHEP::m, 574 * CLHEP::m),
        G4ThreeVector(4 * CLHEP::m, 0 * CLHEP::m, 574 * CLHEP::m),
        G4ThreeVector(8 * CLHEP::m, 0 * CLHEP::m, 574 * CLHEP::m),
        G4ThreeVector(12 * CLHEP::m, 0 * CLHEP::m, 574 * CLHEP::m),
        G4ThreeVector(16 * CLHEP::m, 0 * CLHEP::m, 574 * CLHEP::m),
        G4ThreeVector(20 * CLHEP::m, 0 * CLHEP::m, 574 * CLHEP::m),
        G4ThreeVector(24 * CLHEP::m, 0 * CLHEP::m, 574 * CLHEP::m),
        G4ThreeVector(28 * CLHEP::m, 0 * CLHEP::m, 574 * CLHEP::m)};
  };

}

#endif // DETECTORCONSTRUCTION_HH
