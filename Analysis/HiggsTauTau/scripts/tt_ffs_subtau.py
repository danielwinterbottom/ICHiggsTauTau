import ROOT
import UserCode.ICHiggsTauTau.plotting as plotting
import re
import os


output_folder='mvadm_ff_v1_subtau'
config='scripts/plot_cpdecays_2016_newff.cfg'

fout = ROOT.TFile('%(output_folder)s/ff_histograms_subtau.root' % vars(),'RECREATE')

baseline_bothiso = 'mva_olddm_tight_1>0.5 && mva_olddm_tight_2>0.5 && antiele_1 && antimu_1 && antiele_2 && antimu_2 && leptonveto==0 && trg_doubletau'

baseline_antiiso1 = 'mva_olddm_tight_1<0.5 && mva_olddm_vloose_1>0.5 && mva_olddm_tight_2>0.5 && antiele_1 && antimu_1 && antiele_2 && antimu_2 && leptonveto==0 && trg_doubletau'

baseline_antiiso2 = 'mva_olddm_tight_1>0.5 && mva_olddm_tight_2<0.5 && mva_olddm_vloose_2>0.5 && antiele_1 && antimu_1 && antiele_2 && antimu_2 && leptonveto==0 && trg_doubletau'

#mva_olddm_tight_1<0.5 && mva_olddm_vloose_1>0.5 && mva_olddm_tight_2>0.5 && antiele_1 && antimu_1 && antiele_2 && antimu_2 && leptonveto==0 && trg_doubletau

njets_bins = {
              'njets0':'n_jets==0', 
              'njets1':'n_jets>0'
}
mva_dm_bins = {
              'mvadm0':'(mva_dm_X==0)',
              'mvadm1':'(mva_dm_X==1)',
              'mvadm2':'(mva_dm_X==2)',
              'mvadm10':'(mva_dm_X==10)'
}

c1 = ROOT.TCanvas()


