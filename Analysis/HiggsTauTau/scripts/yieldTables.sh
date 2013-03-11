./bin/YieldTable --channel=mt --columns="0-Jet:0+1,1-Jet low \$p_{T}\$:2,1-Jet high \$p_{T}\$:3,VBF:5" --eras="7TeV,8TeV" \
--datacard_path=bin-by-bin/cmb/125/ --root_file_path=bin-by-bin/cmb/common/ --pulls_file=bin-by-bin/cmb/125/out/mlfit.txt \
--signal_mass=125 --postfit=false

./bin/YieldTable --channel=et --columns="0-Jet:0+1,1-Jet low \$p_{T}\$:2,1-Jet high \$p_{T}\$:3,VBF:5" --eras="7TeV,8TeV" \
--datacard_path=bin-by-bin/cmb/125/ --root_file_path=bin-by-bin/cmb/common/ --pulls_file=bin-by-bin/cmb/125/out/mlfit.txt \
--signal_mass=125 --postfit=true

./bin/YieldTable --channel=em --columns="0-Jet:0+1,1-Jet low \$p_{T}\$:2,1-Jet high \$p_{T}\$:3,VBF:5" --eras="7TeV,8TeV" \
--datacard_path=bin-by-bin/cmb/125/ --root_file_path=bin-by-bin/cmb/common/ --pulls_file=bin-by-bin/cmb/125/out/mlfit.txt \
--signal_mass=125 --postfit=true
