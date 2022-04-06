
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

  double tmin = 0.125, tmax = 50.15;

  int nbins = 200;

  auto file = new TFile("mag.root");

  auto tree = file->Get<TTree>("events");

  auto h = new TH1D("Aluminum", "Aluminum;#Deltat [#mus]; ", nbins, tmin, tmax);

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

      Double_t dt = (times_ii - times_i - 15e-9) * 1e6;

      if ((ch_ii > ch_i) && ch_i == 1 && ch_iii <= ch_i && dt >= tmin && dt <= tmax) {
          h->Fill(dt);
      }

  }

  auto file1 = new TFile("../iron/doublestop.root");

  auto tree1 = file1->Get<TTree>("events");

  tree1->SetBranchAddress("channel", &channel);
  tree1->SetBranchAddress("times", &times);

  for (auto i=0; i<tree1->GetEntries()-1; i++) {

      int ch_i, ch_ii, ch_iii;
      double times_i, times_ii, times_iii;

      tree1->GetEntry(i);
      ch_i = channel;
      times_i = times;

      tree1->GetEntry(i+1);
      ch_ii = channel;
      times_ii = times;

      tree1->GetEntry(i+2);
      ch_iii = channel;
      times_iii = times;

      Double_t dt = (times_ii - times_i - 15e-9) * 1e6;

      if ((ch_ii > ch_i) && ch_i == 1 && ch_iii <= ch_i && dt >= tmin && dt <= tmax) {
          h->Fill(dt);
      }

  }


  int entries = h->GetEntries();

  auto c1 = new TCanvas("c1", "c1");
  auto c2 = new TCanvas("c2", "c2");
  c2->SetGrid();
  c1->cd();
  c1->SetGrid();

  auto decay = new TF1("decay", fitFunc, tmin, tmax, 5);

  //decay->SetParameters(1, 1, 1, 1, 1);
  //h->Fit(decay, "L I R B 0 ");

  decay->SetParameters(10, 300, 2.2, 200, 0.88);
  // decay->FixParameter(2, 4);

  h->Fit(decay, "L R B I ");
  //h->SetMarkerStyle(21);
  //h->SetMarkerSize(0.5);
  h->Draw("E");
  gStyle->SetOptStat(10);
  gStyle->SetOptFit(1111);

  c1->SaveAs("figures/final.eps");
  c1->SaveAs("figures/final.png");

  c2->cd();
  h->Fit(decay, "L R B I ");
  //h->SetMarkerStyle(21);
  //h->SetMarkerSize(0.5);
  h->Draw("E");
  h->SetAxisRange(tmin, 6, "X");
  //gStyle->SetOptStat(0);

  c2->SaveAs("figures/final_zoom.eps");
  c2->SaveAs("figures/final_zoom.png");


  double a1 = decay->GetParameter(1), tau1 = decay->GetParameter(2), a2 = decay->GetParameter(3), tau2 = decay->GetParameter(4);
  double da1 = decay->GetParError(1), dtau1 = decay->GetParError(2), da2 = decay->GetParError(3), dtau2 = decay->GetParError(4);

  double x = a2*tau2/(a1*tau1);
  double dx = x*((da1/a1) + (dtau1/tau1) + (da2/a2) + (dtau2/tau2));

  cout << "Rapporto abbondanze = " << x << " +- " << dx <<endl;

}
