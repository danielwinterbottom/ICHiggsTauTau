## Examples

mass=125
et_input=cmb
mt_input=cmb
em_input=cmb
et_scheme="et_default"
mt_scheme="mt_with_zmm"
em_scheme="em_with_hww"
postfit="true"
lumi_title_7tev="4.9 fb^{-1} at 7 TeV"
lumi_title_8tev="19.7 fb^{-1} at 8 TeV"
blind=" --blind=false"
cfg="scripts/new_plot_sm_2012.cfg"
fitresult_file="cmb/125/out/mlfit.root:fit_s"
signal_scheme=sm_nomult
###################################### mu-tau 8TeV

./bin/PostFitPlot2 --channel=mt --selection="0-jet low p_{T}^{#tau_{h}}:1" --eras="8TeV" \
 --datacard_path=$mt_input/$mass/ --fitresult=$fitresult_file \
 --signal_mass=$mass --postfit=$postfit --mssm=false --log_y=false \
 --title_left="$lumi_title_8tev" $blind --norm_bins=true --cfg=$cfg \
 --background_scheme=$mt_scheme --draw_error_band=true \
 --x_axis_label="m_{#tau#tau} [GeV]" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" \
 --extra_pad=1.1 --signal_scheme=$signal_scheme

./bin/PostFitPlot2 --channel=mt --selection="0-jet high p_{T}^{#tau_{h}}:2" --eras="8TeV" \
 --datacard_path=$mt_input/$mass/ --fitresult=$fitresult_file \
 --signal_mass=$mass --postfit=$postfit --mssm=false --log_y=false \
 --title_left="$lumi_title_8tev" $blind --norm_bins=true --cfg=$cfg \
 --background_scheme=$mt_scheme --draw_error_band=true \
 --x_axis_label="m_{#tau#tau} [GeV]" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" \
 --extra_pad=1.1 --signal_scheme=$signal_scheme

./bin/PostFitPlot2 --channel=mt --selection="1-jet low p_{T}^{#tau_{h}}:3" --eras="8TeV" \
 --datacard_path=$mt_input/$mass/ --fitresult=$fitresult_file \
 --signal_mass=$mass --postfit=$postfit --mssm=false --log_y=false \
 --title_left="$lumi_title_8tev" $blind --norm_bins=true --cfg=$cfg \
 --background_scheme=$mt_scheme --draw_error_band=true \
 --x_axis_label="m_{#tau#tau} [GeV]" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" \
 --extra_pad=1.1 --signal_scheme=$signal_scheme

./bin/PostFitPlot2 --channel=mt --selection="1-jet high p_{T}^{#tau_{h}}:4" --eras="8TeV" \
 --datacard_path=$mt_input/$mass/ --fitresult=$fitresult_file \
 --signal_mass=$mass --postfit=$postfit --mssm=false --log_y=false \
 --title_left="$lumi_title_8tev" $blind --norm_bins=true --cfg=$cfg \
 --background_scheme=$mt_scheme --draw_error_band=true \
 --x_axis_label="m_{#tau#tau} [GeV]" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" \
 --extra_pad=1.1 --signal_scheme=$signal_scheme

./bin/PostFitPlot2 --channel=mt --selection="1-jet high p_{T}^{#tau_{h}}:5" --thirdline="boosted" --eras="8TeV" \
 --datacard_path=$mt_input/$mass/ --fitresult=$fitresult_file \
 --signal_mass=$mass --postfit=$postfit --mssm=false --log_y=false \
 --title_left="$lumi_title_8tev" $blind --norm_bins=true --cfg=$cfg \
 --background_scheme=$mt_scheme --draw_error_band=true \
 --x_axis_label="m_{#tau#tau} [GeV]" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" \
 --extra_pad=1.1  --signal_scheme=$signal_scheme

./bin/PostFitPlot2 --channel=mt --selection="Loose VBF Tag:6" --eras="8TeV" \
 --datacard_path=$mt_input/$mass/ --fitresult=$fitresult_file \
 --signal_mass=$mass --postfit=$postfit --mssm=false --log_y=false \
 --title_left="$lumi_title_8tev" $blind --norm_bins=true --cfg=$cfg \
 --background_scheme=$mt_scheme --draw_error_band=true \
 --x_axis_label="m_{#tau#tau} [GeV]" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" \
 --extra_pad=1.4  --signal_scheme=$signal_scheme

