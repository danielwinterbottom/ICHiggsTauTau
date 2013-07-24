## Examples

mass=125
et_input=Paper-LIMITS/sm-v1/et
mt_input=Paper-LIMITS/sm-v1/mt
em_input=Paper-LIMITS/sm-v1/em
do_et=false
et_scheme="et_default"
mt_scheme="mt_with_zmm"
em_scheme="em_default"
do_mt=true
do_em=false
postfit="true"
lumi_title_7tev="CMS Preliminary, #sqrt{s} = 7 TeV, L = 4.9 fb^{-1}"
lumi_title_8tev="CMS Preliminary, #sqrt{s} = 8 TeV, L = 19.8 fb^{-1}"
blind="--blind=true --x_blind_min=100 --x_blind_max=160"
cfg="scripts/new_plot_sm_2012.cfg"

###################################### mu-tau 8TeV
./bin/PostFitPlot2 --channel=mt --selection="0-Jet Low:0" --eras="8TeV" \
 --datacard_path=$mt_input/$mass/ --root_file_path=$mt_input/common/ --pulls_file=$mt_input/$mass/out/mlfit.txt \
 --signal_mass=$mass --postfit=$postfit --mssm=false --log_y=false \
 --title_left="$lumi_title_8tev" $blind --norm_bins=true --cfg=$cfg \
 --background_scheme=$mt_scheme --draw_error_band=true \
 --x_axis_label="M_{#tau#tau} [GeV]" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" \
 --extra_pad=1.2 --draw_ratio=true

./bin/PostFitPlot2 --channel=mt --selection="0-Jet Medium:1" --eras="8TeV" \
 --datacard_path=$mt_input/$mass/ --root_file_path=$mt_input/common/ --pulls_file=$mt_input/$mass/out/mlfit.txt \
 --signal_mass=$mass --postfit=$postfit --mssm=false --log_y=false \
 --title_left="$lumi_title_8tev" $blind --norm_bins=true --cfg=$cfg \
 --background_scheme=$mt_scheme --draw_error_band=true \
 --x_axis_label="M_{#tau#tau} [GeV]" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" \
 --extra_pad=1.2 --draw_ratio=true

./bin/PostFitPlot2 --channel=mt --selection="0-Jet High:2" --eras="8TeV" \
 --datacard_path=$mt_input/$mass/ --root_file_path=$mt_input/common/ --pulls_file=$mt_input/$mass/out/mlfit.txt \
 --signal_mass=$mass --postfit=$postfit --mssm=false --log_y=false \
 --title_left="$lumi_title_8tev" $blind --norm_bins=true --cfg=$cfg \
 --background_scheme=$mt_scheme --draw_error_band=true \
 --x_axis_label="M_{#tau#tau} [GeV]" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" \
 --extra_pad=1.2 --draw_ratio=true

./bin/PostFitPlot2 --channel=mt --selection="1-Jet Medium:3" --eras="8TeV" \
 --datacard_path=$mt_input/$mass/ --root_file_path=$mt_input/common/ --pulls_file=$mt_input/$mass/out/mlfit.txt \
 --signal_mass=$mass --postfit=$postfit --mssm=false --log_y=false \
 --title_left="$lumi_title_8tev" $blind --norm_bins=true --cfg=$cfg \
 --background_scheme=$mt_scheme --draw_error_band=true \
 --x_axis_label="M_{#tau#tau} [GeV]" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" \
 --extra_pad=1.2 --draw_ratio=true --signal_scheme=sm_default

./bin/PostFitPlot2 --channel=mt --selection="1-Jet High, Low p_{T}^{#tau#tau}:4" --eras="8TeV" \
 --datacard_path=$mt_input/$mass/ --root_file_path=$mt_input/common/ --pulls_file=$mt_input/$mass/out/mlfit.txt \
 --signal_mass=$mass --postfit=$postfit --mssm=false --log_y=false \
 --title_left="$lumi_title_8tev" $blind --norm_bins=true --cfg=$cfg \
 --background_scheme=$mt_scheme --draw_error_band=true \
 --x_axis_label="M_{#tau#tau} [GeV]" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" \
 --extra_pad=1.2 --draw_ratio=true --signal_scheme=sm_default

