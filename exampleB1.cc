/// \file exampleB1.cc
/// \brief Main program of the B1 example

#include "DetectorConstruction.hh"
#include "ActionInitialization.hh"

#include "G4RunManagerFactory.hh"
#include "G4SteppingVerbose.hh"
#include "G4UImanager.hh"
#include "QBBC.hh"

#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

#include "Randomize.hh"
#include <TApplication.h>

using namespace B1;

int main(int argc,char** argv)
{

  // TApplication app("MyApp", &argc, argv);
    std::cout << "hello" <<std::endl;
    // Create the integer file and initialize it with the value "0"
    std::ofstream outputFile("integer.txt");
    outputFile << "0";
    outputFile.close();

  // Detect interactive mode (if no arguments) and define UI session

  G4UIExecutive* ui = nullptr;
  if ( argc == 1 )   
  { ui = new G4UIExecutive(argc, argv); }

  //use G4SteppingVerboseWithUnits
  G4int precision = 4;
  G4SteppingVerbose::UseBestUnit(precision);

  // Construct the run manager
  G4RunManager* runManager = new G4RunManager;


  // Physics list
  G4VModularPhysicsList* physicsList = new QBBC;
  physicsList->SetVerboseLevel(1);
  runManager->SetUserInitialization(physicsList);

// set the detector construction to the run manager
  runManager->SetUserInitialization(new DetectorConstruction());

// set the user action initialization to the run manager
  runManager->SetUserInitialization(new ActionInitialization());

  // Initialize visualization
  //
  G4VisManager* visManager = new G4VisExecutive;
  // G4VisExecutive can take a verbosity argument - see /vis/verbose guidance.
  // G4VisManager* visManager = new G4VisExecutive("Quiet");
  visManager->Initialize();

  // Get the pointer to the User Interface manager
  G4UImanager* UImanager = G4UImanager::GetUIpointer();


// initialize Geant4 kernel
runManager->Initialize();

  // Process macro or start UI session
  //
  if ( ! ui ) {
    // batch mode
    G4String command = "/control/execute ";
    G4String fileName = argv[1];
    UImanager->ApplyCommand(command+fileName);
  }
  else {
    // interactive mode
    UImanager->ApplyCommand("/control/execute init_vis.mac");
    ui->SessionStart();
    delete ui;
  }

  // Job termination
  // Free the store: user actions, physics_list and detector_description are
  // owned and deleted by the run manager, so they should not be deleted
  // in the main() program !

  delete visManager;
  delete runManager;

}