./bin/PostFitPlot2 --channel=mt --selection="Tight VBF Tag:7" --eras="8TeV" \
 --datacard_path=$mt_input/$mass/ --fitresult=$fitresult_file \
 --signal_mass=$mass --postfit=$postfit --mssm=false --log_y=false \
 --title_left="$lumi_title_8tev" $blind --norm_bins=true --cfg=$cfg \
 --background_scheme=$mt_scheme --draw_error_band=true \
 --x_axis_label="m_{#tau#tau} [GeV]" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" \
 --extra_pad=1.7  --signal_scheme=$signal_scheme

###################################### mu-tau 7TeV
./bin/PostFitPlot2 --channel=mt --selection="0-jet low p_{T}^{#tau_{h}}:1" --eras="7TeV" \
 --datacard_path=$mt_input/$mass/ --fitresult=$fitresult_file \
 --signal_mass=$mass --postfit=$postfit --mssm=false --log_y=false \
 --title_left="$lumi_title_7tev" $blind --norm_bins=true --cfg=$cfg \
 --background_scheme=$mt_scheme --draw_error_band=true \
 --x_axis_label="m_{#tau#tau} [GeV]" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" \
 --extra_pad=1.1 --signal_scheme=$signal_scheme

./bin/PostFitPlot2 --channel=mt --selection="0-jet high p_{T}^{#tau_{h}}:2" --eras="7TeV" \
 --datacard_path=$mt_input/$mass/ --fitresult=$fitresult_file \
 --signal_mass=$mass --postfit=$postfit --mssm=false --log_y=false \
 --title_left="$lumi_title_7tev" $blind --norm_bins=true --cfg=$cfg \
 --background_scheme=$mt_scheme --draw_error_band=true \
 --x_axis_label="m_{#tau#tau} [GeV]" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" \
 --extra_pad=1.1 --signal_scheme=$signal_scheme

./bin/PostFitPlot2 --channel=mt --selection="1-jet low p_{T}^{#tau_{h}}:3" --eras="7TeV" \
 --datacard_path=$mt_input/$mass/ --fitresult=$fitresult_file \
 --signal_mass=$mass --postfit=$postfit --mssm=false --log_y=false \
 --title_left="$lumi_title_7tev" $blind --norm_bins=true --cfg=$cfg \
 --background_scheme=$mt_scheme --draw_error_band=true \
 --x_axis_label="m_{#tau#tau} [GeV]" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" \
 --extra_pad=1.1 --signal_scheme=$signal_scheme

./bin/PostFitPlot2 --channel=mt --selection="1-jet high p_{T}^{#tau_{h}}:4" --eras="7TeV" \
 --datacard_path=$mt_input/$mass/ --fitresult=$fitresult_file \
 --signal_mass=$mass --postfit=$postfit --mssm=false --log_y=false \
 --title_left="$lumi_title_7tev" $blind --norm_bins=true --cfg=$cfg \
 --background_scheme=$mt_scheme --draw_error_band=true \
 --x_axis_label="m_{#tau#tau} [GeV]" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" \
 --extra_pad=1.1 --signal_scheme=$signal_scheme

./bin/PostFitPlot2 --channel=mt --selection="1-jet high p_{T}^{#tau_{h}}:5" --thirdline="boosted" --eras="7TeV" \
 --datacard_path=$mt_input/$mass/ --fitresult=$fitresult_file \
 --signal_mass=$mass --postfit=$postfit --mssm=false --log_y=false \
 --title_left="$lumi_title_7tev" $blind --norm_bins=true --cfg=$cfg \
 --background_scheme=$mt_scheme --draw_error_band=true \
 --x_axis_label="m_{#tau#tau} [GeV]" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" \
 --extra_pad=1.1  --signal_scheme=$signal_scheme

./bin/PostFitPlot2 --channel=mt --selection="VBF Tag:6" --eras="7TeV" \
 --datacard_path=$mt_input/$mass/ --fitresult=$fitresult_file \
 --signal_mass=$mass --postfit=$postfit --mssm=false --log_y=false \
 --title_left="$lumi_title_7tev" $blind --norm_bins=true --cfg=$cfg \
 --background_scheme=$mt_scheme --draw_error_band=true \
 --x_axis_label="m_{#tau#tau} [GeV]" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" \
 --extra_pad=1.4  --signal_scheme=$signal_scheme

