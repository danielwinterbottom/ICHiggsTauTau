
MASSES=(
'260' 
'270'
'280'
'290'
'300'
'310'
'320'
'330'
'340'
'350'
)
for mass in "${MASSES[@]}"
do
    ./bin/PlotCompare -p "ggHTohh-$mass:ggHTohh-$mass:htt_mt.inputs-Hhh-8TeV-mttbb-finebin.root:muTau_2jet1tag:ggHTohhTo2Tau2B$mass:-1:0:4" \
 -p "TT:TT:htt_mt.inputs-Hhh-8TeV-mttbb-finebin.root:muTau_2jet1tag:TT:-1:0:2" \
 --norm_mode=3 --big_label="#mu#tau_{h}" \
 --outname="mjj_h_signal_vs_ttbar_mt_2jet1tag-mH$mass" --x_axis_title="m_{#tau#taubb}  [GeV]"  --norm_bins=true \
 --custom_y_axis_min=true --y_axis_min=0.0
    
    ./bin/PlotCompare -p "ggHTohh-$mass:ggHTohh-$mass:htt_mt.inputs-Hhh-8TeV-mttbb-finebin.root:muTau_2jet2tag:ggHTohhTo2Tau2B$mass:-1:0:4" \
 -p "TT:TT:htt_mt.inputs-Hhh-8TeV-mttbb-finebin.root:muTau_2jet2tag:TT:-1:0:2" \
 --norm_mode=3 --big_label="#mu#tau_{h}" \
 --outname="mjj_h_signal_vs_ttbar_mt_2jet2tag-mH$mass" --x_axis_title="m_{#tau#taubb}  [GeV]"  --norm_bins=true \
 --custom_y_axis_min=true --y_axis_min=0.0

 done
