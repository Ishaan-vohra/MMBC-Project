#ifndef B1ActionInitialization_h
#define B1ActionInitialization_h 1

#include "G4VUserActionInitialization.hh"
#include "RunAction.hh"
#include "EventAction.hh"

namespace B1
{

  class ActionInitialization : public G4VUserActionInitialization
  {
  public:
    ActionInitialization();
    ~ActionInitialization() override;

    void BuildForMaster() const override;
    void Build() const override;
  };

}

#endif
