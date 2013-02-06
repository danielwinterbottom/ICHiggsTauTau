PROD=Moriond

PARAMS=./scripts/"$PROD"_params_2012.dat
TSCALE=(
  '1' 
  '2' 
  '0'
  )

for j in "${TSCALE[@]}"
do
  CATS=('8' '3' '2' '1' '0')
  for i in "${CATS[@]}"
  do
    ./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg  --tau_scale_mode=$j --channel=et --rebin=2  \
      --method="$i" --plot_name="m_sv"  --x_axis_label="m_{#tau#tau} [GeV]" \
      --blind=false --x_blind_min=100 --x_blind_max=160 --make_datacard=true --norm_bins=true --verbose=false \
      --paramfile=$PARAMS

    ./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg  --tau_scale_mode=$j --channel=mt --rebin=2 \
      --method="$i" --plot_name="m_sv"  --x_axis_label="m_{#tau#tau} [GeV]" \
      --blind=false --x_blind_min=100 --x_blind_max=160 --make_datacard=true  --norm_bins=true --verbose=false \
      --paramfile=$PARAMS

    ./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg  --tau_scale_mode=$j --channel=em --rebin=2 \
      --method="$i" --plot_name="m_sv"  --x_axis_label="m_{#tau#tau} [GeV]" \
    --blind=false --x_blind_min=100 --x_blind_max=160 --make_datacard=true  --norm_bins=true --verbose=false \
    --paramfile=$PARAMS
  done

  CATS=('5')
  for i in "${CATS[@]}"
  do
    ./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg  --tau_scale_mode=$j --channel=et --rebin=5  \
      --method="$i" --plot_name="m_sv"  --x_axis_label="m_{#tau#tau} [GeV]" \
      --blind=false --x_blind_min=100 --x_blind_max=160 --make_datacard=true --norm_bins=true --verbose=false \
      --paramfile=$PARAMS

    ./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg  --tau_scale_mode=$j --channel=mt --rebin=5 \
      --method="$i" --plot_name="m_sv"  --x_axis_label="m_{#tau#tau} [GeV]" \
      --blind=false --x_blind_min=100 --x_blind_max=160 --make_datacard=true  --norm_bins=true --verbose=false \
      --paramfile=$PARAMS

    ./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg  --tau_scale_mode=$j --channel=em --rebin=5 \
      --method="$i" --plot_name="m_sv"  --x_axis_label="m_{#tau#tau} [GeV]" \
      --blind=false --x_blind_min=100 --x_blind_max=160 --make_datacard=true  --norm_bins=true --verbose=false \
      --paramfile=$PARAMS --extra_pad=1.4
  done
done

hadd -f htt_et.inputs-sm-8TeV-2GeVbins.root datacard_eleTau_*.root
rm datacard_eleTau_*.root
hadd -f htt_mt.inputs-sm-8TeV-2GeVbins.root datacard_muTau_*.root
rm datacard_muTau_*.root
hadd -f htt_em.inputs-sm-8TeV-2GeVbins.root datacard_emu_*.root
rm datacard_emu_*.root

###### DO SM VISIBLE MASS DATACARDS ########
for j in "${TSCALE[@]}"
do
  CATS=(
   '8'
   '3'
   '2'
   '1'
   '0'
  )
  for i in "${CATS[@]}"
  do
    ./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg  --tau_scale_mode=$j --channel=et --rebin=2  \
      --method="$i" --plot_name="m_vis"  --x_axis_label="Visible Mass [GeV]" \
      --blind=false --x_blind_min=60 --x_blind_max=120 --make_datacard=true --norm_bins=true \
      --paramfile=$PARAMS
    ./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg  --tau_scale_mode=$j --channel=mt --rebin=2 \
      --method="$i" --plot_name="m_vis"  --x_axis_label="Visible Mass [GeV]" \
      --blind=false --x_blind_min=60 --x_blind_max=120 --make_datacard=true  --norm_bins=true \
      --paramfile=$PARAMS
    ./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg  --tau_scale_mode=$j --channel=em --rebin=2 \
      --method="$i" --plot_name="m_vis"  --x_axis_label="Visible Mass [GeV]" \
      --blind=false --x_blind_min=60 --x_blind_max=120 --make_datacard=true  --norm_bins=true \
      --paramfile=$PARAMS
  done
  CATS=(
    '5'
  )
  for i in "${CATS[@]}"
  do
    ./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg  --tau_scale_mode=$j --channel=et --rebin=5  \
      --method="$i" --plot_name="m_vis"  --x_axis_label="Visible Mass [GeV]" \
      --blind=false --x_blind_min=60 --x_blind_max=120 --make_datacard=true --norm_bins=true \
      --paramfile=$PARAMS
    ./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg  --tau_scale_mode=$j --channel=mt --rebin=5 \
      --method="$i" --plot_name="m_vis"  --x_axis_label="Visible Mass [GeV]" \
      --blind=false --x_blind_min=60 --x_blind_max=120 --make_datacard=true  --norm_bins=true \
      --paramfile=$PARAMS
    ./bin/HiggsTauTauPlot3 --cfg=scripts/plot_sm_2012.cfg  --tau_scale_mode=$j --channel=em --rebin=5 \
      --method="$i" --plot_name="m_vis"  --x_axis_label="Visible Mass [GeV]" \
      --blind=false --x_blind_min=60 --x_blind_max=120 --make_datacard=true  --norm_bins=true \
      --paramfile=$PARAMS
  done