./bin/PostFitPlot2 --channel=mt --selection="1-Jet High, High p_{T}^{#tau#tau}:5" --eras="8TeV" \
 --datacard_path=$mt_input/$mass/ --root_file_path=$mt_input/common/ --pulls_file=$mt_input/$mass/out/mlfit.txt \
 --signal_mass=$mass --postfit=$postfit --mssm=false --log_y=false \
 --title_left="$lumi_title_8tev" $blind --norm_bins=true --cfg=$cfg \
 --background_scheme=$mt_scheme --draw_error_band=true \
 --x_axis_label="M_{#tau#tau} [GeV]" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" \
 --extra_pad=1.2  --draw_ratio=true --signal_scheme=sm_default

./bin/PostFitPlot2 --channel=mt --selection="VBF Loose:6" --eras="8TeV" \
 --datacard_path=$mt_input/$mass/ --root_file_path=$mt_input/common/ --pulls_file=$mt_input/$mass/out/mlfit.txt \
 --signal_mass=$mass --postfit=$postfit --mssm=false --log_y=false \
 --title_left="$lumi_title_8tev" $blind --norm_bins=true --cfg=$cfg \
 --background_scheme=$mt_scheme --draw_error_band=true \
 --x_axis_label="M_{#tau#tau} [GeV]" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" \
 --extra_pad=1.6  --draw_ratio=true --signal_scheme=sm_default

./bin/PostFitPlot2 --channel=mt --selection="VBF Tight:7" --eras="8TeV" \
 --datacard_path=$mt_input/$mass/ --root_file_path=$mt_input/common/ --pulls_file=$mt_input/$mass/out/mlfit.txt \
 --signal_mass=$mass --postfit=$postfit --mssm=false --log_y=false \
 --title_left="$lumi_title_8tev" $blind --norm_bins=true --cfg=$cfg \
 --background_scheme=$mt_scheme --draw_error_band=true \
 --x_axis_label="M_{#tau#tau} [GeV]" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" \
 --extra_pad=2.0  --draw_ratio=true --signal_scheme=sm_default

###################################### mu-tau 7TeV
./bin/PostFitPlot2 --channel=mt --selection="0-Jet Low:0" --eras="7TeV" \
 --datacard_path=$mt_input/$mass/ --root_file_path=$mt_input/common/ --pulls_file=$mt_input/$mass/out/mlfit.txt \
 --signal_mass=$mass --postfit=$postfit --mssm=false --log_y=false \
 --title_left="$lumi_title_7tev" $blind --norm_bins=true --cfg=$cfg \
 --background_scheme=$mt_scheme --draw_error_band=true \
 --x_axis_label="M_{#tau#tau} [GeV]" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" \
 --extra_pad=1.2 --draw_ratio=true

./bin/PostFitPlot2 --channel=mt --selection="0-Jet Medium:1" --eras="7TeV" \
 --datacard_path=$mt_input/$mass/ --root_file_path=$mt_input/common/ --pulls_file=$mt_input/$mass/out/mlfit.txt \
 --signal_mass=$mass --postfit=$postfit --mssm=false --log_y=false \
 --title_left="$lumi_title_7tev" $blind --norm_bins=true --cfg=$cfg \
 --background_scheme=$mt_scheme --draw_error_band=true \
 --x_axis_label="M_{#tau#tau} [GeV]" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" \
 --extra_pad=1.2 --draw_ratio=true

./bin/PostFitPlot2 --channel=mt --selection="0-Jet High:2" --eras="7TeV" \
 --datacard_path=$mt_input/$mass/ --root_file_path=$mt_input/common/ --pulls_file=$mt_input/$mass/out/mlfit.txt \
 --signal_mass=$mass --postfit=$postfit --mssm=false --log_y=false \
 --title_left="$lumi_title_7tev" $blind --norm_bins=true --cfg=$cfg \
 --background_scheme=$mt_scheme --draw_error_band=true \
 --x_axis_label="M_{#tau#tau} [GeV]" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" \
 --extra_pad=1.2 --draw_ratio=true

./bin/PostFitPlot2 --channel=mt --selection="1-Jet Medium:3" --eras="7TeV" \
 --datacard_path=$mt_input/$mass/ --root_file_path=$mt_input/common/ --pulls_file=$mt_input/$mass/out/mlfit.txt \
 --signal_mass=$mass --postfit=$postfit --mssm=false --log_y=false \
 --title_left="$lumi_title_7tev" $blind --norm_bins=true --cfg=$cfg \
 --background_scheme=$mt_scheme --draw_error_band=true \
 --x_axis_label="M_{#tau#tau} [GeV]" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" \
 --extra_pad=1.2 --draw_ratio=true --signal_scheme=sm_default

