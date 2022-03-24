void up_down() {


  double tmin = 0.1, tmax = 55.0;

  int nbins_u = 80;
  int nbins_d = 80;

  auto file = new TFile("doublestop.root");

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

  double n_up = h_up->GetEntries()*1.;
  double n_down = h_down->GetEntries()*1.;

  double eff_up = (0.968 * 0.930);
  double deff_up = TMath::Sqrt(pow(0.005/0.968, 2) + pow(0.007/0.930, 2));

  double eff_down = (0.92 * 0.856);
  double deff_down = TMath::Sqrt(pow(0.01/0.92, 2) + pow(0.009/0.856, 2));

  n_up = n_up / eff_up;
  n_down = n_down / eff_down;

  double dn_up = (n_up / pow(eff_up, 2) ) * deff_up;
  double dn_down = (n_down / pow(eff_down, 2)) * deff_down;

  double x = (n_up - n_down) / (n_up + n_down);
  double dx_bin = (2/(n_up+n_down))*TMath::Sqrt(n_up*n_down/(n_up+n_down));
  double dx_eff = (2/pow(n_up+n_down, 2))*TMath::Sqrt(pow(n_down * dn_up, 2) + pow(n_up *dn_down, 2));

  double dx = TMath::Sqrt(pow(dx_bin, 2) + pow(dx_eff, 2));

  cout << "Asimmetria: " << x << " +- " << dx << endl;
  cout << "Eventi totali = " << n_up+n_down << endl;

  auto c1 = new TCanvas("c1", "c1");
  h_up->Draw("E1");
  auto c2 = new TCanvas("c2", "c2");
  h_down->Draw("E1");

}
