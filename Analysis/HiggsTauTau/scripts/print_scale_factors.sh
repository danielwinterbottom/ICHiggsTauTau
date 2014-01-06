
splusb_pulls="true"

#./bin/PrintShifts --channel=et --selection="btag:9" --eras="8TeV" --splusb_pulls=$splusb_pulls \
#    --datacard_path=output/mloutput_0313-bbb-full/et/160 --mssm=true \
#    --root_file_path=output/mloutput_0313-bbb-full/et/common \
#    --pulls_file=output/mloutput_0313-bbb-full/et/160/out/mlfit.txt --signal_mass=160
#
#./bin/PrintShifts --channel=mt --selection="btag:9" --eras="8TeV" --splusb_pulls=$splusb_pulls \
#    --datacard_path=output/mloutput_0313-bbb-full/mt/160 --mssm=true \
#    --root_file_path=output/mloutput_0313-bbb-full/mt/common \
#    --pulls_file=output/mloutput_0313-bbb-full/mt/160/out/mlfit.txt --signal_mass=160
#
#./bin/PrintShifts --channel=em --selection="btag:9" --eras="8TeV" --splusb_pulls=$splusb_pulls \
#    --datacard_path=output/mloutput_0313-bbb-full/em/160 --mssm=true \
#    --root_file_path=output/mloutput_0313-bbb-full/em/common \
#    --pulls_file=output/mloutput_0313-bbb-full/em/160/out/mlfit.txt --signal_mass=160
#
#./bin/PrintShifts --channel=et --selection="btag:9" --eras="7TeV" --splusb_pulls=$splusb_pulls \
#    --datacard_path=output/mloutput_0313-bbb-full/et/160 --mssm=true \
#    --root_file_path=output/mloutput_0313-bbb-full/et/common \
#    --pulls_file=output/mloutput_0313-bbb-full/et/160/out/mlfit.txt --signal_mass=160
#
#./bin/PrintShifts --channel=mt --selection="btag:9" --eras="7TeV" --splusb_pulls=$splusb_pulls \
#    --datacard_path=output/mloutput_0313-bbb-full/mt/160 --mssm=true \
#    --root_file_path=output/mloutput_0313-bbb-full/mt/common \
#    --pulls_file=output/mloutput_0313-bbb-full/mt/160/out/mlfit.txt --signal_mass=160
#
#./bin/PrintShifts --channel=em --selection="btag:9" --eras="7TeV" --splusb_pulls=$splusb_pulls \
#    --datacard_path=output/mloutput_0313-bbb-full/em/160 --mssm=true \
#    --root_file_path=output/mloutput_0313-bbb-full/em/common \
#    --pulls_file=output/mloutput_0313-bbb-full/em/160/out/mlfit.txt --signal_mass=160

./bin/PrintShifts --channel=et --selection="inclusive:0" --eras="8TeV" \
    --datacard_path=Paper-LIMITS/control-plots/et/125 \
      --root_file_path=Paper-LIMITS/control-plots/et/common \
        --pulls_file=Paper-LIMITS/control-plots/et/125/out/mlfit.txt --signal_mass=125

./bin/PrintShifts --channel=et --selection="1jet:3" --eras="8TeV" \
    --datacard_path=Paper-LIMITS/control-plots/et/125 \
      --root_file_path=Paper-LIMITS/control-plots/et/common \
        --pulls_file=Paper-LIMITS/control-plots/et/125/out/mlfit.txt --signal_mass=125

./bin/PrintShifts --channel=et --selection="twojet:6" --eras="8TeV" \
    --datacard_path=Paper-LIMITS/control-plots/et/125 \
      --root_file_path=Paper-LIMITS/control-plots/et/common \
        --pulls_file=Paper-LIMITS/control-plots/et/125/out/mlfit.txt --signal_mass=125

./bin/PrintShifts --channel=mt --selection="inclusive:0" --eras="8TeV" \
    --datacard_path=Paper-LIMITS/control-plots/mt/125 \
      --root_file_path=Paper-LIMITS/control-plots/mt/common \
        --pulls_file=Paper-LIMITS/control-plots/mt/125/out/mlfit.txt --signal_mass=125

./bin/PrintShifts --channel=mt --selection="1jet:3" --eras="8TeV" \
    --datacard_path=Paper-LIMITS/control-plots/mt/125 \
      --root_file_path=Paper-LIMITS/control-plots/mt/common \
        --pulls_file=Paper-LIMITS/control-plots/mt/125/out/mlfit.txt --signal_mass=125

