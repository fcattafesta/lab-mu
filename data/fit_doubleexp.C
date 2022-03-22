void fit_doubleexp() {

  double tmin = 0.3, tmax = 20;

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

  auto c1 = new TCanvas("c1", "c1");

  auto decay = new TF1("decay", "expo + expo + [4]", tmin, tmax);
  //auto decay = new TF1("decay", "[0]*exp(x*[1]) + [2]*exp(x*[3]) + [4]", tmin, tmax);

  /*decay->SetParLimits(0, 30, 40);
  decay->SetParLimits(1, -5.0, -4.0);
  decay->SetParLimits(2, 30, 50);
  decay->SetParLimits(3, -2.0, -1.0);
  decay->SetParLimits(4, 5.0, 10.0);
  */
  decay->SetParameters(6, -0.45, 4, -1.1, 10);
  //decay->FixParameter(1, -0.45);  decay->FixParameter(3, -1.1);

  h->Fit(decay, "L I R B ");
  h->Draw("E1");

  gStyle->SetOptFit(1111);
}
