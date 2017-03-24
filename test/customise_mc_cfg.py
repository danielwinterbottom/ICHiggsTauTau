import FWCore.ParameterSet.Config as cms

from hlt_VBF_noCSVL1em import process # adapt to your case


process.source.fileNames          = cms.untracked.vstring([
    # put here your MINIAOD files
	'root://cms-xrd-global.cern.ch//store/mc/RunIISummer16MiniAODv2/VBFHToTauTau_M125_13TeV_powheg_pythia8/MINIAODSIM/FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/50000/0A171627-64B9-E611-9270-0025904C7DFA.root',
])

process.source.secondaryFileNames = cms.untracked.vstring([
    # put here your RAW parent files
	'root://cms-xrd-global.cern.ch//store/mc/RunIISummer16DR80/VBFHToTauTau_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/130000/2E22E1F4-52A3-E611-944A-001E674FAF23.root',
	'root://cms-xrd-global.cern.ch//store/mc/RunIISummer16DR80/VBFHToTauTau_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/130000/32AF1C0E-54A3-E611-B3F6-001E674FB577.root',
	'root://cms-xrd-global.cern.ch//store/mc/RunIISummer16DR80/VBFHToTauTau_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/130000/48648043-FFA2-E611-9A78-001E67A41FCC.root',
	'root://cms-xrd-global.cern.ch//store/mc/RunIISummer16DR80/VBFHToTauTau_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/130000/5EAE1965-57A3-E611-84A4-001E674570BC.root',
	'root://cms-xrd-global.cern.ch//store/mc/RunIISummer16DR80/VBFHToTauTau_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/130000/623AB6BB-56A3-E611-B9BD-001E674FB4D7.root',
	'root://cms-xrd-global.cern.ch//store/mc/RunIISummer16DR80/VBFHToTauTau_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/130000/648C340C-4DA3-E611-BABB-001E674577CE.root',
	'root://cms-xrd-global.cern.ch//store/mc/RunIISummer16DR80/VBFHToTauTau_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/130000/6E76F23F-53A3-E611-A0E1-0242AC130005.root',
	'root://cms-xrd-global.cern.ch//store/mc/RunIISummer16DR80/VBFHToTauTau_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/130000/925F5164-F0A2-E611-ADC2-001E674FB847.root',
	'root://cms-xrd-global.cern.ch//store/mc/RunIISummer16DR80/VBFHToTauTau_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/130000/A6C29822-5AA3-E611-886E-0CC47A546E5E.root',
	'root://cms-xrd-global.cern.ch//store/mc/RunIISummer16DR80/VBFHToTauTau_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/130000/EEB6C112-00A3-E611-9C2B-0CC47A537688.root',
	'root://cms-xrd-global.cern.ch//store/mc/RunIISummer16DR80/VBFHToTauTau_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/130000/56BDE9A2-10A3-E611-B3E7-001E674FBEC3.root',
	'root://cms-xrd-global.cern.ch//store/mc/RunIISummer16DR80/VBFHToTauTau_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/130000/58395DBD-0EA3-E611-B2D1-001E674FBEC3.root',
	'root://cms-xrd-global.cern.ch//store/mc/RunIISummer16DR80/VBFHToTauTau_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/130000/942CC85A-1DA3-E611-9968-001E6739842B.root',
	'root://cms-xrd-global.cern.ch//store/mc/RunIISummer16DR80/VBFHToTauTau_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/130000/94E0E9BB-22A3-E611-95C9-001E674FB577.root',
	'root://cms-xrd-global.cern.ch//store/mc/RunIISummer16DR80/VBFHToTauTau_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/130000/BC0A0E7E-0DA3-E611-A183-001E674570BC.root',
	'root://cms-xrd-global.cern.ch//store/mc/RunIISummer16DR80/VBFHToTauTau_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/130000/BEB0AA04-10A3-E611-98A3-0CC47A537688.root',
	'root://cms-xrd-global.cern.ch//store/mc/RunIISummer16DR80/VBFHToTauTau_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/130000/D81D814B-0CA3-E611-962B-001E674FB577.root',
	'root://cms-xrd-global.cern.ch//store/mc/RunIISummer16DR80/VBFHToTauTau_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/130000/E2AAC302-21A3-E611-9DEF-001E674FBEC3.root',
	'root://cms-xrd-global.cern.ch//store/mc/RunIISummer16DR80/VBFHToTauTau_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/130000/E46B3369-0FA3-E611-8177-001E674570BC.root',
	'root://cms-xrd-global.cern.ch//store/mc/RunIISummer16DR80/VBFHToTauTau_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/130000/000054DC-16A3-E611-802D-001E674FB577.root',
	'root://cms-xrd-global.cern.ch//store/mc/RunIISummer16DR80/VBFHToTauTau_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/130000/0AC39578-25A3-E611-B8E7-001E674FBEC3.root',
	'root://cms-xrd-global.cern.ch//store/mc/RunIISummer16DR80/VBFHToTauTau_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/130000/366FE610-19A3-E611-92AC-001E674FB207.root',
	'root://cms-xrd-global.cern.ch//store/mc/RunIISummer16DR80/VBFHToTauTau_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/130000/5E90EFD1-1EA3-E611-8666-001E67444C3B.root',
	'root://cms-xrd-global.cern.ch//store/mc/RunIISummer16DR80/VBFHToTauTau_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/130000/723E09EF-07A3-E611-AD47-001E674FB149.root',
	'root://cms-xrd-global.cern.ch//store/mc/RunIISummer16DR80/VBFHToTauTau_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/130000/80B0B681-1AA3-E611-A5F0-001E674FB847.root',
	'root://cms-xrd-global.cern.ch//store/mc/RunIISummer16DR80/VBFHToTauTau_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/130000/9AFF6C31-23A3-E611-B80F-001E674FBEC3.root',
	'root://cms-xrd-global.cern.ch//store/mc/RunIISummer16DR80/VBFHToTauTau_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/130000/E2F3F694-21A3-E611-BD9F-001E67444C3B.root',
	'root://cms-xrd-global.cern.ch//store/mc/RunIISummer16DR80/VBFHToTauTau_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/130000/E2FB0930-1BA3-E611-B93C-001E674FB207.root',
	'root://cms-xrd-global.cern.ch//store/mc/RunIISummer16DR80/VBFHToTauTau_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/130000/281FC68A-C3A2-E611-9446-001E674FBFC2.root',
	'root://cms-xrd-global.cern.ch//store/mc/RunIISummer16DR80/VBFHToTauTau_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/130000/4EC16278-DFA2-E611-A53E-001E674DA347.root',
	'root://cms-xrd-global.cern.ch//store/mc/RunIISummer16DR80/VBFHToTauTau_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/130000/6AB14CE3-D8A2-E611-95BB-001E67398313.root',
	'root://cms-xrd-global.cern.ch//store/mc/RunIISummer16DR80/VBFHToTauTau_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/130000/74DEDCFC-E3A2-E611-95B2-001E674DA347.root',
	'root://cms-xrd-global.cern.ch//store/mc/RunIISummer16DR80/VBFHToTauTau_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/130000/7838029A-DCA2-E611-A29C-001E674FB24D.root',
	'root://cms-xrd-global.cern.ch//store/mc/RunIISummer16DR80/VBFHToTauTau_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/130000/7E50D2BC-E4A2-E611-8F43-0CC47A537688.root',
	'root://cms-xrd-global.cern.ch//store/mc/RunIISummer16DR80/VBFHToTauTau_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/130000/802A5A76-E0A2-E611-875A-001E674FB847.root',
	'root://cms-xrd-global.cern.ch//store/mc/RunIISummer16DR80/VBFHToTauTau_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/130000/8077E3E5-DBA2-E611-BFC0-001E67398313.root',
	'root://cms-xrd-global.cern.ch//store/mc/RunIISummer16DR80/VBFHToTauTau_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/130000/929D9778-DFA2-E611-999D-001E67444128.root',
	'root://cms-xrd-global.cern.ch//store/mc/RunIISummer16DR80/VBFHToTauTau_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/130000/C6B94236-D6A2-E611-BEE8-001E674FB149.root',
	'root://cms-xrd-global.cern.ch//store/mc/RunIISummer16DR80/VBFHToTauTau_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/130000/DECB8ACA-DEA2-E611-85D5-001E673987C8.root',
	'root://cms-xrd-global.cern.ch//store/mc/RunIISummer16DR80/VBFHToTauTau_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/130000/EA051BB3-E3A2-E611-93DA-001E674FAF23.root',
	'root://cms-xrd-global.cern.ch//store/mc/RunIISummer16DR80/VBFHToTauTau_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/60000/025B23B6-98A2-E611-A564-0025905A60DA.root',
	'root://cms-xrd-global.cern.ch//store/mc/RunIISummer16DR80/VBFHToTauTau_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/60000/147E2D52-9AA2-E611-99E6-0025905B85B8.root',
	'root://cms-xrd-global.cern.ch//store/mc/RunIISummer16DR80/VBFHToTauTau_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/60000/4C340821-99A2-E611-9B42-0CC47A78A3EE.root',
	'root://cms-xrd-global.cern.ch//store/mc/RunIISummer16DR80/VBFHToTauTau_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/60000/6264D7DA-97A2-E611-884D-0025905A608E.root',
	'root://cms-xrd-global.cern.ch//store/mc/RunIISummer16DR80/VBFHToTauTau_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/60000/6A8794B4-99A2-E611-A8FE-0025905A60BE.root',
	'root://cms-xrd-global.cern.ch//store/mc/RunIISummer16DR80/VBFHToTauTau_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/60000/6CEF33DA-97A2-E611-9368-0025905A608C.root',
	'root://cms-xrd-global.cern.ch//store/mc/RunIISummer16DR80/VBFHToTauTau_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/60000/74CA8135-98A2-E611-9972-0CC47A7C3444.root',
	'root://cms-xrd-global.cern.ch//store/mc/RunIISummer16DR80/VBFHToTauTau_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/60000/80130B39-98A2-E611-B217-0025905A609E.root',
	'root://cms-xrd-global.cern.ch//store/mc/RunIISummer16DR80/VBFHToTauTau_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/60000/8465A1BF-98A2-E611-A528-0025905A6136.root',
	'root://cms-xrd-global.cern.ch//store/mc/RunIISummer16DR80/VBFHToTauTau_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/60000/8635C8D6-97A2-E611-9E30-0CC47A7C351E.root',
	'root://cms-xrd-global.cern.ch//store/mc/RunIISummer16DR80/VBFHToTauTau_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/60000/922C3DD7-97A2-E611-AE92-0CC47A4C8E86.root',
	'root://cms-xrd-global.cern.ch//store/mc/RunIISummer16DR80/VBFHToTauTau_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/60000/9A37C1D6-97A2-E611-B841-0CC47A7C3444.root',
	'root://cms-xrd-global.cern.ch//store/mc/RunIISummer16DR80/VBFHToTauTau_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/60000/A0D75FBD-99A2-E611-AFA8-0025905A60DA.root',
	'root://cms-xrd-global.cern.ch//store/mc/RunIISummer16DR80/VBFHToTauTau_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/60000/A47DCCDA-97A2-E611-A5D3-0025905A6092.root',
	'root://cms-xrd-global.cern.ch//store/mc/RunIISummer16DR80/VBFHToTauTau_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/60000/A6F31D72-9BA2-E611-9680-0CC47A745298.root',
	'root://cms-xrd-global.cern.ch//store/mc/RunIISummer16DR80/VBFHToTauTau_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/60000/A83D88B2-9AA2-E611-8EF8-0025905A612C.root',
	'root://cms-xrd-global.cern.ch//store/mc/RunIISummer16DR80/VBFHToTauTau_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/60000/B051D315-99A2-E611-82F3-0CC47A4C8E38.root',
	'root://cms-xrd-global.cern.ch//store/mc/RunIISummer16DR80/VBFHToTauTau_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/60000/B4B9DF50-9AA2-E611-A4B1-0CC47A4D75EE.root',
	'root://cms-xrd-global.cern.ch//store/mc/RunIISummer16DR80/VBFHToTauTau_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/60000/B6E02FAF-9AA2-E611-ADFA-0CC47A4D75EE.root',
	'root://cms-xrd-global.cern.ch//store/mc/RunIISummer16DR80/VBFHToTauTau_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/60000/D21DF23E-9AA2-E611-9B16-0025905A609A.root',
	'root://cms-xrd-global.cern.ch//store/mc/RunIISummer16DR80/VBFHToTauTau_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/60000/E236CD15-9CA2-E611-8E5F-0025905A6084.root',
	'root://cms-xrd-global.cern.ch//store/mc/RunIISummer16DR80/VBFHToTauTau_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/60000/0248A7FB-91A2-E611-A2B5-0CC47A78A42E.root',
	'root://cms-xrd-global.cern.ch//store/mc/RunIISummer16DR80/VBFHToTauTau_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/60000/0A7BAA9E-92A2-E611-95F8-0025905A60B0.root',
	'root://cms-xrd-global.cern.ch//store/mc/RunIISummer16DR80/VBFHToTauTau_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/60000/1C069B70-92A2-E611-9126-0025905A606A.root',
	'root://cms-xrd-global.cern.ch//store/mc/RunIISummer16DR80/VBFHToTauTau_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/60000/22EBD932-92A2-E611-9DDE-003048FFD798.root',
	'root://cms-xrd-global.cern.ch//store/mc/RunIISummer16DR80/VBFHToTauTau_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/60000/32581071-91A2-E611-848F-0CC47A78A42E.root',
	'root://cms-xrd-global.cern.ch//store/mc/RunIISummer16DR80/VBFHToTauTau_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/60000/36BE7E92-92A2-E611-815C-0025905B859A.root',
	'root://cms-xrd-global.cern.ch//store/mc/RunIISummer16DR80/VBFHToTauTau_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/60000/38BD0A6A-92A2-E611-A798-0025905A60B6.root',
	'root://cms-xrd-global.cern.ch//store/mc/RunIISummer16DR80/VBFHToTauTau_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/60000/5C0FB31D-93A2-E611-B648-0CC47A4D7604.root',
	'root://cms-xrd-global.cern.ch//store/mc/RunIISummer16DR80/VBFHToTauTau_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/60000/66DBA0F7-90A2-E611-86B9-0025905A60DA.root',
	'root://cms-xrd-global.cern.ch//store/mc/RunIISummer16DR80/VBFHToTauTau_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/60000/76EA1A08-92A2-E611-989D-0025905A60FE.root',
	'root://cms-xrd-global.cern.ch//store/mc/RunIISummer16DR80/VBFHToTauTau_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/60000/86EC39EA-92A2-E611-9EB3-0CC47A78A42E.root',
	'root://cms-xrd-global.cern.ch//store/mc/RunIISummer16DR80/VBFHToTauTau_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/60000/8E5B25EC-92A2-E611-8CD9-0025905A6136.root',
	'root://cms-xrd-global.cern.ch//store/mc/RunIISummer16DR80/VBFHToTauTau_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/60000/920DDA6B-94A2-E611-9AB9-0CC47A4C8EB6.root',
	'root://cms-xrd-global.cern.ch//store/mc/RunIISummer16DR80/VBFHToTauTau_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/60000/A6EBDBBE-93A2-E611-B9DC-0CC47A78A42E.root',
	'root://cms-xrd-global.cern.ch//store/mc/RunIISummer16DR80/VBFHToTauTau_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/60000/B4437D3B-92A2-E611-9036-0025905A60DE.root',
	'root://cms-xrd-global.cern.ch//store/mc/RunIISummer16DR80/VBFHToTauTau_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/60000/BC64A3EB-92A2-E611-A9F2-0025905A6138.root',
	'root://cms-xrd-global.cern.ch//store/mc/RunIISummer16DR80/VBFHToTauTau_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/60000/C2A3D2F2-8FA2-E611-959F-0CC47A78A458.root',
	'root://cms-xrd-global.cern.ch//store/mc/RunIISummer16DR80/VBFHToTauTau_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/60000/2A09BEA7-9AA2-E611-A748-002590FD5A72.root',
	'root://cms-xrd-global.cern.ch//store/mc/RunIISummer16DR80/VBFHToTauTau_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/60000/306F2158-9DA2-E611-A1D6-00304867FE5F.root',
	'root://cms-xrd-global.cern.ch//store/mc/RunIISummer16DR80/VBFHToTauTau_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/60000/3A82E6A8-9CA2-E611-8ADF-0CC47A57CC26.root',
	'root://cms-xrd-global.cern.ch//store/mc/RunIISummer16DR80/VBFHToTauTau_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/60000/6242A56F-9DA2-E611-A6C6-0025907DE266.root',
	'root://cms-xrd-global.cern.ch//store/mc/RunIISummer16DR80/VBFHToTauTau_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/60000/9C2835BF-98A2-E611-95ED-002590D9D8BA.root',
	'root://cms-xrd-global.cern.ch//store/mc/RunIISummer16DR80/VBFHToTauTau_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/60000/C6EDF475-A1A2-E611-982C-002590D9D8A4.root',
	'root://cms-xrd-global.cern.ch//store/mc/RunIISummer16DR80/VBFHToTauTau_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/60000/C888AAE5-9DA2-E611-B2C1-0CC47A57CC26.root',
	'root://cms-xrd-global.cern.ch//store/mc/RunIISummer16DR80/VBFHToTauTau_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/60000/E44D915D-9FA2-E611-B6DE-002590D9D8BC.root',
	'root://cms-xrd-global.cern.ch//store/mc/RunIISummer16DR80/VBFHToTauTau_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/60000/E493F725-99A2-E611-80A4-0025901FB438.root',
	'root://cms-xrd-global.cern.ch//store/mc/RunIISummer16DR80/VBFHToTauTau_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/60000/00626DD1-9CA2-E611-B189-0025905A609A.root',
	'root://cms-xrd-global.cern.ch//store/mc/RunIISummer16DR80/VBFHToTauTau_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/60000/2820CF79-9DA2-E611-BC7C-0025905A6092.root',
	'root://cms-xrd-global.cern.ch//store/mc/RunIISummer16DR80/VBFHToTauTau_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/60000/28D06530-9BA2-E611-A698-0CC47A78A3EE.root',
	'root://cms-xrd-global.cern.ch//store/mc/RunIISummer16DR80/VBFHToTauTau_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/60000/708FE935-9CA2-E611-BCC3-0025905A6118.root',
	'root://cms-xrd-global.cern.ch//store/mc/RunIISummer16DR80/VBFHToTauTau_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/60000/7A22F212-9CA2-E611-9749-0025905A48F2.root',
	'root://cms-xrd-global.cern.ch//store/mc/RunIISummer16DR80/VBFHToTauTau_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/60000/8465A1BF-98A2-E611-A528-0025905A6136.root',
	'root://cms-xrd-global.cern.ch//store/mc/RunIISummer16DR80/VBFHToTauTau_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/60000/AA67D777-9BA2-E611-930F-0025905A611C.root',
	'root://cms-xrd-global.cern.ch//store/mc/RunIISummer16DR80/VBFHToTauTau_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/60000/AA6F6C76-9BA2-E611-9C04-0025905A48F2.root',
	'root://cms-xrd-global.cern.ch//store/mc/RunIISummer16DR80/VBFHToTauTau_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/60000/B42710C4-9CA2-E611-8420-0025905A611C.root',
	'root://cms-xrd-global.cern.ch//store/mc/RunIISummer16DR80/VBFHToTauTau_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/60000/C46F88B4-9AA2-E611-8FFA-0025905A60A6.root',
	'root://cms-xrd-global.cern.ch//store/mc/RunIISummer16DR80/VBFHToTauTau_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/60000/D8B2CED3-9CA2-E611-A72F-0025905B85BA.root',
	'root://cms-xrd-global.cern.ch//store/mc/RunIISummer16DR80/VBFHToTauTau_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/60000/DA7C29D0-9CA2-E611-90C5-0CC47A4C8E1C.root',
	'root://cms-xrd-global.cern.ch//store/mc/RunIISummer16DR80/VBFHToTauTau_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/60000/FAF465FF-9DA2-E611-A06F-0025905A609A.root',
	'root://cms-xrd-global.cern.ch//store/mc/RunIISummer16DR80/VBFHToTauTau_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/60000/080A19EA-96A2-E611-9391-0025905A6138.root',
	'root://cms-xrd-global.cern.ch//store/mc/RunIISummer16DR80/VBFHToTauTau_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/60000/0AEB7936-98A2-E611-B393-0CC47A4C8EB6.root',
	'root://cms-xrd-global.cern.ch//store/mc/RunIISummer16DR80/VBFHToTauTau_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/60000/0C533261-96A2-E611-9F6C-0CC47A4D7604.root',
	'root://cms-xrd-global.cern.ch//store/mc/RunIISummer16DR80/VBFHToTauTau_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/60000/10CA0D71-94A2-E611-A75F-0025905A6136.root',
	'root://cms-xrd-global.cern.ch//store/mc/RunIISummer16DR80/VBFHToTauTau_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/60000/16A44A75-96A2-E611-96F7-0025905A609A.root',
	'root://cms-xrd-global.cern.ch//store/mc/RunIISummer16DR80/VBFHToTauTau_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/60000/36A0FC43-95A2-E611-9027-0025905A60CE.root',
	'root://cms-xrd-global.cern.ch//store/mc/RunIISummer16DR80/VBFHToTauTau_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/60000/4CF5DD56-9AA2-E611-853F-0025905A6068.root',
	'root://cms-xrd-global.cern.ch//store/mc/RunIISummer16DR80/VBFHToTauTau_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/60000/54CBA9F4-93A2-E611-BF70-0CC47A7C34A6.root',
	'root://cms-xrd-global.cern.ch//store/mc/RunIISummer16DR80/VBFHToTauTau_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/60000/5A5C227F-95A2-E611-AE5E-0025905A60A6.root',
	'root://cms-xrd-global.cern.ch//store/mc/RunIISummer16DR80/VBFHToTauTau_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/60000/66AF3D4A-96A2-E611-A2BF-0025905B85EE.root',
	'root://cms-xrd-global.cern.ch//store/mc/RunIISummer16DR80/VBFHToTauTau_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/60000/74CA8135-98A2-E611-9972-0CC47A7C3444.root',
	'root://cms-xrd-global.cern.ch//store/mc/RunIISummer16DR80/VBFHToTauTau_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/60000/8635C8D6-97A2-E611-9E30-0CC47A7C351E.root',
	'root://cms-xrd-global.cern.ch//store/mc/RunIISummer16DR80/VBFHToTauTau_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/60000/8E88202D-93A2-E611-92D3-0025905B8586.root',
	'root://cms-xrd-global.cern.ch//store/mc/RunIISummer16DR80/VBFHToTauTau_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/60000/920DDA6B-94A2-E611-9AB9-0CC47A4C8EB6.root',
	'root://cms-xrd-global.cern.ch//store/mc/RunIISummer16DR80/VBFHToTauTau_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/60000/9A37C1D6-97A2-E611-B841-0CC47A7C3444.root',
	'root://cms-xrd-global.cern.ch//store/mc/RunIISummer16DR80/VBFHToTauTau_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/60000/A878C3D4-98A2-E611-8AEE-0CC47A7C3444.root',
	'root://cms-xrd-global.cern.ch//store/mc/RunIISummer16DR80/VBFHToTauTau_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/60000/BEFB0480-95A2-E611-98DF-0025905A60E0.root',
	'root://cms-xrd-global.cern.ch//store/mc/RunIISummer16DR80/VBFHToTauTau_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/60000/C46A5564-96A2-E611-9F07-00248C55CC9D.root',
	'root://cms-xrd-global.cern.ch//store/mc/RunIISummer16DR80/VBFHToTauTau_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/60000/D45243E9-94A2-E611-8007-0025905B85DE.root',
	'root://cms-xrd-global.cern.ch//store/mc/RunIISummer16DR80/VBFHToTauTau_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/60000/EC221E80-95A2-E611-A1D1-0025905A6132.root',
	'root://cms-xrd-global.cern.ch//store/mc/RunIISummer16DR80/VBFHToTauTau_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/60000/F079BCF5-94A2-E611-839D-0025905B8612.root',
	'root://cms-xrd-global.cern.ch//store/mc/RunIISummer16DR80/VBFHToTauTau_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/60000/FE168AF6-93A2-E611-9019-003048FF9ABC.root',
	'root://cms-xrd-global.cern.ch//store/mc/RunIISummer16DR80/VBFHToTauTau_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/60000/0248A7FB-91A2-E611-A2B5-0CC47A78A42E.root',
	'root://cms-xrd-global.cern.ch//store/mc/RunIISummer16DR80/VBFHToTauTau_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/60000/10CA0D71-94A2-E611-A75F-0025905A6136.root',
	'root://cms-xrd-global.cern.ch//store/mc/RunIISummer16DR80/VBFHToTauTau_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/60000/1C069B70-92A2-E611-9126-0025905A606A.root',
	'root://cms-xrd-global.cern.ch//store/mc/RunIISummer16DR80/VBFHToTauTau_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/60000/20FA4EF2-91A2-E611-B7B8-0CC47A4D768C.root',
	'root://cms-xrd-global.cern.ch//store/mc/RunIISummer16DR80/VBFHToTauTau_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/60000/2C556D4C-94A2-E611-AAD1-0025905B8586.root',
	'root://cms-xrd-global.cern.ch//store/mc/RunIISummer16DR80/VBFHToTauTau_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/60000/36A0FC43-95A2-E611-9027-0025905A60CE.root',
	'root://cms-xrd-global.cern.ch//store/mc/RunIISummer16DR80/VBFHToTauTau_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/60000/38BD0A6A-92A2-E611-A798-0025905A60B6.root',
	'root://cms-xrd-global.cern.ch//store/mc/RunIISummer16DR80/VBFHToTauTau_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/60000/4411CEF9-93A2-E611-AE1E-0025905A60CE.root',
	'root://cms-xrd-global.cern.ch//store/mc/RunIISummer16DR80/VBFHToTauTau_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/60000/4C13D1A2-92A2-E611-9700-0025905A612A.root',
	'root://cms-xrd-global.cern.ch//store/mc/RunIISummer16DR80/VBFHToTauTau_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/60000/5A5C227F-95A2-E611-AE5E-0025905A60A6.root',
	'root://cms-xrd-global.cern.ch//store/mc/RunIISummer16DR80/VBFHToTauTau_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/60000/5C0FB31D-93A2-E611-B648-0CC47A4D7604.root',
	'root://cms-xrd-global.cern.ch//store/mc/RunIISummer16DR80/VBFHToTauTau_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/60000/86EC39EA-92A2-E611-9EB3-0CC47A78A42E.root',
	'root://cms-xrd-global.cern.ch//store/mc/RunIISummer16DR80/VBFHToTauTau_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/60000/8E5B25EC-92A2-E611-8CD9-0025905A6136.root',
	'root://cms-xrd-global.cern.ch//store/mc/RunIISummer16DR80/VBFHToTauTau_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/60000/8E88202D-93A2-E611-92D3-0025905B8586.root',
	'root://cms-xrd-global.cern.ch//store/mc/RunIISummer16DR80/VBFHToTauTau_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/60000/92CA3772-91A2-E611-AEC1-0025905A6064.root',
	'root://cms-xrd-global.cern.ch//store/mc/RunIISummer16DR80/VBFHToTauTau_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/60000/BC64A3EB-92A2-E611-A9F2-0025905A6138.root',
	'root://cms-xrd-global.cern.ch//store/mc/RunIISummer16DR80/VBFHToTauTau_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/60000/C2A3D2F2-8FA2-E611-959F-0CC47A78A458.root',
	'root://cms-xrd-global.cern.ch//store/mc/RunIISummer16DR80/VBFHToTauTau_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/60000/DEC88529-92A2-E611-B836-0CC47A7C3458.root',
	'root://cms-xrd-global.cern.ch//store/mc/RunIISummer16DR80/VBFHToTauTau_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/60000/ECEE8A73-91A2-E611-881C-0025905A612A.root',
])