for njetbin in njets_bins:
  njetbincut = njets_bins[njetbin]
  for mvadmbin in mva_dm_bins:
    mvadmbincut = mva_dm_bins[mvadmbin]
    name= mvadmbin+'_'+njetbin
    cuts = '%s && %s' % (mvadmbincut, njetbincut)
    var='pt_X[40,45,50,55,60,65,70,80,90,100,120,140]'
    cmd_iso_1=re.sub('X', '2', 'python scripts/HiggsTauTauPlot.py --channel=tt --method=0 --cfg=%(config)s  --var="%(var)s"  --ratio  --do_ss --outputfolder=%(output_folder)s --cat=inclusive --datacard="%(name)s_iso" --set_alias="inclusive:(%(cuts)s)" --set_alias="baseline:(%(baseline_bothiso)s)"' % vars())

    cmd_aiso1_1=re.sub('X', '2', 'python scripts/HiggsTauTauPlot.py --channel=tt --method=0 --cfg=%(config)s  --var="%(var)s"  --ratio  --do_ss --outputfolder=%(output_folder)s --cat=inclusive --datacard="%(name)s_aiso1" --set_alias="inclusive:(%(cuts)s)" --set_alias="baseline:(%(baseline_antiiso2)s)"' % vars())

    print '\n'
    print name
    print '\n'
    print cmd_iso_1
    print '\n'
    print cmd_aiso1_1
    print '\n'

    #var2 = re.sub('X','2',var)


    #cmd_aiso1_2=re.sub('X', '1', 'python scripts/HiggsTauTauPlot.py --channel=tt --method=0 --cfg=%(config)s  --var="%(var2)s"  --ratio  --do_ss --outputfolder=%(output_folder)s --cat=inclusive --datacard="%(name)s_aiso1" --set_alias="inclusive:(%(cuts)s)" --set_alias="baseline:(%(baseline_antiiso1)s)"' % vars())
 
    #cmd_iso_1=re.sub('X', '1', 'python scripts/HiggsTauTauPlot.py --channel=tt --method=0 --cfg=%(config)s  --var="%(var2)s"  --ratio  --do_ss --outputfolder=%(output_folder)s --cat=inclusive --datacard="%(name)s_iso" --set_alias="inclusive:(%(cuts)s)" --set_alias="baseline:(%(baseline_bothiso)s)"' % vars())

    os.system(cmd_iso_1)
    os.system(cmd_aiso1_1)

    # calculate fake factors

    name= mvadmbin+'_'+njetbin
    fiso = ROOT.TFile('%(output_folder)s/datacard_pt_2_%(name)s_iso_tt_2016.root' % vars())
    faiso1 = ROOT.TFile('%(output_folder)s/datacard_pt_2_%(name)s_aiso1_tt_2016.root' % vars())

    dataiso = fiso.Get('tt_%(name)s_iso/data_obs' % vars())
    dataiso.Add(fiso.Get('tt_%(name)s_iso/ZTT' % vars()), -1)
    dataiso.Add(fiso.Get('tt_%(name)s_iso/ZL' % vars()), -1)
    dataiso.Add(fiso.Get('tt_%(name)s_iso/TTT' % vars()), -1)
    dataiso.Add(fiso.Get('tt_%(name)s_iso/TTJ' % vars()), -1)
    dataiso.Add(fiso.Get('tt_%(name)s_iso/VVT' % vars()), -1)
    dataiso.Add(fiso.Get('tt_%(name)s_iso/VVJ' % vars()), -1)
    dataiso.Add(fiso.Get('tt_%(name)s_iso/TTT' % vars()), -1)
    dataiso.Add(fiso.Get('tt_%(name)s_iso/W' % vars()), -1)

    dataaiso1 = faiso1.Get('tt_%(name)s_aiso1/data_obs' % vars())
    dataaiso1.Add(faiso1.Get('tt_%(name)s_aiso1/ZTT' % vars()),-1)
    dataaiso1.Add(faiso1.Get('tt_%(name)s_aiso1/ZL' % vars()), -1)
    dataaiso1.Add(faiso1.Get('tt_%(name)s_aiso1/TTT' % vars()), -1)
    dataaiso1.Add(faiso1.Get('tt_%(name)s_aiso1/TTJ' % vars()), -1)
    dataaiso1.Add(faiso1.Get('tt_%(name)s_aiso1/VVT' % vars()), -1)
    dataaiso1.Add(faiso1.Get('tt_%(name)s_aiso1/VVJ' % vars()), -1)
    dataaiso1.Add(faiso1.Get('tt_%(name)s_aiso1/TTT' % vars()), -1)
    dataaiso1.Add(faiso1.Get('tt_%(name)s_aiso1/W' % vars()), -1)

    dataiso.Divide(dataaiso1)
    
    #do fits
    f1 = ROOT.TF1("f1","landau")
    if 'mvadm2' in name: f2 = ROOT.TF1("f2","[0]*TMath::Landau(x,[1],[2])+[3]+[4]*x",0,200)
    else: f2 = ROOT.TF1("f2","[0]*TMath::Landau(x,[1],[2])+[3]",0,200)
    # first fit to set default parameters for landau
    dataiso.Fit("f1")
    f2.SetParameter(0,f1.GetParameter(0)); f2.SetParameter(1,f1.GetParameter(1)); f2.SetParameter(2,f1.GetParameter(2)); f2.SetParameter(3,0)
    dataiso.Fit("f2")
    dataiso.SetMaximum(min(dataiso.GetMaximum()*1.2,1))
    dataiso.SetStats(0)
    dataiso.Draw()
    c1.Print('%(output_folder)s/%(name)s_subtau.pdf' % vars())
    fout.cd()
    dataiso.Clone().Write(name+'_subtau')
    f2.Clone().Write(name+'_fit_subtau')

# pT of other tau closure correction

ff_eqn = 'p0*TMath::Landau(pt_2,p1,p2)+p3'
ff_eqn_2 = 'p0*TMath::Landau(pt_2,p1,p2)+p3+p4*pt_2'
ff_params = {}
for njetbin in [0,1]:
  for mvadmbin in [0,1,2,10]:
    fout.cd()
    f = fout.Get('mvadm%(mvadmbin)i_njets%(njetbin)i_fit_subtau' % vars())
    p = f.GetParameters()
    ff_params['mvadm%(mvadmbin)i_njets%(njetbin)i' % vars()] = p

ff_func_mvadm0_njets0 = ff_eqn.replace('p0','%f'%ff_params['mvadm0_njets0'][0]).replace('p1','%f'%ff_params['mvadm0_njets0'][1]).replace('p2','%f'%ff_params['mvadm0_njets0'][2]).replace('p3','%f'%ff_params['mvadm0_njets0'][3])

ff_func_mvadm1_njets0 = ff_eqn.replace('p0','%f'%ff_params['mvadm1_njets0'][0]).replace('p1','%f'%ff_params['mvadm1_njets0'][1]).replace('p2','%f'%ff_params['mvadm1_njets0'][2]).replace('p3','%f'%ff_params['mvadm1_njets0'][3])

