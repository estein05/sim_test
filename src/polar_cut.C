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
#include "TMath.h"
#include <vector>
#include "TEllipse.h"
#include "TGraphPolar.h"
#include "TMultiGraph.h"
#include "TLine.h"
#include "TLegend.h"


#define R_INNER1_DET 23.0
#define W_INNER1_DET 120.0

#define R_INNER2_DET 29.0
#define W_INNER2_DET 120.0

#define R_OUTER1_DET 70.0
#define W_OUTER1_DET 360.0

double getx(double phi, double r);
double gety(double phi, double r);

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

    auto thetal1  = new double [hitsL1->GetEntries()];
    auto r1       = new double [hitsL1->GetEntries()];
    auto z1       = new double [hitsL1->GetEntries()];
    auto thetal2  = new double [hitsL1->GetEntries()*hitsL2->GetEntries()];
    auto r2       = new double [hitsL1->GetEntries()*hitsL2->GetEntries()];
    auto z2       = new double [hitsL1->GetEntries()*hitsL2->GetEntries()];

    uint8_t i = 0;
    uint8_t j = 0;

    while ((hitL1 = (hit *)l1iterator.Next())) {
      thetal1[i] = hitL1->getTheta();
      z1[i] = hitL1->getZ();
      r1[i] = R_INNER2_DET;
       while ((hitL2 = (hit *)l2iterator.Next())) {
         thetal2[j] = hitL2->getTheta();
         z2[j] = hitL2->getZ();
         if (hitL1->deltaTheta(hitL1, hitL2) < 0.15) {
           r2[j] = i;
         }
         else {
           r2[j] = 1000;
         }
         j++;
       }
       i++;
       l2iterator.Reset();
    }

    // Plotting--------------------------------------------------------------
    TCanvas * CPol = new TCanvas("CPol","Detector cut of single collision",1000,1000);
    //CPol->cd();


    Double_t r_0 = TMath::Sqrt(ptc[0]*ptc[0] + ptc[1]*ptc[1]);
    Double_t rho = 0;
    Double_t x = ptc[0];
    Double_t y = ptc[1];

    if (x > 0 && y > 0) {
       rho = TMath::ATan(y / x); //ok
    } else if (x == 0) {
       rho = (y > 0 ? 1. : -1.) * TMath::Pi() / 2;
    } else if (x > 0 && y < 0) {
       rho = 2 * TMath::Pi() + TMath::ATan(y / x);
    } else if (x < 0 && y > 0) {
       rho = TMath::Pi() + TMath::ATan(y / x);
    } else if (x < 0 && y < 0) {
       rho = TMath::Pi() + TMath::ATan(y / x);
    }

    Double_t theta_vertex = rho;
    Double_t radius_vertex = r_0;
    // Double_t z[] = {0, 50, 60, 100};



    //Draw detector layers
    TEllipse *c1 = new TEllipse(0,0, 0.70, 0.70);
    c1->SetLineColor(kGreen+2);
    c1->SetLineWidth(3);
    c1->Draw();
    TEllipse *c2 = new TEllipse(0,0,0.29, 0.29);
    c2->SetLineColor(kGreen+2);
    c2->SetLineWidth(3);
    c2->Draw("same");
    TEllipse *c3 = new TEllipse(0,0,0.23, 0.23);
    c3->SetLineColor(kRed+3);
    //c3->SetLineWidth(2);
    c3->Draw("same");


    //TLine *line = new TLine(0,0,1,1);
    TGraphPolar * grP1 = new TGraphPolar();
    //grP1->SetTitle("Detector Cut");
    //grP1->SetPoint(grP1->GetN(), theta_vertex, radius_vertex);
    auto *line_pos = new TLine();
    line_pos->SetLineColor(kRed);
    line_pos->SetLineWidth(4);
    auto *line_neg = new TLine();
    line_neg->SetLineColor(kBlue);
    line_neg->SetLineWidth(4);

    for(int u = 0; u < i; u++){
      TLine *line = new TLine(getx(theta_vertex,radius_vertex/100),gety(theta_vertex,radius_vertex/100),getx(thetal1[u],0.29),gety(thetal1[u],0.29));
      grP1->SetPoint(grP1->GetN(), thetal1[u], r1[u]);
      if (z1[u]>0){
        line->SetLineColor((int)(kRed+4-(int)(z1[u]/20)));
      }
      else {
        line->SetLineColor((int)(kBlue+4+(int)(z1[u]/20)));
      }
      line->SetLineWidth(4);
      line->Draw("same");
    }
    for(int u = 0; u < j; u++){
      if (r2[u] < 100 ) {
        TLine *line = new TLine(getx(thetal2[u], R_OUTER1_DET/100),gety(thetal2[u],R_OUTER1_DET/100),getx(thetal1[(int)r2[u]],0.29),gety(thetal1[(int)r2[u]],0.29));
        grP1->SetPoint(grP1->GetN(), thetal2[u], R_OUTER1_DET);
        if (z2[u]>0){
          line->SetLineColor((int)(kRed+4-(int)(z2[u]/60)));
        }
        else {
          line->SetLineColor((int)(kBlue+4+(int)(z2[u]/60)));
        }
        line->SetLineWidth(4);
        line->Draw("same");
      }
    }
    //line->Draw("same");
    //setting style
    grP1->SetMarkerStyle(20);
    grP1->SetMarkerSize(1.5);
    grP1->SetMarkerColor(kBlack);
    grP1->SetLineColor(2);
    grP1->SetLineWidth(3);
    //grP1->SetFillStyle(0);
    grP1->Draw("P same");

    // Legend
    auto legend = new TLegend();
    legend->SetHeader("Legend","C");
    legend->AddEntry(c1, "Detector Pixel Layer", "L");
    legend->AddEntry(grP1, "Detector Hits", "P");
    legend->AddEntry(line_pos, "Trace (z>0)", "L");
    legend->AddEntry(line_neg, "Trace (z<0)", "L");
    legend->Draw();

    // Update, otherwise GetPolargram returns 0
    CPol->Update();
    grP1->GetPolargram()->SetToRadian();
    grP1->GetPolargram()->SetPolarLabelSize(0);
    grP1->GetPolargram()->SetLineWidth(0);
    grP1->GetPolargram()->SetRangeRadial(0,100);
    grP1->GetPolargram()->SetRadialLabelSize(0.025);
    grP1->GetPolargram()->SetRadialOffset(0.01);



    //CPol->Update();
    delete[] thetal1;
    delete[] thetal2;
    delete[] r1;
    delete[] r2;
    return;
}

double getx(double phi, double r) {
  double x    = r * TMath::Cos(phi);
  return x;
}

double gety(double phi, double r) {
  double y    = r * TMath::Sin(phi);
  return y;
}
