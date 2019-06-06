import ROOT
import argparse
import array
import FitFunctions as funcs
import UserCode.ICHiggsTauTau.plotting as plotting

parser = argparse.ArgumentParser()
parser.add_argument('--file', '-f', help= 'Name of inputfile')
args = parser.parse_args()
filename = args.file

cuts='os && gen_match_1==4&&gen_match_2==5&&iso_tight && pass_antilep && id_probe_2 && pt_1>28 && trg_tag_1 && id_tag_1 && iso_1<0.15'

weight = 'wt'
probe = 'trg_probe_2'
channel = 'et'

input_file = ROOT.TFile(filename)
tree = input_file.Get("tagandprobe")


fout = ROOT.TFile('tauTrigSFs/embed_tau_trig_eff_%s.root' % channel,'RECREATE')

xbins=[0,1.5,2.1,2.5]
eta_num = ROOT.TH1F('eta_num','',len(xbins)-1, array.array('d',xbins))
eta_denum = ROOT.TH1F('eta_denum','',len(xbins)-1, array.array('d',xbins))
bins=[20,25,30,35,40,45,50,55,60,65,70,80,90,100,120,140,160,200,500]
if channel in ['et','mt']: bins=[20,22.5,25,27.5,30,35,40,45,50,55,60,65,70,80,90,100,120,140,160,200,500]
num = ROOT.TH1F('num','',len(bins)-1, array.array('d',bins))
denum = ROOT.TH1F('denum','',len(bins)-1, array.array('d',bins))

for iso in ['vloose','loose','medium','tight']: 
#for iso in ['tight']:
   for dm in ['0','1','10']:
     cuts='os && gen_match_1==4&&gen_match_2==5&&iso_%s && pass_antilep && id_probe_2 && pt_1>28 && trg_tag_1 && id_tag_1 && iso_1<0.15 && dm==%s' % (iso,dm)
     plot='pt_2'
     
     tree.Draw("%s>>num" % (plot),'(%s)*(%s)*(%s)' % (weight,cuts,probe),"goff")
     num = tree.GetHistogram()
     tree.Draw("%s>>denum" % (plot),'(%s)*(%s)' % (weight,cuts),"goff")
     denum = tree.GetHistogram()

     print '(%s)*(%s)' % (weight,cuts)

     tree.Draw("fabs(eta_2)>>eta_num" ,'(%s)*(%s)*(%s)*(pt_2>40)' % (weight,cuts,probe),"goff")
     eta_num = tree.GetHistogram()
     tree.Draw("fabs(eta_2)>>eta_denum" ,'(%s)*(%s)*(pt_2>40)' % (weight,cuts),"goff")
     eta_denum = tree.GetHistogram()

     r3 = eta_num.Clone()
     r3.Divide(eta_denum.Clone())

     r4 = eta_num.Clone()
     r4_denum = eta_denum.Clone()
     newbins = [0,2.5]
     nbins = len(newbins)-1
     r4 = r4.Rebin(nbins,'',array.array('d', newbins))
     r4_denum = r4_denum.Rebin(nbins,'',array.array('d', newbins))
     r4.Divide(r4_denum)


     r2 = num.Clone()
     r2.Divide(denum.Clone())

     r = ROOT.TGraphAsymmErrors(num.GetNbinsX())
     r.Divide(num,denum,"n")

     fit = ROOT.TF1('fit', '[5] - ROOT::Math::crystalball_cdf(-x, [0], [1], [2], [3])*([4])',20,200)
     par = [0.1,50.,2.,-25.,1.,1.]
     fit.SetParameter(0,par[0])
     fit.SetParameter(1,par[1])
     fit.SetParameter(2,par[2])
     fit.SetParameter(3,par[3])
     fit.SetParameter(4,par[4])
     fit.SetParameter(5,par[5])

     print 'Fitting for dm = %s' % dm

     r.Fit("fit","r")
     #pars = f.GetParameters()
     #print '\t\t\"alpha\": %.6f,\n\t\t\"m_{0}\": %.6f,\n\t\t\"sigma\": %.6f,\n\t\t\"norm\": %.6f,\n\t\t\"n\": %.6f\n\t},\n' % (pars[2],pars[0],pars[1],pars[4],pars[3])  

     fout.cd()
     fit.Write('eff_gr_%siso_%s_dm%s_pt_fit' % (iso,channel,dm))
     r.Write('eff_gr_%siso_%s_dm%s_pt' % (iso,channel,dm))
     r2.Write('eff_%siso_%s_dm%s_pt' % (iso,channel,dm))
     r4.Write('eff_%siso_%s_dm%s_aveeta' % (iso,channel,dm))
     r3.Write('eff_%siso_%s_dm%s_eta' % (iso,channel,dm))

     plotting.TagAndProbePlot([r],
                ['#mu#rightarrow#tau embedding'],
                "",
                False,
                False,
                False,
                "0.7,1.3",
                True,
                200,
                0,
                False,
                4000,
                0,
                "p_{T}^{#tau} (GeV)",
                "efficiency",
                0,
                "tauTrigSFs/embed_tau_trg_%s_effs_%siso_dm%s" %(channel,iso,dm),
                "",
                None,
                None)



input_file.Close()  