ff_func_mvadm2_njets0 = ff_eqn_2.replace('p0','%f'%ff_params['mvadm2_njets0'][0]).replace('p1','%f'%ff_params['mvadm2_njets0'][1]).replace('p2','%f'%ff_params['mvadm2_njets0'][2]).replace('p3','%f'%ff_params['mvadm2_njets0'][3]).replace('p4','%f'%ff_params['mvadm2_njets0'][4])

ff_func_mvadm10_njets0 = ff_eqn.replace('p0','%f'%ff_params['mvadm10_njets0'][0]).replace('p1','%f'%ff_params['mvadm10_njets0'][1]).replace('p2','%f'%ff_params['mvadm10_njets0'][2]).replace('p3','%f'%ff_params['mvadm10_njets0'][3])

ff_func_mvadm0_njets1 = ff_eqn.replace('p0','%f'%ff_params['mvadm0_njets1'][0]).replace('p1','%f'%ff_params['mvadm0_njets1'][1]).replace('p2','%f'%ff_params['mvadm0_njets1'][2]).replace('p3','%f'%ff_params['mvadm0_njets1'][3])

ff_func_mvadm1_njets1 = ff_eqn.replace('p0','%f'%ff_params['mvadm1_njets1'][0]).replace('p1','%f'%ff_params['mvadm1_njets1'][1]).replace('p2','%f'%ff_params['mvadm1_njets1'][2]).replace('p3','%f'%ff_params['mvadm1_njets1'][3])

ff_func_mvadm2_njets1 = ff_eqn_2.replace('p0','%f'%ff_params['mvadm2_njets1'][0]).replace('p1','%f'%ff_params['mvadm2_njets1'][1]).replace('p2','%f'%ff_params['mvadm2_njets1'][2]).replace('p3','%f'%ff_params['mvadm2_njets1'][3]).replace('p4','%f'%ff_params['mvadm2_njets1'][4])

ff_func_mvadm10_njets1 = ff_eqn.replace('p0','%f'%ff_params['mvadm10_njets1'][0]).replace('p1','%f'%ff_params['mvadm10_njets1'][1]).replace('p2','%f'%ff_params['mvadm10_njets1'][2]).replace('p3','%f'%ff_params['mvadm10_njets1'][3])

ff_eqn_tot = '(n_jets==0)*((mva_dm_2==0)*(%(ff_func_mvadm0_njets0)s)+(mva_dm_2==1)*(%(ff_func_mvadm1_njets0)s)+(mva_dm_2==2)*(%(ff_func_mvadm2_njets0)s)+(mva_dm_2==10)*(%(ff_func_mvadm10_njets0)s)) + (n_jets>0)*((mva_dm_2==0)*(%(ff_func_mvadm0_njets1)s)+(mva_dm_2==1)*(%(ff_func_mvadm1_njets1)s)+(mva_dm_2==2)*(%(ff_func_mvadm2_njets1)s)+(mva_dm_2==10)*(%(ff_func_mvadm10_njets1)s))' % vars()

print ff_eqn_tot

var='pt_1[40,45,50,55,60,70,80,100,120]'

cmd_iso='python scripts/HiggsTauTauPlot.py --channel=tt --method=0 --cfg=%(config)s   --ratio --cat=inclusive --do_ss --set_alias="inclusive:((mva_dm_1==0 || mva_dm_1==1||mva_dm_1==2||mva_dm_1==10)&&(mva_dm_2==0 || mva_dm_2==1||mva_dm_2==2||mva_dm_2==10))" --var="%(var)s"  --outputfolder=%(output_folder)s --datacard=closure_iso ' % vars()

cmd_aiso='python scripts/HiggsTauTauPlot.py --channel=tt --method=0 --cfg=%(config)s   --ratio --cat=inclusive --do_ss --set_alias="inclusive:((mva_dm_1==0 || mva_dm_1==1||mva_dm_1==2||mva_dm_1==10)&&(mva_dm_2==0 || mva_dm_2==1||mva_dm_2==2||mva_dm_2==10))" --var="%(var)s"  --outputfolder=%(output_folder)s --set_alias="baseline:(%(baseline_antiiso2)s)" --add_wt="(%(ff_eqn_tot)s)" --datacard=closure_aiso' % vars()

