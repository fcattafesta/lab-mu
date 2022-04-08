void up_down() {


  double tmin = 0.1, tmax = 7;

  int nbins_u = 15;
  int nbins_d = nbins_u;

  auto file = new TFile("asymmetry.root");



  auto tree = file->Get<TTree>("events");

  auto h_up = new TH1D("up_mu", ";#Delta t [#mus]; ", nbins_u, tmin, tmax);
  auto h_down = new TH1D("down_mu", ";#Delta t [#mus]; ", nbins_d, tmin, tmax);

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

          if (ch_ii == 2) h_up->Fill(dt);
          if (ch_ii == 3) h_down->Fill(dt);
      }

  }

  double eff_up = 0.886;
  double deff_up = 0.008;

  double eff_down = 0.737;
  double deff_down = 0.012;

  double x_t[nbins_u], err_x[nbins_u], t[nbins_u], err_t[nbins_u];

  for (auto i=1; i<=nbins_u; i++) {
    auto n_up = h_up->GetBinContent(i);
    auto n_down = h_down->GetBinContent(i);

    if (n_up==0 || n_down==0) {
      x_t[i-1] = 58;
      err_x[i-1] = 58;
      t[i-1] = h_up->GetBinCenter(i);
      err_t[i-1] = h_up->GetBinWidth(i) * 0.5;
    }

    else {
    n_up = n_up / eff_up;
    n_down = n_down / eff_down;

    double dn_up = (n_up / pow(eff_up, 2) ) * deff_up;
    double dn_down = (n_down / pow(eff_down, 2)) * deff_down;

    double x = (n_up - n_down) / (n_up + n_down);
    double dx_bin = (2/(n_up+n_down))*TMath::Sqrt(n_up*n_down/(n_up+n_down));
    double dx_eff = (2/pow(n_up+n_down, 2))*TMath::Sqrt(pow(n_down * dn_up, 2) + pow(n_up *dn_down, 2));
    double dx = TMath::Sqrt(pow(dx_bin, 2) + pow(dx_eff, 2));

    x_t[i-1] = x;
    err_x[i-1] = dx;
    t[i-1] = h_up->GetBinCenter(i);
    err_t[i-1] = h_up->GetBinWidth(i) * 0.5;
    }
  }
  double n_up = h_up->GetEntries() / eff_up;
  double n_down = h_down->GetEntries() / eff_down;

  double dn_up = (n_up / pow(eff_up, 2) ) * deff_up;
  double dn_down = (n_down / pow(eff_down, 2)) * deff_down;

  double x = (n_up - n_down) / (n_up + n_down);
  double dx_bin = (2/(n_up+n_down))*TMath::Sqrt(n_up*n_down/(n_up+n_down));
  double dx_eff = (2/pow(n_up+n_down, 2))*TMath::Sqrt(pow(n_down * dn_up, 2) + pow(n_up *dn_down, 2));
  double dx = TMath::Sqrt(pow(dx_bin, 2) + pow(dx_eff, 2));

  cout << x << " +- " << dx << endl;
  cout << "sist. " << dx_bin << "stat." << dx_eff << endl;
  cout << h_up->GetBinContent(8) << endl;
  cout << h_down->GetBinContent(8) << endl;
  cout << "Up entries: " << h_up->GetEntries() <<endl;
  cout << "Down entries: " << h_down->GetEntries() <<endl;
  cout << "Tot entries: " << h_down->GetEntries() + h_up->GetEntries() <<endl;

  auto c1 = new TCanvas();

  auto func = new TF1("func", "[0] + [1]*cos([2]*x)", tmin, tmax);
  auto cte = new TF1("const", "pol0", tmin, tmax);
  func->SetParameters(0., 1, 1.7);
  c1->SetGrid();

  auto g_x = new TGraphErrors();
  g_x->SetTitle("Asymmetry; #Deltat [#mus];");

  for (auto i=0; i<nbins_u; i++) {
    auto n = g_x->GetN();
    if (x_t[i] != 58) {
      g_x->AddPoint(t[i], x_t[i]);
      g_x->SetPointError(n, err_t[i], err_x[i]);
    }
  }

  //g_x->Fit(func);
  g_x->Fit(cte);
  g_x->Draw("PA");
  g_x->SetMarkerStyle(21);
  gStyle->SetOptFit(0);
  auto s_entries = Form("Entries: %.0f", h_up->GetEntries()+h_down->GetEntries());
  auto pt = new TPaveText(tmax - 2, 0.1, tmax+2, 0.2, "nb");
  pt->AddText(s_entries);
  pt->AddText("Tempo di acquisizione: 5 giorni");
  pt->AddText("Plot salvato in data 06/04/2022");
  pt->Draw();

 c1->SaveAs("figures/asymmetry.pdf");

ofstream newfile;
newfile.open("Asymmetry_iron.txt");
newfile << "#time    #asymm" << "\n";
for (int idx=0; idx < nbins_u ;idx++) {
newfile << t[idx] << "    " << x_t[idx] << "\n"; //write to file
}

newfile.close();



}
