/************************************************************
 * Project: AMPT to ROOT Data Converter
 * Author: Dibakar Dhar
 * GitHub: https://github.com/dibakardhar
 *
 * Description:
 * This ROOT script reads raw output data generated from AMPT
 * (A Multi-Phase Transport Model) simulations and converts
 * it into structured ROOT format (.root file) for further
 * analysis in high-energy physics studies.
 *
 * Features:
 * - Reads AMPT generated event data
 * - Extracts relevant physics observables
 * - Stores processed data into ROOT TTrees
 * - Optimized for fast analysis workflows
 *
 * Usage:
 * root -l ampt_to_root.C
 *
 * Dependencies:
 * - ROOT Framework (https://root.cern)
 *
 * Notes:
 * Ensure input AMPT data format matches expected structure
 * before running the script.
 *
 ************************************************************/

#include <TFile.h>
#include <TTree.h>
#include <TMath.h>

#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>
#include <string>

void ampt_to_root()
{
    const char* infile  = "ampt.dat";
    const char* outfile = "ampt_event.root";
    int maxEvents = 4000;

    std::ifstream fin(infile);
    if (!fin.is_open()) {
        std::cerr << "Error opening file!" << std::endl;
        return;
    }

    TFile *fout = new TFile(outfile, "RECREATE");
    TTree *tree = new TTree("tree", "Event-wise AMPT data");

    Int_t event_id;
    Int_t mult;

    std::vector<int> pdg;
    std::vector<float> pt, eta, phi, mass;

    tree->Branch("event_id", &event_id, "event_id/I");
    tree->Branch("mult", &mult, "mult/I");

    tree->Branch("pdg", &pdg);
    tree->Branch("pt", &pt);
    tree->Branch("eta", &eta);
    tree->Branch("phi", &phi);
    tree->Branch("mass", &mass);

    int event, test, npart;
    float b;
    int npart1, npart2;
    int npart1_el, npart1_inel, npart2_el, npart2_inel;

    int pid;
    float px, py, pz, m;
    float x, y, z, t;

    int eventCounter = 0;
    std::string line;

    while (std::getline(fin, line))
    {
        if (line.empty()) continue;

        std::stringstream header(line);

        if (!(header >> event >> test >> npart >> b
                      >> npart1 >> npart2
                      >> npart1_el >> npart1_inel
                      >> npart2_el >> npart2_inel))
        {
            std::cout << "Skipping malformed header line\n";
            continue;
        }

        if (maxEvents > 0 && eventCounter >= maxEvents) break;

        event_id = event;

        pdg.clear();
        pt.clear();
        eta.clear();
        phi.clear();
        mass.clear();

        eventCounter++;

        std::cout << "Processing event " << event_id 
                  << " npart = " << npart << std::endl;

        for (int i = 0; i < npart; i++)
        {
            if (!std::getline(fin, line)) break;

            if (line.empty()) {
                i--;
                continue;
            }

            std::stringstream pdata(line);

            if (!(pdata >> pid >> px >> py >> pz >> m >> x >> y >> z >> t)) {
                std::cout << "Skipping malformed particle line\n";
                continue;
            }

            // =========================
            // Particle selection
            // =========================
            if (!(pid == 211 || pid == -211 ||
                  pid == 321 || pid == -321 ||
                  pid == 2212 || pid == -2212))
                continue;

            float pt_val = TMath::Sqrt(px*px + py*py);

            // CUT 1: remove low pT (fixes phi spike + noise)
            if (pt_val < 0.1) continue;

            float p = TMath::Sqrt(px*px + py*py + pz*pz);

            // CUT 2: remove singular eta region
            if (p - fabs(pz) < 1e-8) continue;

            float phi_val = TMath::ATan2(py, px);
            float eta_val = 0.5 * TMath::Log((p + pz) / (p - pz));

            pdg.push_back(pid);
            pt.push_back(pt_val);
            eta.push_back(eta_val);
            phi.push_back(phi_val);
            mass.push_back(m);
        }

        mult = pdg.size();
        tree->Fill();
    }

    tree->Write();
    fout->Close();

    std::cout << "\nTotal processed events: " << eventCounter << std::endl;
    std::cout << "Output file: " << outfile << std::endl;
}