./bin/PostFitPlot2 --channel=mt --selection="1-Jet High, Low p_{T}^{#tau#tau}:4" --eras="7TeV" \
 --datacard_path=$mt_input/$mass/ --root_file_path=$mt_input/common/ --pulls_file=$mt_input/$mass/out/mlfit.txt \
 --signal_mass=$mass --postfit=$postfit --mssm=false --log_y=false \
 --title_left="$lumi_title_7tev" $blind --norm_bins=true --cfg=$cfg \
 --background_scheme=$mt_scheme --draw_error_band=true \
 --x_axis_label="M_{#tau#tau} [GeV]" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" \
 --extra_pad=1.2 --draw_ratio=true --signal_scheme=sm_default

./bin/PostFitPlot2 --channel=mt --selection="1-Jet High, High p_{T}^{#tau#tau}:5" --eras="7TeV" \
 --datacard_path=$mt_input/$mass/ --root_file_path=$mt_input/common/ --pulls_file=$mt_input/$mass/out/mlfit.txt \
 --signal_mass=$mass --postfit=$postfit --mssm=false --log_y=false \
 --title_left="$lumi_title_7tev" $blind --norm_bins=true --cfg=$cfg \
 --background_scheme=$mt_scheme --draw_error_band=true \
 --x_axis_label="M_{#tau#tau} [GeV]" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" \
 --extra_pad=1.2  --draw_ratio=true --signal_scheme=sm_default

./bin/PostFitPlot2 --channel=mt --selection="VBF:6" --eras="7TeV" \
 --datacard_path=$mt_input/$mass/ --root_file_path=$mt_input/common/ --pulls_file=$mt_input/$mass/out/mlfit.txt \
 --signal_mass=$mass --postfit=$postfit --mssm=false --log_y=false \
 --title_left="$lumi_title_7tev" $blind --norm_bins=true --cfg=$cfg \
 --background_scheme=$mt_scheme --draw_error_band=true \
 --x_axis_label="M_{#tau#tau} [GeV]" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" \
 --extra_pad=1.6  --draw_ratio=true --signal_scheme=sm_default

###################################### ele-tau 8TeV
./bin/PostFitPlot2 --channel=et --selection="0-Jet Low:0" --eras="8TeV" \
 --datacard_path=$et_input/$mass/ --root_file_path=$et_input/common/ --pulls_file=$et_input/$mass/out/mlfit.txt \
 --signal_mass=$mass --postfit=$postfit --mssm=false --log_y=false \
 --title_left="$lumi_title_8tev" $blind --norm_bins=true --cfg=$cfg \
 --background_scheme=$et_scheme --draw_error_band=true \
 --x_axis_label="M_{#tau#tau} [GeV]" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" \
 --extra_pad=1.2 --draw_ratio=true

./bin/PostFitPlot2 --channel=et --selection="0-Jet Medium:1" --eras="8TeV" \
 --datacard_path=$et_input/$mass/ --root_file_path=$et_input/common/ --pulls_file=$et_input/$mass/out/mlfit.txt \
 --signal_mass=$mass --postfit=$postfit --mssm=false --log_y=false \
 --title_left="$lumi_title_8tev" $blind --norm_bins=true --cfg=$cfg \
 --background_scheme=$et_scheme --draw_error_band=true \
 --x_axis_label="M_{#tau#tau} [GeV]" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" \
 --extra_pad=1.2 --draw_ratio=true

./bin/PostFitPlot2 --channel=et --selection="0-Jet High:2" --eras="8TeV" \
 --datacard_path=$et_input/$mass/ --root_file_path=$et_input/common/ --pulls_file=$et_input/$mass/out/mlfit.txt \
 --signal_mass=$mass --postfit=$postfit --mssm=false --log_y=false \
 --title_left="$lumi_title_8tev" $blind --norm_bins=true --cfg=$cfg \
 --background_scheme=$et_scheme --draw_error_band=true \
 --x_axis_label="M_{#tau#tau} [GeV]" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" \
 --extra_pad=1.2 --draw_ratio=true

./bin/PostFitPlot2 --channel=et --selection="1-Jet Medium:3" --eras="8TeV" \
 --datacard_path=$et_input/$mass/ --root_file_path=$et_input/common/ --pulls_file=$et_input/$mass/out/mlfit.txt \
 --signal_mass=$mass --postfit=$postfit --mssm=false --log_y=false \
 --title_left="$lumi_title_8tev" $blind --norm_bins=true --cfg=$cfg \
 --background_scheme=$et_scheme --draw_error_band=true \
 --x_axis_label="M_{#tau#tau} [GeV]" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" \
 --extra_pad=1.2 --draw_ratio=true --signal_scheme=sm_default

