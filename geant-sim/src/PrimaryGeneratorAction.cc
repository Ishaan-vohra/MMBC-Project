#include "PrimaryGeneratorAction.hh"

#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4RunManager.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "EventAction.hh"

#include <filesystem>
#include <cstdlib>
#include <iostream>

namespace fs = std::filesystem;

B1::PrimaryGeneratorAction::PrimaryGeneratorAction()
{
  particleGun = new G4ParticleGun();

  // 1. default particle for the fallback gun
  auto muMinus = G4ParticleTable::GetParticleTable()->FindParticle("mu-");
  particleGun->SetParticleDefinition(muMinus);

  // 2. build dk2nu chain
  chain = new TChain("dk2nuTree");

  const char* envDir = std::getenv("DK2NU_DIR");
  std::string baseDir = envDir ? envDir
                               : "../OptEngNov2017_150cmTargetCone_NoMod2_Neutrino";

  for (int i = 1; i <= 25; ++i) {
    std::string file = baseDir + "/" +
        TString::Format("g4lbne_v3r5p6_QGSP_BERT_OptEngNov2017_150cmTargetCone_NoMod2_neutrino_%05d.dk2nu.root", i)
            .Data();
    if (fs::exists(file))
      chain->AddFile(file.c_str());
    else
      G4cout << "dk2nu file not found, skipping: " << file << G4endl;
  }

  numEntries = chain->GetEntries();
  G4cout << "[PrimaryGeneratorAction] Entries in dk2nu chain: "
         << numEntries << G4endl;

  if (numEntries == 0) {                // fallback gun activates
    G4cout << ">>> No dk2nu input – using 1 GeV mu‑ gun" << G4endl;
    fallbackMode = true;
    return;
  }

  // 3. TTreeReader
  reader = new TTreeReader(chain);
  initX = new TTreeReaderValue<double>(*reader, "dk2nu.ppvx");
  initY = new TTreeReaderValue<double>(*reader, "dk2nu.ppvy");
  initZ = new TTreeReaderValue<double>(*reader, "dk2nu.ppvz");
  px    = new TTreeReaderArray<double>(*reader, "dk2nu.nuray.px");
  py    = new TTreeReaderArray<double>(*reader, "dk2nu.nuray.py");
  pz    = new TTreeReaderArray<double>(*reader, "dk2nu.nuray.pz");
  E     = new TTreeReaderArray<double>(*reader, "dk2nu.nuray.E");
  wgt   = new TTreeReaderArray<double>(*reader, "dk2nu.nuray.wgt");

  entryIter = 0;
}

B1::PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  delete particleGun;
  delete chain;

  if (!fallbackMode) {
    delete reader;
    delete initX; delete initY; delete initZ;
    delete px; delete py; delete pz; delete E; delete wgt;
  }

  G4cout << "Total dk2nu entries processed: " << entryIter << G4endl;
}

void B1::PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  // ---------- fallback gun ----------
  if (fallbackMode) {
    particleGun->SetParticlePosition({0,0,-1*m});
    particleGun->SetParticleMomentumDirection({0,0,1});
    particleGun->SetParticleEnergy(1.*GeV);
    particleGun->GeneratePrimaryVertex(anEvent);
    return;
  }

  // ---------- dk2nu‑driven ----------
  if (entryIter >= numEntries) entryIter = 0;   // wrap

  reader->SetEntry(entryIter);

  // dereference twice to get the double value
  G4ThreeVector position{ (**initX)*cm, (**initY)*cm, (**initZ)*cm };

  G4ThreeVector pVec{ px->At(0), py->At(0), pz->At(0) };
  G4ThreeVector direction = pVec.unit();        // normalised
  G4double      energy    = (E->At(0))*GeV;

  auto eventAction =
    const_cast<EventAction*>(
      static_cast<const EventAction*>(G4RunManager::GetRunManager()->GetUserEventAction()));
  eventAction->SetEntry(entryIter);
  eventAction->SetWeight(wgt->At(0));

  particleGun->SetParticlePosition(position);
  particleGun->SetParticleMomentumDirection(direction);
  particleGun->SetParticleEnergy(energy);
  particleGun->GeneratePrimaryVertex(anEvent);

  ++entryIter;
}
