void make_simulation(int n_run) {

	float h_triple = 20.5;// +- 1.3

	double lx = 48.27, ly = 39.92;
	double llx = lx, lly = ly;
	float h_down = 3.2; // + 12;
	float h_up = 6.9; //+ 11.5;
	TRandom3 generator;

	TF1 * f_cos = new TF1("f_cos", "( 4/TMath::Pi() )*TMath::Cos(x)*TMath::Cos(x)", 0., TMath::Pi()/2);

	int counter=0;

	for (int i = 0; i < n_run; i++) {

		double theta = f_cos->GetRandom();
		double phi = generator.Uniform(-TMath::Pi(), TMath::Pi());
		double x1 = generator.Uniform(0., lx);
		double y1 = generator.Uniform(0., ly);

		double depth = generator.Uniform(0., 1.);

		double x2 = x1 + (h_up + depth) * (TMath::Sin(theta) / TMath::Cos(theta)) * TMath::Cos(phi);
		double y2 = y1 + (h_up + depth) * (TMath::Sin(theta) / TMath::Cos(theta)) * TMath::Sin(phi);

		if ((y2 >= 0. && y2 <= lly) && (x2 >= 0. && x2 <= llx)) {

			double theta_e = generator.Uniform(0, TMath::Pi());
			double phi_e = generator.Uniform(-TMath::Pi(), TMath::Pi());

			if (theta_e > TMath::Pi()/2) {
				double x3 = x2 + (h_up + 11.5 + depth) * (TMath::Sin(theta_e) / TMath::Abs(TMath::Cos(theta_e))) * TMath::Cos(phi_e);
				double y3 = y2 + (h_up + 11.5 + depth) * (TMath::Sin(theta_e) / TMath::Abs(TMath::Cos(theta_e))) * TMath::Sin(phi_e);

				if ((y3 >= 0. && y3 <= lly) && (x3 >= 0. && x3 <= llx)) counter++;
			}

			if (theta_e < TMath::Pi()/2) {
				double x3 = x2 + (h_down + 12 + (1.-depth) ) * (TMath::Sin(theta_e) / TMath::Cos(theta_e)) * TMath::Cos(phi_e);
				double y3 = y2 + (h_down + 12 + (1.-depth) ) * (TMath::Sin(theta_e) / TMath::Cos(theta_e)) * TMath::Sin(phi_e);

				if ((y3 >= 0. && y3 <= lly) && (x3 >= 0. && x3 <= llx)) counter ++;
			}
		}
	}

	double cnt = counter * 1.;

	cout << cnt / n_run << endl;
	return;

}
