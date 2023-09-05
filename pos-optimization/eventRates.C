//////////////////////////////////////////////////////////////////////////////////////////////////
//Adapted for the FNAL MMBC Project by I. Vohra (ivohra@exeter.edu, ishaanklv@gmail.com)
//////////////////////////////////////////////////////////////////////////////////////////////////

#define eventRates_cxx
#include "eventRates.h"
#include "OscLib/OscCalculator.cxx"
#include <TH2.h>
#include <TH1D.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <math.h>
#include <TGaxis.h>
#include <TParameter.h>
#include <TMath.h>

using namespace std;

int iread = 0;

void eventRates::Loop()
{

   double refpot = 1;
   std::string potref_str = eventRates::GetPOTAsString(refpot);

   //
   // Set Histogram Binning
   //

   // simple binning for some alignment plots __________ADJUST RANGES/BINS HERE
   int nbins = 200;
   double xmin = 0.0;
   double xmax = 5.0;

   // Define flux histograms

   for (int i = 0; i < 6; i++)
   {
      std::string name = neutrino_types_array[i] + "_flux_forplots";
      fluxHists[i] = new TH1D(name.c_str(), name.c_str(), nbins / 2, xmin, xmax);
   }

   // Define event rate histograms with format [neutrino type][current][process]

   for (int i = 0; i < 6; i++)
   {
      for (int j = 0; j < 2; j++)
      {
         for (int k = 0; k < 6; k++)
         {
            std::string name = neutrino_types_array[i] + "_" + currents_array[j] + "_" + processes_array[k] + "_eventrate_forplots";
            eventRateHists[i][j][k] = new TH1D(name.c_str(), name.c_str(), nbins / 2, xmin, xmax);
         }
      }
   }

   std::string fluxtitle = "Neutrinos / GeV / m^{2} / POT";
   std::string oscfluxtitle = "Oscillated Neutrinos / GeV / m^{2} / POT";
   std::string cceventratetitle = "CC Events / POT";
   std::string nceventratetitle = "CC Events / POT";

   SetTitles(fluxHists[0], "#nu_{#mu} Energy (GeV) ", "Unosc #nu_{#mu}s / GeV / m^{2} / POT");
   SetTitles(fluxHists[1], "#bar{#nu}_{#mu} Energy (GeV)", "Unosc #bar{#nu}_{#mu}s / GeV / m^{2} / POT");
   SetTitles(fluxHists[2], "#nu_{e} Energy (GeV)", "Unosc #nu_{e}s / GeV / m^{2} / POT");
   SetTitles(fluxHists[3], "#bar{#nu}_{e} Energy (GeV)", "Unosc #bar{#nu}_{e}s / GeV / m^{2} / POT");
   SetTitles(fluxHists[4], "#nu_{#tau} Energy (GeV)", "Unosc #nu_{#tau}s / GeV / m^{2} / POT");
   SetTitles(fluxHists[5], "#bar{#nu}_{#tau} Energy (GeV)", "Unosc #bar{#nu}_{#tau}s / GeV / m^{2} / POT");

   // CC

   // Set titles for event rate plots
   for (int j = 0; j < 2; j++)
   {
      for (int k = 0; k < 6; k++)
      {
         SetTitles(eventRateHists[0][j][k], "Energy (GeV)", ("#nu_{#mu} " + currents_array[j] + " " + processes_array[k] + " Events / GeV / kTon / POT").c_str());
         SetTitles(eventRateHists[1][j][k], "Energy (GeV)", ("#bar{#nu}_{#mu} " + currents_array[j] + " " + processes_array[k] + " Events / GeV / kTon / POT").c_str());
         SetTitles(eventRateHists[2][j][k], "Energy (GeV)", ("#nu_{e} " + currents_array[j] + " " + processes_array[k] + " Events / GeV / kTon / POT").c_str());
         SetTitles(eventRateHists[3][j][k], "Energy (GeV)", ("#bar{#nu}_{e} " + currents_array[j] + " " + processes_array[k] + " Events / GeV / kTon / POT").c_str());
         SetTitles(eventRateHists[4][j][k], "Energy (GeV)", ("#nu_{#tau} " + currents_array[j] + " " + processes_array[k] + " Events / GeV / kTon / POT").c_str());
         SetTitles(eventRateHists[5][j][k], "Energy (GeV)", ("#bar{#nu}_{#tau} " + currents_array[j] + " " + processes_array[k] + " Events / GeV / kTon / POT").c_str());
      }
   }

   //
   // start loop over entries in ntuple
   //

   Long64_t nentries = fChain->GetEntries();
   std::cout << "Total number of Entries = " << nentries << std::endl;

   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry = 0; jentry < nentries; jentry++)
   // for (Long64_t jentry=0; jentry<1000;jentry++) //fast -- for testing
   {
      Long64_t ientry = LoadTree(jentry);
      nb = fChain->GetEntry(jentry);
      nbytes += nb;
      if (ientry < 0)
         break;

      ++iread;

      double nuenergyatsomedet = -999.0;
      double detectorwghtatsomedet = -999.0;
      std::vector<double> detvec;
      detvec.push_back(detx);
      detvec.push_back(dety);
      detvec.push_back(detz);

      eventRates::GetWeight(detvec, detectorwghtatsomedet, nuenergyatsomedet);

      double fluxwghtsomedet = (detectorwghtatsomedet * Nimpwt / 3.1415) * (refpot / fTotalPOT);

      std::string current_string = "CC";

      double cc_eventratewghtsomedet[6];

      for (int k = 0; k < 6; k++)
      {
         cc_eventratewghtsomedet[k] = fluxwghtsomedet * GetXSec((double)Ntype, nuenergyatsomedet, current_string, k);
      }

      current_string = "NC";

      double nc_eventratewghtsomedet[6];

      for (int k = 0; k < 6; k++)
      {
         nc_eventratewghtsomedet[k] = fluxwghtsomedet * GetXSec((double)Ntype, nuenergyatsomedet, current_string, k);
      }

      if (Ntype == 56) // numu
      {
         // Flux
         fluxHists[0]->Fill(nuenergyatsomedet, fluxwghtsomedet);

         // Event rates
         for (int k = 0; k < 6; k++)
         {
            // 0 and 1 signify cc and nc
            eventRateHists[0][0][k]->Fill(nuenergyatsomedet, cc_eventratewghtsomedet[k] / 22);
            eventRateHists[0][1][k]->Fill(nuenergyatsomedet, nc_eventratewghtsomedet[k] / 40);
         }
      }

      if (Ntype == 55) // numubar
      {
         // Flux
         fluxHists[1]->Fill(nuenergyatsomedet, fluxwghtsomedet);

         // Event rates
         for (int k = 0; k < 6; k++)
         {
            // 0 and 1 signify cc and nc
            eventRateHists[1][0][k]->Fill(nuenergyatsomedet, cc_eventratewghtsomedet[k] / 18);
            eventRateHists[1][1][k]->Fill(nuenergyatsomedet, nc_eventratewghtsomedet[k] / 40);
         }
      }

      if (Ntype == 53) // nue
      {
         // Flux
         fluxHists[2]->Fill(nuenergyatsomedet, fluxwghtsomedet);

         // Event rates
         for (int k = 0; k < 6; k++)
         {
            // 0 and 1 signify cc and nc
            eventRateHists[2][0][k]->Fill(nuenergyatsomedet, cc_eventratewghtsomedet[k] / 22);
            eventRateHists[2][1][k]->Fill(nuenergyatsomedet, nc_eventratewghtsomedet[k] / 40);
         }
      }
      if (Ntype == 52) // nuebar
      {
         // Flux
         fluxHists[3]->Fill(nuenergyatsomedet, fluxwghtsomedet);

         // Event rates
         for (int k = 0; k < 6; k++)
         {
            // 0 and 1 signify cc and nc
            eventRateHists[3][0][k]->Fill(nuenergyatsomedet, cc_eventratewghtsomedet[k] / 18);
            eventRateHists[3][1][k]->Fill(nuenergyatsomedet, nc_eventratewghtsomedet[k] / 40);
         }
      }

      if (Ntype == 58) // nutau
      {
         // Flux
         fluxHists[4]->Fill(nuenergyatsomedet, fluxwghtsomedet);

         for (int k = 0; k < 6; k++)
         {
            // 0 and 1 signify cc and nc
            eventRateHists[4][0][k]->Fill(nuenergyatsomedet, cc_eventratewghtsomedet[k] / 22);
            eventRateHists[4][1][k]->Fill(nuenergyatsomedet, nc_eventratewghtsomedet[k] / 40);
         }
      }
      if (Ntype == 59) // nutaubar
      {
         // Flux
         fluxHists[5]->Fill(nuenergyatsomedet, fluxwghtsomedet);

         for (int k = 0; k < 6; k++)
         {
            // 0 and 1 signify cc and nc
            eventRateHists[5][0][k]->Fill(nuenergyatsomedet, cc_eventratewghtsomedet[k] / 18);
            eventRateHists[5][1][k]->Fill(nuenergyatsomedet, nc_eventratewghtsomedet[k] / 40);
         }
      }

   } // end loop over entries

   // normalize by bin width (essentially convert to density histogram)

   for (int i = 0; i < 6; i++)
   {
      fluxHists[i]->Scale(1.0, "width");

      for (int j = 0; j < 2; j++)
      {
         for (int k = 0; k < 6; k++)
         {
            eventRateHists[i][j][k]->Scale(1.0, "width");
         }
      }
   }

   // put the location of the detector in the file
   TParameter<double> det_x("det_x", detx);
   TParameter<double> det_y("det_y", dety);
   TParameter<double> det_z("det_z", detz);

   // Open count.txt in append mode
   ofstream count_file("count.txt", ios_base::app);

   // Write the distance
   count_file << sqrt(pow(det_x.GetVal(), 2) + pow(det_y.GetVal(), 2) + pow(det_z.GetVal(), 2));

   // Write the angle – will be undefined for (0,0,0), so don't choose (0,0,0) as a coordinate!
   count_file << "\t" << TMath::ATan((sqrt(pow(det_x.GetVal(), 2) + pow(det_y.GetVal(), 2))) / det_z.GetVal());

   // Calculate and write the integrals for the histograms

   for (int i = 0; i < 6; i++)
   {
      count_file << "\t" << fluxHists[i]->Integral();

      for (int j = 0; j < 2; j++)
      {
         for (int k = 0; k < 6; k++)
         {
            count_file << "\t" << eventRateHists[i][j][k]->Integral();
         }
      }
   }

   // Add a new line to the file
   count_file << std::endl;

   // Close the file
   count_file.close();

   // Save histograms to a root file
   std::cout << "writing " + ffilename + ".root" << std::endl;
   TFile f((ffilename + ".root").c_str(), "recreate");
   det_x.Write();
   det_y.Write();
   det_z.Write();

   for (int i = 0; i < 6; i++)
   {
      fluxHists[i]->Write();

      for (int j = 0; j < 2; j++)
      {
         for (int k = 0; k < 6; k++)
         {
            eventRateHists[i][j][k]->Write();
         }
      }
   }

   f.Close();
}

