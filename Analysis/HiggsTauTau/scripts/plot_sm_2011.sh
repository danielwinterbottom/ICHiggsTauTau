make -j4


 TSCALE=(
  '1'
  '2'
  '0'
 )


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
    ./bin/HiggsTauTauPlot2 --cfg=scripts/plot_sm_2011.cfg  --tau_scale_mode=$j --mode=0 --rebin=1  \
      --category="$i" --plot_name="svfit_sm_fine"  --x_axis_label="m_{#tau#tau} [GeV]" \
      --blind=false --x_blind_min=100 --x_blind_max=160 --make_datacard=true --norm_bins=true --verbose=false
    ./bin/HiggsTauTauPlot2 --cfg=scripts/plot_sm_2011.cfg  --tau_scale_mode=$j --mode=1 --rebin=1 \
      --category="$i" --plot_name="svfit_sm_fine"  --x_axis_label="m_{#tau#tau} [GeV]" \
      --blind=false --x_blind_min=100 --x_blind_max=160 --make_datacard=true  --norm_bins=true --verbose=true
  done

  CATS=(
    '5'
  )
  for i in "${CATS[@]}"
  do
    ./bin/HiggsTauTauPlot2 --cfg=scripts/plot_sm_2011.cfg  --tau_scale_mode=$j --mode=0 --rebin=1  \
      --category="$i" --plot_name="svfit_sm"  --x_axis_label="m_{#tau#tau} [GeV]" \
      --blind=false --x_blind_min=100 --x_blind_max=160 --make_datacard=true --norm_bins=true --verbose=false
    ./bin/HiggsTauTauPlot2 --cfg=scripts/plot_sm_2011.cfg  --tau_scale_mode=$j --mode=1 --rebin=1 \
      --category="$i" --plot_name="svfit_sm"  --x_axis_label="m_{#tau#tau} [GeV]" \
      --blind=false --x_blind_min=100 --x_blind_max=160 --make_datacard=true  --norm_bins=true --verbose=true
  done
done
hadd -f htt_et.inputs-sm-7TeV.root datacard_eleTau_*.root
rm datacard_eleTau_*.root
hadd -f htt_mt.inputs-sm-7TeV.root datacard_muTau_*.root
rm datacard_muTau_*.root

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
    ./bin/HiggsTauTauPlot2 --cfg=scripts/plot_sm_2011.cfg  --tau_scale_mode=$j --mode=0 --rebin=1  \
      --category="$i" --plot_name="vis_sm_fine"  --x_axis_label="Visible Mass [GeV]" \
      --blind=false --x_blind_min=60 --x_blind_max=120 --make_datacard=true --norm_bins=true
    ./bin/HiggsTauTauPlot2 --cfg=scripts/plot_sm_2011.cfg  --tau_scale_mode=$j --mode=1 --rebin=1 \
      --category="$i" --plot_name="vis_sm_fine"  --x_axis_label="Visible Mass [GeV]" \
      --blind=false --x_blind_min=60 --x_blind_max=120 --make_datacard=true  --norm_bins=true
  done
  CATS=(
    '5'
  )
  for i in "${CATS[@]}"
  do
    ./bin/HiggsTauTauPlot2 --cfg=scripts/plot_sm_2011.cfg  --tau_scale_mode=$j --mode=0 --rebin=1  \
      --category="$i" --plot_name="vis_sm"  --x_axis_label="Visible Mass [GeV]" \
      --blind=false --x_blind_min=60 --x_blind_max=120 --make_datacard=true --norm_bins=true
    ./bin/HiggsTauTauPlot2 --cfg=scripts/plot_sm_2011.cfg  --tau_scale_mode=$j --mode=1 --rebin=1 \
      --category="$i" --plot_name="vis_sm"  --x_axis_label="Visible Mass [GeV]" \
      --blind=false --x_blind_min=60 --x_blind_max=120 --make_datacard=true  --norm_bins=true
  done
done

hadd -f htt_et.inputs-sm-7TeV-mvis.root datacard_eleTau_*.root
rm datacard_eleTau_*.root
hadd -f htt_mt.inputs-sm-7TeV-mvis.root datacard_muTau_*.root
rm datacard_muTau_*.root

