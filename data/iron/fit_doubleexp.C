
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

  double tmin = 0.125, tmax = 50.125;

  int nbins = 625;

  auto file = new TFile("doublestop.root");

  auto tree = file->Get<TTree>("events");

  auto h = new TH1D("Iron", "Iron;#Delta t [#mus]; ", nbins, tmin, tmax);

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


  int entries = h->GetEntries();

  auto c1 = new TCanvas("c1", "c1");
  TPad *pad1 = new TPad("","",0,0.33,1,1);
  TPad *pad2 = new TPad("","",0,0,1,0.33);
  pad1->SetBottomMargin(0.00001);
  pad1->SetBorderMode(0);
  pad2->SetTopMargin(0.00001);
  pad2->SetBottomMargin(0.2);
  pad2->SetBorderMode(0);
  pad1->Draw();
  pad2->Draw();

  auto decay = new TF1("decay", fitFunc, tmin, tmax, 5);

  //decay->SetParameters(1, 1, 1, 1, 1);
  //h->Fit(decay, "L I R B 0 ");

  decay->SetParameters(0.5, 75, 2.197, 150, 0.2);
  // decay->FixParameter(2, 4);
  pad1->cd();
  pad1->SetGrid();
  h->Fit(decay, "L R B I V ");
  //h->SetMarkerStyle(21);
  //h->SetMarkerSize(0.5);
  h->Draw("E");
  gStyle->SetOptStat(0);
  gStyle->SetOptFit(1111);
  auto s_entries = Form("Entries: %.0f", h->GetEntries());
  auto s_mean = Form("Mean: %.2f", h->GetMean());
  auto pt = new TPaveText(tmax - 20, 160, tmax, 200, "nb");
  pt->AddText(s_entries);
  pt->AddText(s_mean);
  pt->AddText("Tempo di acquisizione: 121 h");
  pt->AddText("Plot salvato in data 08/04/2022");
  pt->Draw();

  pad2->cd();
  pad2->SetGrid();
  auto res = new TGraph();

  for (auto i=1; i<=nbins; i++) {
    double diff = h->GetBinContent(i) - decay->Eval(h->GetBinCenter(i));
    res->AddPoint(h->GetBinCenter(i), diff);
  }
  res->SetMarkerStyle(8);
  res->SetMarkerSize(0.8);
  res->Draw("AP");
  res->GetXaxis()->SetLabelSize(0.07);
  res->GetXaxis()->SetLimits(tmin, tmax);
  res->GetXaxis()->SetTitleSize(0.1);
  res->GetXaxis()->SetTitle("#Deltat [#mus]");
  res->GetYaxis()->SetLabelSize(0.07);

  auto zero = new TF1("zero", "0", tmin, tmax);
  zero->Draw("SAME");
  //res->SetAxisRange(-10, 10, "Y");
  c1->SaveAs("figures/final.eps");
  c1->SaveAs("figures/final.png");

  /*c2->cd();
  res->Draw();
  //h->Fit(decay, "L R B I ");
  //h->SetMarkerStyle(21);
  //h->SetMarkerSize(0.5);
  //h->Draw("E");
  h->SetAxisRange(tmin, 6, "X");
  auto pt1 = new TPaveText(3, 160, 6, 200, "nb");
  pt1->AddText(s_entries);
  pt1->AddText(s_mean);
  pt1->AddText("Tempo di acquisizione: 5 giorni");
  pt1->AddText("Plot salvato in data 06/04/2022");
  pt1->Draw();
  //gStyle->SetOptStat(0);

  c2->SaveAs("figures/final_zoom.eps");
  c2->SaveAs("figures/final_zoom.png");
  */
  double a1 = decay->GetParameter(1), tau1 = decay->GetParameter(2), a2 = decay->GetParameter(3), tau2 = decay->GetParameter(4);
  double da1 = decay->GetParError(1), dtau1 = decay->GetParError(2), da2 = decay->GetParError(3), dtau2 = decay->GetParError(4);

  double n1 = a1 * tau1; double dn1 = sqrt( pow(a1 * dtau1, 2) + pow(tau1 * da1, 2) + (2 * n1) * (-3.678e-01));
  double n2 = a2 * tau2; double dn2 = sqrt( pow(a2 * dtau2, 2) + pow(tau2 * da2, 2) + (2 * n2) * (-1.371e+00));

  double x = (n1 - n2) / (n1 + n2);
  double dx = (dn1 + dn2) * (1.0/(n1 + n2) + 1.0/(n1 - n2));

  cout << "Rapporto abbondanze = " << x << " +- " << dx <<endl;


}
