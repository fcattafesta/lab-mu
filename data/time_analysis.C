
void time_analysis() {


  double time_sep = 5.136;
  double tmin = 0.25, tmax = 55.0;

  int nbins_r = 60;
  int nbins_l = 60;

  auto file = new TFile("aluminum/doublestop.root");
  //auto f2 = new TFile("iron/bkg.root");

  auto tree = file->Get<TTree>("events");
  //auto t2 = f2->Get<TTree>("events");

  auto h_bound = new TH1D("bound_mu", ";#Delta t [#mus]; ", nbins_l, tmin, time_sep);
  auto h_free = new TH1D("free_mu", ";#Delta t [#mus]; ", nbins_r, time_sep, tmax);


  //auto tree_reader = [=](TTree* tree, TH1D* hist) {

  int channel; Double_t times;
  int random = 0;

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

          if (dt < time_sep) h_bound->Fill(dt);
          if (dt > time_sep) h_free->Fill(dt);
      }

  }

  //cout << "random events" << random << endl;

  int entries_free = h_free->GetEntries();
  int entries_bound = h_bound->GetEntries();

  //int bkg_entries = tree_reader(t2, h2);

  //cout << "Eventi nel range selezionato = " << entries << endl;


  // ----- ANALISI MUONE LIBERO ------------


  auto decay = new TF1("decay", "expo", time_sep, tmax);
  //auto noise = new TF1("noise", "gaus", 4.1, 5.5);
  auto unif = new TF1("unif", "pol1", time_sep, tmax);

  auto tot_right = new TF1("tot_right", "expo + [2]", time_sep, tmax);

  auto c1 = new TCanvas("c1", "c1");
  c1->cd();
  //c->SetLogy();

  h_free->Fit(tot_right, "L I R");
  //h2->Fit(unif, "L");

  double p0 = tot_right->GetParameter(0), p1 = tot_right->GetParameter(1), p2 = tot_right->GetParameter(2);
  decay->SetParameters(p0, p1);
  unif->SetParameters(p2, 0);

  h_free->Draw("E1");
  tot_right->SetLineColor(kBlue);
  tot_right->Draw("same");

  //decay->SetLineColor(kGreen);
  //decay->Draw("same");
  //unif->SetLineColor(kRed);
  //unif->Draw("same");

  gStyle->SetOptFit(1111);

  double n_pos_dx = entries_free*(decay->Integral(time_sep, tmax)/tot_right->Integral(time_sep, tmax));

  cout << "Decadimenti del muone libero (right) = " << n_pos_dx << endl;



  // ----- ANALISI MUONE LEGATO ------------

  //la funzione del decadimento libero va "portata indietro" di 1.71? In quel caso le cose peggiorano.
  double p0_1 = TMath::Exp(p0) *((time_sep-tmin)/nbins_l)/((tmax-time_sep)/nbins_r);
  double p2_1 = p2*((time_sep-tmin)/nbins_l)/((tmax-time_sep)/nbins_r);
  cout << p0_1 << endl;
  //double n_pos_dx = 298.15;

  char * expo_const = Form("%f*TMath::Exp(-0.458*x)", p0_1);
  //char * expo_slope = Form("*TMath::Exp(%f*x)", p1);
  char * backg_const = Form("%f", p2_1);

  //Funzioni da sommare a quella di fit, usando 30 bin nel range [0.1, 1.81]:
  auto free_mu = new TF1("free_mu", expo_const, tmin, time_sep);
  auto unif_1 = new TF1("unif_1", backg_const, tmin, time_sep);

  auto bound_mu = new TF1("bound_mu", "expo", tmin, time_sep);


  auto tot_left = new TF1("tot_left", "bound_mu + free_mu + unif_1", tmin, time_sep);

  auto c2 = new TCanvas("c2", "c2");
  c2->cd();
  //c->SetLogy();

  h_bound->Fit(tot_left, "L I R");

  bound_mu->SetParameters(tot_left->GetParameter(0), tot_left->GetParameter(1));

  h_bound->Draw("E1");
  tot_left->SetLineColor(kBlue);   tot_left->Draw("same");
  bound_mu->SetLineColor(kGreen); bound_mu->Draw("same");
  free_mu->SetLineColor(kOrange);  free_mu->Draw("same");
  unif_1->SetLineColor(kRed);  unif_1->Draw("same");

  gStyle->SetOptFit(1111);

  double n_neg = entries_bound*(bound_mu->Integral(tmin, time_sep)/tot_left->Integral(tmin, time_sep));
  double n_pos_sx = entries_bound*(free_mu->Integral(tmin, time_sep)/tot_left->Integral(tmin, time_sep));

  cout << "Rapporto abbondanze n-/n+ = " <<  n_neg/(n_pos_sx + n_pos_dx) << endl;
  cout << "Mu+ totali = " <<  n_pos_sx + n_pos_dx << endl;
  cout << "Mu- totali = " <<  n_neg << endl;




  /*auto legend = new TLegend(0.3, 0.3, 0.3, 0.3);
  legend->AddEntry(h, "Measured distribution");
  legend->AddEntry(l, "Likelihood expo fit");
  legend->AddEntry(h, "Chi2 expo fit");
  legend->Draw();*/
  //c->SaveAs("figures/fit.png");





}
