#include "DetectorConstruction.hh"

namespace B1
{

  DetectorConstruction::DetectorConstruction()
  {

    // Define hydrogen gas
    G4double z = 1.00;                      // atomic number
    G4double a = 1.01 * g / mole;           // atomic mass
    G4double density = 0.0000899 * g / cm3; // density at STP
    fHydrogenGasMaterial = new G4Material("HydrogenGas", z, a, density);


    // Detector names
    detector_names = {"0_0_574", "4_0_574", "8_0_574", "12_0_574", "16_0_574", "20_0_574", "24_0_574", "28_0_574"};
  }

  DetectorConstruction::~DetectorConstruction() {}

  G4VPhysicalVolume *DetectorConstruction::Construct()
  {
    // Define shapes and logical volumes

    // World volume (box)
    G4double worldSize = 650 * m;
    G4Box *worldBox = new G4Box("World", worldSize, worldSize, worldSize);
    G4LogicalVolume *worldLogical = new G4LogicalVolume(worldBox, fHydrogenGasMaterial, "World_Logical");

    // Hydrogen sphere
    G4Sphere *hydrogenSphere = new G4Sphere("HydrogenSphere", 0, hydrogenRadius, 0. * deg, 360. * deg, 0. * deg, 180. * deg); //hydrogenRadius defined in globalvariables

    // Create logical volumes map
    for (const auto &name : detector_names)
    {
      HydrogenSphere_Logicals[name] = new G4LogicalVolume(hydrogenSphere, fHydrogenGasMaterial, "HydrogenSphere_Logical_" + name);
    }

    // Placement of hydrogen sphere detectors (map)

    for (size_t i = 0; i < detector_names.size(); ++i)
    {
      new G4PVPlacement(0, positions[i], HydrogenSphere_Logicals[detector_names[i]], "HydrogenSphere_Physical_" + detector_names[i], worldLogical, false, 0);
    }

    // Define the world physical volume (must be last)
    G4VPhysicalVolume *worldPhysical = new G4PVPlacement(0, G4ThreeVector(), worldLogical, "World_Physical", 0, false, 0);

    return worldPhysical;
  }

  void DetectorConstruction::ConstructSDandField()
  {
    G4VSensitiveDetector *hydrogenSD = new SensitiveDetector("hydrogenSD");
    G4SDManager::GetSDMpointer()->AddNewDetector(hydrogenSD);


    // Set the sensitive detectors (map)
    for (const auto &pair : HydrogenSphere_Logicals)
    {
      pair.second->SetSensitiveDetector(hydrogenSD);
    }
  }

}
