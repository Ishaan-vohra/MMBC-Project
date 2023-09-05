//////////////////////////////////////////////////////////////////////////////////////////////////
//Created for the FNAL MMBC Project by I. Vohra (ivohra@exeter.edu, ishaanklv@gmail.com)
//////////////////////////////////////////////////////////////////////////////////////////////////


#include <TH2.h>
#include <iostream>
#include <fstream>
#include <string>
#include <TFile.h>
#include <TGraph2D.h>

void readCountFile()
{
    const int NUM_NEUTRINOS = 6;
    const int NUM_CURRENTS = 2;
    const int NUM_PROCESSES = 6;

    std::string neutrino_types_array[NUM_NEUTRINOS] = {"numu", "numubar", "nue", "nuebar", "nutau", "nutaubar"};
    std::string currents_array[NUM_CURRENTS] = {"cc", "nc"};
    std::string processes_array[NUM_PROCESSES] = {"coh", "dis", "mec", "qel", "res", "tot"};

    // Define a structure for the neutrino data
    struct NeutrinoData {
        double flux;
        double eventRate[NUM_CURRENTS][NUM_PROCESSES];
    };

    // Open the count.txt file
    std::ifstream count_file("count.txt");
    if (!count_file.is_open())
    {
        std::cerr << "Error: Could not open count.txt" << std::endl;
        return;
    }

    int numRows = 0;
    std::string line;
    while (std::getline(count_file, line))
    {
        numRows++;
    }

    std::cout << "numberofrows: " << numRows << std::endl;

    // Reset the file stream
    count_file.clear();
    count_file.seekg(0, std::ios::beg);

    // Initialize the TH2D histogram
     TH2D *hist = new TH2D("hist", "#nu_{#mu} Total Flux at Various Detector Locations;Angle to z axis (rad);Distance from origin (cm);Unosc #nu_{#mu}s / GeV / m^{2} / POT", 40, 0, 1.6, 40, 0, 58000); // ADJUST HERE
    // TH2D *hist = new TH2D("hist", "#nu_{#mu} CC COH Total Spectra at Various Detector Locations;Angle to z axis (rad);Distance from origin (cm);#nu_{#mu} CC COH Events / GeV / kTon / POT", 40, 0, 1.6, 40, 0, 58000); // ADJUST HERE

    // Variables to hold the columns data
    double distance, angle;
    NeutrinoData neutrinoDatas[NUM_NEUTRINOS];

    // Read through each row of count.txt
    while (count_file >> distance >> angle)
    {
        for (int i = 0; i < NUM_NEUTRINOS; ++i)
        {
            count_file >> neutrinoDatas[i].flux;
            for (int j = 0; j < NUM_CURRENTS; ++j)
                for (int k = 0; k < NUM_PROCESSES; ++k)
                    count_file >> neutrinoDatas[i].eventRate[j][k]; // i = neutrino type, j = cc/nc, k = process
        }

        int binx = hist->GetXaxis()->FindBin(angle);
        int biny = hist->GetYaxis()->FindBin(distance);
        hist->SetBinContent(binx, biny, neutrinoDatas[0].flux); //use neutrinoDatas[x].flux for flux, use neutrinoDatas[x].eventRate[x][x] for a specific event rate
    }

    // Close the file
    count_file.close();

    // Draw the histogram
    hist->SetStats(0);
    hist->Draw("colz");

    // Save the histogram to a file
    TFile out("other_root_files/dist_angle.root", "RECREATE");
    hist->Write();
    out.Close();
}
