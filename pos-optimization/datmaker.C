//////////////////////////////////////////////////////////////////////////////////////////////////
//Created for the FNAL MMBC Project by I. Vohra (ivohra@exeter.edu, ishaanklv@gmail.com)
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <fstream>
#include <vector>
#include <TFile.h>
#include <TDirectoryFile.h>
#include <TGraph.h>
#include <TKey.h>

void datmaker()
{
    // Read the values from the `first_column_entries.txt` file
    std::ifstream inputFile("first_column_entries.txt");
    double energy;
    std::vector<double> energy_vector;

    while (inputFile >> energy)
    {
        energy_vector.push_back(energy);
    }
    inputFile.close();

    // Open the ROOT file
    TFile *rootFile = TFile::Open("other_root_files/nuall_NNBarAtm_hA_BR.root");

    // Lists for neutrino flavors, interaction types, and current types
    std::vector<std::pair<std::string, std::string>> flavors = {
        {"nu_e", "nue"}, {"nu_e_bar", "nuebar"}, {"nu_mu", "numu"}, {"nu_mu_bar", "numubar"}, {"nu_tau", "nutau"}, {"nu_tau_bar", "nutaubar"}};
    std::vector<std::string> interactions = {"coh", "dis", "mec", "qel", "res", "TOT"};
    std::vector<std::string> currents = {"nc", "cc"};

    for (auto &flavor : flavors)
    {
        for (auto &interaction : interactions)
        {
            for (auto &current : currents)
            {
                std::string directoryName = flavor.first + "_Ar40";
                TDirectoryFile *dir = (TDirectoryFile *)rootFile->Get(directoryName.c_str());

                // Find all TGraphs with names beginning with interaction type and current type
                TList *keys = dir->GetListOfKeys();
                std::vector<TGraph *> graphs;
                TIter next(keys);
                TKey *key;
                while ((key = (TKey *)next()))
                {
                    if (interaction == "coh" && current == "cc")
                    {
                        if (std::string(key->GetName()) == "coh_cc")
                        {
                            graphs.push_back((TGraph *)dir->Get(key->GetName()));
                        }
                    }

                    if (interaction == "coh" && current == "nc")
                    {
                        if (std::string(key->GetName()) == "coh_nc")
                        {
                            graphs.push_back((TGraph *)dir->Get(key->GetName()));
                        }
                    }

                    if (interaction == "dis" && current == "cc")
                    {
                        if (std::string(key->GetName()) == "dis_cc_n" || std::string(key->GetName()) == "dis_cc_p")
                        {
                            graphs.push_back((TGraph *)dir->Get(key->GetName()));
                        }
                    }

                    if (interaction == "dis" && current == "nc")
                    {
                        if (std::string(key->GetName()) == "dis_nc_n" || std::string(key->GetName()) == "dis_nc_p")
                        {
                            graphs.push_back((TGraph *)dir->Get(key->GetName()));
                        }
                    }

                    if (interaction == "mec" && current == "cc")
                    {
                        if (std::string(key->GetName()) == "mec_cc")
                        {
                            graphs.push_back((TGraph *)dir->Get(key->GetName()));
                        }
                    }

                    if (interaction == "mec" && current == "nc")
                    {
                        if (std::string(key->GetName()) == "mec_nc")
                        {
                            graphs.push_back((TGraph *)dir->Get(key->GetName()));
                        }
                    }

                    if (interaction == "qel" && current == "cc")
                    {
                        if (std::string(key->GetName()) == "qel_cc_n" || std::string(key->GetName()) == "qel_cc_p")
                        {
                            graphs.push_back((TGraph *)dir->Get(key->GetName()));
                        }
                    }

                    if (interaction == "qel" && current == "nc")
                    {
                        if (std::string(key->GetName()) == "qel_nc_n" || std::string(key->GetName()) == "qel_nc_p")
                        {
                            graphs.push_back((TGraph *)dir->Get(key->GetName()));
                        }
                    }

                    if (interaction == "res" && current == "cc")
                    {
                        if (std::string(key->GetName()) == "res_cc_n" || std::string(key->GetName()) == "res_cc_p")
                        {
                            graphs.push_back((TGraph *)dir->Get(key->GetName()));
                        }
                    }

                    if (interaction == "res" && current == "nc")
                    {
                        if (std::string(key->GetName()) == "res_nc_n" || std::string(key->GetName()) == "res_nc_p")
                        {
                            graphs.push_back((TGraph *)dir->Get(key->GetName()));
                        }
                    }

                    if (interaction == "TOT" && current == "cc")
                    {
                        if (std::string(key->GetName()) == "tot_cc")
                        {
                            graphs.push_back((TGraph *)dir->Get(key->GetName()));
                        }
                    }

                    if (interaction == "TOT" && current == "nc")
                    {
                        if (std::string(key->GetName()) == "tot_nc")
                        {
                            graphs.push_back((TGraph *)dir->Get(key->GetName()));
                        }
                    }


                    //    if (std::string(key->GetName()).find(interaction + "_" + current) == 0) {
                    //     graphs.push_back((TGraph*)dir->Get(key->GetName()));
                    // }
                }

                // Define the output filename for this combination

                std::string outputFileName;

                if (interaction != "TOT"){
                outputFileName = "data/genie/xsec_" + current + "_" + interaction + "_" + flavor.second + ".dat";
                } else if (interaction == "TOT")  {
                outputFileName = "data/genie/xsec_" + current + "_" + flavor.second + ".dat"; //don't put tot in the file name if we're doing the total cc/nc event rate
                } else {
                    std::cerr << "invalid interaction type!" << std::endl;
                }

                std::ofstream outputFile(outputFileName);

                // For each energy value, evaluate the value of each TGraph and sum them together
                for (double e : energy_vector)
                {
                    double sum = 0.0;
                    for (TGraph *graph : graphs)
                    {
                        sum += graph->Eval(e);
                    }
                    outputFile << e << "\t" << sum << std::endl;
                }

                // Close the output file for this combination
                outputFile.close();
            }
        }
    }

    // Close the ROOT file
    rootFile->Close();
}
