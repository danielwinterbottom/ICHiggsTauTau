# ./bin/YieldTable --channel=mt --columns="0-Jet:0+1,1-Jet low \$p_{T}\$:2,1-Jet high \$p_{T}\$:3,VBF:5" --eras="7TeV,8TeV" \
# --datacard_path=bin-by-bin/cmb/125/ --root_file_path=bin-by-bin/cmb/common/ --pulls_file=bin-by-bin/cmb/125/out/mlfit.txt \
# --signal_mass=125 --postfit=true

# ./bin/YieldTable --channel=et --columns="0-Jet:0+1,1-Jet low \$p_{T}\$:2,1-Jet high \$p_{T}\$:3,VBF:5" --eras="7TeV,8TeV" \
# --datacard_path=bin-by-bin/cmb/125/ --root_file_path=bin-by-bin/cmb/common/ --pulls_file=bin-by-bin/cmb/125/out/mlfit.txt \
# --signal_mass=125 --postfit=true

# ./bin/YieldTable --channel=em --columns="0-Jet:0+1,1-Jet low \$p_{T}\$:2,1-Jet high \$p_{T}\$:3,VBF:5" --eras="7TeV,8TeV" \
# --datacard_path=bin-by-bin/cmb/125/ --root_file_path=bin-by-bin/cmb/common/ --pulls_file=bin-by-bin/cmb/125/out/mlfit.txt \
# --signal_mass=125 --postfit=true

./bin/PostFitPlot --channel=mt --selection="no btag:8" --eras="8TeV" \
--datacard_path=output/mssm-3hits-loose/mt/800/ --root_file_path=output/mssm-3hits-loose/mt/common/ --pulls_file=bin-by-bin/cmb/125/out/mlfit.txt \
--signal_mass=500 --postfit=false --mssm=true --log_y=true --tanb=90

./bin/PostFitPlot --channel=mt --selection="btag:9" --eras="8TeV" \
--datacard_path=output/mssm-3hits-loose/mt/160/ --root_file_path=output/mssm-3hits-loose/mt/common/ --pulls_file=bin-by-bin/cmb/125/out/mlfit.txt \
--signal_mass=160 --postfit=false --mssm=true --log_y=true --tanb=20 \
--title_left="CMS Preliminary, #sqrt{s} = 8 TeV, L = 19.4 fb^{-1}" \
--blind=true --x_blind_min=100 --x_blind_max=1500

./bin/PostFitPlot --channel=mt --selection="0-Jet:0+1" --eras="7TeV,8TeV" \
--datacard_path=bin-by-bin/cmb/125/ --root_file_path=bin-by-bin/cmb/common/ --pulls_file=bin-by-bin/cmb/125/out/mlfit.txt \
--signal_mass=125 --postfit=false

./bin/PostFitPlot --channel=mt --selection="1-Jet Low p_{T}:2" --eras="7TeV,8TeV" \
--datacard_path=bin-by-bin/cmb/125/ --root_file_path=bin-by-bin/cmb/common/ --pulls_file=bin-by-bin/cmb/125/out/mlfit.txt \
--signal_mass=125 --postfit=false

./bin/PostFitPlot --channel=mt --selection="1-Jet High p_{T}:3" --eras="7TeV,8TeV" \
--datacard_path=bin-by-bin/cmb/125/ --root_file_path=bin-by-bin/cmb/common/ --pulls_file=bin-by-bin/cmb/125/out/mlfit.txt \
--signal_mass=125 --postfit=false

./bin/PostFitPlot --channel=mt --selection="VBF:5" --eras="7TeV,8TeV" \
--datacard_path=bin-by-bin/cmb/125/ --root_file_path=bin-by-bin/cmb/common/ --pulls_file=bin-by-bin/cmb/125/out/mlfit.txt \
--signal_mass=125 --postfit=false

./bin/PostFitPlot --channel=et --selection="0-Jet:0+1" --eras="7TeV,8TeV" \
--datacard_path=bin-by-bin/cmb/125/ --root_file_path=bin-by-bin/cmb/common/ --pulls_file=bin-by-bin/cmb/125/out/mlfit.txt \
--signal_mass=125 --postfit=false

./bin/PostFitPlot --channel=et --selection="1-Jet Low p_{T}:2" --eras="7TeV,8TeV" \
--datacard_path=bin-by-bin/cmb/125/ --root_file_path=bin-by-bin/cmb/common/ --pulls_file=bin-by-bin/cmb/125/out/mlfit.txt \
--signal_mass=125 --postfit=false

./bin/PostFitPlot --channel=et --selection="1-Jet High p_{T}:3" --eras="7TeV,8TeV" \
--datacard_path=bin-by-bin/cmb/125/ --root_file_path=bin-by-bin/cmb/common/ --pulls_file=bin-by-bin/cmb/125/out/mlfit.txt \
--signal_mass=125 --postfit=false

./bin/PostFitPlot --channel=et --selection="VBF:5" --eras="7TeV,8TeV" \
--datacard_path=bin-by-bin/cmb/125/ --root_file_path=bin-by-bin/cmb/common/ --pulls_file=bin-by-bin/cmb/125/out/mlfit.txt \
--signal_mass=125 --postfit=false
