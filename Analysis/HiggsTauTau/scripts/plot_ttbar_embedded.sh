
#CATEGORIES=(
#'inclusive'
#'0jet_low'
#'0jet_medium'
#'0jet_high'
#'1jet_medium'
#'1jet_high_lowhiggs'
#'1jet_high_mediumhiggs'
#'vbf_loose'
#'vbf_tight'
#)
#
#
##for j in "${CATEGORIES[@]}"
##do
##  ./bin/PlotCompare  \
##  -p "Ztt:Ztt:$FILE:emu_$j/:Ztt:-1:2:1" \
##  -p "ttbarEmbedded:ttbarEmbedded:$FILE:emu_$j/:ttbarEmbedded:-1:2:2" \
##  --x_axis_title="M_{#tau#tau}^{vis} [GeV]" --norm_mode=0  --big_label="e#mu" \
##  --outname="emu_"$j"_ttbar_study.pdf" --log_y=true --norm_bins=true \
##  --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=300 \
##  --title_left=emu_$j
##done
#
#CHN="eleTau"
#FILE=htt_et.inputs-sm-8TeV.root
#for j in "${CATEGORIES[@]}"
#do
#  ./bin/PlotCompare  \
#  -p "ZTT:ZTT:$FILE:"$CHN"_$j/:ZTT:-1:2:1" \
#  -p "TTEmbedded:TopEmbedded:$FILE:"$CHN"_$j/:TTEmbedded:-1:2:2" \
#  --x_axis_title="M_{#tau#tau} [GeV]" --norm_mode=0  --big_label="e#tau_{h}" \
#  --outname="$CHN"_"$j"_ttbar_study.pdf --log_y=true --norm_bins=true \
#  --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=350 \
#  --title_left="$CHN"_$j
#done
#
#
#CHN="muTau"
#FILE=htt_mt.inputs-sm-8TeV.root
#for j in "${CATEGORIES[@]}"
#do
#  ./bin/PlotCompare  \
#  -p "ZTT:ZTT:$FILE:"$CHN"_$j/:ZTT:-1:2:1" \
#  -p "TTEmbedded:TopEmbedded:$FILE:"$CHN"_$j/:TTEmbedded:-1:2:2" \
#  --x_axis_title="M_{#tau#tau} [GeV]" --norm_mode=0  --big_label="#mu#tau_{h}" \
#  --outname="$CHN"_"$j"_ttbar_study.pdf --log_y=true --norm_bins=true \
#  --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=350 \
#  --title_left="$CHN"_$j
#done
#
#CATEGORIES=(
#'inclusive'
#'0jet_low'
#'0jet_high'
#'1jet_low'
#'1jet_high'
#'vbf_loose'
#'vbf_tight'
#)
#CHN="emu"
#FILE=htt_em.inputs-sm-8TeV.root
#for j in "${CATEGORIES[@]}"
#do
#  ./bin/PlotCompare  \
#  -p "ZTT:ZTT:$FILE:"$CHN"_$j/:Ztt:-1:2:1" \
#  -p "TTEmbedded:TopEmbedded:$FILE:"$CHN"_$j/:ttbarEmbedded:-1:2:2" \
#  --x_axis_title="M_{#tau#tau} [GeV]" --norm_mode=0  --big_label="e#mu" \
#  --outname="$CHN"_"$j"_ttbar_study.pdf --log_y=true --norm_bins=true \
#  --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=350 \
#  --title_left="$CHN"_$j
#done
#

CATEGORIES=(
'btag'
'nobtag'
)

#CHN="eleTau"
#FILE=htt_et.inputs-mssm-8TeV-0.root
#for j in "${CATEGORIES[@]}"
#do
#  ./bin/PlotCompare  \
#  -p "TTEmbedded:TopEmbedded:$FILE:"$CHN"_$j/:TTEmbedded:-1:2:2" \
#  -p "TT:Top:$FILE:"$CHN"_$j/:TT:-1:2:4" \
#  --x_axis_title="M_{#tau#tau} [GeV]" --norm_mode=3  --big_label="e#tau_{h}" \
#  --outname="$CHN"_"$j"_ttbar_study.pdf --log_y=false --norm_bins=true \
#  --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=700 \
#  --title_left="$CHN"_$j
#done


CHN="muTau"
FILE=htt_mt.inputs-mssm-8TeV-0.root
for j in "${CATEGORIES[@]}"
do
  ./bin/PlotCompare  \
  -p "TTEmbedded:t#bar{t}(embedded):$FILE:"$CHN"_$j/:TTEmbedded:-1:2:2" \
  -p "TT:t#bar{t}:$FILE:"$CHN"_$j/:TT:-1:2:592" \
  -p "QCD:QCD:$FILE:"$CHN"_$j/:QCD:-1:2:606" \
  -p "W:W:$FILE:"$CHN"_$j/:W:-1:2:634" \
  -p "ZTTMC:Z#rightarrow#tau#tau(MC):htt_mt.inputs-mssm-8TeV-ztt.root:"$CHN"_$j/:ZTT:-1:2:1" \
  -p "ZTT:Z#rightarrow#tau#tau(Embedded):$FILE:"$CHN"_$j/:ZTT:-1:2:796" \
  --x_axis_title="M_{#tau#tau} [GeV]" --norm_mode=0  --big_label=" " \
  --outname="$CHN"_"$j"_ttbar_study.pdf --log_y=true --norm_bins=true --extra_pad=10 \
  --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=1500 \
  --title_left="$CHN"_$j \
  --ratios="TTEmbedded/ZTT/2:ZTTMC/ZTT/1" --ratio_axis_label="Process/ZTT" --ratio_y_min=0.0 --ratio_y_max=2.0
done

CHN="emu"
FILE=htt_em.inputs-mssm-8TeV-0.root
for j in "${CATEGORIES[@]}"
do
  ./bin/PlotCompare  \
  -p "TTEmbedded:TopEmbedded:$FILE:"$CHN"_$j/:ttbarEmbedded:-1:2:2" \
  -p "TT:Top:$FILE:"$CHN"_$j/:ttbar:-1:2:4" \
  --x_axis_title="M_{#tau#tau} [GeV]" --norm_mode=3  --big_label="e#mu" \
  --outname="$CHN"_"$j"_ttbar_study.pdf --log_y=false --norm_bins=true \
  --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=700 \
  --title_left="$CHN"_$j
done
