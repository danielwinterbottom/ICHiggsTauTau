import os
import ROOT
import copy
import UserCode.ICHiggsTauTau.plotting as plotting
import argparse

parser = argparse.ArgumentParser()
parser.add_argument('--outputfolder',help= 'Options folder to store plots/datacards', default='output')
args = parser.parse_args()


year_cfg = {
            "2016_preVFP" :("2016","scripts/plot_UL_2016_preVFP.cfg"),
            "2016_postVFP":("2016","scripts/plot_UL_2016_postVFP.cfg"),
            "2017"        :("2017","scripts/plot_UL_2017.cfg"),
            "2018"        :("2018","scripts/plot_UL_2018.cfg"),
            }
methods = [8,17]
channels = ["et","mt","tt"]
os.mkdir(args.outputfolder)
for year, config in year_cfg.items():
   year2 = config[0]
   cfg = config[1]
   os.mkdir(args.outputfolder + '/%(year)s'%vars())
   for method in methods:
       os.mkdir(args.outputfolder + '/%(year)s/method%(method)s/'%vars())
       for channel in channels:
          os.mkdir(args.outputfolder + '/%(year)s/method%(method)s/%(channel)s'%vars())
          out_folder = args.outputfolder + '/%(year)s/method%(method)s/%(channel)s'%vars()
          print("Running %(year)s method%(method)s %(channel)s"%vars())
          #print ("python scripts/HiggsTauTauPlot.py --cfg=%(cfg)s --channel=%(channel)s --method=%(method)s --cat=Nbtag0_NjetsGt1 --outputfolder={} --method=%(method)s --var='pt_1+pt_2+jpt_1+met[150,160,170,180,190,200,225,250,275,300,325,350,400,450,500,600,700,800,900,1100,1300,1500,1700,1900,2100,2300,2500]' --log_x --log_y --y_axis_min=0.0001 --y_axis_max=10000 --custom_y_range --ratio_range=0,3 --bbh_nlo_masses= --ggh_masses= --ggh_masses_powheg= --bbh_masses_powheg= --sel='m_vis>100 && pt_1>50 && pt_2>50'".format(out_folder)%vars())
          os.system("python scripts/HiggsTauTauPlot.py --cfg=%(cfg)s --channel=%(channel)s --method=%(method)s --cat=Nbtag0_NjetsGt1 --outputfolder={} --method=%(method)s --var='pt_1+pt_2+jpt_1+met[150,160,170,180,190,200,225,250,275,300,325,350,400,450,500,600,700,800,900,1100,1300,1500,1700,1900,2100,2300,2500]' --log_x --log_y --y_axis_min=0.0001 --y_axis_max=10000 --custom_y_range --ratio_range=0,3 --bbh_nlo_masses= --ggh_masses= --ggh_masses_powheg= --bbh_masses_powheg= --sel='m_vis>100 && pt_1>50 && pt_2>50'".format(out_folder)%vars())


os.mkdir(args.outputfolder + '/combined/')
for channel in channels:
   os.mkdir(args.outputfolder + '/combined/%(channel)s'%vars())
   for method in methods:
      os.mkdir(args.outputfolder + '/combined/%(channel)s/method%(method)s/'%vars())
      dir_name = "%(channel)s_Nbtag0_NjetsGt1"%vars()#"excess_jobs/combined/%(channel)s/method%(method)s/"%vars()
      rfn = args.outputfolder + "/combined/%(channel)s/method%(method)s/%(channel)s_method%(method)s_all.root"%vars()                  
      if os.path.exists(rfn): os.system("rm {}".format(rfn))
      cf = ROOT.TFile(rfn,"RECREATE")
      cf.mkdir(dir_name)
      hist_dict = {}
      for year, config in year_cfg.items():
         #print year
         year2 = config[0]
         cfg = config[1]
         out_folder = args.outputfolder + '/%(year)s/method%(method)s/%(channel)s'%vars()
         f = ROOT.TFile(out_folder + "/datacard_pt_1+pt_2+jpt_1+met_Nbtag0_NjetsGt1_%(channel)s_%(year2)s.root"%vars())

         for dkey in f.GetListOfKeys():
            d = f.Get(dkey.GetName())
            fnames = []
            for hkey in d.GetListOfKeys():
               if ".subnodes" not in hkey.GetName() and hkey.GetName() not in fnames:
                  fnames.append(hkey.GetName())
                  hist = d.Get(hkey.GetName())

                  if hkey.GetName() not in hist_dict.keys():          
                     hist_dict[hkey.GetName()] = copy.deepcopy(hist.Clone())
                  else:
                     hist_dict[hkey.GetName()].Add(copy.deepcopy(hist.Clone()))
      for key, val in hist_dict.items():
         cd = cf.Get(dir_name)
         cd.cd()
         val.Write()

      cf.Close()
      plot_file = ROOT.TFile(rfn, 'READ')
      nodename='%(channel)s_Nbtag0_NjetsGt1' % vars()
      if method==8:
         FF=False
      if method==17:
         FF=True
      norm_bins = False
      log_y = True
      log_x = False
      ratio_range = '0.,3.5'
      do_custom_uncerts = False
      scheme=channel
      x_title = 'S_{T}^{MET} (GeV)'
      y_title = 'dN/dS_{T}^{MET} (1/GeV)'
      extra_pad = 0
      plot_name = args.outputfolder + '/combined/%(channel)s/method%(method)s/Nbtag0_NjetsGt1_%(channel)s_all' % vars()
      embedding = False
      qcd_ff_closure = False
      w_ff_closure = False 
      plotting.HTTPlot(nodename,plot_file,1.0,"",FF,norm_bins,channel,False,-9999.,-9999.,True,False,False,log_y,log_x,ratio_range,True,2100,0.,False,1e4,-1e4,x_title,y_title,extra_pad,'run2_mssm',do_custom_uncerts,False,False,'Background uncertainty','138 fb^{-1} (13 TeV)',plot_name,'','',scheme,embedding,False,False,'powheg','Nbtag0_NjetsGt1',False,False,False,False,qcd_ff_closure,w_ff_closure,False,[])