process.maxEvents.input = cms.untracked.int32(-1)

process.options = cms.untracked.PSet(
    SkipEvent = cms.untracked.vstring('ProductNotFound')
)

# save only taus with some non too small pt
#process.hltDoublePFTau35Reg.MinPt = cms.double( 20. )


# create new trigger objects from your trigger results
process_name = process.name_()
process.patTriggerCustom = cms.EDProducer(
    'PATTriggerProducer',
    TriggerResults              = cms.InputTag('TriggerResults'      , '', process_name),
    hltTriggerSummaryAOD        = cms.InputTag('hltTriggerSummaryAOD', '', process_name),
    l1tAlgBlkInputTag           = cms.InputTag('hltgtStage2Digis'    , '', process_name),
    l1tExtBlkInputTag           = cms.InputTag('hltgtStage2Digis'    , '', process_name),
    onlyStandAlone              = cms.bool(True),
    packTriggerPathNames        = cms.bool(True),
    processName                 = cms.string(process_name)
)

process.selectedPatTriggerCustom = cms.EDFilter(
    'PATTriggerObjectStandAloneSelector',
    cut = cms.string('!filterLabels.empty()'),
    src = cms.InputTag('patTriggerCustom')
)

if not hasattr(process, 'HLTAnalyzerEndpath'):
    print 'added needed endpath'
    process.HLTAnalyzerEndpath = cms.EndPath(process.patTriggerCustom * process.selectedPatTriggerCustom)
    process.HLTSchedule.append(process.HLTAnalyzerEndpath)