print '\n'
print cmd_iso
print '\n'
print cmd_aiso
print '\n'

os.system(cmd_iso)
os.system(cmd_aiso)


fiso = ROOT.TFile('%(output_folder)s/datacard_pt_1_closure_iso_tt_2016.root' % vars())
faiso = ROOT.TFile('%(output_folder)s/datacard_pt_1_closure_aiso_tt_2016.root' % vars())

hiso = fiso.Get('tt_closure_iso/QCD')
haiso = faiso.Get('tt_closure_aiso/QCD')

fout.cd()

hcorr = hiso.Clone()
hcorr.Divide(haiso)
hcorr.Write('closure_hist_subtau')
f = ROOT.TF1('f',"pol1")
hcorr.Fit("f")
f.Write('closure_fit_subtau')

var='n_jets(4,0,4)'

cmd_iso='python scripts/HiggsTauTauPlot.py --channel=tt --method=0 --cfg=%(config)s   --ratio --cat=inclusive --do_ss --set_alias="inclusive:((mva_dm_1==0 || mva_dm_1==1||mva_dm_1==2||mva_dm_1==10)&&(mva_dm_2==0 || mva_dm_2==1||mva_dm_2==2||mva_dm_2==10))" --var="%(var)s"  --outputfolder=%(output_folder)s --datacard=closure_iso ' % vars()

cmd_aiso='python scripts/HiggsTauTauPlot.py --channel=tt --method=0 --cfg=%(config)s   --ratio --cat=inclusive --do_ss --set_alias="inclusive:((mva_dm_1==0 || mva_dm_1==1||mva_dm_1==2||mva_dm_1==10)&&(mva_dm_2==0 || mva_dm_2==1||mva_dm_2==2||mva_dm_2==10))" --var="%(var)s"  --outputfolder=%(output_folder)s --set_alias="baseline:(%(baseline_antiiso2)s)" --add_wt="(%(ff_eqn_tot)s)" --datacard=closure_aiso' % vars()

print '\n'
print cmd_iso
print '\n'
print cmd_aiso
print '\n'

os.system(cmd_iso)
os.system(cmd_aiso)


fiso = ROOT.TFile('%(output_folder)s/datacard_n_jets_closure_iso_tt_2016.root' % vars())
faiso = ROOT.TFile('%(output_folder)s/datacard_n_jets_closure_aiso_tt_2016.root' % vars())

hiso = fiso.Get('tt_closure_iso/QCD')
haiso = faiso.Get('tt_closure_aiso/QCD')

fout.cd()

hcorr = hiso.Clone()
hcorr.Divide(haiso)
hcorr.Write('njets_closure_hist_subtau')

# os/ss correction

baseline_bothiso = 'mva_olddm_tight_2>0.5 && mva_olddm_tight_1<0.5 && mva_olddm_vloose_1>0.5 && antiele_1 && antimu_1 && antiele_2 && antimu_2 && leptonveto==0 && trg_doubletau'

baseline_antiiso1 = 'mva_olddm_tight_2<0.5 && mva_olddm_vloose_2>0.5 && mva_olddm_tight_1<0.5 && mva_olddm_vloose_1>0.5 && antiele_1 && antimu_1 && antiele_2 && antimu_2 && leptonveto==0 && trg_doubletau'


njets_bins = {
              'njets0':'n_jets==0', 
              'njets1':'n_jets>0'
}
mva_dm_bins = {
              'mvadm0':'(mva_dm_X==0)',
              'mvadm1':'(mva_dm_X==1)',
              'mvadm2':'(mva_dm_X==2)',
              'mvadm10':'(mva_dm_X==10)'
}

c1 = ROOT.TCanvas()


