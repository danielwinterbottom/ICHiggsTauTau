## Examples

mass=160
et_input=output/mloutput_0313-hcp-tails/cmb
mt_input=output/mloutput_0313-hcp-tails/cmb
em_input=output/mloutput_0313-hcp-tails/cmb
tt_input=output/mloutput_0313-hcp-tails/cmb
et_scheme="et_default"
mt_scheme="mt_with_zmm"
em_scheme="em_default"
tt_scheme="mt_default"

blind="--blind=true --x_blind_min=100 --x_blind_max=2000"
cfg="scripts/new_plot_mssm_2012.cfg"

POSTFITS=(
	'false' 
	'true'
	)
ERAS=(
	'7TeV' 
	'8TeV'
	'7TeV,8TeV'
	)
ERA_LABELS=(
	'CMS Preliminary, 4.9 fb^{-1} at 7TeV'
	'CMS Preliminary, 19.8 fb^{-1} at 8TeV'
	'CMS Preliminary, 4.9 fb^{-1} at 7TeV, 19.8 fb^{-1} at 8TeV'
	)

XAXES=(
	'--custom_x_axis_range=true --x_axis_min=0 --x_axis_max=350'
	'--custom_y_axis_min=true --y_axis_min=0.001 --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=2000'
	)
LOGS=(
	'false'
	'true'
	)
SIGS=(
	'mssm_nopars'
	'mssm_nostack_nopars'
	)
PADDING=(
	'1.2'
	'10'
	)