//-------------------------------------------------------------------------------------
std::string eventRates::GetPOTAsString(const double dpot)
{

   std::stringstream potstrm;
   potstrm << scientific << dpot;

   string potstr = potstrm.str();

   //
   // get base
   //
   size_t baselength;
   if (potstr.find("e", 0) != string::npos)
   {
      baselength = potstr.find("e", 0);
   }
   else if (potstr.find("E", 0) != string::npos)
   {
      baselength = potstr.find("E", 0);
   }
   else
   {
      cout << "eventRates::GetPOTAsString - PROBLEM: pot is not in scientific notation" << endl;
      return "Problem";
   }

   string base = potstr.substr(0, baselength);

   //
   // get exp
   //
   size_t exppos;
   if (potstr.find("+", baselength) != string::npos)
   {
      exppos = potstr.find("+", baselength);
   }
   else if (potstr.find("-", baselength) != string::npos)
   {
      exppos = potstr.find("-", baselength);
   }
   else
   {
      cout << "eventRates::GetPOTAsString - PROBLEM: pot is not in scientific notation" << endl;
      return "Problem";
   }

   string exp = potstr.substr(exppos);

   //
   // modify base string if needed
   //
   string baseNumber = base;

   size_t baseDecimalpos = base.find(".", 0);
   if (baseDecimalpos != string::npos)
   {
      size_t baseNotZeropos = base.find_last_not_of("0", string::npos);
      if (baseNotZeropos != string::npos)
      {
         if (baseNotZeropos > baseDecimalpos)
         {
            baseNumber = base.substr(0, baseNotZeropos + 1);
         }
         else
         {
            baseNumber = base.substr(0, baseDecimalpos + 2);
         }
      }
   }
   else
   {
      baseNumber = baseNumber + ".0";
   }

   //
   // modify exp string if needed
   //

   string expSign = exp.substr(0, 1);
   string expNumber = exp.substr(1, string::npos);

   size_t expNotZeropos = expNumber.find_first_not_of("0", 0);
   if (expNotZeropos != string::npos)
   {
      expNumber = expNumber.substr(expNotZeropos, string::npos);
   }

   string potfinalstr;

   //
   // put base and exp together
   //

   if (baseNumber.empty() && expNumber.empty())
   {
      cout << "eventRates::GetPOTAsString - PROBLEM: base number and exp number are both empty" << endl;
      return "Problem";
   }

   if (baseNumber == "1.0")
   {
      if (expSign == "-")
         potfinalstr = "10^{" + expSign + expNumber + "}";
      else
         potfinalstr = "10^{" + expNumber + "}";
   }
   else
   {
      if (expSign == "-")
         potfinalstr = baseNumber + "#times10^{" + expSign + expNumber + "}";
      else
         potfinalstr = baseNumber + "#times10^{" + expNumber + "}";
   }

   return potfinalstr;
}

