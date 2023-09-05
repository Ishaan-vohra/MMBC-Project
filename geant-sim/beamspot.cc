#include <TChain.h>
#include <TTreeReader.h>
#include <TTreeReaderValue.h>
#include <TTreeReaderArray.h>
#include <iostream>
#include <TH3.h>
#include <TH2.h>

void beamspot()
{
    // Setup the TChain
    TChain* chain = new TChain("dk2nuTree");
    for (int i = 1; i <= 25; i++)
    {
        TString fileName = TString::Format("OptEngNov2017_150cmTargetCone_NoMod2_Neutrino/g4lbne_v3r5p6_QGSP_BERT_OptEngNov2017_150cmTargetCone_NoMod2_neutrino_%05d.dk2nu.root", i); //%05d automatically does padding with zeros to ensure 5 digits
        chain->AddFile(fileName);
    }

    // Setup histogram

    TH3D* hist = new TH3D("hist", "Initial position hist;y (m);z (m);x (m)", 100, 0,0, 100,0,0, 100,0,0);
    // TH2D* histxy = new TH2D("histxy", "Initial position in x-y plane;x (m);y (m)", 100, 0,0, 100,0,0);


    // Set up TTreeReader
    TTreeReader* reader = new TTreeReader(chain);
    TTreeReaderValue<double>* initX = new TTreeReaderValue<double>(*reader, "dk2nu.ppvx");
    TTreeReaderValue<double>* initY = new TTreeReaderValue<double>(*reader, "dk2nu.ppvy");
    TTreeReaderValue<double>* initZ = new TTreeReaderValue<double>(*reader, "dk2nu.ppvz");
    TTreeReaderArray<double>* px = new TTreeReaderArray<double>(*reader, "dk2nu.nuray.px");
    TTreeReaderArray<double>* py = new TTreeReaderArray<double>(*reader, "dk2nu.nuray.py");
    TTreeReaderArray<double>* pz = new TTreeReaderArray<double>(*reader, "dk2nu.nuray.pz");
    TTreeReaderArray<double>* E = new TTreeReaderArray<double>(*reader, "dk2nu.nuray.E");

    while (reader->Next())
    {

         hist->Fill((*(initY->Get()))/100,(*(initZ->Get()))/100,(*(initX->Get()))/100);
        //  histxy->Fill((*(initX->Get()))/100,(*(initY->Get()))/100);


        // std::cout << "initX: " << (*(initX->Get()))/100 << " m" << std::endl;
        // std::cout << "initY: " << (*(initY->Get()))/100 << " m" << std::endl;
        // std::cout << "initZ: " << (*(initZ->Get()))/100 << " m" << std::endl;
        // std::cout << "px: " << px->At(0) << " GeV" << std::endl;
        // std::cout << "py: " << py->At(0) << " GeV" << std::endl;
        // std::cout << "pz: " << pz->At(0) << " GeV" << std::endl;
        // std::cout << "E: " << E->At(0) << " GeV" << std::endl;

    }

    // histxy->Draw("COLZ");
     hist->Draw("COLZ");

    delete chain;
    delete reader;
    delete initX;
    delete initY;
    delete initZ;
    delete px;
    delete py;
    delete pz;
    delete E;
}