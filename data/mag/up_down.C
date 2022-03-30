void up_down() {


  double tmin = 0.1, tmax = 30;

  int nbins_u = 30;
  int nbins_d = 15;

  auto file = new TFile("mag.root");

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
    cout << "Asymmetry: " << x_t[i-1] <<endl;
    }
  }
  cout << h_up->GetBinContent(8) << endl;
  cout << h_down->GetBinContent(8) << endl;
  cout << "Up entries: " << h_up->GetEntries() <<endl;
  cout << "Down entries: " << h_down->GetEntries() <<endl;
  cout << "Up entries: " << h_down->GetEntries() + h_up->GetEntries() <<endl;

  auto c1 = new TCanvas();

  auto func = new TF1("func", "[0] + [1]*sin([2]*x + [3])", tmin, tmax);
  func->SetParameters(-0.07, 0.2, 1.8, 1.7);
  c1->SetGrid();

  for (auto i=0; i<nbins_u; i++) {
    if (x_t[i] == 58) {

    }
  }

  auto g_x = new TGraphErrors(nbins_u, t, x_t, err_t, err_x);
  g_x->SetTitle("Asymmetry; #Deltat [#mus];");
  g_x->Fit(func, "");
  g_x->Draw("PA");
  gStyle->SetOptFit(1111);

}