# ###################################### ele-tau 8TeV
./bin/PostFitPlot2 --channel=et --selection="0-jet low p_{T}^{#tau_{h}}:1" --eras="8TeV" \
 --datacard_path=$et_input/$mass/ --fitresult=$fitresult_file \
 --signal_mass=$mass --postfit=$postfit --mssm=false --log_y=false \
 --title_left="$lumi_title_8tev" $blind --norm_bins=true --cfg=$cfg \
 --background_scheme=$et_scheme --draw_error_band=true \
 --x_axis_label="m_{#tau#tau} [GeV]" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" \
 --extra_pad=1.1 --signal_scheme=$signal_scheme

./bin/PostFitPlot2 --channel=et --selection="0-jet high p_{T}^{#tau_{h}}:2" --eras="8TeV" \
 --datacard_path=$et_input/$mass/ --fitresult=$fitresult_file \
 --signal_mass=$mass --postfit=$postfit --mssm=false --log_y=false \
 --title_left="$lumi_title_8tev" $blind --norm_bins=true --cfg=$cfg \
 --background_scheme=$et_scheme --draw_error_band=true \
 --x_axis_label="m_{#tau#tau} [GeV]" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" \
 --extra_pad=1.1 --signal_scheme=$signal_scheme

./bin/PostFitPlot2 --channel=et --selection="1-jet low p_{T}^{#tau_{h}}:3" --eras="8TeV" \
 --datacard_path=$et_input/$mass/ --fitresult=$fitresult_file \
 --signal_mass=$mass --postfit=$postfit --mssm=false --log_y=false \
 --title_left="$lumi_title_8tev" $blind --norm_bins=true --cfg=$cfg \
 --background_scheme=$et_scheme --draw_error_band=true \
 --x_axis_label="m_{#tau#tau} [GeV]" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" \
 --extra_pad=1.1 --signal_scheme=$signal_scheme

./bin/PostFitPlot2 --channel=et --selection="1-jet high p_{T}^{#tau_{h}}:5" --thirdline="boosted" --eras="8TeV" \
 --datacard_path=$et_input/$mass/ --fitresult=$fitresult_file \
 --signal_mass=$mass --postfit=$postfit --mssm=false --log_y=false \
 --title_left="$lumi_title_8tev" $blind --norm_bins=true --cfg=$cfg \
 --background_scheme=$et_scheme --draw_error_band=true \
 --x_axis_label="m_{#tau#tau} [GeV]" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" \
 --extra_pad=1.1  --signal_scheme=$signal_scheme

./bin/PostFitPlot2 --channel=et --selection="Loose VBF Tag:6" --eras="8TeV" \
 --datacard_path=$et_input/$mass/ --fitresult=$fitresult_file \
 --signal_mass=$mass --postfit=$postfit --mssm=false --log_y=false \
 --title_left="$lumi_title_8tev" $blind --norm_bins=true --cfg=$cfg \
 --background_scheme=$et_scheme --draw_error_band=true \
 --x_axis_label="m_{#tau#tau} [GeV]" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" \
 --extra_pad=1.4  --signal_scheme=$signal_scheme

./bin/PostFitPlot2 --channel=et --selection="Tight VBF Tag:7" --eras="8TeV" \
 --datacard_path=$et_input/$mass/ --fitresult=$fitresult_file \
 --signal_mass=$mass --postfit=$postfit --mssm=false --log_y=false \
 --title_left="$lumi_title_8tev" $blind --norm_bins=true --cfg=$cfg \
 --background_scheme=$et_scheme --draw_error_band=true \
 --x_axis_label="m_{#tau#tau} [GeV]" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" \
 --extra_pad=1.7  --signal_scheme=$signal_scheme

###################################### e-tau 7TeV
./bin/PostFitPlot2 --channel=et --selection="0-jet low p_{T}^{#tau_{h}}:1" --eras="7TeV" \
 --datacard_path=$et_input/$mass/ --fitresult=$fitresult_file \
 --signal_mass=$mass --postfit=$postfit --mssm=false --log_y=false \
 --title_left="$lumi_title_7tev" $blind --norm_bins=true --cfg=$cfg \
 --background_scheme=$et_scheme --draw_error_band=true \
 --x_axis_label="m_{#tau#tau} [GeV]" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" \
 --extra_pad=1.1 --signal_scheme=$signal_scheme

