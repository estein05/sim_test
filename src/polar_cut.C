#include "TGraphPolar.h"

#define R_INNER1_DET 23.0
#define W_INNER1_DET 120.0

#define R_INNER2_DET 29.0
#define W_INNER2_DET 120.0

#define R_OUTER1_DET 70.0
#define W_OUTER1_DET 360.0

void polar_cut(){
    TCanvas * CPol = new TCanvas("CPol","TGraphPolar Example",500,500);

    Double_t theta[8];
    Double_t radius[8];
    Double_t etheta[8];
    Double_t eradius[8];
    
    for (int i=0; i<8; i++) {
        theta[i]   = (i+1)*(TMath::Pi()/4.);
        radius[i]  = (i+1)*0.05;
    }
    
    TGraphPolar * grP1 = new TGraphPolar(8, theta, radius);
    grP1->SetTitle("Detector Cut");

    //Draw detector layers
    TEllipse *c1 = new TEllipse(0,0,R_INNER1_DET, R_INNER1_DET);
    c1->Draw();
    TEllipse *c2 = new TEllipse(0,0,R_INNER2_DET, R_INNER2_DET);
    c2->Draw("Same");
    TEllipse *c3 = new TEllipse(0,0,R_OUTER1_DET, R_OUTER1_DET);
    c3->Draw("Same");
    
    //setting style
    grP1->SetMarkerStyle(20);
    grP1->SetMarkerSize(2.);
    grP1->SetMarkerColor(4);
    grP1->SetLineColor(2);
    grP1->SetLineWidth(3);
    grP1->Draw("Psame");
    
    // Update, otherwise GetPolargram returns 0
    CPol->Update();
    grP1->GetPolargram()->SetToRadian();
    grP1->GetPolargram()->SetLineWidth(0);
    grP1->GetPolargram()->SetRangeRadial(0,100);
    
    //CPol->Update();
    
    return CPol;
}

