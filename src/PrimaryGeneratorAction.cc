#include "PrimaryGeneratorAction.hh"

#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4RunManager.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "EventAction.hh"
#include <iostream>

B1::PrimaryGeneratorAction::PrimaryGeneratorAction()
{
  particleGun = new G4ParticleGun();

  // Define the Geantino
  G4ParticleDefinition *geantino = G4ParticleTable::GetParticleTable()->FindParticle("geantino");
  particleGun->SetParticleDefinition(geantino);

  // Setup the TChain
  chain = new TChain("dk2nuTree");
  for (int i = 1; i <= 25; i++)
  {
    TString fileName = TString::Format("../OptEngNov2017_150cmTargetCone_NoMod2_Neutrino/g4lbne_v3r5p6_QGSP_BERT_OptEngNov2017_150cmTargetCone_NoMod2_neutrino_%05d.dk2nu.root", i); //%05d automatically does padding with zeros to ensure 5 digits
    chain->AddFile(fileName);
  }

  // Set up TTreeReader
  reader = new TTreeReader(chain);
  initX = new TTreeReaderValue<double>(*reader, "dk2nu.ppvx");
  initY = new TTreeReaderValue<double>(*reader, "dk2nu.ppvy");
  initZ = new TTreeReaderValue<double>(*reader, "dk2nu.ppvz");
  px = new TTreeReaderArray<double>(*reader, "dk2nu.nuray.px");
  py = new TTreeReaderArray<double>(*reader, "dk2nu.nuray.py");
  pz = new TTreeReaderArray<double>(*reader, "dk2nu.nuray.pz");
  E = new TTreeReaderArray<double>(*reader, "dk2nu.nuray.E");
  wgt = new TTreeReaderArray<double>(*reader, "dk2nu.nuray.wgt");

  entryIter = 0;
  numEntries = chain->GetEntries();

}

B1::PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  delete particleGun;
  delete chain;
  delete reader;
  delete initX;
  delete initY;
  delete initZ;
  delete px;
  delete py;
  delete pz;
  delete E;
  delete wgt;
    G4cout << "number of entriesss: " << numEntries << G4endl;
}

void B1::PrimaryGeneratorAction::GeneratePrimaries(G4Event *anEvent)
{
  reader->SetEntry(entryIter);

  // Retrieve properties from the randomly selected entry
  G4ThreeVector position(*(initX->Get()) * cm, *(initY->Get()) * cm, *(initZ->Get()) * cm);

  // Get the EventAction object
  auto eventAction = const_cast<EventAction *>(static_cast<const EventAction *>(G4RunManager::GetRunManager()->GetUserEventAction())); // Const cast?

  // Set the entry and weight in the EventAction
  eventAction->SetEntry(entryIter);
  eventAction->SetWeight(wgt->At(0));

  G4ThreeVector momentum((px->At(0)) * GeV, (py->At(0)) * GeV, (pz->At(0)) * GeV);
  G4double energy = (E->At(0)) * GeV;

  // G4cerr << "initX: " << *(initX->Get()) << " cm" << G4endl;
  // G4cerr << "initY: " << *(initY->Get()) << " cm" << G4endl;
  // G4cerr << "initZ: " << *(initZ->Get()) << " cm" << G4endl;
  // G4cerr << "px: " << px->At(0) << " GeV" << G4endl;
  // G4cerr << "py: " << py->At(0) << " GeV" << G4endl;
  // G4cerr << "pz: " << pz->At(0) << " GeV" << G4endl;
  // G4cerr << "E: " << E->At(0) << " GeV" << G4endl;

  //  G4cerr << "PGA input entry: " << entryIter << G4endl;
  // G4cerr << "PGA input weight: " << wgt->At(0) << G4endl;


  if (entryIter == 500000){G4cerr << "PGA input entry: " << entryIter << G4endl;}
  if (entryIter == 1000000){G4cerr << "PGA input entry: " << entryIter << G4endl;}
  if (entryIter == 1500000){G4cerr << "PGA input entry: " << entryIter << G4endl;}
  if (entryIter == 2000000){G4cerr << "PGA input entry: " << entryIter << G4endl;}



  // Setting particle properties for this event
  particleGun->SetParticlePosition(position);
  particleGun->SetParticleMomentumDirection(momentum);
  particleGun->SetParticleEnergy(energy);
  particleGun->GeneratePrimaryVertex(anEvent);

  // Iterate entryIter for the next event;
  entryIter = entryIter + 1;
}
