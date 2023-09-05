#include "eventRates.h"

void makeFluxHistograms(std::string flux_directory,std::string pot_per_file, std::string user_position_x, std::string user_position_y,std::string user_position_z, std::string output_name) {

  gROOT->ProcessLine(".L eventRates.C++");

  std::string command = "eventRates t(\""+flux_directory+"\", "+pot_per_file+", "+user_position_x+", "+user_position_y+", "+user_position_z+",\""+output_name+"\")";

  std::cout<<"Executing: "<<command<<std::endl;

  gROOT->ProcessLine(command.c_str());

  gROOT->ProcessLine("t.Loop()");
}