//---------------------------------------------------------------------------------------------
void eventRates::SetTitles(TH1 *h, const std::string &xtitle, const std::string &ytitle)
{
   if (!ytitle.empty())
   {
      h->GetYaxis()->SetTitle(ytitle.c_str());
      h->GetYaxis()->CenterTitle();
   }
   if (!xtitle.empty())
   {
      h->GetXaxis()->SetTitle(xtitle.c_str());
      h->GetXaxis()->CenterTitle();
   }
}

//---------------------------------------------------------------------------------------------
double eventRates::GetWeight(const std::vector<double> xdet,
                             double &nu_wght,
                             double &nu_energy)
{

   // assumes units are GeV and cm

   const double rdet = 100.0;     // in cm
   const double pimass = 0.13957; // in GeV
   const double kmass = 0.49368;
   const double k0mass = 0.49767;
   const double mumass = 0.105658389;
   const double taumass = 1.77682;

   // these are geant codes not PDG
   const int nue = 53;
   const int nuebar = 52;
   const int numu = 56;
   const int numubar = 55;
   const int nutau = 58;    // is this right?
   const int nutaubar = 59; // is this right?
   const int muplus = 5;
   const int muminus = 6;

   double parent_mass = 0.;
   if (ptype == 8 || ptype == 9)
      parent_mass = pimass;
   else if (ptype == 11 || ptype == 12)
      parent_mass = kmass;
   else if (ptype == 10)
      parent_mass = k0mass;
   else if (ptype == 5 || ptype == 6)
      parent_mass = mumass;
   else
   {
      cout << "eventRates::GetWeight - Wrong parent type!! " << ptype << " = "
           << ptype << " Decay code = " << Ndecay << endl;

      return -999;
   }

   double parent_energy = sqrt(pdPx * pdPx +
                               pdPy * pdPy +
                               pdPz * pdPz +
                               parent_mass * parent_mass);
   double gamma = parent_energy / parent_mass;
   double gamma_sqr = gamma * gamma;
   double beta_mag = sqrt((gamma_sqr - 1.) / gamma_sqr);

   double enuzr = Necm;

   double rad = sqrt((xdet[0] - Vx) * (xdet[0] - Vx) +
                     (xdet[1] - Vy) * (xdet[1] - Vy) +
                     (xdet[2] - Vz) * (xdet[2] - Vz));

   double parentp = sqrt((pdPx * pdPx) +
                         (pdPy * pdPy) +
                         (pdPz * pdPz));
   double costh_pardet = (pdPx * (xdet[0] - Vx) +
                          pdPy * (xdet[1] - Vy) +
                          pdPz * (xdet[2] - Vz)) /
                         (parentp * rad);

   if (costh_pardet > 1.)
      costh_pardet = 1.;
   else if (costh_pardet < -1.)
      costh_pardet = -1.;
   double theta_pardet = acos(costh_pardet);

   double emrat = 1. / (gamma * (1. - beta_mag * cos(theta_pardet)));

   nu_energy = emrat * enuzr;

   double sangdet = (rdet * rdet / (rad * rad) / 4.);

   nu_wght = sangdet * emrat * emrat;

   // done for all except polarized muon
   //  in which case need to modify weight
   if (ptype == muplus || ptype == muminus)
   {
      // boost new neutrino to mu decay cm
      double beta[3];
      double p_nu[3]; // nu momentum
      beta[0] = pdPx / parent_energy;
      beta[1] = pdPy / parent_energy;
      beta[2] = pdPz / parent_energy;

      p_nu[0] = (xdet[0] - Vx) * nu_energy / rad;
      p_nu[1] = (xdet[1] - Vy) * nu_energy / rad;
      p_nu[2] = (xdet[2] - Vz) * nu_energy / rad;

      double partial = gamma * (beta[0] * p_nu[0] +
                                beta[1] * p_nu[1] +
                                beta[2] * p_nu[2]);
      partial = nu_energy - partial / (gamma + 1.);
      double p_dcm_nu[4];
      for (int i = 0; i < 3; i++)
         p_dcm_nu[i] = p_nu[i] - beta[i] * gamma * partial;
      p_dcm_nu[3] = 0.;
      for (int i = 0; i < 3; i++)
         p_dcm_nu[3] += p_dcm_nu[i] * p_dcm_nu[i];
      p_dcm_nu[3] = sqrt(p_dcm_nu[3]);

      // boost parent of mu to mu production cm
      gamma = ppenergy / parent_mass;
      beta[0] = ppdxdz * pppz / ppenergy;
      beta[1] = ppdydz * pppz / ppenergy;
      beta[2] = pppz / ppenergy;
      partial = gamma * (beta[0] * muparpx +
                         beta[1] * muparpy +
                         beta[2] * muparpz);
      partial = mupare - partial / (gamma + 1.);
      double p_pcm_mp[4];
      p_pcm_mp[0] = muparpx - beta[0] * gamma * partial;
      p_pcm_mp[1] = muparpy - beta[1] * gamma * partial;
      p_pcm_mp[2] = muparpz - beta[2] * gamma * partial;
      p_pcm_mp[3] = 0.;
      for (int i = 0; i < 3; i++)
         p_pcm_mp[3] += p_pcm_mp[i] * p_pcm_mp[i];
      p_pcm_mp[3] = sqrt(p_pcm_mp[3]);

      double wt_ratio = 1.;
      // have to check p_pcm_mp
      // it can be 0 if mupar..=0. (I guess muons created in target??)
      if (p_pcm_mp[3] != 0.)
      {
         // calc new decay angle w.r.t. (anti)spin direction
         double costh = (p_dcm_nu[0] * p_pcm_mp[0] +
                         p_dcm_nu[1] * p_pcm_mp[1] +
                         p_dcm_nu[2] * p_pcm_mp[2]) /
                        (p_dcm_nu[3] * p_pcm_mp[3]);

         if (costh > 1.)
            costh = 1.;
         else if (costh < -1.)
            costh = -1.;

         // calc relative weight due to angle difference
         if (Ntype == nue || Ntype == nuebar)
         {
            wt_ratio = 1. - costh;
         }
         else if (Ntype == numu || Ntype == numubar)
         {
            double xnu = 2. * enuzr / mumass;
            wt_ratio = ((3. - 2. * xnu) - (1. - 2. * xnu) * costh) / (3. - 2. * xnu);
         }
         else if (Ntype == nutau || Ntype == nutaubar)
         {
            double xnu = 2. * enuzr / taumass;
            wt_ratio = ((3. - 2. * xnu) - (1. - 2. * xnu) * costh) / (3. - 2. * xnu);
            std::cout << "calculating weight for tau neutrino; this may not be correct" << std::endl;
         }
         else
         {
            std::cout << "eventRates:: Bad neutrino type = " << Ntype << std::endl;
         }
      }
      nu_wght *= wt_ratio;
   }

   return nu_wght;
}

