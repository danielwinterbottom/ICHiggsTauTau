import ROOT
import UserCode.ICHiggsTauTau.plotting as plotting

do_custom_uncerts=False
norm_bins=False
FF=True
embedding=True
#ratio_range='0,2'
ratio_range='auto'
log_y=False
log_x=False
extra_pad=0.55
gU='3.9'
x_title=''
y_title=''
plot_name=''
qcd_ff_closure=False
w_ff_closure=False

channels = ['et','mt','tt']
#channels = ['et']
cats = []
variables = ['mjj','jeta_1', 'fabs_dphi', 'mt_tot', 'pt_1+pt_2+met+jpt_1', 'n_jets', 'jpt_1', 'jpt_2', 'met', 'pt_1', 'pt_2', 'ip_sig_1', 'ip_mag_1']
#variables = ['fabs_dphi', 'pt_1+pt_2+met+jpt_1', 'n_jets', 'jpt_1', 'jpt_2', 'met', 'pt_1', 'pt_2', 'ip_sig_1', 'ip_sig_2', 'ip_mag_1', 'ip_mag_2']
#variables = ['pt_1+pt_2+met+jpt_1']

directory='vlq_distributions_v14'
#directory='vlq_jpt_1'

for chan in channels:
    for var in variables:
      if ('ip_mag' in var or 'ip_sig' in var) and chan=="tt": continue
      log_x = False
      log_y = False 
      if var == 'pt_1+pt_2+met+jpt_1' or var == 'mt_tot': 
        log_x = True
        log_y = True

      if chan in ['et', 'mt']:
        cats = [
                "MTLt70",
#                "NbtagGt1",
                "Nbtag0_NjetsGt1",
                "Nbtag0_Njets0",
                "Nbtag0_NjetsGt1_STMETGt800",
                "Nbtag0_NjetsGt1_STMETGt1000",
                "Nbtag0_NjetsGt1_STMET400to800",
                #"NbtagGt1_MTLt70",
                "Nbtag0_NjetsGt1_MTLt70",
                "Nbtag0_Njets0_MTLt70",
                "Nbtag0_NjetsGt1_MTLt70_STMETGt800",
                "Nbtag0_NjetsGt1_MTLt70_STMETGt1000",
                "Nbtag0_NjetsGt1_MTLt70_STMET400to800",
                "NLooseCSVbtag0_NjetsGt1_STPFMETGt800",
                "NLooseCSVbtag0_NjetsGt1_STMETGt800",
               ]
      else:
        cats = [
               "inclusive",
              # "Nbtag0",
              # "NbtagGt1",
               "Nbtag0_NjetsGt1",
               "Nbtag0_Njets0",
               "Nbtag0_NjetsGt1_STMETGt800",
               "Nbtag0_NjetsGt1_STPFMETGt800",
               "Nbtag0_NjetsGt1_STMETGt1000",
               "Nbtag0_NjetsGt1_STMET400to800",
               "NLooseCSVbtag0_NjetsGt1_STPFMETGt800",
               "NLooseCSVbtag0_NjetsGt1_STMETGt800",
               ]

      titles = plotting.SetAxisTitles(var,chan)
      x_title = titles[0]
      y_title = titles[1]

      var_alt=var

      if var == 'fabs_dphi': 
        var= 'fabs'
        var_alt = 'fabsdphi'
        x_title='|#Delta#phi(#tau_{1},#tau_{2})|'
        y_title='dN/d|#Delta#phi(#tau_{1},#tau_{2})|'
        y_title='Events / bin'
      if var == 'pt_1+pt_2+met+jpt_1': 
        x_title='S_{T}^{MET} (GeV)'
        y_title = 'dN/dS_{T}^{MET} (1/GeV)'
        y_title='Events / GeV'

      for cat in cats:

        print chan, var, cat

        if 'STMET' in cat and (var == 'pt_1+pt_2+met+jpt_1' or var == 'mt_tot'): continue

        
        closures = ['']        
        if chan in ['et','mt'] and 'MTLt70' not in cat: closures=['','_qcd_ff_closure','_w_ff_closure']
        elif chan == 'tt': closures += ['_qcd_ff_closure']


        for x in closures:
          qcd_ff_closure=(x=='_qcd_ff_closure')
          w_ff_closure=(x=='_w_ff_closure')
          file_name='%(directory)s/%(chan)s/all/datacard_%(var)s_%(cat)s_%(var_alt)s%(x)s_%(chan)s_all.root' % vars()
          print file_name

          plot_file = ROOT.TFile(file_name, 'READ')
          nodename='%(chan)s_%(cat)s' % vars()
          plot_name = '%(directory)s/%(chan)s/all/%(var)s_%(cat)s_%(chan)s_all' % vars()
          if log_x: plot_name += "_logx"
          if log_y: plot_name += "_logy"
          plot_name+=x

          if not plot_file.GetDirectory(nodename): continue 

          scheme=chan
          plotting.HTTPlot(nodename,
            plot_file,
            1.0,
            "",
            FF,
            norm_bins,
            chan,
            False,
            -9999.,
            -9999.,
            True,
            False,
            False,
            log_y,
            log_x,
            ratio_range,
            False,
            0.,
            100.,
            False,
            0.,
            100.,
            x_title,
            y_title,
            extra_pad,
            'run2_mssm',
            do_custom_uncerts,
            False,
            False,
            'Background uncertainty',
            '138 fb^{-1} (13 TeV)',
            plot_name,
            '',
            '',
            scheme,
            embedding,
            False,
            False,
            'powheg',
            '',
            False,
            False,
            False, #discrete_x_axis
            False, #discrete_x_labels
            qcd_ff_closure,
            w_ff_closure,
            False,
            [],
            False,
            gU
          )
