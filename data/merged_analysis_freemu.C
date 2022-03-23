void merged_analysis() {


  double tmin = 5.136, tmax = 55.0;

  int nbins = 60;

  auto file1 = new TFile("iron/doublestop.root");
  auto file2 = new TFile("aluminum/doublestop.root");

  auto tree1 = file1->Get<TTree>("events");
  auto tree2 = file2->Get<TTree>("events");

  auto h_free = new TH1D("free_mu", ";#Delta t [#mus]; ", nbins, tmin, tmax);


  int channel; Double_t times;
  int random = 0;

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

      Double_t dt = (times_ii - times_i) * 1e6;

      if ((ch_ii > ch_i) && ch_i == 1 && ch_iii <= ch_i && dt >= tmin && dt <= tmax) h_free->Fill(dt);

  }

  tree2->SetBranchAddress("channel", &channel);
  tree2->SetBranchAddress("times", &times);

  for (auto i=0; i<tree2->GetEntries()-1; i++) {

      int ch_i, ch_ii, ch_iii;
      double times_i, times_ii, times_iii;

      tree2->GetEntry(i);
      ch_i = channel;
      times_i = times;

      tree2->GetEntry(i+1);
      ch_ii = channel;
      times_ii = times;

      tree2->GetEntry(i+2);
      ch_iii = channel;
      times_iii = times;

      Double_t dt = (times_ii - times_i) * 1e6;

      if ((ch_ii > ch_i) && ch_i == 1 && ch_iii <= ch_i && dt >= tmin && dt <= tmax) h_free->Fill(dt);

  }


  int entries_free = h_free->GetEntries();


  auto decay = new TF1("decay", "expo", tmin, tmax);
  //auto noise = new TF1("noise", "gaus", 4.1, 5.5);
  auto unif = new TF1("unif", "pol1", tmin, tmax);

  auto tot_right = new TF1("tot_right", "expo + [2]", tmin, tmax);

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

  double n_pos_dx = entries_free*(decay->Integral(tmin, tmax)/tot_right->Integral(tmin, tmax));

  cout << "Decadimenti del muone libero (right) = " << n_pos_dx << endl;



}