# ##### MSSM #######
for j in "${TSCALE[@]}"
do
  CATS=(
   '8'
   '11'
   '3'
   '2'
   '1'
   '0'
  )
  for i in "${CATS[@]}"
  do
    ./bin/HiggsTauTauPlot2 --cfg=scripts/plot_mssm_2011.cfg  --tau_scale_mode=$j --mode=0 --rebin=1  \
      --category="$i" --plot_name="svfit_mssm_fine"  --x_axis_label="m_{#tau#tau} [GeV]" \
      --blind=false --x_blind_min=100 --x_blind_max=1500 --make_datacard=true --norm_bins=true
    ./bin/HiggsTauTauPlot2 --cfg=scripts/plot_mssm_2011.cfg  --tau_scale_mode=$j --mode=1 --rebin=1 \
      --category="$i" --plot_name="svfit_mssm_fine"  --x_axis_label="m_{#tau#tau} [GeV]" \
      --blind=false --x_blind_min=100 --x_blind_max=1500 --make_datacard=true  --norm_bins=true
  done
  CATS=(
    '7'
    '6'
    '12'
  )
  for i in "${CATS[@]}"
  do
    ./bin/HiggsTauTauPlot2 --cfg=scripts/plot_mssm_2011.cfg  --tau_scale_mode=$j --mode=0 --rebin=1  \
      --category="$i" --plot_name="svfit_mssm"  --x_axis_label="m_{#tau#tau} [GeV]" \
      --blind=false --x_blind_min=100 --x_blind_max=1500 --make_datacard=true --norm_bins=true
    ./bin/HiggsTauTauPlot2 --cfg=scripts/plot_mssm_2011.cfg  --tau_scale_mode=$j --mode=1 --rebin=1 \
      --category="$i" --plot_name="svfit_mssm"  --x_axis_label="m_{#tau#tau} [GeV]" \
      --blind=false --x_blind_min=100 --x_blind_max=1500 --make_datacard=true  --norm_bins=true
  done
done

hadd -f htt_et.inputs-mssm-7TeV.root datacard_eleTau_*.root
rm datacard_eleTau_*.root
hadd -f htt_mt.inputs-mssm-7TeV.root datacard_muTau_*.root
rm datacard_muTau_*.root



for j in "${TSCALE[@]}"
do
  CATS=(
   '8'
   '11'
   '3'
   '2'
   '1'
   '0'
  )
  for i in "${CATS[@]}"
  do
    ./bin/HiggsTauTauPlot2 --cfg=scripts/plot_mssm_2011.cfg  --tau_scale_mode=$j --mode=0 --rebin=1  \
      --category="$i" --plot_name="vis_mssm_fine"  --x_axis_label="Visible Mass [GeV]" \
      --blind=false --x_blind_min=60 --x_blind_max=1500 --make_datacard=true --norm_bins=true
    ./bin/HiggsTauTauPlot2 --cfg=scripts/plot_mssm_2011.cfg  --tau_scale_mode=$j --mode=1 --rebin=1 \
      --category="$i" --plot_name="vis_mssm_fine"  --x_axis_label="Visible Mass [GeV]" \
      --blind=false --x_blind_min=60 --x_blind_max=1500 --make_datacard=true  --norm_bins=true
  done
  CATS=(
    '12'
    '7'
    '6'
  )
  for i in "${CATS[@]}"
  do
    ./bin/HiggsTauTauPlot2 --cfg=scripts/plot_mssm_2011.cfg  --tau_scale_mode=$j --mode=0 --rebin=1  \
      --category="$i" --plot_name="vis_mssm"  --x_axis_label="Visible Mass [GeV]" \
      --blind=false --x_blind_min=60 --x_blind_max=1500 --make_datacard=true --norm_bins=true
    ./bin/HiggsTauTauPlot2 --cfg=scripts/plot_mssm_2011.cfg  --tau_scale_mode=$j --mode=1 --rebin=1 \
      --category="$i" --plot_name="vis_mssm"  --x_axis_label="Visible Mass [GeV]" \
      --blind=false --x_blind_min=60 --x_blind_max=1500 --make_datacard=true  --norm_bins=true
  done
done
hadd -f htt_et.inputs-mssm-7TeV-mvis.root datacard_eleTau_*.root
rm datacard_eleTau_*.root
hadd -f htt_mt.inputs-mssm-7TeV-mvis.root datacard_muTau_*.root
rm datacard_muTau_*.root