./bin/PostFitPlot2 --channel=et --selection="1-Jet High, High p_{T}^{#tau#tau}:5" --eras="8TeV" \
 --datacard_path=$et_input/$mass/ --root_file_path=$et_input/common/ --pulls_file=$et_input/$mass/out/mlfit.txt \
 --signal_mass=$mass --postfit=$postfit --mssm=false --log_y=false \
 --title_left="$lumi_title_8tev" $blind --norm_bins=true --cfg=$cfg \
 --background_scheme=$et_scheme --draw_error_band=true \
 --x_axis_label="M_{#tau#tau} [GeV]" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" \
 --extra_pad=1.2  --draw_ratio=true --signal_scheme=sm_default

./bin/PostFitPlot2 --channel=et --selection="VBF Loose:6" --eras="8TeV" \
 --datacard_path=$et_input/$mass/ --root_file_path=$et_input/common/ --pulls_file=$et_input/$mass/out/mlfit.txt \
 --signal_mass=$mass --postfit=$postfit --mssm=false --log_y=false \
 --title_left="$lumi_title_8tev" $blind --norm_bins=true --cfg=$cfg \
 --background_scheme=$et_scheme --draw_error_band=true \
 --x_axis_label="M_{#tau#tau} [GeV]" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" \
 --extra_pad=1.6  --draw_ratio=true --signal_scheme=sm_default

./bin/PostFitPlot2 --channel=et --selection="VBF Tight:7" --eras="8TeV" \
 --datacard_path=$et_input/$mass/ --root_file_path=$et_input/common/ --pulls_file=$et_input/$mass/out/mlfit.txt \
 --signal_mass=$mass --postfit=$postfit --mssm=false --log_y=false \
 --title_left="$lumi_title_8tev" $blind --norm_bins=true --cfg=$cfg \
 --background_scheme=$et_scheme --draw_error_band=true \
 --x_axis_label="M_{#tau#tau} [GeV]" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" \
 --extra_pad=2.0  --draw_ratio=true --signal_scheme=sm_default

###################################### ele-tau 7TeV
./bin/PostFitPlot2 --channel=et --selection="0-Jet Low:0" --eras="7TeV" \
 --datacard_path=$et_input/$mass/ --root_file_path=$et_input/common/ --pulls_file=$et_input/$mass/out/mlfit.txt \
 --signal_mass=$mass --postfit=$postfit --mssm=false --log_y=false \
 --title_left="$lumi_title_7tev" $blind --norm_bins=true --cfg=$cfg \
 --background_scheme=$et_scheme --draw_error_band=true \
 --x_axis_label="M_{#tau#tau} [GeV]" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" \
 --extra_pad=1.2 --draw_ratio=true

./bin/PostFitPlot2 --channel=et --selection="0-Jet Medium:1" --eras="7TeV" \
 --datacard_path=$et_input/$mass/ --root_file_path=$et_input/common/ --pulls_file=$et_input/$mass/out/mlfit.txt \
 --signal_mass=$mass --postfit=$postfit --mssm=false --log_y=false \
 --title_left="$lumi_title_7tev" $blind --norm_bins=true --cfg=$cfg \
 --background_scheme=$et_scheme --draw_error_band=true \
 --x_axis_label="M_{#tau#tau} [GeV]" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" \
 --extra_pad=1.2 --draw_ratio=true

./bin/PostFitPlot2 --channel=et --selection="0-Jet High:2" --eras="7TeV" \
 --datacard_path=$et_input/$mass/ --root_file_path=$et_input/common/ --pulls_file=$et_input/$mass/out/mlfit.txt \
 --signal_mass=$mass --postfit=$postfit --mssm=false --log_y=false \
 --title_left="$lumi_title_7tev" $blind --norm_bins=true --cfg=$cfg \
 --background_scheme=$et_scheme --draw_error_band=true \
 --x_axis_label="M_{#tau#tau} [GeV]" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" \
 --extra_pad=1.2 --draw_ratio=true

