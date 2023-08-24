#include "ActionInitialization.hh"
#include "PrimaryGeneratorAction.hh"
#include "SteppingAction.hh"

namespace B1
{

  ActionInitialization::ActionInitialization()
  {
    G4cerr << "ACI checker" << G4endl;
  }

  ActionInitialization::~ActionInitialization()
  {
  }

  void ActionInitialization::BuildForMaster() const
  {
    auto runAction = new RunAction;
    SetUserAction(runAction);
  }

  void ActionInitialization::Build() const
  {
    SetUserAction(new PrimaryGeneratorAction);

    auto runAction = new RunAction;
    SetUserAction(runAction);

    auto eventAction = new EventAction;
    SetUserAction(eventAction);
  }

}