for ((j=0 ; j<${#ERAS[*]} ; j++))
do
	for ((i=0 ; i<${#POSTFITS[*]} ; i++))
	do
		for ((k=0 ; k<${#XAXES[*]} ; k++))
		do
			./bin/PostFitPlot2 --channel=mt --selection="no b-tag:8" --eras=${ERAS[$j]} \
			 --datacard_path=$mt_input/$mass/ --root_file_path=$mt_input/common/ --pulls_file=$mt_input/$mass/out/mlfit.txt \
			 --signal_mass=$mass --postfit=${POSTFITS[$i]} --mssm=true --log_y=${LOGS[$k]} \
			 --title_left="${ERA_LABELS[$j]}" $blind --norm_bins=true --cfg=$cfg \
			 --background_scheme=$mt_scheme --draw_error_band=true \
			 ${XAXES[$k]} \
			 --x_axis_label="M_{#tau#tau} [GeV]" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" \
			 --extra_pad=${PADDING[$k]} --draw_ratio=true --signal_scheme=${SIGS[$k]}

			./bin/PostFitPlot2 --channel=mt --selection="b-tag:9" --eras=${ERAS[$j]} \
			 --datacard_path=$mt_input/$mass/ --root_file_path=$mt_input/common/ --pulls_file=$mt_input/$mass/out/mlfit.txt \
			 --signal_mass=$mass --postfit=${POSTFITS[$i]} --mssm=true --log_y=${LOGS[$k]} \
			 --title_left="${ERA_LABELS[$j]}" $blind --norm_bins=true --cfg=$cfg \
			 --background_scheme=$mt_scheme --draw_error_band=true \
			 ${XAXES[$k]} \
			 --x_axis_label="M_{#tau#tau} [GeV]" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" \
			 --extra_pad=${PADDING[$k]} --draw_ratio=true --signal_scheme=${SIGS[$k]}

		  ./bin/PostFitPlot2 --channel=et --selection="no b-tag:8" --eras=${ERAS[$j]} \
		   --datacard_path=$et_input/$mass/ --root_file_path=$et_input/common/ --pulls_file=$et_input/$mass/out/mlfit.txt \
		   --signal_mass=$mass --postfit=${POSTFITS[$i]} --mssm=true --log_y=${LOGS[$k]} \
		   --title_left="${ERA_LABELS[$j]}" $blind --norm_bins=true --cfg=$cfg \
		   --background_scheme=$et_scheme --draw_error_band=true \
		   ${XAXES[$k]} \
		   --x_axis_label="M_{#tau#tau} [GeV]" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" \
		   --extra_pad=${PADDING[$k]} --draw_ratio=true --signal_scheme=${SIGS[$k]}
 
		  ./bin/PostFitPlot2 --channel=et --selection="b-tag:9" --eras=${ERAS[$j]} \
		   --datacard_path=$et_input/$mass/ --root_file_path=$et_input/common/ --pulls_file=$et_input/$mass/out/mlfit.txt \
		   --signal_mass=$mass --postfit=${POSTFITS[$i]} --mssm=true --log_y=${LOGS[$k]} \
		   --title_left="${ERA_LABELS[$j]}" $blind --norm_bins=true --cfg=$cfg \
		   --background_scheme=$et_scheme --draw_error_band=true \
		   ${XAXES[$k]} \
		   --x_axis_label="M_{#tau#tau} [GeV]" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" \
		   --extra_pad=${PADDING[$k]} --draw_ratio=true --signal_scheme=${SIGS[$k]}

		  ./bin/PostFitPlot2 --channel=em --selection="no b-tag:8" --eras=${ERAS[$j]} \
		   --datacard_path=$em_input/$mass/ --root_file_path=$em_input/common/ --pulls_file=$em_input/$mass/out/mlfit.txt \
		   --signal_mass=$mass --postfit=${POSTFITS[$i]} --mssm=true --log_y=${LOGS[$k]} \
		   --title_left="${ERA_LABELS[$j]}" $blind --norm_bins=true --cfg=$cfg \
		   --background_scheme=$em_scheme --draw_error_band=true \
		   ${XAXES[$k]} \
		   --x_axis_label="M_{#tau#tau} [GeV]" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" \
		   --extra_pad=${PADDING[$k]} --draw_ratio=true --signal_scheme=${SIGS[$k]}
		  
		  ./bin/PostFitPlot2 --channel=em --selection="b-tag:9" --eras=${ERAS[$j]} \
		   --datacard_path=$em_input/$mass/ --root_file_path=$em_input/common/ --pulls_file=$em_input/$mass/out/mlfit.txt \
		   --signal_mass=$mass --postfit=${POSTFITS[$i]} --mssm=true --log_y=${LOGS[$k]} \
		   --title_left="${ERA_LABELS[$j]}" $blind --norm_bins=true --cfg=$cfg \
		   --background_scheme=$em_scheme --draw_error_band=true \
		   ${XAXES[$k]} \
		   --x_axis_label="M_{#tau#tau} [GeV]" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" \
		   --extra_pad=${PADDING[$k]} --draw_ratio=true --signal_scheme=${SIGS[$k]}			 
		done
	done
done

ERAS=(
	'8TeV'
	)
ERA_LABELS=(
	'CMS Preliminary, 18.4 fb^{-1} at 8TeV'
	)

for ((j=0 ; j<${#ERAS[*]} ; j++))
do
	for ((i=0 ; i<${#POSTFITS[*]} ; i++))
	do
		for ((k=0 ; k<${#XAXES[*]} ; k++))
		do
			./bin/PostFitPlot2 --channel=tt --selection="no b-tag:8" --eras=${ERAS[$j]} \
			 --datacard_path=$tt_input/$mass/ --root_file_path=$tt_input/common/ --pulls_file=$tt_input/$mass/out/mlfit.txt \
			 --signal_mass=$mass --postfit=${POSTFITS[$i]} --mssm=true --log_y=${LOGS[$k]} \
			 --title_left="${ERA_LABELS[$j]}" $blind --norm_bins=true --cfg=$cfg \
			 --background_scheme=$tt_scheme --draw_error_band=true \
			 ${XAXES[$k]} \
			 --x_axis_label="M_{#tau#tau} [GeV]" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" \
			 --extra_pad=${PADDING[$k]} --draw_ratio=true --signal_scheme=${SIGS[$k]}

			./bin/PostFitPlot2 --channel=tt --selection="b-tag:9" --eras=${ERAS[$j]} \
			 --datacard_path=$tt_input/$mass/ --root_file_path=$tt_input/common/ --pulls_file=$tt_input/$mass/out/mlfit.txt \
			 --signal_mass=$mass --postfit=${POSTFITS[$i]} --mssm=true --log_y=${LOGS[$k]} \
			 --title_left="${ERA_LABELS[$j]}" $blind --norm_bins=true --cfg=$cfg \
			 --background_scheme=$tt_scheme --draw_error_band=true \
			 ${XAXES[$k]} \
			 --x_axis_label="M_{#tau#tau} [GeV]" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" \
			 --extra_pad=${PADDING[$k]} --draw_ratio=true --signal_scheme=${SIGS[$k]}	 
		done
	done
done