./bin/PostFitPlot2 --channel=et --selection="0-jet high p_{T}^{#tau_{h}}:2" --eras="7TeV" \
 --datacard_path=$et_input/$mass/ --fitresult=$fitresult_file \
 --signal_mass=$mass --postfit=$postfit --mssm=false --log_y=false \
 --title_left="$lumi_title_7tev" $blind --norm_bins=true --cfg=$cfg \
 --background_scheme=$et_scheme --draw_error_band=true \
 --x_axis_label="m_{#tau#tau} [GeV]" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" \
 --extra_pad=1.1 --signal_scheme=$signal_scheme

./bin/PostFitPlot2 --channel=et --selection="1-jet low p_{T}^{#tau_{h}}:3" --eras="7TeV" \
 --datacard_path=$et_input/$mass/ --fitresult=$fitresult_file \
 --signal_mass=$mass --postfit=$postfit --mssm=false --log_y=false \
 --title_left="$lumi_title_7tev" $blind --norm_bins=true --cfg=$cfg \
 --background_scheme=$et_scheme --draw_error_band=true \
 --x_axis_label="m_{#tau#tau} [GeV]" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" \
 --extra_pad=1.1 --signal_scheme=$signal_scheme

./bin/PostFitPlot2 --channel=et --selection="1-jet high p_{T}^{#tau_{h}}:5" --thirdline="boosted" --eras="7TeV" \
 --datacard_path=$et_input/$mass/ --fitresult=$fitresult_file \
 --signal_mass=$mass --postfit=$postfit --mssm=false --log_y=false \
 --title_left="$lumi_title_7tev" $blind --norm_bins=true --cfg=$cfg \
 --background_scheme=$et_scheme --draw_error_band=true \
 --x_axis_label="m_{#tau#tau} [GeV]" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" \
 --extra_pad=1.1  --signal_scheme=$signal_scheme

./bin/PostFitPlot2 --channel=et --selection="VBF Tag:6" --eras="7TeV" \
 --datacard_path=$et_input/$mass/ --fitresult=$fitresult_file \
 --signal_mass=$mass --postfit=$postfit --mssm=false --log_y=false \
 --title_left="$lumi_title_7tev" $blind --norm_bins=true --cfg=$cfg \
 --background_scheme=$et_scheme --draw_error_band=true \
 --x_axis_label="m_{#tau#tau} [GeV]" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" \
 --extra_pad=1.4  --signal_scheme=$signal_scheme


# ###################################### e-mu 8TeV
./bin/PostFitPlot2 --channel=em --selection="0-jet low p_{T}^{#mu}:0" --eras="8TeV" \
 --datacard_path=$em_input/$mass/ --fitresult=$fitresult_file \
 --signal_mass=$mass --postfit=$postfit --mssm=false --log_y=false \
 --title_left="$lumi_title_8tev" $blind --norm_bins=true --cfg=$cfg \
 --background_scheme=$em_scheme --draw_error_band=true \
 --x_axis_label="m_{#tau#tau} [GeV]" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" \
 --extra_pad=1.1 --signal_scheme=$signal_scheme

./bin/PostFitPlot2 --channel=em --selection="0-jet high p_{T}^{#mu}:1" --eras="8TeV" \
 --datacard_path=$em_input/$mass/ --fitresult=$fitresult_file \
 --signal_mass=$mass --postfit=$postfit --mssm=false --log_y=false \
 --title_left="$lumi_title_8tev" $blind --norm_bins=true --cfg=$cfg \
 --background_scheme=$em_scheme --draw_error_band=true \
 --x_axis_label="m_{#tau#tau} [GeV]" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" \
 --extra_pad=1.1 --signal_scheme=$signal_scheme

./bin/PostFitPlot2 --channel=em --selection="1-jet low p_{T}^{#mu}:2" --eras="8TeV" \
 --datacard_path=$em_input/$mass/ --fitresult=$fitresult_file \
 --signal_mass=$mass --postfit=$postfit --mssm=false --log_y=false \
 --title_left="$lumi_title_8tev" $blind --norm_bins=true --cfg=$cfg \
 --background_scheme=$em_scheme --draw_error_band=true \
 --x_axis_label="m_{#tau#tau} [GeV]" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" \
 --extra_pad=1.1 --signal_scheme=$signal_scheme

./bin/PostFitPlot2 --channel=em --selection="1-jet high p_{T}^{#mu}:3" --eras="8TeV" \
 --datacard_path=$em_input/$mass/ --fitresult=$fitresult_file \
 --signal_mass=$mass --postfit=$postfit --mssm=false --log_y=false \
 --title_left="$lumi_title_8tev" $blind --norm_bins=true --cfg=$cfg \
 --background_scheme=$em_scheme --draw_error_band=true \
 --x_axis_label="m_{#tau#tau} [GeV]" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" \
 --extra_pad=1.1  --signal_scheme=$signal_scheme