for njetbin in njets_bins:
  njetbincut = njets_bins[njetbin]
  for mvadmbin in mva_dm_bins:
    mvadmbincut = mva_dm_bins[mvadmbin]
    name= mvadmbin+'_'+njetbin
    cuts = '%s && %s' % (mvadmbincut, njetbincut)
    var='pt_X[40,45,50,55,60,65,70,80,90,100,120,140]'
    cmd_iso_1=re.sub('X', '2', 'python scripts/HiggsTauTauPlot.py --channel=tt --method=0 --cfg=%(config)s  --var="%(var)s"  --ratio  --do_ss --outputfolder=%(output_folder)s --cat=inclusive --datacard="%(name)s_iso_aiso2_ss" --set_alias="inclusive:(%(cuts)s)" --set_alias="baseline:(%(baseline_bothiso)s)"' % vars())

    cmd_aiso1_1=re.sub('X', '2', 'python scripts/HiggsTauTauPlot.py --channel=tt --method=0 --cfg=%(config)s  --var="%(var)s"  --ratio  --do_ss --outputfolder=%(output_folder)s --cat=inclusive --datacard="%(name)s_aiso1_aiso2_ss" --set_alias="inclusive:(%(cuts)s)" --set_alias="baseline:(%(baseline_antiiso1)s)"' % vars())

    cmd_iso_1_os=re.sub('X', '2', 'python scripts/HiggsTauTauPlot.py --channel=tt --method=0 --cfg=%(config)s  --var="%(var)s"  --ratio  --outputfolder=%(output_folder)s --cat=inclusive --datacard="%(name)s_iso_aiso2_os" --set_alias="inclusive:(%(cuts)s)" --set_alias="baseline:(%(baseline_bothiso)s)"' % vars())

    cmd_aiso1_1_os=re.sub('X', '2', 'python scripts/HiggsTauTauPlot.py --channel=tt --method=0 --cfg=%(config)s  --var="%(var)s"  --ratio  --outputfolder=%(output_folder)s --cat=inclusive --datacard="%(name)s_aiso1_aiso2_os" --set_alias="inclusive:(%(cuts)s)" --set_alias="baseline:(%(baseline_antiiso1)s)"' % vars())


    os.system(cmd_iso_1)
    os.system(cmd_aiso1_1)

    os.system(cmd_iso_1_os)
    os.system(cmd_aiso1_1_os)

    # calculate fake factors

    name= mvadmbin+'_'+njetbin
    fiso = ROOT.TFile('%(output_folder)s/datacard_pt_2_%(name)s_iso_aiso2_ss_tt_2016.root' % vars())
    faiso1 = ROOT.TFile('%(output_folder)s/datacard_pt_2_%(name)s_aiso1_aiso2_ss_tt_2016.root' % vars())

    dataiso = fiso.Get('tt_%(name)s_iso_aiso2_ss/data_obs' % vars())
    dataiso.Add(fiso.Get('tt_%(name)s_iso_aiso2_ss/ZTT' % vars()), -1)
    dataiso.Add(fiso.Get('tt_%(name)s_iso_aiso2_ss/ZL' % vars()), -1)
    dataiso.Add(fiso.Get('tt_%(name)s_iso_aiso2_ss/TTT' % vars()), -1)
    dataiso.Add(fiso.Get('tt_%(name)s_iso_aiso2_ss/TTJ' % vars()), -1)
    dataiso.Add(fiso.Get('tt_%(name)s_iso_aiso2_ss/VVT' % vars()), -1)
    dataiso.Add(fiso.Get('tt_%(name)s_iso_aiso2_ss/VVJ' % vars()), -1)
    dataiso.Add(fiso.Get('tt_%(name)s_iso_aiso2_ss/TTT' % vars()), -1)
    dataiso.Add(fiso.Get('tt_%(name)s_iso_aiso2_ss/W' % vars()), -1)

    dataaiso1 = faiso1.Get('tt_%(name)s_aiso1_aiso2_ss/data_obs' % vars())
    dataaiso1.Add(faiso1.Get('tt_%(name)s_aiso1_aiso2_ss/ZTT' % vars()),-1)
    dataaiso1.Add(faiso1.Get('tt_%(name)s_aiso1_aiso2_ss/ZL' % vars()), -1)
    dataaiso1.Add(faiso1.Get('tt_%(name)s_aiso1_aiso2_ss/TTT' % vars()), -1)
    dataaiso1.Add(faiso1.Get('tt_%(name)s_aiso1_aiso2_ss/TTJ' % vars()), -1)
    dataaiso1.Add(faiso1.Get('tt_%(name)s_aiso1_aiso2_ss/VVT' % vars()), -1)
    dataaiso1.Add(faiso1.Get('tt_%(name)s_aiso1_aiso2_ss/VVJ' % vars()), -1)
    dataaiso1.Add(faiso1.Get('tt_%(name)s_aiso1_aiso2_ss/TTT' % vars()), -1)
    dataaiso1.Add(faiso1.Get('tt_%(name)s_aiso1_aiso2_ss/W' % vars()), -1)

    dataiso.Divide(dataaiso1)
    
    #do fits
    f1 = ROOT.TF1("f1","landau")
    if 'mvadm2' in name: f2 = ROOT.TF1("f2","[0]*TMath::Landau(x,[1],[2])+[3]+[4]*x")
    else: f2 = ROOT.TF1("f2","[0]*TMath::Landau(x,[1],[2])+[3]")
    # first fit to set default parameters for landau
    dataiso.Fit("f1")
    f2.SetParameter(0,f1.GetParameter(0)); f2.SetParameter(1,f1.GetParameter(1)); f2.SetParameter(2,f1.GetParameter(2)); f2.SetParameter(3,0)
    dataiso.Fit("f2")
    dataiso.Draw()
    c1.Print('%(output_folder)s/%(name)s_aiso2_ss_subtau.pdf' % vars())
    fout.cd()
    dataiso.Clone().Write(name+'_aiso2_ss_subtau')
    f2.Clone().Write(name+'_aiso2_ss_fit_subtau')

    fiso = ROOT.TFile('%(output_folder)s/datacard_pt_2_%(name)s_iso_aiso2_os_tt_2016.root' % vars())
    faiso1 = ROOT.TFile('%(output_folder)s/datacard_pt_2_%(name)s_aiso1_aiso2_os_tt_2016.root' % vars())

    dataiso = fiso.Get('tt_%(name)s_iso_aiso2_os/data_obs' % vars())
    dataiso.Add(fiso.Get('tt_%(name)s_iso_aiso2_os/ZTT' % vars()), -1)
    dataiso.Add(fiso.Get('tt_%(name)s_iso_aiso2_os/ZL' % vars()), -1)
    dataiso.Add(fiso.Get('tt_%(name)s_iso_aiso2_os/TTT' % vars()), -1)
    dataiso.Add(fiso.Get('tt_%(name)s_iso_aiso2_os/TTJ' % vars()), -1)
    dataiso.Add(fiso.Get('tt_%(name)s_iso_aiso2_os/VVT' % vars()), -1)
    dataiso.Add(fiso.Get('tt_%(name)s_iso_aiso2_os/VVJ' % vars()), -1)
    dataiso.Add(fiso.Get('tt_%(name)s_iso_aiso2_os/TTT' % vars()), -1)
    dataiso.Add(fiso.Get('tt_%(name)s_iso_aiso2_os/W' % vars()), -1)

    dataaiso1 = faiso1.Get('tt_%(name)s_aiso1_aiso2_os/data_obs' % vars())
    dataaiso1.Add(faiso1.Get('tt_%(name)s_aiso1_aiso2_os/ZTT' % vars()),-1)
    dataaiso1.Add(faiso1.Get('tt_%(name)s_aiso1_aiso2_os/ZL' % vars()), -1)
    dataaiso1.Add(faiso1.Get('tt_%(name)s_aiso1_aiso2_os/TTT' % vars()), -1)
    dataaiso1.Add(faiso1.Get('tt_%(name)s_aiso1_aiso2_os/TTJ' % vars()), -1)
    dataaiso1.Add(faiso1.Get('tt_%(name)s_aiso1_aiso2_os/VVT' % vars()), -1)
    dataaiso1.Add(faiso1.Get('tt_%(name)s_aiso1_aiso2_os/VVJ' % vars()), -1)
    dataaiso1.Add(faiso1.Get('tt_%(name)s_aiso1_aiso2_os/TTT' % vars()), -1)
    dataaiso1.Add(faiso1.Get('tt_%(name)s_aiso1_aiso2_os/W' % vars()), -1)

    dataiso.Divide(dataaiso1)

    #do fits
    f1 = ROOT.TF1("f1","landau")
    if 'mvadm2' in name: f2 = ROOT.TF1("f2","[0]*TMath::Landau(x,[1],[2])+[3]+[4]*x")
    else: f2 = ROOT.TF1("f2","[0]*TMath::Landau(x,[1],[2])+[3]")
    # first fit to set default parameters for landau
    dataiso.Fit("f1")
    f2.SetParameter(0,f1.GetParameter(0)); f2.SetParameter(1,f1.GetParameter(1)); f2.SetParameter(2,f1.GetParameter(2)); f2.SetParameter(3,0)
    dataiso.Fit("f2")
    dataiso.Draw()
    c1.Print('%(output_folder)s/%(name)s_aiso2_os_subtau.pdf' % vars())
    fout.cd()
    dataiso.Clone().Write(name+'_aiso2_os_subtau')
    f2.Clone().Write(name+'_aiso2_os_fit_subtau')

fout.Close()
