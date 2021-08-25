#define R_INNER1_DET 23.0
#define W_INNER1_DET 120.0

#define R_INNER2_DET 29.0
#define W_INNER2_DET 120.0

#define R_OUTER1_DET 70.0
#define W_OUTER1_DET 360.0

void polar_cut(){
    TCanvas * CPol = new TCanvas("CPol","TGraphPolar Example",1000,1000);

    CPol->cd();

    Double_t theta[] = {0, 0.1,0.2,0.5};
    Double_t radius[] = {0, 23, 29, 70};
    Double_t z[] = {0, 50, 60, 100};



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