./bin/PostFitPlot2 --channel=et --selection="1-Jet Medium:3" --eras="7TeV" \
 --datacard_path=$et_input/$mass/ --root_file_path=$et_input/common/ --pulls_file=$et_input/$mass/out/mlfit.txt \
 --signal_mass=$mass --postfit=$postfit --mssm=false --log_y=false \
 --title_left="$lumi_title_7tev" $blind --norm_bins=true --cfg=$cfg \
 --background_scheme=$et_scheme --draw_error_band=true \
 --x_axis_label="M_{#tau#tau} [GeV]" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" \
 --extra_pad=1.2 --draw_ratio=true --signal_scheme=sm_default

./bin/PostFitPlot2 --channel=et --selection="1-Jet High, High p_{T}^{#tau#tau}:5" --eras="7TeV" \
 --datacard_path=$et_input/$mass/ --root_file_path=$et_input/common/ --pulls_file=$et_input/$mass/out/mlfit.txt \
 --signal_mass=$mass --postfit=$postfit --mssm=false --log_y=false \
 --title_left="$lumi_title_7tev" $blind --norm_bins=true --cfg=$cfg \
 --background_scheme=$et_scheme --draw_error_band=true \
 --x_axis_label="M_{#tau#tau} [GeV]" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" \
 --extra_pad=1.2  --draw_ratio=true --signal_scheme=sm_default

./bin/PostFitPlot2 --channel=et --selection="VBF:6" --eras="7TeV" \
 --datacard_path=$et_input/$mass/ --root_file_path=$et_input/common/ --pulls_file=$et_input/$mass/out/mlfit.txt \
 --signal_mass=$mass --postfit=$postfit --mssm=false --log_y=false \
 --title_left="$lumi_title_7tev" $blind --norm_bins=true --cfg=$cfg \
 --background_scheme="et_zll" --draw_error_band=true \
 --x_axis_label="M_{#tau#tau} [GeV]" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" \
 --extra_pad=1.6  --draw_ratio=true --signal_scheme=sm_default

###################################### e-mu 8TeV
./bin/PostFitPlot2 --channel=em --selection="0-Jet Low:0" --eras="8TeV" \
 --datacard_path=$em_input/$mass/ --root_file_path=$em_input/common/ --pulls_file=$em_input/$mass/out/mlfit.txt \
 --signal_mass=$mass --postfit=$postfit --mssm=false --log_y=false \
 --title_left="$lumi_title_8tev" $blind --norm_bins=true --cfg=$cfg \
 --background_scheme=$em_scheme --draw_error_band=true \
 --x_axis_label="M_{#tau#tau} [GeV]" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" \
 --extra_pad=1.2 --draw_ratio=true

./bin/PostFitPlot2 --channel=em --selection="0-Jet High:1" --eras="8TeV" \
 --datacard_path=$em_input/$mass/ --root_file_path=$em_input/common/ --pulls_file=$em_input/$mass/out/mlfit.txt \
 --signal_mass=$mass --postfit=$postfit --mssm=false --log_y=false \
 --title_left="$lumi_title_8tev" $blind --norm_bins=true --cfg=$cfg \
 --background_scheme=$em_scheme --draw_error_band=true \
 --x_axis_label="M_{#tau#tau} [GeV]" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" \
 --extra_pad=1.2 --draw_ratio=true

./bin/PostFitPlot2 --channel=em --selection="1-Jet Low:2" --eras="8TeV" \
 --datacard_path=$em_input/$mass/ --root_file_path=$em_input/common/ --pulls_file=$em_input/$mass/out/mlfit.txt \
 --signal_mass=$mass --postfit=$postfit --mssm=false --log_y=false \
 --title_left="$lumi_title_8tev" $blind --norm_bins=true --cfg=$cfg \
 --background_scheme=$em_scheme --draw_error_band=true \
 --x_axis_label="M_{#tau#tau} [GeV]" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" \
 --extra_pad=1.2 --draw_ratio=true --signal_scheme=sm_default

./bin/PostFitPlot2 --channel=em --selection="1-Jet High:3" --eras="8TeV" \
 --datacard_path=$em_input/$mass/ --root_file_path=$em_input/common/ --pulls_file=$em_input/$mass/out/mlfit.txt \
 --signal_mass=$mass --postfit=$postfit --mssm=false --log_y=false \
 --title_left="$lumi_title_8tev" $blind --norm_bins=true --cfg=$cfg \
 --background_scheme=$em_scheme --draw_error_band=true \
 --x_axis_label="M_{#tau#tau} [GeV]" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" \
 --extra_pad=1.2  --draw_ratio=true --signal_scheme=sm_default

