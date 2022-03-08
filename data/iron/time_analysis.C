#include "ROOT/RDataFrame.hxx"
#include "ROOT/RVec.hxx"
#include "TH1F.h"
#include <iostream>

void time_analysis() {

  TFile *file = new TFile("0803_1530.root");
  TTree	* tree = (TTree*)file->Get("events");
  TH1F * h = new TH1F("hist", "#Delta t", 20, 0, 10);
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
  h->Draw();
}
