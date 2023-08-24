#include "SensitiveDetector.hh"
#include <fstream> // for file operations

namespace B1
{

  SensitiveDetector::SensitiveDetector(const G4String &name) : G4VSensitiveDetector(name)
  {
    // Detector names
    detector_names = {"0_0_574", "4_0_574", "8_0_574", "12_0_574", "16_0_574", "20_0_574", "24_0_574", "28_0_574"};

    // Initialize histograms using loop

    for (const auto &suffix : detector_names)
    {
      hEnergy_map[suffix] = new TH1D(("hEnergy_" + suffix).c_str(), ("Geantino Energy Distribution for Detector at (" + suffix + ")").c_str(), 200, 0, 7);
      hScaledEnergy_map[suffix] = new TH1D(("hScaledEnergy_" + suffix).c_str(), ("Geantino Scaled Energy Distribution for Detector at (" + suffix + ")").c_str(), 200, 0, 7);
      hXMomentum_map[suffix] = new TH1D(("hXMomentum_" + suffix).c_str(), ("Geantino X Momentum Distribution for Detector at (" + suffix + ")").c_str(), 100, 0, 0);
      hYMomentum_map[suffix] = new TH1D(("hYMomentum_" + suffix).c_str(), ("Geantino Y Momentum Distribution for Detector at (" + suffix + ")").c_str(), 100, 0, 0);
      hZMomentum_map[suffix] = new TH1D(("hZMomentum_" + suffix).c_str(), ("Geantino Z Momentum Distribution for Detector at (" + suffix + ")").c_str(), 100, 0, 0);
      hEntryNumber_map[suffix] = new TH1D(("hEntryNumber_" + suffix).c_str(), ("Geantino Entry Number Distribution for Detector at (" + suffix + ")").c_str(), 100, 0, 0);
      hWeight_map[suffix] = new TH1D(("hWeight_" + suffix).c_str(), ("Geantino Weight Distribution for Detector at (" + suffix + ")").c_str(), 100, 0, 0);
    }
  }

  SensitiveDetector::~SensitiveDetector()
  {

    TFile *file = new TFile("output.root", "RECREATE");

    // Scaling info
    G4double pot = 2500000.;
    G4double detectorArea = TMath::Pi() * 2.25 * 10000.; // Pi * r^2 * 10000 (such that area is in units of cm^2)
    G4double energyBinWidth = 7. / 200.;                 // 0 to 7 GeV range, 200 bins

    // Write histograms using loop
    for (const auto &suffix : detector_names)
    {
      TDirectory *dir = file->mkdir(("dir_" + suffix).c_str());
      dir->cd();

      hEnergy_map[suffix]->Write();

      // Scale then write
      G4cerr << "pot: " << pot << G4endl;
      G4cerr << "detector area: " << detectorArea << G4endl;
      G4cerr << "bin width: " << energyBinWidth << G4endl;
      G4cerr << "scale factor: " << 1 / (pot * detectorArea * energyBinWidth) << G4endl;
      hScaledEnergy_map[suffix]->SetDrawOption("HIST p"); // Doesn't work??
      hScaledEnergy_map[suffix]->Scale(1 / (pot * detectorArea * energyBinWidth));
      hScaledEnergy_map[suffix]->Sumw2();           // Also scales uncertainty

      hScaledEnergy_map[suffix]->Write();

      hXMomentum_map[suffix]->Write();
      hYMomentum_map[suffix]->Write();
      hZMomentum_map[suffix]->Write();
      hEntryNumber_map[suffix]->Write();
      hWeight_map[suffix]->Write();

      file->cd();
    }

    file->Close();
  }

  G4bool SensitiveDetector::ProcessHits(G4Step *aStep, G4TouchableHistory *)
  {

    if (aStep->GetTrack()->GetDefinition() == G4Geantino::GeantinoDefinition())
    {

      G4double momentumX = (aStep->GetTrack()->GetMomentum().x()) / 1000; // Geant4 spits it out in MeV, so this converts them back to GeV
      G4double momentumY = (aStep->GetTrack()->GetMomentum().y()) / 1000;
      G4double momentumZ = (aStep->GetTrack()->GetMomentum().z()) / 1000;
      G4double energy = (aStep->GetTrack()->GetKineticEnergy()) / 1000;

      // Get the EventAction object and retrieve randomEntry
      auto eventAction = const_cast<EventAction *>(static_cast<const EventAction *>(G4RunManager::GetRunManager()->GetUserEventAction())); // more const cast?
      G4double entry = eventAction->GetEntry();
      G4double weight = eventAction->GetWeight();

      // G4cerr << "entry: " << entry << G4endl;
      // G4cerr << "weight: " << weight << G4endl;
      // G4cerr << "momentumX: " << momentumX << G4endl;
      // G4cerr << "momentumY: " << momentumY << G4endl;
      // G4cerr << "momentumZ: " << momentumZ << G4endl;
      // G4cerr << "energy: " << energy << G4endl;

      volumeName = aStep->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetName();
      // G4cerr << "volumeName: " << volumeName << G4endl;

      // Skip the first 24 characters of volume name and fill the map entry corresponding to that result (e.g "0_0_574")

      hEnergy_map[volumeName.substr(24)]->Fill(energy);
      hScaledEnergy_map[volumeName.substr(24)]->Fill(energy); // Also fill scaled energy with energy
      hXMomentum_map[volumeName.substr(24)]->Fill(momentumX);
      hYMomentum_map[volumeName.substr(24)]->Fill(momentumY);
      hZMomentum_map[volumeName.substr(24)]->Fill(momentumZ);
      hEntryNumber_map[volumeName.substr(24)]->Fill(entry);
      hWeight_map[volumeName.substr(24)]->Fill(weight);
    }

    return true;
  }

}