else:
    process.HLTAnalyzerEndpath += process.patTriggerCustom
    process.HLTAnalyzerEndpath += process.selectedPatTriggerCustom

reportInterval = 1
process.load('FWCore.MessageLogger.MessageLogger_cfi')
process.MessageLogger.cerr.FwkReport.reportEvery = reportInterval


# adjust the ouput module
process.hltOutputFULL = cms.OutputModule( 'PoolOutputModule',
    fileName = cms.untracked.string( 'outputFULL.root' ),
    fastCloning = cms.untracked.bool( False ),
    dataset = cms.untracked.PSet(
        dataTier = cms.untracked.string( 'RECO' ),
        filterName = cms.untracked.string( '' )
    ),
    outputCommands = cms.untracked.vstring( 'keep *' ),
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('HLT_*',)
    )
)

# keep all the MINIAOD content, and the updated trigger collections
process.hltOutputFULL.outputCommands = cms.untracked.vstring(
    'drop *'                                                              ,
    'keep *_TriggerResults_*_*'                                           ,
    'keep *_addPileupInfo_*_*'                                            ,
    'keep LHEEventProduct_externalLHEProducer__LHE'                       ,
    'keep GenEventInfoProduct_generator__SIM'                             ,
    'keep *TriggerObjectStandAlone_*_*_*'                                 ,
    'keep *_*atTrigger*_*_*'                                              ,
    'keep *_hltTriggerSummaryAOD_*_*'                                     ,
    'keep *_*Stage2*_*_*'                                                 ,
    'keep *PFTauDiscriminator_*Open*_*_*'                                 ,
    'keep double_*ixedGridRho*_*_*'                                       ,
#    'keep *_hltL1JetsHLTDoublePFTauTrackPt0OpenIsolationMatchReg_*_TEST'  ,
#    'keep *_hltL1JetsHLTDoublePFTauTrackPt1MediumIsolationMatchReg_*_TEST',
#    'keep *_hltPFTausReg_*_TEST'                                          ,
#    'keep *_hltPFTausSansRefReg_*_TEST'                                   ,
#    'keep *_hltSelectedPFTausTrackPt0OpenIsolationReg_*_TEST'             ,
#    'keep *_hltSelectedPFTausTrackPt0Reg_*_TEST'                          ,
#    'keep *_hltSelectedPFTausTrackPt1MediumIsolationReg_*_TEST'           ,
#    'keep *_hltSelectedPFTausTrackPt1Reg_*_TEST'                          ,
#    'keep GenEventInfoProduct_generator__SIM'                             ,
    'keep HcalNoiseSummary_hcalnoise__RECO'                               ,
    'keep *BeamSpot_offlineBeamSpot__RECO'                                ,
    'keep *_*limmed*__*'                                                  ,
    'keep *_lostTracks__*'                                                ,
    'keep *_packedPFCandidates__*'                                        ,
    'keep *_packedGenParticles__*'                                        ,
    'keep *_prunedGenParticles__*'                                        ,
    'keep unsignedint_bunchSpacingProducer__*'                            ,
    'keep *_reducedEgamma_reducedGedGsfElectronCores_*'                   ,
    'keep *_reducedEgamma_reducedSuperClusters_*'                         ,

#    'keep *_hltPFTauCharged3HitsPtSumReg_*_TEST'                          ,
#    'keep *_hltPFTauCharged5HitsPtSumReg_*_TEST'                          ,
#    'keep *_hltPFTauCharged8HitsPtSumReg_*_TEST'                          ,
#    'keep *_hltPFTauNeutralPtSumReg_*_TEST'                               ,
#    'keep *_hltPFTauPUcorrDBeta0p2Cone0p8Reg_*_TEST'                      ,
#    'keep *_hltPFTauPUcorrRhoCone0p5Reg_*_TEST'                           ,
#    'keep *_hltPFTauPhotonPtOutsideSignalConeReg_*_TEST'                  ,

	'keep *TriggerFilterObjectWithRefs_hltDoubleJetOpen__TEST'            ,
	'keep *TriggerFilterObjectWithRefs_hltPFDoubleJetLooseIDOpen__TEST'	  ,
    'keep *_hltParticleFlowReg_*_*'                                       ,
#    'keep *'                                       ,
)

process.hltOutputFULL.fileName = cms.untracked.string(
    'outputFULLVBFnoCSVTEST.root'
)

process.FULLOutput = cms.EndPath( process.hltOutputFULL )
