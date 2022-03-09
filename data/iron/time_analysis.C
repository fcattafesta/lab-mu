#include "ROOT/RDataFrame.hxx"
#include "ROOT/RVec.hxx"
#include "TH1F.h"
#include "TF1.h"
#include "TCanvas.h"
#include "TLegend.h"
#include <iostream>

void time_analysis() {

  TFile *file = new TFile("total.root");
  TTree	* tree = (TTree*)file->Get("events");
  TH1F * h = new TH1F("hist", ";#Delta t [#mus]; ", 20, 1.5, 22.5);
  TCanvas * c = new TCanvas();

  int channel; double times;

  tree->SetBranchAddress("channel", &channel);
  tree->SetBranchAddress("times", &times);

  for (auto i=0; i<tree->GetEntries()-1; i++) {
    int ch_i, ch_ii;
    double times_i, times_ii;

    tree->GetEntry(i);
    ch_i = channel;
    times_i = times;

    tree->GetEntry(i+1);
    ch_ii = channel;
    times_ii = times;

    if (ch_ii > ch_i) {
      h->Fill((times_ii - times_i) * 1e6 );
    }
  }

  TF1 * f1 = new TF1("f1", "expo", 0, 22.5);

  h->Fit(f1, "L");
  auto l = h->GetFunction("f1");
  l->SetLineColor(kBlue);
  h->Fit("expo", "+");
  auto chi = h->GetFunction("expo");
  chi->SetLineColor(kOrange);
  h->Draw("E1");

  auto legend = new TLegend(0.5, 0.5, 0.5, 0.5);
  legend->AddEntry(h, "Measured distribution");
  legend->AddEntry(l, "Likelihood expo fit");
  legend->AddEntry(h, "Chi2 expo fit");
  legend->Draw();
  c->SaveAs("figures/fit.png");

}