## Just for the MSSM plots with SM binning, no datacards
 TSCALE=(
 '0'
 )

for j in "${TSCALE[@]}"
 do
  CATS=(
   '8'
   '11'
   '3'
   '2'
   '1'
   '0'
  )
  for i in "${CATS[@]}"
  do
    ./bin/HiggsTauTauPlot2 --cfg=scripts/plot_mssm_2011.cfg  --tau_scale_mode=$j --mode=0 --rebin=1  \
      --category="$i" --plot_name="svfit_sm_fine"  --x_axis_label="m_{#tau#tau} [GeV]" --log_y=false --signal_no_stack=false \
      --blind=false --x_blind_min=100 --x_blind_max=1500 --make_datacard=false --norm_bins=true
    ./bin/HiggsTauTauPlot2 --cfg=scripts/plot_mssm_2011.cfg  --tau_scale_mode=$j --mode=1 --rebin=1 \
      --category="$i" --plot_name="svfit_sm_fine"  --x_axis_label="m_{#tau#tau} [GeV]"  --log_y=false --signal_no_stack=false \
      --blind=false --x_blind_min=100 --x_blind_max=1500 --make_datacard=false  --norm_bins=true
  done
  CATS=(
    '7'
    '6'
    '12'
  )
  for i in "${CATS[@]}"
  do
    ./bin/HiggsTauTauPlot2 --cfg=scripts/plot_mssm_2011.cfg  --tau_scale_mode=$j --mode=0 --rebin=1  \
      --category="$i" --plot_name="svfit_sm"  --x_axis_label="m_{#tau#tau} [GeV]"  --log_y=false --signal_no_stack=false \
      --blind=false --x_blind_min=100 --x_blind_max=1500 --make_datacard=false --norm_bins=true --extra_pad=1.4
    ./bin/HiggsTauTauPlot2 --cfg=scripts/plot_mssm_2011.cfg  --tau_scale_mode=$j --mode=1 --rebin=1 \
      --category="$i" --plot_name="svfit_sm"  --x_axis_label="m_{#tau#tau} [GeV]"  --log_y=false --signal_no_stack=false \
      --blind=false --x_blind_min=100 --x_blind_max=1500 --make_datacard=false  --norm_bins=true --extra_pad=1.2
  done
done


  CATS=(
   '8'
   '11'
   '3'
   '2'
   '1'
   '0'
  )
  for i in "${CATS[@]}"
  do
    ./bin/HiggsTauTauPlot2 --cfg=scripts/plot_mssm_2011.cfg  --tau_scale_mode=0 --mode=0 --rebin=1  \
      --category="$i" --plot_name="vis_sm_fine"  --x_axis_label="Visible Mass [GeV]"  --log_y=false --signal_no_stack=false \
      --blind=false --x_blind_min=60 --x_blind_max=1500 --make_datacard=false --norm_bins=true
    ./bin/HiggsTauTauPlot2 --cfg=scripts/plot_mssm_2011.cfg  --tau_scale_mode=0 --mode=1 --rebin=1 \
      --category="$i" --plot_name="vis_sm_fine"  --x_axis_label="Visible Mass [GeV]"  --log_y=false --signal_no_stack=false \
      --blind=false --x_blind_min=60 --x_blind_max=1500 --make_datacard=false  --norm_bins=true
  done
  CATS=(
    '7'
    '6'
    '12'
  )
  for i in "${CATS[@]}"
  do
    ./bin/HiggsTauTauPlot2 --cfg=scripts/plot_mssm_2011.cfg  --tau_scale_mode=0 --mode=0 --rebin=1  \
      --category="$i" --plot_name="vis_sm"  --x_axis_label="Visible Mass [GeV]"  --log_y=false --signal_no_stack=false \
      --blind=false --x_blind_min=60 --x_blind_max=1500 --make_datacard=false --norm_bins=true
    ./bin/HiggsTauTauPlot2 --cfg=scripts/plot_mssm_2011.cfg  --tau_scale_mode=0 --mode=1 --rebin=1 \
      --category="$i" --plot_name="vis_sm"  --x_axis_label="Visible Mass [GeV]"  --log_y=false --signal_no_stack=false \
      --blind=false --x_blind_min=60 --x_blind_max=1500 --make_datacard=false  --norm_bins=true
  done




