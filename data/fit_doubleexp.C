void fit_doubleexp() {

  double tmin = 0.1, tmax = 6;

  int nbins = 80;

  auto file = new TFile("aluminum/doublestop.root");

  auto tree = file->Get<TTree>("events");

  auto h = new TH1D("h", ";#Delta t [#mus]; ", nbins, tmin, tmax);

  int channel; Double_t times;

  tree->SetBranchAddress("channel", &channel);
  tree->SetBranchAddress("times", &times);

  for (auto i=0; i<tree->GetEntries()-1; i++) {

      int ch_i, ch_ii, ch_iii;
      double times_i, times_ii, times_iii;

      tree->GetEntry(i);
      ch_i = channel;
      times_i = times;

      tree->GetEntry(i+1);
      ch_ii = channel;
      times_ii = times;

      tree->GetEntry(i+2);
      ch_iii = channel;
      times_iii = times;

      Double_t dt = (times_ii - times_i) * 1e6;

      if ((ch_ii > ch_i) && ch_i == 1 && ch_iii <= ch_i && dt >= tmin && dt <= tmax) {
          h->Fill(dt);
      }

  }


  int entries = h->GetEntries();


  auto decay = new TF1("decay", "expo + expo + [4]", tmin, tmax);

  decay->SetParLimits(0, 3.0, 4.0);
  decay->SetParLimits(1, -5.0, -4.0);
  decay->SetParLimits(2, 3.0, 5.0);
  decay->SetParLimits(3, -2.0, -1.0);
  decay->SetParLimits(4, 5.0, 10.0);
  //decay->FixParameter(3, -1.13);

  auto c1 = new TCanvas("c1", "c1");
  c1->cd();

  h->Fit(decay, "L I R");
  h->Draw("E1");
  gStyle->SetOptFit(1111);
}
