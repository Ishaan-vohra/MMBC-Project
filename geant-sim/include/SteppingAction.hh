#include "G4UserSteppingAction.hh"
#include "globals.hh"

namespace B1
{
  class SteppingAction : public G4UserSteppingAction
  {
  public:
    SteppingAction() {}
    ~SteppingAction() {}

    void UserSteppingAction(const G4Step *) override {}
  };
}