double eventRates::GetXSec(double nu_type,
                           double nu_energy,
                           std::string current, int process_type)

{

   if (current != "NC" && current != "CC")
   {
      cout << " eventRates::GetXSec: Current other than NC or CC specified... I don't know what to do." << endl;
      return -999;
   }

   int file_index = 0; // File index matches up with suffix array

   // numu
   if (nu_type == 56)
   {
      file_index = process_type;
   }

   // numubar
   if (nu_type == 55)
   {
      file_index = 6 + process_type;
   }

   // nue
   if (nu_type == 53)
   {
      file_index = 12 + process_type;
   }

   // nuebar
   if (nu_type == 52)
   {
      file_index = 18 + process_type;
   }

   // nutau
   if (nu_type == 58)
   {
      file_index = 24 + process_type;
   }

   // nutaubar
   if (nu_type == 59)
   {
      file_index = 30 + process_type;
   }

   int current_index = 0;
   if (current == "NC")
      current_index = 1;

   // calculate cross section
   double thexsec = 0.;

   // scale factor
   double scale_factor = 6.026e-10;
   //      xseccc = xseccc * 4.09e9
   //     10**-38 cm2 * 10**6 kg/kton * 3.8e20 POT/year *1iron/56/1.66e-27kg
   //     * 10**-4 m2/cm2 = 4.09e9 conversion factor
   //     since table was 26*sigma(nu-proton) + 30*sigma(nu-neutron)
   //
   //     10**-38 cm2 * 10**6 kg/kton *1iron/56/1.66e-27kg
   //     * 10**-4 m2/cm2 = 1.076e-11 conversion factor
   //
   //     10**-38 cm2 * 10**6 kg/kton * 1 nucleon / 1.66e-27 kg
   //     * 10**-4 m2/cm2 = 6.026e-10

   // if energy is higher than any available xsection point
   // return xsection of highest available energy point
   if (nu_energy > f_e_arr[fnlines - 1][file_index][current_index])
   {
      thexsec = f_xsec_arr[fnlines - 1][file_index][current_index] * f_e_arr[fnlines - 1][file_index][current_index] * scale_factor;
   }
   // if energy is lower than any available xsection point
   // return xsection of lowest available energy point
   else if (nu_energy < f_e_arr[0][file_index][current_index])
   {
      thexsec = f_xsec_arr[0][file_index][current_index] * f_e_arr[0][file_index][current_index] * scale_factor;
   }
   else
   {
      // if not, find the xsections for energy values immendiately above and
      // below the requested energy
      int energy_index = 0;
      for (int i = 0; i < fnlines - 1; i++)
         if (nu_energy > f_e_arr[i][file_index][current_index] &&
             nu_energy < f_e_arr[i + 1][file_index][current_index])
         {
            energy_index = i;
            break;
         }

      double sig1 = f_xsec_arr[energy_index][file_index][current_index];
      double sig2 = f_xsec_arr[energy_index + 1][file_index][current_index];

      thexsec = sig1 + ((sig2 - sig1) / (f_e_arr[energy_index + 1][file_index][current_index] - f_e_arr[energy_index][file_index][current_index])) * (nu_energy - f_e_arr[energy_index][file_index][current_index]);
      thexsec = thexsec * nu_energy;

      thexsec = thexsec * scale_factor;
   }

   // std::cout<<"nu_type "<<nu_type<<" current "<<current<<" energy "<<nu_energy <<" xsec "<<thexsec/nu_energy/scale_factor<<std::endl;

   return thexsec;
}

