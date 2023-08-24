#include "EventAction.hh"
#include "RunAction.hh"

#include "G4Event.hh"
#include "G4RunManager.hh"

namespace B1
{
    EventAction::EventAction() : fEntry(0) {}

    EventAction::~EventAction() {}

    void EventAction::BeginOfEventAction(const G4Event *)
    {
    }

    void EventAction::EndOfEventAction(const G4Event *)
    {
    }

}
