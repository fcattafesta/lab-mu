
double free(double* x, double* par) {
  return par[0] + par[1] * exp(-x[0] / par[2]);
}

double bound(double* x, double* par) {
  return par[0] * exp(-x[0] / par[1]);
}

double fitFunc(double* x, double* par) {
  return free(x, par) + bound(x, &par[3]);
}

void fit_doubleexp() {

  double tmin = 0.16, tmax = 22.5;

  int nbins = 425;

  auto file = new TFile("doublestop.root");

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

  auto decay = new TF1("decay", fitFunc, tmin, tmax, 5);

  //decay->SetParameters(1, 1, 1, 1, 1);
  //h->Fit(decay, "L I R B 0 ");

  decay->SetParameters(0.5, 75, 2.197, 150, 0.2);
  // decay->FixParameter(2, 4);

  h->Fit(decay, "L R B I ");
  //h->SetMarkerStyle(21);
  //h->SetMarkerSize(0.5);
  h->Draw("E1");

  gStyle->SetOptFit(1111);
}
