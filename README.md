# MMBC-Project
Repository for Ishaan Vohra's work on the Fermilab MMBC project.

Contact details: ivohra@mit.edu or ishaanklv@gmail.com

### geant-sim
Contains a Geant4 simulation to calculate fluxes and event 
rates at various detector locations. Although this simulation
is very fast, it does not include certain importance weight values, 
making its results less physically accurate. Critically, it cannot
get beyond some fundamental statistical issues, in that the full
input sample contains only a few million event.

### pos-optimization
Contains a C++ based simulation to calculate fluxes and event 
rates at various detector locations. This simulation is slower, 
but it provides more physically accurate fluxes and event rates.
