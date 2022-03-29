import os
import numpy as np

datapath = os.path.join(os.path.dirname(__file__), 'counts')
eff_path = os.path.join(datapath, '..', 'efficiencies')

_, eff_old, err_eff_old, _ = np.loadtxt(
    os.path.join(eff_path, 'working_efficiencies.txt'), unpack=True)
_, new_triple, new_double = np.loadtxt(
    os.path.join(datapath, 'new_coinc.txt'), unpack=True)

new_eff = new_triple / new_double
err_eff_new = np.sqrt(new_triple * (new_double - new_triple) / new_double**3)

X = np.stack([new_eff, err_eff_new], axis=1)

np.savetxt(os.path.join(eff_path, 'new_efficiencies.txt'), X, fmt='%.3f')


old_up = eff_old[3] * eff_old[4]
err_old_up = np.sqrt((err_eff_old[3]/eff_old[3])
                     ** 2 + (err_eff_old[4]/eff_old[4])**2) * old_up

old_down = eff_old[2] * eff_old[1]
err_old_down = np.sqrt((err_eff_old[2]/eff_old[2])**2
                       + (err_eff_old[1]/eff_old[1])**2) * old_down

new_up = new_eff[3] * new_eff[4]
err_new_up = np.sqrt((err_eff_new[3]/new_eff[3])
                     ** 2 + (err_eff_new[4]/new_eff[4])**2) * new_up

new_down = new_eff[2] * new_eff[1]
err_new_down = np.sqrt((err_eff_new[2]/new_eff[2])**2
                       + (err_eff_new[1]/new_eff[1])**2) * new_down

print(f'old up: {old_up:.3f} +- {err_old_up:.3f}')
print(f'new up: {new_up:.3f} +- {err_new_up:.3f}')
print(f'old down: {old_down:.3f} +- {err_old_down:.3f}')
print(f'new down: {new_down:.3f} +- {err_new_down:.3f}')

mean_up = 0.5 * (old_up + new_up)
err_up = err_old_up + 0.5 * (np.abs(old_up - new_up))

mean_down = 0.5 * (old_down + new_down)
err_down = err_old_down + 0.5 * (np.abs(old_down - new_down))

print(f'up: {mean_up:.3f} +- {err_up:.3f} [{err_up/mean_up:.3f}]')
print(f'down: {mean_down:.3f} +- {err_down:.3f} [{err_down/mean_down:.3f}]')
