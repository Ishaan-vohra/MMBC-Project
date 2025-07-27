#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "globals.hh"
#include "G4Geantino.hh"

#include <TChain.h>
#include <TTreeReader.h>
#include <TTreeReaderValue.h>
#include <TTreeReaderArray.h>

class G4ParticleGun;
class G4Event;

namespace B1
{

  class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
  {
  public:
    PrimaryGeneratorAction();
    ~PrimaryGeneratorAction();

    void GeneratePrimaries(G4Event *anEvent);

  private:
    G4ParticleGun *particleGun;

    // For reading the ROOT file
    TChain* chain;
    TTreeReader* reader;
    TTreeReaderValue<double>* initX;
    TTreeReaderValue<double>* initY;
    TTreeReaderValue<double>* initZ;
    TTreeReaderArray<double>* px;
    TTreeReaderArray<double>* py;
    TTreeReaderArray<double>* pz;
    TTreeReaderArray<double>* E;
    TTreeReaderArray<double>* wgt;

    G4int entryIter; //ROOT file entry iterator
    G4int numEntries; //Total number of entries
    bool fallbackMode = false;
  };

}
