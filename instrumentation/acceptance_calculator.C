



void make_simulation(int n_run, float h, float lx, float ly, float llx, float lly) {

	TRandom generator;


	TH2F* histo = new TH2F("histo", "histo", 50, 0., lx, 50, 0., ly);

	TH1F* h1 = new TH1F("h1", "h1", 50, 0., TMath::Pi() / 2);

	TF1 * f_cos = new TF1("f_cos", "( 4/TMath::Pi() )*TMath::Cos(x)*TMath::Cos(x)", 0., TMath::Pi()/2);

	int counter=0;

	for (int i = 0; i < n_run; i++) {

		double theta = f_cos->GetRandom();
		//double cos_theta = 3*pow(generator.Uniform(0., 1.), 1. / 3.);
		double phi = generator.Uniform(-TMath::Pi(), TMath::Pi());
		double x1 = generator.Uniform(0., lx);
		double y1 = generator.Uniform(0., ly);

		double x3 = x1 + h * (TMath::Sin(theta) / TMath::Cos(theta)) * TMath::Cos(phi);
		double y3 = y1 + h * (TMath::Sin(theta) / TMath::Cos(theta)) * TMath::Sin(phi);

		//double x3 = x1 + h * (TMath::Sqrt(1 - (cos_theta * cos_theta)) / cos_theta) * TMath::Cos(phi);
		//double y3 = y1 + h * (TMath::Sqrt(1 - (cos_theta * cos_theta)) / cos_theta) * TMath::Sin(phi);

		if ((y3 >= 0. && y3 <= lly) && (x3 >= 0. && x3 <= llx)) counter++;

		//h1->Fill(theta);
		histo->Fill(x3, y3);



	}

	double cnt = counter * 1.;

	//histo->Draw("COLZ");

	cout << "Acc = " << (cnt / n_run) << endl;



}