./bin/PostFitPlot2 --channel=em --selection="Loose VBF Tag:4" --eras="8TeV" \
 --datacard_path=$em_input/$mass/ --fitresult=$fitresult_file \
 --signal_mass=$mass --postfit=$postfit --mssm=false --log_y=false \
 --title_left="$lumi_title_8tev" $blind --norm_bins=true --cfg=$cfg \
 --background_scheme=$em_scheme --draw_error_band=true \
 --x_axis_label="m_{#tau#tau} [GeV]" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" \
 --extra_pad=1.4  --signal_scheme=$signal_scheme

./bin/PostFitPlot2 --channel=em --selection="Tight VBF Tag:5" --eras="8TeV" \
 --datacard_path=$em_input/$mass/ --fitresult=$fitresult_file \
 --signal_mass=$mass --postfit=$postfit --mssm=false --log_y=false \
 --title_left="$lumi_title_8tev" $blind --norm_bins=true --cfg=$cfg \
 --background_scheme=$em_scheme --draw_error_band=true \
 --x_axis_label="m_{#tau#tau} [GeV]" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" \
 --extra_pad=1.7  --signal_scheme=$signal_scheme

###################################### e-mu 7TeV
./bin/PostFitPlot2 --channel=em --selection="0-jet low p_{T}^{#mu}:0" --eras="7TeV" \
 --datacard_path=$em_input/$mass/ --fitresult=$fitresult_file \
 --signal_mass=$mass --postfit=$postfit --mssm=false --log_y=false \
 --title_left="$lumi_title_7tev" $blind --norm_bins=true --cfg=$cfg \
 --background_scheme=$em_scheme --draw_error_band=true \
 --x_axis_label="m_{#tau#tau} [GeV]" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" \
 --extra_pad=1.1 --signal_scheme=$signal_scheme

./bin/PostFitPlot2 --channel=em --selection="0-jet high p_{T}^{#mu}:1" --eras="7TeV" \
 --datacard_path=$em_input/$mass/ --fitresult=$fitresult_file \
 --signal_mass=$mass --postfit=$postfit --mssm=false --log_y=false \
 --title_left="$lumi_title_7tev" $blind --norm_bins=true --cfg=$cfg \
 --background_scheme=$em_scheme --draw_error_band=true \
 --x_axis_label="m_{#tau#tau} [GeV]" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" \
 --extra_pad=1.1 --signal_scheme=$signal_scheme

./bin/PostFitPlot2 --channel=em --selection="1-jet low p_{T}^{#mu}:2" --eras="7TeV" \
 --datacard_path=$em_input/$mass/ --fitresult=$fitresult_file \
 --signal_mass=$mass --postfit=$postfit --mssm=false --log_y=false \
 --title_left="$lumi_title_7tev" $blind --norm_bins=true --cfg=$cfg \
 --background_scheme=$em_scheme --draw_error_band=true \
 --x_axis_label="m_{#tau#tau} [GeV]" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" \
 --extra_pad=1.1 --signal_scheme=$signal_scheme

./bin/PostFitPlot2 --channel=em --selection="1-jet high p_{T}^{#mu}:3" --eras="7TeV" \
 --datacard_path=$em_input/$mass/ --fitresult=$fitresult_file \
 --signal_mass=$mass --postfit=$postfit --mssm=false --log_y=false \
 --title_left="$lumi_title_7tev" $blind --norm_bins=true --cfg=$cfg \
 --background_scheme=$em_scheme --draw_error_band=true \
 --x_axis_label="m_{#tau#tau} [GeV]" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" \
 --extra_pad=1.1  --signal_scheme=$signal_scheme

./bin/PostFitPlot2 --channel=em --selection="VBF Tag:4" --eras="7TeV" \
 --datacard_path=$em_input/$mass/ --fitresult=$fitresult_file \
 --signal_mass=$mass --postfit=$postfit --mssm=false --log_y=false \
 --title_left="$lumi_title_7tev" $blind --norm_bins=true --cfg=$cfg \
 --background_scheme=$em_scheme --draw_error_band=true \
 --x_axis_label="m_{#tau#tau} [GeV]" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" \
 --extra_pad=1.4  --signal_scheme=$signal_scheme
