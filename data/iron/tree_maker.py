import os
from array import array
import numpy as np
import ROOT


def merge_dat(filepaths, datapath, name='total.dat'):
    finalpath = os.path.join(datapath, name)
    x = np.array([])
    for f in filepaths:
        data = np.genfromtxt(f)
        if x.size:
            x = np.row_stack((x, data))
        else:
            x = data
        del data
    np.savetxt(finalpath, x)
    return finalpath


def make_tree(filepath, datapath, name='total.root'):
    treepath = os.path.join(datapath, name)

    ch, t = np.genfromtxt(filepath, unpack=True, dtype=None)

    f = ROOT.TFile.Open(treepath, "RECREATE")
    tree = ROOT.TTree("events", "events")
    var1 = array('i', [0])
    var2 = array('d', [0])
    tree.Branch("channel", var1, "channel/I")
    tree.Branch("times", var2, "times/D")
    for elm1, elm2 in zip(ch, t):
        var1[0] = int(elm1)
        var2[0] = elm2
        tree.Fill()

    tree.Write()
    f.Close()


datapath = os.path.join(os.path.dirname(__file__), 'iron', 'acquisitions')

filenames = ['0803_1530.dat', '0803_1606.dat',
             '0803_1635.dat', '0903_1125.dat']

filepaths = [os.path.join(datapath, name) for name in filenames]
rootpath = os.path.join(os.path.dirname(__file__), 'iron', 'root_data')

make_tree(merge_dat(filepaths, datapath), rootpath)
make_tree(os.path.join(datapath, '1003_1807_bkg.dat'),
          rootpath, name='bkg.root')
