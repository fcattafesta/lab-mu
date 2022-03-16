
void time_analysis() {

  double tmin1 = 1.8, tmax1 = 22.5;
  double tmin2 = tmin1, tmax2 = 55.0;

  int nbins = 30;

  auto f1 = new TFile("total.root");
  auto f2 = new TFile("bkg.root");

  auto t1 = f1->Get<TTree>("events");
  auto t2 = f2->Get<TTree>("events");

  auto h1 = new TH1D("signal", ";#Delta t [#mus]; ", nbins, tmin1, tmax1);
  auto h2 = new TH1D("background", ";#Delta t [#mus]; ", nbins, tmin2, tmax2);

  auto c = new TCanvas("c", "c");

  auto tree_reader = [=](TTree* tree, TH1D* hist) {

    int channel; double times;
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

      double dt = (times_ii - times_i) * 1e6;

      if ((ch_ii > ch_i) && dt <= 55.) {

        hist->Fill(dt);
        if (dt>= tmin1 && dt<=tmax1) entries ++;
      }
    }
      return entries;
  };

  int entries = tree_reader(t1, h1);

  int bkg_entries = tree_reader(t2, h2);

  /*double r_fake = 0.03;
  double noise_const = 0.03 * tree_reader(t1, h1) / nbins;

  cout << "Eventi nel range selezionato = " << entries << endl;
  cout << "Costante di rumore = " << noise_const << endl;
  */

  auto exp = new TF1("exp", "[2] + expo", tmin1, tmax1);
  auto unif = new TF1("unif", "pol0", tmin2, tmax2);

  //c->SetLogy();

  h1->Fit(exp, "L", "", tmin1, tmax1);
  //h2->Fit(unif, "L");
  auto e = h1->GetFunction("exp");
  //auto u = h2->GetFunction("unif");
  e->SetLineColor(kBlue);
  e->Draw("same");
  //u->SetLineColor(kRed);
  //u->Draw("same");
  h1->Draw("E1");
  //h2->Draw("E1 same");


  /*auto legend = new TLegend(0.3, 0.3, 0.3, 0.3);
  legend->AddEntry(h, "Measured distribution");
  legend->AddEntry(l, "Likelihood expo fit");
  legend->AddEntry(h, "Chi2 expo fit");
  legend->Draw();*/
  //c->SaveAs("figures/fit.png");

}