done

hadd -f htt_et.inputs-sm-8TeV-mvis-2GeVbins.root datacard_eleTau_*.root
rm datacard_eleTau_*.root
hadd -f htt_mt.inputs-sm-8TeV-mvis-2GeVbins.root datacard_muTau_*.root
rm datacard_muTau_*.root
hadd -f htt_em.inputs-sm-8TeV-mvis-2GeVbins.root datacard_emu_*.root
rm datacard_emu_*.root

# # ##### MSSM #######
# for j in "${TSCALE[@]}"
# do
#   CATS=(
#    '8'
#    '11'
#    '3'
#    '2'
#    '1'
#    '0'
#   )
#   for i in "${CATS[@]}"
#   do
#     ./bin/HiggsTauTauPlot3 --cfg=scripts/plot_mssm_2012.cfg  --tau_scale_mode=$j --channel=et --rebin=2  \
#       --method="$i" --plot_name="svfit_mssm_fine"  --x_axis_label="m_{#tau#tau} [GeV]" \
#       --blind=false --x_blind_min=100 --x_blind_max=1500 --make_datacard=true --norm_bins=true 
#     ./bin/HiggsTauTauPlot3 --cfg=scripts/plot_mssm_2012.cfg  --tau_scale_mode=$j --channel=mt --rebin=2 \
#       --method="$i" --plot_name="svfit_mssm_fine"  --x_axis_label="m_{#tau#tau} [GeV]" \
#       --blind=false --x_blind_min=100 --x_blind_max=1500 --make_datacard=true  --norm_bins=true
#   done
#   CATS=(
#     '7'
#     '6'
#     '12'
#   )
#   for i in "${CATS[@]}"
#   do
#     ./bin/HiggsTauTauPlot3 --cfg=scripts/plot_mssm_2012.cfg  --tau_scale_mode=$j --channel=et --rebin=2  \
#       --method="$i" --plot_name="svfit_mssm"  --x_axis_label="m_{#tau#tau} [GeV]" \
#       --blind=false --x_blind_min=100 --x_blind_max=1500 --make_datacard=true --norm_bins=true
#     ./bin/HiggsTauTauPlot3 --cfg=scripts/plot_mssm_2012.cfg  --tau_scale_mode=$j --channel=mt --rebin=2 \
#       --method="$i" --plot_name="svfit_mssm"  --x_axis_label="m_{#tau#tau} [GeV]" \
#       --blind=false --x_blind_min=100 --x_blind_max=1500 --make_datacard=true  --norm_bins=true
#   done
# done

# hadd -f htt_et.inputs-mssm-8TeV.root datacard_eleTau_*.root
# rm datacard_eleTau_*.root
# hadd -f htt_mt.inputs-mssm-8TeV.root datacard_muTau_*.root
# rm datacard_muTau_*.root



# for j in "${TSCALE[@]}"
# do
#   CATS=(
#    '8'
#    '11'
#    '3'
#    '2'
#    '1'
#    '0'
#   )
#   for i in "${CATS[@]}"
#   do
#     ./bin/HiggsTauTauPlot3 --cfg=scripts/plot_mssm_2012.cfg  --tau_scale_mode=$j --channel=et --rebin=2  \
#       --method="$i" --plot_name="vis_mssm_fine"  --x_axis_label="Visible Mass [GeV]" \
#       --blind=false --x_blind_min=60 --x_blind_max=1500 --make_datacard=true --norm_bins=true
#     ./bin/HiggsTauTauPlot3 --cfg=scripts/plot_mssm_2012.cfg  --tau_scale_mode=$j --channel=mt --rebin=2 \
#       --method="$i" --plot_name="vis_mssm_fine"  --x_axis_label="Visible Mass [GeV]" \
#       --blind=false --x_blind_min=60 --x_blind_max=1500 --make_datacard=true  --norm_bins=true
#   done
#   CATS=(
#     '12'
#     '7'
#     '6'
#   )
#   for i in "${CATS[@]}"
#   do
#     ./bin/HiggsTauTauPlot3 --cfg=scripts/plot_mssm_2012.cfg  --tau_scale_mode=$j --channel=et --rebin=2  \
#       --method="$i" --plot_name="vis_mssm"  --x_axis_label="Visible Mass [GeV]" \
#       --blind=false --x_blind_min=60 --x_blind_max=1500 --make_datacard=true --norm_bins=true
#     ./bin/HiggsTauTauPlot3 --cfg=scripts/plot_mssm_2012.cfg  --tau_scale_mode=$j --channel=mt --rebin=2 \
#       --method="$i" --plot_name="vis_mssm"  --x_axis_label="Visible Mass [GeV]" \
#       --blind=false --x_blind_min=60 --x_blind_max=1500 --make_datacard=true  --norm_bins=true
#   done
# done
# hadd -f htt_et.inputs-mssm-8TeV-mvis.root datacard_eleTau_*.root
# rm datacard_eleTau_*.root
# hadd -f htt_mt.inputs-mssm-8TeV-mvis.root datacard_muTau_*.root
# rm datacard_muTau_*.root


