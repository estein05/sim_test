#include <Riostream.h>

#include "lib/detector.h"
#include "lib/collision.h"
#include "lib/hit.h"
#include "lib/tracklet.h"

#include "TClonesArray.h"
#include "TStopwatch.h"
#include "TObject.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include <vector>
#include "TEllipse.h"
#include "TGraphPolar.h"


#define R_INNER1_DET 23.0
#define W_INNER1_DET 120.0

#define R_INNER2_DET 29.0
#define W_INNER2_DET 120.0

#define R_OUTER1_DET 70.0
#define W_OUTER1_DET 360.0

void polar_cut(int Event){
    TFile *sourceFile = new TFile("simulation.root", "READ");
    TTree *tree = (TTree *)(sourceFile->Get("hits"));
    if (!tree) return;
    TClonesArray *hitsL1 = new TClonesArray("hit", 100);
    TClonesArray *hitsL2 = new TClonesArray("hit", 100);
    double        ptc[4];
    tree->GetBranch("Vertex")->SetAddress(&ptc[0]);
    tree->GetBranch("L1hit")->SetAddress(&hitsL1);
    tree->GetBranch("L2hit")->SetAddress(&hitsL2);
    tree->SetBranchStatus("*", 1); // Activates reading of all branches
    TStopwatch watch;
    uint32_t   nEntries = tree->GetEntries();
    std::cout << nEntries << '\n';

    tree->GetEvent(Event);

    std::cout << "Vertex: " << ptc[0]
              << " " << ptc[1]
              << " " << ptc[2]
              << " " << ptc[3] << std::endl;

    TIter     l1iterator(hitsL1);
    TIter     l2iterator(hitsL2);
    hit *     hitL1;
    hit *     hitL2;

    l1iterator.Reset();
    l2iterator.Reset();
    while ((hitL1 = (hit *)l1iterator.Next())) {
      std::cout << "Theta L1: " << hitL1->getTheta() <<'\n';
       while ((hitL2 = (hit *)l2iterator.Next())) {
         std::cout << "Theta L2: " << hitL2->getTheta() <<'\n';
       }
       l2iterator.Reset();
    }






    TCanvas * CPol = new TCanvas("CPol","TGraphPolar Example",1000,1000);

    CPol->cd();


    Double_t r_0 = TMath::Sqrt(ptc[0]*ptc[0] + ptc[1]*ptc[1]);
    Double_t theta[] = {0, 0.1, 0.2,0.5};
    Double_t radius[] = {r_0, 23, 29, 70};
    // Double_t z[] = {0, 50, 60, 100};



    //Draw detector layers
    TEllipse *c1 = new TEllipse(0,0, 0.70, 0.70);
    c1->SetLineColor(kRed+3);
    c1->Draw();
    TEllipse *c2 = new TEllipse(0,0,0.29, 0.29);
    c2->SetLineColor(kRed+3);
    c2->Draw("same");
    TEllipse *c3 = new TEllipse(0,0,0.23, 0.23);
    c3->SetLineColor(kRed+3);
    c3->Draw("same");

    TGraphPolar * grP1 = new TGraphPolar(4, theta, radius);
    grP1->SetTitle("Detector Cut");


    //setting style
    grP1->SetMarkerStyle(20);
    grP1->SetMarkerSize(2);
    grP1->SetMarkerColor(4);
    grP1->SetLineColor(2);
    grP1->SetLineWidth(3);
    //grP1->SetFillStyle(0);
    grP1->Draw("CP same");

    // Update, otherwise GetPolargram returns 0
    CPol->Update();
    grP1->GetPolargram()->SetToRadian();
    grP1->GetPolargram()->SetLineWidth(0);
    grP1->GetPolargram()->SetRangeRadial(0,100);

    //CPol->Update();

    return;
}
