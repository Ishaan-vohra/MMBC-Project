#ifndef EVENTACTION_HH
#define EVENTACTION_HH

#include "G4UserEventAction.hh"
#include "globals.hh"
#include "RunAction.hh"
#include <cstdint>

namespace B1
{
  class RunAction;
}

namespace B1
{
  class EventAction : public G4UserEventAction
  {
  public:
    EventAction();
    virtual ~EventAction();

    virtual void BeginOfEventAction(const G4Event *) override;
    virtual void EndOfEventAction(const G4Event *) override;
    
    // Getter and Setter for entry
    void SetEntry(int64_t entry) { fEntry = entry; }
    int64_t GetEntry() const { return fEntry; }

    // Getter and Setter for weight
    void SetWeight(int64_t weight) { fWeight = weight; }
    int64_t GetWeight() const { return fWeight; }
    
  private:
    int64_t fEntry;  // Data member to store entry
    int64_t fWeight;  // Data member to store weight
  };
}


#endif // EVENTACTION_HH