# ## Just for the MSSM plots with SM binning, no datacards
#  TSCALE=(
#  '0'
#  )

# for j in "${TSCALE[@]}"
#  do
#   CATS=(
#    '8'
#    '11'
#    '3'
#    '2'
#    '1'
#    '0'
#   )
#   for i in "${CATS[@]}"
#   do
#     ./bin/HiggsTauTauPlot3 --cfg=scripts/plot_mssm_2012.cfg  --tau_scale_mode=$j --channel=et --rebin=2  \
#       --method="$i" --plot_name="svfit_sm_fine"  --x_axis_label="m_{#tau#tau} [GeV]" --log_y=false --signal_no_stack=false \
#       --blind=false --x_blind_min=100 --x_blind_max=1500 --make_datacard=false --norm_bins=true
#     ./bin/HiggsTauTauPlot3 --cfg=scripts/plot_mssm_2012.cfg  --tau_scale_mode=$j --channel=mt --rebin=2 \
#       --method="$i" --plot_name="svfit_sm_fine"  --x_axis_label="m_{#tau#tau} [GeV]"  --log_y=false --signal_no_stack=false \
#       --blind=false --x_blind_min=100 --x_blind_max=1500 --make_datacard=false  --norm_bins=true
#   done
#   CATS=(
#     '7'
#     '6'
#     '12'
#   )
#   for i in "${CATS[@]}"
#   do
#     ./bin/HiggsTauTauPlot3 --cfg=scripts/plot_mssm_2012.cfg  --tau_scale_mode=$j --channel=et --rebin=2  \
#       --method="$i" --plot_name="svfit_sm"  --x_axis_label="m_{#tau#tau} [GeV]"  --log_y=false --signal_no_stack=false \
#       --blind=false --x_blind_min=100 --x_blind_max=1500 --make_datacard=false --norm_bins=true --extra_pad=1.4
#     ./bin/HiggsTauTauPlot3 --cfg=scripts/plot_mssm_2012.cfg  --tau_scale_mode=$j --channel=mt --rebin=2 \
#       --method="$i" --plot_name="svfit_sm"  --x_axis_label="m_{#tau#tau} [GeV]"  --log_y=false --signal_no_stack=false \
#       --blind=false --x_blind_min=100 --x_blind_max=1500 --make_datacard=false  --norm_bins=true --extra_pad=1.2
#   done
# done


#   CATS=(
#    '8'
#    '11'
#    '3'
#    '2'
#    '1'
#    '0'
#   )
#   for i in "${CATS[@]}"
#   do
#     ./bin/HiggsTauTauPlot3 --cfg=scripts/plot_mssm_2012.cfg  --tau_scale_mode=0 --channel=et --rebin=2  \
#       --method="$i" --plot_name="vis_sm_fine"  --x_axis_label="Visible Mass [GeV]"  --log_y=false --signal_no_stack=false \
#       --blind=false --x_blind_min=60 --x_blind_max=1500 --make_datacard=false --norm_bins=true
#     ./bin/HiggsTauTauPlot3 --cfg=scripts/plot_mssm_2012.cfg  --tau_scale_mode=0 --channel=mt --rebin=2 \
#       --method="$i" --plot_name="vis_sm_fine"  --x_axis_label="Visible Mass [GeV]"  --log_y=false --signal_no_stack=false \
#       --blind=false --x_blind_min=60 --x_blind_max=1500 --make_datacard=false  --norm_bins=true
#   done
#   CATS=(
#     '7'
#     '6'
#     '12'
#   )
#   for i in "${CATS[@]}"
#   do
#     ./bin/HiggsTauTauPlot3 --cfg=scripts/plot_mssm_2012.cfg  --tau_scale_mode=0 --channel=et --rebin=2  \
#       --method="$i" --plot_name="vis_sm"  --x_axis_label="Visible Mass [GeV]"  --log_y=false --signal_no_stack=false \
#       --blind=false --x_blind_min=60 --x_blind_max=1500 --make_datacard=false --norm_bins=true
#     ./bin/HiggsTauTauPlot3 --cfg=scripts/plot_mssm_2012.cfg  --tau_scale_mode=0 --channel=mt --rebin=2 \
#       --method="$i" --plot_name="vis_sm"  --x_axis_label="Visible Mass [GeV]"  --log_y=false --signal_no_stack=false \
#       --blind=false --x_blind_min=60 --x_blind_max=1500 --make_datacard=false  --norm_bins=true
#   done




