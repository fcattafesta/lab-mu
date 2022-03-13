#include "ROOT/RDataFrame.hxx"
#include "ROOT/RVec.hxx"
#include "TH1F.h"
#include "TF1.h"
#include "TCanvas.h"
#include "TLegend.h"
#include <iostream>

void time_analysis() {

  double tmin = 1.81, tmax = 22.5;
  int nbins = 40;
  TFile *file = new TFile("total.root");
  TTree	* tree = (TTree*)file->Get("events");
  TH1F * h = new TH1F("hist", ";#Delta t [#mus]; ", nbins, tmin, tmax);
  TCanvas * c = new TCanvas();

  int channel; double times;

  auto fillHist = [](TTree* tree, TH1F* hist ) {
    int channel; double times;

  int events_in_range=0;

  for (auto i=0; i<tree->GetEntries()-1; i++) {
    int ch_i, ch_ii;
    double times_i, times_ii;

    for (auto i=0; i<tree->GetEntries()-1; i++) {
      int ch_i, ch_ii;
      double times_i, times_ii;

      tree->GetEntry(i);
      ch_i = channel;
      times_i = times;

    if (ch_ii > ch_i) {
      double dt = (times_ii - times_i) * 1e6;
      h->Fill(dt);
      if (dt>= tmin && dt<=tmax) events_in_range ++;
    }
  };

  double r_fake = 0.03;
  double noise_const = 0.03*events_in_range/nbins;
  cout << "Eventi nel range selezionato = " << events_in_range << endl;
  cout << "Costante di rumore = " << noise_const << endl;

  TF1 * func1 = new TF1("myExpo", "[2]+expo", tmin, tmax);

  TF1 * f1 = new TF1("f1", "expo + 1.92975", tmin, tmax);
  //f1->SetParameter(2, 8.22);

  h->Fit(f1, "L");
  auto l = h->GetFunction("f1");
  l->SetLineColor(kBlue);
  //h->Fit(f2, "+");
  //auto chi = h->GetFunction("f2");
  //chi->SetLineColor(kOrange);
  h->Draw("E1");
  l->Draw("same");


  /*auto legend = new TLegend(0.3, 0.3, 0.3, 0.3);
  legend->AddEntry(h, "Measured distribution");
  legend->AddEntry(l, "Likelihood expo fit");
  legend->AddEntry(h, "Chi2 expo fit");
  legend->Draw();*/
  //c->SaveAs("figures/fit.png");

}
