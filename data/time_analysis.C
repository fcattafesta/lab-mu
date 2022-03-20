
void time_analysis() {

  double tmin1 = 0.1, tmax1 = 1.81;
  double tmin2 = tmin1, tmax2 = 55.0;

  int nbins = 30;

  auto f1 = new TFile("iron/doublestop.root");
  auto f2 = new TFile("iron/bkg.root");

  auto t1 = f1->Get<TTree>("events");
  auto t2 = f2->Get<TTree>("events");

  auto h1 = new TH1D("signal", ";#Delta t [#mus]; ", nbins, tmin1, tmax1);
  auto h2 = new TH1D("background", ";#Delta t [#mus]; ", nbins, tmin2, tmax2);

  auto c = new TCanvas("c", "c");

  auto tree_reader = [=](TTree* tree, TH1D* hist) {

    int channel; Double_t times;
    int entries = 0;

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

      Double_t dt = (times_ii - times_i) * 1e6;

      if ((ch_ii > ch_i) && dt >= 0.1 && dt <= 55.) {

        hist->Fill(dt);
        if (dt>= tmin1 && dt<=tmax1) entries ++;
      }
    }
      return entries;
  };

  int entries = tree_reader(t1, h1);

  int bkg_entries = tree_reader(t2, h2);

  cout << "Eventi nel range selezionato = " << entries << endl;


  // ----- ANALISI MUONE LIBERO ------------

/*
  auto decay = new TF1("decay", "expo", tmin1, tmax1);
  //auto noise = new TF1("noise", "gaus", 4.1, 5.5);
  auto unif = new TF1("unif", "pol1", tmin1, tmax1);

  auto tot = new TF1("tot", "expo + [2]", tmin1, tmax1);

  //c->SetLogy();

  h1->Fit(tot, "L I R");
  //h2->Fit(unif, "L");

  double p0 = tot->GetParameter(0), p1 = tot->GetParameter(1), p2 = tot->GetParameter(2);
  decay->SetParameters(p0, p1);
  unif->SetParameters(p2, 0);

  h1->Draw("E1");
  auto e = h1->GetFunction("tot");
  e->SetLineColor(kBlue);
  e->Draw("same");
  decay->SetLineColor(kGreen);
  decay->Draw("same");
  unif->SetLineColor(kRed);
  unif->Draw("same");

  double free_mu_events = entries*(decay->Integral(1.81, 55.)/tot->Integral(1.81, 55.));

  cout << "Decadimenti del muone libero [1.81, 55.0] = " << free_mu_events << endl;

*/


  // ----- ANALISI MUONE LEGATO ------------
  double n_pos_dx = 298.15;

  //Funzioni da sommare a quella di fit, usando 30 bin nel range [0.1, 1.81]:
  auto free_mu = new TF1("free_mu", "19.1148*TMath::Exp(-0.4755*x)", tmin1, tmax1);
  auto unif = new TF1("unif", "0.18409*x/x", tmin1, tmax1);

  auto bound_mu = new TF1("bound_mu", "expo", tmin1, tmax1);


  auto tot = new TF1("tot", "bound_mu + free_mu + unif", tmin1, tmax1);

  //c->SetLogy();

  h1->Fit(tot, "L I R");

  bound_mu->SetParameters(tot->GetParameter(0), tot->GetParameter(1));

  h1->Draw("E1");
  tot->SetLineColor(kBlue);   tot->Draw("same");
  bound_mu->SetLineColor(kGreen); bound_mu->Draw("same");
  free_mu->SetLineColor(kOrange);  free_mu->Draw("same");
  unif->SetLineColor(kRed);  unif->Draw("same");


  double n_neg = entries*(bound_mu->Integral(0.1, 1.81)/tot->Integral(0.1, 1.81));
  double n_pos_sx = entries*(free_mu->Integral(0.1, 1.81)/tot->Integral(0.1, 1.81));

  cout << "Rapporto abbondanze n-/n+ = " <<  n_neg/(n_pos_sx + n_pos_dx) << endl;
  cout << "Mu+ totali = " <<  n_pos_sx + n_pos_dx << endl;




  /*auto legend = new TLegend(0.3, 0.3, 0.3, 0.3);
  legend->AddEntry(h, "Measured distribution");
  legend->AddEntry(l, "Likelihood expo fit");
  legend->AddEntry(h, "Chi2 expo fit");
  legend->Draw();*/
  //c->SaveAs("figures/fit.png");





}