void eventRates::ReadXSecsFromFiles()
{
   fnbins = 1500;

   std::string base("data/genie/");

   for (int current = 1; current <= 2; current++)
   {

      string charge = "cc";

      if (current == 2)
      {
         charge = "nc";
      }

      // declare up suffix array: format is [neutrino type][process], current is included in "charge" part

      const int narr = 36;

      std::string suffix[narr];

      int suffix_iter = 0;

      // Create suffixes in order – numu, numubar, nue, nuebar, nutau, nutaubar

      for (int i = 0; i < 6; i++)
      {
         for (int k = 0; k < 6; k++)
         {
            if (k == 5) // if it's a total xsec, like total cc or total nc
            {
               suffix[suffix_iter] = "_" + neutrino_types_array[i] + ".dat";
            }
            else
            {
               suffix[suffix_iter] = "_" + processes_array[k] + "_" + neutrino_types_array[i] + ".dat";
            }

            suffix_iter++;
         }
      }

      string filename[narr];

      for (int i = 0; i < narr; i++)
      {

         filename[i] = "xsec_" + charge + suffix[i];
         string tmpfilename = base + filename[i];

         fdat_file[i].open(tmpfilename.c_str());

         double row[2];
         fnlines = 0;

         while (fdat_file[i] >> row[0] >> row[1])
         {

            if (fnlines >= fnbins)
            {
               cout << " length of data file exceed array size. Fix me. " << filename << endl;
               assert(0);
            }

            f_e_arr[fnlines][i][current - 1] = row[0];
            f_xsec_arr[fnlines][i][current - 1] = row[1];
            fnlines++;
         }
         fdat_file[i].close();
      }
   }
}
