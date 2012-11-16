./bin/PrintRunYields \
	--title="Electron-Tau Pair Yields by Run, 2011" \
	--yield_file="output/runstats/Data_ElecTau_2011.root.runstats" \
	--lumi_file="output/runstats/lumi_data_2011.csv" \
	--min_lumi=20.0 \
	--coded_runs="Ele15_LooseIsoPFTau20,165088,167913:Ele15_TightIsoPFTau20,170249,173198:Ele18_MediumIsoPFTau20,173236,178380:Ele20_MediumIsoPFTau20,178420,180252" \
	--output_name="output/runstats/etau_2011.pdf"

./bin/PrintRunYields \
	--title="Muon-Tau Pair Yields by Run, 2011" \
	--yield_file="output/runstats/Data_MuTau_2011.root.runstats" \
	--lumi_file="output/runstats/lumi_data_2011.csv" \
	--min_lumi=20.0 \
	--coded_runs="IsoMu15_LooseIsoPFTau15,165088,173198:IsoMu15_LooseIsoPFTau20,173236,180252" \
	--output_name="output/runstats/mutau_2011.pdf"

./bin/PrintRunYields \
	--title="Electron-Tau Pair Yields by Run, 2012" \
	--yield_file="output/runstats/Data_ElecTau_2012.root.runstats" \
	--lumi_file="output/runstats/lumi_data_2012.csv" \
	--min_lumi=40.0 \
	--coded_runs="Ele20_LooseIsoPFTau20,190456,193751:Ele22_LooseIsoPFTau20,193752,203002" \
	--output_name="output/runstats/etau_2012.pdf"

./bin/PrintRunYields \
	--title="Muon-Tau Pair Yields by Run, 2012" \
	--yield_file="output/runstats/Data_MuTau_2012.root.runstats" \
	--lumi_file="output/runstats/lumi_data_2012.csv" \
	--min_lumi=40.0 \
	--coded_runs="IsoMu18_LooseIsoPFTau20,190456,193751:IsoMu17_LooseIsoPFTau20,193752,203002" \
	--output_name="output/runstats/mutau_2012.pdf"

