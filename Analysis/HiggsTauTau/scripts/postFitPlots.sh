## Examples
./bin/PostFitPlot --channel=mt --selection="btag:9" --eras="8TeV" \
 --datacard_path=output/mssm-3hits-loose/mt/160/ \
 --root_file_path=output/mssm-3hits-loose/mt/common/ \
 --pulls_file=output/mssm-3hits-loose/mt/160/out/mlfit.txt \
 --signal_mass=160 --postfit=false --mssm=true --log_y=true --tanb=20 \
 --title_left="CMS Preliminary, #sqrt{s} = 8 TeV, L = 19.4 fb^{-1}" \
 --blind=true --x_blind_min=100 --x_blind_max=1500

