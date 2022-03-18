#!/usr/bin/env python
import os

years_dict = {
              '2016':{'--cfg':'scripts/plot_mssm_2016.cfg','--folder':'/vols/cms/gu18/Offline/output/MSSM/mssm_2016_b_eff'},
              '2017':{'--cfg':'scripts/plot_mssm_2017.cfg','--folder':'/vols/cms/gu18/Offline/output/MSSM/mssm_2017_b_eff'},
              '2018':{'--cfg':'scripts/plot_mssm_2018.cfg','--folder':'/vols/cms/gu18/Offline/output/MSSM/mssm_2018_b_eff'}
              }

channels_dict = {'tt':{'--cat':["Nbtag0","NbtagGt1"]},
                 'mt':{'--cat':["Nbtag0","NbtagGt1"]},
                 'et':{'--cat':["Nbtag0","NbtagGt1"]}
                 }

for channel, cat_list in channels_dict.items():
  for year, year_parsers in years_dict.items():
    for cat in cat_list['--cat']:
      file_name = 'b_eff_%(channel)s_%(cat)s_%(year)s_output.log'%vars()
      f = open(file_name, "r")
      nom = 0
      Xup = 0
      Xdown = 0
      Yup = 0
      Ydown = 0
      for x in f:
        if 'VVT_XUp ' in x: Xup += float(x.split('VVT_XUp')[1].split('+')[0].replace(" ","")) 
        elif 'TTT_XUp ' in x: Xup += float(x.split('TTT_XUp')[1].split('+')[0].replace(" ",""))
        elif 'VVT_XDown ' in x: Xdown += float(x.split('VVT_XDown')[1].split('+')[0].replace(" ",""))
        elif 'TTT_XDown ' in x: Xdown += float(x.split('TTT_XDown')[1].split('+')[0].replace(" ",""))
        elif 'VVT_YUp ' in x: Yup += float(x.split('VVT_YUp')[1].split('+')[0].replace(" ",""))
        elif 'TTT_YUp ' in x: Yup += float(x.split('TTT_YUp')[1].split('+')[0].replace(" ",""))
        elif 'VVT_YDown ' in x: Ydown += float(x.split('VVT_YDown')[1].split('+')[0].replace(" ",""))
        elif 'TTT_YDown ' in x: Ydown += float(x.split('TTT_YDown')[1].split('+')[0].replace(" ",""))
        elif 'VVT ' in x: nom += float(x.split('VVT')[1].split('+')[0].replace(" ",""))
        elif 'TTT ' in x: nom += float(x.split('TTT')[1].split('+')[0].replace(" ",""))
      #print year+": "+channel+"_"+cat
      #print "syst_fake_b: ("+str(round(min(Xup/nom,Xdown/nom),4))+','+str(round(max(Xup/nom,Xdown/nom),4))+')'
      down = str(round(Xdown/nom,4))
      up = str(round(Xup/nom,4))
      if cat == 'Nbtag0':
        print '({"%(channel)s"}, {"%(year)s"}, nobtag_catagories, %(down)s, %(up)s)' % vars()
      elif cat == 'NbtagGt1':
        print '({"%(channel)s"}, {"%(year)s"}, btag_catagories, %(down)s, %(up)s)' % vars()

      #print "syst_eff_b: ("+str(round(min(Yup/nom,Ydown/nom),4))+','+str(round(max(Yup/nom,Ydown/nom),4))+')'





        
      
    
