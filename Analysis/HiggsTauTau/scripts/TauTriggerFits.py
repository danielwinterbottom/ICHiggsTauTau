import ROOT
import FitFunctions as funcs
import argparse

conf_parser = argparse.ArgumentParser(
    description=__doc__,
    formatter_class=argparse.RawDescriptionHelpFormatter,
    add_help=False
    )
conf_parser.add_argument("--cfg",
                    help="Specify config file", metavar="FILE")
options, remaining_argv = conf_parser.parse_known_args()

defaults = { "taudm":"1" }

if options.cfg:
    config = ConfigParser.SafeConfigParser()
    config.read([options.cfg])
    defaults.update(dict(config.items("Defaults")))

parser = argparse.ArgumentParser(
    parents=[conf_parser]
    )
parser.set_defaults(**defaults)

parser.add_argument("--taudm", dest="taudm", type=str,
    help="If specified then does efficincies for set tau decay mode.")

options = parser.parse_args(remaining_argv)   

c = ROOT.TCanvas("c","",600,500)
f = ROOT.TFile("tagandprobe/TauTau_dm%s/tau_SFs.root" % options.taudm)
g = f.Get("data_trg_proj")
g.SetTitle("decay-mode = %s" % options.taudm)
#g.SetTitleSize(0.04)
g.GetYaxis().SetTitle("Efficiency")
g.GetXaxis().SetTitle("p_{T} (GeV)")
c.SetTicks()
g.Draw("ap")


par = [36.6,4.7,5.63,1.64,1.]
# par[m0,sigma,alpha,n,norm]
f = ROOT.TF1("f",funcs.crystalballEfficiency,20.,160.,5)
f.SetParameter(0,par[0])
f.SetParameter(1,par[1])
f.SetParameter(2,par[2])
f.SetParameter(3,par[3])
f.SetParameter(4,par[4])
f.SetParLimits(4,par[4]*0.9,par[4]*1.1)

g.Fit("f","r")
f.Draw("same")

c.Print("tau_trigger_fit_dm%s.pdf" % options.taudm)