./bin/PrintShifts --channel=mt --selection="twojet:6" --eras="8TeV" \
    --datacard_path=Paper-LIMITS/control-plots/mt/125 \
      --root_file_path=Paper-LIMITS/control-plots/mt/common \
        --pulls_file=Paper-LIMITS/control-plots/mt/125/out/mlfit.txt --signal_mass=125

./bin/PrintShifts --channel=em --selection="inclusive:0" --eras="8TeV" \
    --datacard_path=Paper-LIMITS/control-plots/em/125 \
      --root_file_path=Paper-LIMITS/control-plots/em/common \
        --pulls_file=Paper-LIMITS/control-plots/em/125/out/mlfit.txt --signal_mass=125

./bin/PrintShifts --channel=em --selection="1jet:2" --eras="8TeV" \
    --datacard_path=Paper-LIMITS/control-plots/em/125 \
      --root_file_path=Paper-LIMITS/control-plots/em/common \
        --pulls_file=Paper-LIMITS/control-plots/em/125/out/mlfit.txt --signal_mass=125

./bin/PrintShifts --channel=em --selection="twojet:4" --eras="8TeV" \
    --datacard_path=Paper-LIMITS/control-plots/em/125 \
      --root_file_path=Paper-LIMITS/control-plots/em/common \
        --pulls_file=Paper-LIMITS/control-plots/em/125/out/mlfit.txt --signal_mass=125

./bin/PrintShifts --channel=et --selection="inclusive:0" --eras="7TeV" \
    --datacard_path=Paper-LIMITS/control-plots/et/125 \
      --root_file_path=Paper-LIMITS/control-plots/et/common \
        --pulls_file=Paper-LIMITS/control-plots/et/125/out/mlfit.txt --signal_mass=125

./bin/PrintShifts --channel=et --selection="1jet:3" --eras="7TeV" \
    --datacard_path=Paper-LIMITS/control-plots/et/125 \
      --root_file_path=Paper-LIMITS/control-plots/et/common \
        --pulls_file=Paper-LIMITS/control-plots/et/125/out/mlfit.txt --signal_mass=125

./bin/PrintShifts --channel=et --selection="twojet:6" --eras="7TeV" \
    --datacard_path=Paper-LIMITS/control-plots/et/125 \
      --root_file_path=Paper-LIMITS/control-plots/et/common \
        --pulls_file=Paper-LIMITS/control-plots/et/125/out/mlfit.txt --signal_mass=125

./bin/PrintShifts --channel=mt --selection="inclusive:0" --eras="7TeV" \
    --datacard_path=Paper-LIMITS/control-plots/mt/125 \
      --root_file_path=Paper-LIMITS/control-plots/mt/common \
        --pulls_file=Paper-LIMITS/control-plots/mt/125/out/mlfit.txt --signal_mass=125

./bin/PrintShifts --channel=mt --selection="1jet:3" --eras="7TeV" \
    --datacard_path=Paper-LIMITS/control-plots/mt/125 \
      --root_file_path=Paper-LIMITS/control-plots/mt/common \
        --pulls_file=Paper-LIMITS/control-plots/mt/125/out/mlfit.txt --signal_mass=125

./bin/PrintShifts --channel=mt --selection="twojet:6" --eras="7TeV" \
    --datacard_path=Paper-LIMITS/control-plots/mt/125 \
      --root_file_path=Paper-LIMITS/control-plots/mt/common \
        --pulls_file=Paper-LIMITS/control-plots/mt/125/out/mlfit.txt --signal_mass=125

./bin/PrintShifts --channel=em --selection="inclusive:0" --eras="7TeV" \
    --datacard_path=Paper-LIMITS/control-plots/em/125 \
      --root_file_path=Paper-LIMITS/control-plots/em/common \
        --pulls_file=Paper-LIMITS/control-plots/em/125/out/mlfit.txt --signal_mass=125

./bin/PrintShifts --channel=em --selection="1jet:2" --eras="7TeV" \
    --datacard_path=Paper-LIMITS/control-plots/em/125 \
      --root_file_path=Paper-LIMITS/control-plots/em/common \
        --pulls_file=Paper-LIMITS/control-plots/em/125/out/mlfit.txt --signal_mass=125

./bin/PrintShifts --channel=em --selection="twojet:4" --eras="7TeV" \
    --datacard_path=Paper-LIMITS/control-plots/em/125 \
      --root_file_path=Paper-LIMITS/control-plots/em/common \
        --pulls_file=Paper-LIMITS/control-plots/em/125/out/mlfit.txt --signal_mass=125
