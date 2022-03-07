import numpy as np
import ROOT

cpp_code = """
double make_simulation(int n_run, float h, float lx, float ly, float llx, float lly) {

	TRandom generator;

	TF1 * f_cos = new TF1("f_cos", "( 4/TMath::Pi() )*TMath::Cos(x)*TMath::Cos(x)", 0., TMath::Pi()/2);

	int counter=0;

	for (int i = 0; i < n_run; i++) {

		double theta = f_cos->GetRandom();
		double phi = generator.Uniform(-TMath::Pi(), TMath::Pi());
		double x1 = generator.Uniform(0., lx);
		double y1 = generator.Uniform(0., ly);

		double x3 = x1 + h * (TMath::Sin(theta) / TMath::Cos(theta)) * TMath::Cos(phi);
		double y3 = y1 + h * (TMath::Sin(theta) / TMath::Cos(theta)) * TMath::Sin(phi);

		if ((y3 >= 0. && y3 <= lly) && (x3 >= 0. && x3 <= llx)) counter++;
		}

	double cnt = counter * 1.;

	return cnt / n_run;

}

"""

ROOT.gInterpreter.ProcessLine(cpp_code)

z = [40, 0, 0, 0, 0, 10]
x = 40
y = 48
n_sample = 100000

acceptance = [0, 0, 0, 0, 0, 0]

for i, z in enumerate(z):
    acceptance[i] = ROOT.make_simulation(n_sample, z, x, y, x, y)

acceptance = np.array(acceptance)
print(acceptance)
