## Examples

mass=125
et_input=LIMITSSM160913/bbb/et
mt_input=LIMITSSM160913/bbb/mt
em_input=LIMITSSM160913/bbb/em
et_scheme="et_default"
mt_scheme="mt_with_zmm"
em_scheme="em_default"

blind="--blind=true --x_blind_min=100 --x_blind_max=160"
cfg="scripts/new_plot_sm_2012.cfg"

POSTFITS=(
	'false'
	'true'
	)
ERAS=(
	'7TeV' 
	'8TeV'
	# '7TeV,8TeV'
	)
ERA_LABELS=(
	'CMS Preliminary, 4.9 fb^{-1} at 7TeV'
	'CMS Preliminary, 19.7 fb^{-1} at 8TeV'
	# 'CMS Preliminary, 4.9 fb^{-1} at 7TeV, 19.7 fb^{-1} at 8TeV'
	)


for ((j=0 ; j<${#ERAS[*]} ; j++))
do
	for ((i=0 ; i<${#POSTFITS[*]} ; i++))
	do
			#### mu-tau
			./bin/PostFitPlot2 --channel=mt --selection="0-Jet Low:0" --eras=${ERAS[$j]} \
			 --datacard_path=$mt_input/$mass/ --root_file_path=$mt_input/common/ --pulls_file=$mt_input/$mass/out/mlfit.txt \
			 --signal_mass=$mass --postfit=${POSTFITS[$i]} --mssm=false \
			 --title_left="${ERA_LABELS[$j]}" $blind --norm_bins=true --cfg=$cfg \
			 --background_scheme=$mt_scheme --draw_error_band=true \
			 --x_axis_label="M_{#tau#tau} [GeV]" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" \
			 --extra_pad=1.30 --draw_ratio=true --signal_scheme=sm_default

			./bin/PostFitPlot2 --channel=mt --selection="0-Jet Medium:1" --eras=${ERAS[$j]} \
			 --datacard_path=$mt_input/$mass/ --root_file_path=$mt_input/common/ --pulls_file=$mt_input/$mass/out/mlfit.txt \
			 --signal_mass=$mass --postfit=${POSTFITS[$i]} --mssm=false \
			 --title_left="${ERA_LABELS[$j]}" $blind --norm_bins=true --cfg=$cfg \
			 --background_scheme=$mt_scheme --draw_error_band=true \
			 --x_axis_label="M_{#tau#tau} [GeV]" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" \
			 --extra_pad=1.30 --draw_ratio=true --signal_scheme=sm_default

		  ./bin/PostFitPlot2 --channel=mt --selection="0-Jet High:2" --eras=${ERAS[$j]} \
		   --datacard_path=$mt_input/$mass/ --root_file_path=$mt_input/common/ --pulls_file=$mt_input/$mass/out/mlfit.txt \
		   --signal_mass=$mass --postfit=${POSTFITS[$i]} --mssm=false \
		   --title_left="${ERA_LABELS[$j]}" $blind --norm_bins=true --cfg=$cfg \
		   --background_scheme=$mt_scheme --draw_error_band=true \
		   --x_axis_label="M_{#tau#tau} [GeV]" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" \
		   --extra_pad=1.30 --draw_ratio=true --signal_scheme=sm_default 

		  ./bin/PostFitPlot2 --channel=mt --selection="1-Jet Medium:3" --eras=${ERAS[$j]} \
		   --datacard_path=$mt_input/$mass/ --root_file_path=$mt_input/common/ --pulls_file=$mt_input/$mass/out/mlfit.txt \
		   --signal_mass=$mass --postfit=${POSTFITS[$i]} --mssm=false \
		   --title_left="${ERA_LABELS[$j]}" $blind --norm_bins=true --cfg=$cfg \
		   --background_scheme=$mt_scheme --draw_error_band=true \
		   --x_axis_label="M_{#tau#tau} [GeV]" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" \
		   --extra_pad=1.30 --draw_ratio=true --signal_scheme=sm_default 

		  ./bin/PostFitPlot2 --channel=mt --selection="1-Jet High, Low p_{T}^{H}:4" --eras=${ERAS[$j]} \
		   --datacard_path=$mt_input/$mass/ --root_file_path=$mt_input/common/ --pulls_file=$mt_input/$mass/out/mlfit.txt \
		   --signal_mass=$mass --postfit=${POSTFITS[$i]} --mssm=false \
		   --title_left="${ERA_LABELS[$j]}" $blind --norm_bins=true --cfg=$cfg \
		   --background_scheme=$mt_scheme --draw_error_band=true \
		   --x_axis_label="M_{#tau#tau} [GeV]" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" \
		   --extra_pad=1.30 --draw_ratio=true --signal_scheme=sm_default 

		  ./bin/PostFitPlot2 --channel=mt --selection="1-Jet High, Medium p_{T}^{H}:5" --eras=${ERAS[$j]} \
		   --datacard_path=$mt_input/$mass/ --root_file_path=$mt_input/common/ --pulls_file=$mt_input/$mass/out/mlfit.txt \
		   --signal_mass=$mass --postfit=${POSTFITS[$i]} --mssm=false \
		   --title_left="${ERA_LABELS[$j]}" $blind --norm_bins=true --cfg=$cfg \
		   --background_scheme=$mt_scheme --draw_error_band=true \
		   --x_axis_label="M_{#tau#tau} [GeV]" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" \
		   --extra_pad=1.30 --draw_ratio=true --signal_scheme=sm_default

		   #### e-tau
			./bin/PostFitPlot2 --channel=et --selection="0-Jet Low:0" --eras=${ERAS[$j]} \
			 --datacard_path=$et_input/$mass/ --root_file_path=$et_input/common/ --pulls_file=$et_input/$mass/out/mlfit.txt \
			 --signal_mass=$mass --postfit=${POSTFITS[$i]} --mssm=false \
			 --title_left="${ERA_LABELS[$j]}" $blind --norm_bins=true --cfg=$cfg \
			 --background_scheme=$et_scheme --draw_error_band=true \
			 --x_axis_label="M_{#tau#tau} [GeV]" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" \
			 --extra_pad=1.30 --draw_ratio=true --signal_scheme=sm_default

			./bin/PostFitPlot2 --channel=et --selection="0-Jet Medium:1" --eras=${ERAS[$j]} \
			 --datacard_path=$et_input/$mass/ --root_file_path=$et_input/common/ --pulls_file=$et_input/$mass/out/mlfit.txt \
			 --signal_mass=$mass --postfit=${POSTFITS[$i]} --mssm=false \
			 --title_left="${ERA_LABELS[$j]}" $blind --norm_bins=true --cfg=$cfg \
			 --background_scheme=$et_scheme --draw_error_band=true \
			 --x_axis_label="M_{#tau#tau} [GeV]" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" \
			 --extra_pad=1.30 --draw_ratio=true --signal_scheme=sm_default

		  ./bin/PostFitPlot2 --channel=et --selection="0-Jet High:2" --eras=${ERAS[$j]} \
		   --datacard_path=$et_input/$mass/ --root_file_path=$et_input/common/ --pulls_file=$et_input/$mass/out/mlfit.txt \
		   --signal_mass=$mass --postfit=${POSTFITS[$i]} --mssm=false \
		   --title_left="${ERA_LABELS[$j]}" $blind --norm_bins=true --cfg=$cfg \
		   --background_scheme=$et_scheme --draw_error_band=true \
		   --x_axis_label="M_{#tau#tau} [GeV]" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" \
		   --extra_pad=1.30 --draw_ratio=true --signal_scheme=sm_default 

		  ./bin/PostFitPlot2 --channel=et --selection="1-Jet Medium:3" --eras=${ERAS[$j]} \
		   --datacard_path=$et_input/$mass/ --root_file_path=$et_input/common/ --pulls_file=$et_input/$mass/out/mlfit.txt \
		   --signal_mass=$mass --postfit=${POSTFITS[$i]} --mssm=false \
		   --title_left="${ERA_LABELS[$j]}" $blind --norm_bins=true --cfg=$cfg \
		   --background_scheme=$et_scheme --draw_error_band=true \
		   --x_axis_label="M_{#tau#tau} [GeV]" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" \
		   --extra_pad=1.30 --draw_ratio=true --signal_scheme=sm_default 

		  # ./bin/PostFitPlot2 --channel=et --selection="1-Jet High, Low p_{T}^{H}:4" --eras=${ERAS[$j]} \
		  #  --datacard_path=$et_input/$mass/ --root_file_path=$et_input/common/ --pulls_file=$et_input/$mass/out/mlfit.txt \
		  #  --signal_mass=$mass --postfit=${POSTFITS[$i]} --mssm=false \
		  #  --title_left="${ERA_LABELS[$j]}" $blind --norm_bins=true --cfg=$cfg \
		  #  --background_scheme=$et_scheme --draw_error_band=true \
		  #  --x_axis_label="M_{#tau#tau} [GeV]" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" \
		  #  --extra_pad=1.30 --draw_ratio=true --signal_scheme=sm_default 

		  ./bin/PostFitPlot2 --channel=et --selection="1-Jet High, Medium p_{T}^{H}:5" --eras=${ERAS[$j]} \
		   --datacard_path=$et_input/$mass/ --root_file_path=$et_input/common/ --pulls_file=$et_input/$mass/out/mlfit.txt \
		   --signal_mass=$mass --postfit=${POSTFITS[$i]} --mssm=false \
		   --title_left="${ERA_LABELS[$j]}" $blind --norm_bins=true --cfg=$cfg \
		   --background_scheme=$et_scheme --draw_error_band=true \
		   --x_axis_label="M_{#tau#tau} [GeV]" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" \
		   --extra_pad=1.30 --draw_ratio=true --signal_scheme=sm_default

		   ## e-mu
		  ./bin/PostFitPlot2 --channel=em --selection="0-Jet Low:0" --eras=${ERAS[$j]} \
		   --datacard_path=$em_input/$mass/ --root_file_path=$em_input/common/ --pulls_file=$em_input/$mass/out/mlfit.txt \
		   --signal_mass=$mass --postfit=${POSTFITS[$i]} --mssm=false \
		   --title_left="${ERA_LABELS[$j]}" $blind --norm_bins=true --cfg=$cfg \
		   --background_scheme=$em_scheme --draw_error_band=true \
		   --x_axis_label="M_{#tau#tau} [GeV]" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" \
		   --extra_pad=1.30 --draw_ratio=true --signal_scheme=sm_default
		  
		  ./bin/PostFitPlot2 --channel=em --selection="0-Jet High:1" --eras=${ERAS[$j]} \
		   --datacard_path=$em_input/$mass/ --root_file_path=$em_input/common/ --pulls_file=$em_input/$mass/out/mlfit.txt \
		   --signal_mass=$mass --postfit=${POSTFITS[$i]} --mssm=false \
		   --title_left="${ERA_LABELS[$j]}" $blind --norm_bins=true --cfg=$cfg \
		   --background_scheme=$em_scheme --draw_error_band=true \
		   --x_axis_label="M_{#tau#tau} [GeV]" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" \
		   --extra_pad=1.30 --draw_ratio=true --signal_scheme=sm_default			 

	    ./bin/PostFitPlot2 --channel=em --selection="1-Jet Low:2" --eras=${ERAS[$j]} \
	     --datacard_path=$em_input/$mass/ --root_file_path=$em_input/common/ --pulls_file=$em_input/$mass/out/mlfit.txt \
	     --signal_mass=$mass --postfit=${POSTFITS[$i]} --mssm=false \
	     --title_left="${ERA_LABELS[$j]}" $blind --norm_bins=true --cfg=$cfg \
	     --background_scheme=$em_scheme --draw_error_band=true \
	     --x_axis_label="M_{#tau#tau} [GeV]" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" \
	     --extra_pad=1.30 --draw_ratio=true --signal_scheme=sm_default
	    
	    ./bin/PostFitPlot2 --channel=em --selection="1-Jet High:3" --eras=${ERAS[$j]} \
	     --datacard_path=$em_input/$mass/ --root_file_path=$em_input/common/ --pulls_file=$em_input/$mass/out/mlfit.txt \
	     --signal_mass=$mass --postfit=${POSTFITS[$i]} --mssm=false \
	     --title_left="${ERA_LABELS[$j]}" $blind --norm_bins=true --cfg=$cfg \
	     --background_scheme=$em_scheme --draw_error_band=true \
	     --x_axis_label="M_{#tau#tau} [GeV]" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" \
	     --extra_pad=1.30 --draw_ratio=true --signal_scheme=sm_default			 
	done
done

for ((i=0 ; i<${#POSTFITS[*]} ; i++))
do
	./bin/PostFitPlot2 --channel=mt --selection="VBF:6" --eras=${ERAS[0]} \
	 --datacard_path=$mt_input/$mass/ --root_file_path=$mt_input/common/ --pulls_file=$mt_input/$mass/out/mlfit.txt \
	 --signal_mass=$mass --postfit=${POSTFITS[$i]} --mssm=false \
	 --title_left="${ERA_LABELS[0]}" $blind --norm_bins=true --cfg=$cfg \
	 --background_scheme=$mt_scheme --draw_error_band=true \
	 --x_axis_label="M_{#tau#tau} [GeV]" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" \
	 --extra_pad=1.40 --draw_ratio=true --signal_scheme=sm_default

	./bin/PostFitPlot2 --channel=mt --selection="VBF Loose:6" --eras=${ERAS[1]} \
	 --datacard_path=$mt_input/$mass/ --root_file_path=$mt_input/common/ --pulls_file=$mt_input/$mass/out/mlfit.txt \
	 --signal_mass=$mass --postfit=${POSTFITS[$i]} --mssm=false \
	 --title_left="${ERA_LABELS[1]}" $blind --norm_bins=true --cfg=$cfg \
	 --background_scheme=$mt_scheme --draw_error_band=true \
	 --x_axis_label="M_{#tau#tau} [GeV]" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" \
	 --extra_pad=1.40 --draw_ratio=true --signal_scheme=sm_default

	./bin/PostFitPlot2 --channel=mt --selection="VBF Tight:7" --eras=${ERAS[1]} \
	 --datacard_path=$mt_input/$mass/ --root_file_path=$mt_input/common/ --pulls_file=$mt_input/$mass/out/mlfit.txt \
	 --signal_mass=$mass --postfit=${POSTFITS[$i]} --mssm=false \
	 --title_left="${ERA_LABELS[1]}" $blind --norm_bins=true --cfg=$cfg \
	 --background_scheme=$mt_scheme --draw_error_band=true \
	 --x_axis_label="M_{#tau#tau} [GeV]" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" \
	 --extra_pad=1.40 --draw_ratio=true --signal_scheme=sm_default

	./bin/PostFitPlot2 --channel=et --selection="VBF:6" --eras=${ERAS[0]} \
	 --datacard_path=$et_input/$mass/ --root_file_path=$et_input/common/ --pulls_file=$et_input/$mass/out/mlfit.txt \
	 --signal_mass=$mass --postfit=${POSTFITS[$i]} --mssm=false \
	 --title_left="${ERA_LABELS[0]}" $blind --norm_bins=true --cfg=$cfg \
	 --background_scheme=$et_scheme --draw_error_band=true \
	 --x_axis_label="M_{#tau#tau} [GeV]" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" \
	 --extra_pad=1.40 --draw_ratio=true --signal_scheme=sm_default

	./bin/PostFitPlot2 --channel=et --selection="VBF Loose:6" --eras=${ERAS[1]} \
	 --datacard_path=$et_input/$mass/ --root_file_path=$et_input/common/ --pulls_file=$et_input/$mass/out/mlfit.txt \
	 --signal_mass=$mass --postfit=${POSTFITS[$i]} --mssm=false \
	 --title_left="${ERA_LABELS[1]}" $blind --norm_bins=true --cfg=$cfg \
	 --background_scheme=$et_scheme --draw_error_band=true \
	 --x_axis_label="M_{#tau#tau} [GeV]" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" \
	 --extra_pad=1.40 --draw_ratio=true --signal_scheme=sm_default

	./bin/PostFitPlot2 --channel=et --selection="VBF Tight:7" --eras=${ERAS[1]} \
	 --datacard_path=$et_input/$mass/ --root_file_path=$et_input/common/ --pulls_file=$et_input/$mass/out/mlfit.txt \
	 --signal_mass=$mass --postfit=${POSTFITS[$i]} --mssm=false \
	 --title_left="${ERA_LABELS[1]}" $blind --norm_bins=true --cfg=$cfg \
	 --background_scheme=$et_scheme --draw_error_band=true \
	 --x_axis_label="M_{#tau#tau} [GeV]" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" \
	 --extra_pad=1.40 --draw_ratio=true --signal_scheme=sm_default

 ./bin/PostFitPlot2 --channel=em --selection="VBF:4" --eras=${ERAS[0]} \
  --datacard_path=$em_input/$mass/ --root_file_path=$em_input/common/ --pulls_file=$em_input/$mass/out/mlfit.txt \
  --signal_mass=$mass --postfit=${POSTFITS[$i]} --mssm=false \
  --title_left="${ERA_LABELS[0]}" $blind --norm_bins=true --cfg=$cfg \
  --background_scheme=$em_scheme --draw_error_band=true \
  --x_axis_label="M_{#tau#tau} [GeV]" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" \
  --extra_pad=1.40 --draw_ratio=true --signal_scheme=sm_default

 ./bin/PostFitPlot2 --channel=em --selection="VBF Loose:4" --eras=${ERAS[1]} \
  --datacard_path=$em_input/$mass/ --root_file_path=$em_input/common/ --pulls_file=$em_input/$mass/out/mlfit.txt \
  --signal_mass=$mass --postfit=${POSTFITS[$i]} --mssm=false \
  --title_left="${ERA_LABELS[1]}" $blind --norm_bins=true --cfg=$cfg \
  --background_scheme=$em_scheme --draw_error_band=true \
  --x_axis_label="M_{#tau#tau} [GeV]" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" \
  --extra_pad=1.40 --draw_ratio=true --signal_scheme=sm_default

 ./bin/PostFitPlot2 --channel=em --selection="VBF Tight:5" --eras=${ERAS[1]} \
  --datacard_path=$em_input/$mass/ --root_file_path=$em_input/common/ --pulls_file=$em_input/$mass/out/mlfit.txt \
  --signal_mass=$mass --postfit=${POSTFITS[$i]} --mssm=false \
  --title_left="${ERA_LABELS[1]}" $blind --norm_bins=true --cfg=$cfg \
  --background_scheme=$em_scheme --draw_error_band=true \
  --x_axis_label="M_{#tau#tau} [GeV]" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" \
  --extra_pad=1.40 --draw_ratio=true --signal_scheme=sm_default
done