./bin/PostFitPlot2 --channel=em --selection="VBF Loose:4" --eras="8TeV" \
 --datacard_path=$em_input/$mass/ --root_file_path=$em_input/common/ --pulls_file=$em_input/$mass/out/mlfit.txt \
 --signal_mass=$mass --postfit=$postfit --mssm=false --log_y=false \
 --title_left="$lumi_title_8tev" $blind --norm_bins=true --cfg=$cfg \
 --background_scheme=$em_scheme --draw_error_band=true \
 --x_axis_label="M_{#tau#tau} [GeV]" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" \
 --extra_pad=1.6  --draw_ratio=true --signal_scheme=sm_default

./bin/PostFitPlot2 --channel=em --selection="VBF Tight:5" --eras="8TeV" \
 --datacard_path=$em_input/$mass/ --root_file_path=$em_input/common/ --pulls_file=$em_input/$mass/out/mlfit.txt \
 --signal_mass=$mass --postfit=$postfit --mssm=false --log_y=false \
 --title_left="$lumi_title_8tev" $blind --norm_bins=true --cfg=$cfg \
 --background_scheme=$em_scheme --draw_error_band=true \
 --x_axis_label="M_{#tau#tau} [GeV]" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" \
 --extra_pad=2.0  --draw_ratio=true --signal_scheme=sm_default

###################################### e-mu 7TeV
./bin/PostFitPlot2 --channel=em --selection="0-Jet Low:0" --eras="7TeV" \
 --datacard_path=$em_input/$mass/ --root_file_path=$em_input/common/ --pulls_file=$em_input/$mass/out/mlfit.txt \
 --signal_mass=$mass --postfit=$postfit --mssm=false --log_y=false \
 --title_left="$lumi_title_7tev" $blind --norm_bins=true --cfg=$cfg \
 --background_scheme=$em_scheme --draw_error_band=true \
 --x_axis_label="M_{#tau#tau} [GeV]" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" \
 --extra_pad=1.2 --draw_ratio=true

./bin/PostFitPlot2 --channel=em --selection="0-Jet High:1" --eras="7TeV" \
 --datacard_path=$em_input/$mass/ --root_file_path=$em_input/common/ --pulls_file=$em_input/$mass/out/mlfit.txt \
 --signal_mass=$mass --postfit=$postfit --mssm=false --log_y=false \
 --title_left="$lumi_title_7tev" $blind --norm_bins=true --cfg=$cfg \
 --background_scheme=$em_scheme --draw_error_band=true \
 --x_axis_label="M_{#tau#tau} [GeV]" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" \
 --extra_pad=1.2 --draw_ratio=true

./bin/PostFitPlot2 --channel=em --selection="1-Jet Low:2" --eras="7TeV" \
 --datacard_path=$em_input/$mass/ --root_file_path=$em_input/common/ --pulls_file=$em_input/$mass/out/mlfit.txt \
 --signal_mass=$mass --postfit=$postfit --mssm=false --log_y=false \
 --title_left="$lumi_title_7tev" $blind --norm_bins=true --cfg=$cfg \
 --background_scheme=$em_scheme --draw_error_band=true \
 --x_axis_label="M_{#tau#tau} [GeV]" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" \
 --extra_pad=1.2 --draw_ratio=true --signal_scheme=sm_default

./bin/PostFitPlot2 --channel=em --selection="1-Jet High:3" --eras="7TeV" \
 --datacard_path=$em_input/$mass/ --root_file_path=$em_input/common/ --pulls_file=$em_input/$mass/out/mlfit.txt \
 --signal_mass=$mass --postfit=$postfit --mssm=false --log_y=false \
 --title_left="$lumi_title_7tev" $blind --norm_bins=true --cfg=$cfg \
 --background_scheme=$em_scheme --draw_error_band=true \
 --x_axis_label="M_{#tau#tau} [GeV]" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" \
 --extra_pad=1.2  --draw_ratio=true --signal_scheme=sm_default

./bin/PostFitPlot2 --channel=em --selection="VBF:4" --eras="7TeV" \
 --datacard_path=$em_input/$mass/ --root_file_path=$em_input/common/ --pulls_file=$em_input/$mass/out/mlfit.txt \
 --signal_mass=$mass --postfit=$postfit --mssm=false --log_y=false \
 --title_left="$lumi_title_7tev" $blind --norm_bins=true --cfg=$cfg \
 --background_scheme=$em_scheme --draw_error_band=true \
 --x_axis_label="M_{#tau#tau} [GeV]" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" \
 --extra_pad=1.6  --draw_ratio=true --signal_scheme=sm_default


