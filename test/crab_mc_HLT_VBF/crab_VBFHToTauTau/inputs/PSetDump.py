import FWCore.ParameterSet.Config as cms

process = cms.Process("TEST")

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring('root://cms-xrd-global.cern.ch//store/mc/RunIISummer16MiniAODv2/VBFHToTauTau_M125_13TeV_powheg_pythia8/MINIAODSIM/FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/50000/0A171627-64B9-E611-9270-0025904C7DFA.root'),
    inputCommands = cms.untracked.vstring('keep *'),
    secondaryFileNames = cms.untracked.vstring('root://cms-xrd-global.cern.ch//store/mc/RunIISummer16DR80/VBFHToTauTau_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/130000/2E22E1F4-52A3-E611-944A-001E674FAF23.root', 
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
        'root://cms-xrd-global.cern.ch//store/mc/RunIISummer16DR80/VBFHToTauTau_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/FlatPU28to62HcalNZSRAW_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/60000/ECEE8A73-91A2-E611-881C-0025905A612A.root')
)
process.HLTConfigVersion = cms.PSet(
    tableName = cms.string('/users/adow/VBF_trigger/HLT_VBF_new/V10')
)

process.HLTIter0HighPtTkMuPSetTrajectoryBuilderIT = cms.PSet(
    ComponentType = cms.string('CkfTrajectoryBuilder'),
    TTRHBuilder = cms.string('hltESPTTRHBWithTrackAngle'),
    alwaysUseInvalidHits = cms.bool(True),
    estimator = cms.string('hltESPChi2ChargeMeasurementEstimator30'),
    intermediateCleaning = cms.bool(True),
    lostHitPenalty = cms.double(30.0),
    maxCand = cms.int32(4),
    propagatorAlong = cms.string('PropagatorWithMaterialParabolicMf'),
    propagatorOpposite = cms.string('PropagatorWithMaterialParabolicMfOpposite'),
    trajectoryFilter = cms.PSet(
        refToPSet_ = cms.string('HLTIter0HighPtTkMuPSetTrajectoryFilterIT')
    ),
    updator = cms.string('hltESPKFUpdator')
)

process.HLTIter0HighPtTkMuPSetTrajectoryFilterIT = cms.PSet(
    ComponentType = cms.string('CkfBaseTrajectoryFilter'),
    chargeSignificance = cms.double(-1.0),
    constantValueForLostHitsFractionFilter = cms.double(1.0),
    extraNumberOfHitsBeforeTheFirstLoop = cms.int32(4),
    maxCCCLostHits = cms.int32(9999),
    maxConsecLostHits = cms.int32(1),
    maxLostHits = cms.int32(1),
    maxLostHitsFraction = cms.double(999.0),
    maxNumberOfHits = cms.int32(100),
    minGoodStripCharge = cms.PSet(
        refToPSet_ = cms.string('HLTSiStripClusterChargeCutNone')
    ),
    minHitsMinPt = cms.int32(3),
    minNumberOfHitsForLoopers = cms.int32(13),
    minNumberOfHitsPerLoop = cms.int32(4),
    minPt = cms.double(0.3),
    minimumNumberOfHits = cms.int32(3),
    nSigmaMinPt = cms.double(5.0),
    seedExtension = cms.int32(0),
    seedPairPenalty = cms.int32(0),
    strictSeedExtension = cms.bool(False)
)

process.HLTIter0PSetTrajectoryBuilderIT = cms.PSet(
    ComponentType = cms.string('CkfTrajectoryBuilder'),
    TTRHBuilder = cms.string('hltESPTTRHBWithTrackAngle'),
    alwaysUseInvalidHits = cms.bool(False),
    estimator = cms.string('hltESPChi2ChargeMeasurementEstimator9'),
    intermediateCleaning = cms.bool(True),
    lostHitPenalty = cms.double(30.0),
    maxCand = cms.int32(2),
    propagatorAlong = cms.string('PropagatorWithMaterialParabolicMf'),
    propagatorOpposite = cms.string('PropagatorWithMaterialParabolicMfOpposite'),
    trajectoryFilter = cms.PSet(
        refToPSet_ = cms.string('HLTIter0PSetTrajectoryFilterIT')
    ),
    updator = cms.string('hltESPKFUpdator')
)

process.HLTIter0PSetTrajectoryFilterIT = cms.PSet(
    ComponentType = cms.string('CkfBaseTrajectoryFilter'),
    chargeSignificance = cms.double(-1.0),
    constantValueForLostHitsFractionFilter = cms.double(1.0),
    extraNumberOfHitsBeforeTheFirstLoop = cms.int32(4),
    maxCCCLostHits = cms.int32(0),
    maxConsecLostHits = cms.int32(1),
    maxLostHits = cms.int32(1),
    maxLostHitsFraction = cms.double(999.0),
    maxNumberOfHits = cms.int32(100),
    minGoodStripCharge = cms.PSet(
        refToPSet_ = cms.string('HLTSiStripClusterChargeCutNone')
    ),
    minHitsMinPt = cms.int32(3),
    minNumberOfHitsForLoopers = cms.int32(13),
    minNumberOfHitsPerLoop = cms.int32(4),
    minPt = cms.double(0.3),
    minimumNumberOfHits = cms.int32(3),
    nSigmaMinPt = cms.double(5.0),
    seedExtension = cms.int32(0),
    seedPairPenalty = cms.int32(0),
    strictSeedExtension = cms.bool(False)
)

process.HLTIter1PSetTrajectoryBuilderIT = cms.PSet(
    ComponentType = cms.string('CkfTrajectoryBuilder'),
    MeasurementTrackerName = cms.string('hltIter1ESPMeasurementTracker'),
    TTRHBuilder = cms.string('hltESPTTRHBWithTrackAngle'),
    alwaysUseInvalidHits = cms.bool(False),
    estimator = cms.string('hltESPChi2ChargeMeasurementEstimator16'),
    intermediateCleaning = cms.bool(True),
    lostHitPenalty = cms.double(30.0),
    maxCand = cms.int32(2),
    propagatorAlong = cms.string('PropagatorWithMaterialParabolicMf'),
    propagatorOpposite = cms.string('PropagatorWithMaterialParabolicMfOpposite'),
    trajectoryFilter = cms.PSet(
        refToPSet_ = cms.string('HLTIter1PSetTrajectoryFilterIT')
    ),
    updator = cms.string('hltESPKFUpdator')
)

process.HLTIter1PSetTrajectoryFilterIT = cms.PSet(
    ComponentType = cms.string('CkfBaseTrajectoryFilter'),
    chargeSignificance = cms.double(-1.0),
    constantValueForLostHitsFractionFilter = cms.double(1.0),
    extraNumberOfHitsBeforeTheFirstLoop = cms.int32(4),
    maxCCCLostHits = cms.int32(0),
    maxConsecLostHits = cms.int32(1),
    maxLostHits = cms.int32(1),
    maxLostHitsFraction = cms.double(999.0),
    maxNumberOfHits = cms.int32(100),
    minGoodStripCharge = cms.PSet(
        refToPSet_ = cms.string('HLTSiStripClusterChargeCutNone')
    ),
    minHitsMinPt = cms.int32(3),
    minNumberOfHitsForLoopers = cms.int32(13),
    minNumberOfHitsPerLoop = cms.int32(4),
    minPt = cms.double(0.2),
    minimumNumberOfHits = cms.int32(3),
    nSigmaMinPt = cms.double(5.0),
    seedExtension = cms.int32(0),
    seedPairPenalty = cms.int32(0),
    strictSeedExtension = cms.bool(False)
)

process.HLTIter2HighPtTkMuPSetTrajectoryBuilderIT = cms.PSet(
    ComponentType = cms.string('CkfTrajectoryBuilder'),
    MeasurementTrackerName = cms.string('hltIter2HighPtTkMuESPMeasurementTracker'),
    TTRHBuilder = cms.string('hltESPTTRHBWithTrackAngle'),
    alwaysUseInvalidHits = cms.bool(False),
    estimator = cms.string('hltESPChi2ChargeMeasurementEstimator30'),
    intermediateCleaning = cms.bool(True),
    lostHitPenalty = cms.double(30.0),
    maxCand = cms.int32(2),
    propagatorAlong = cms.string('PropagatorWithMaterialParabolicMf'),
    propagatorOpposite = cms.string('PropagatorWithMaterialParabolicMfOpposite'),
    trajectoryFilter = cms.PSet(
        refToPSet_ = cms.string('HLTIter2HighPtTkMuPSetTrajectoryFilterIT')
    ),
    updator = cms.string('hltESPKFUpdator')
)

process.HLTIter2HighPtTkMuPSetTrajectoryFilterIT = cms.PSet(
    ComponentType = cms.string('CkfBaseTrajectoryFilter'),
    chargeSignificance = cms.double(-1.0),
    constantValueForLostHitsFractionFilter = cms.double(1.0),
    extraNumberOfHitsBeforeTheFirstLoop = cms.int32(4),
    maxCCCLostHits = cms.int32(9999),
    maxConsecLostHits = cms.int32(3),
    maxLostHits = cms.int32(1),
    maxLostHitsFraction = cms.double(999.0),
    maxNumberOfHits = cms.int32(100),
    minGoodStripCharge = cms.PSet(
        refToPSet_ = cms.string('HLTSiStripClusterChargeCutNone')
    ),
    minHitsMinPt = cms.int32(3),
    minNumberOfHitsForLoopers = cms.int32(13),
    minNumberOfHitsPerLoop = cms.int32(4),
    minPt = cms.double(0.3),
    minimumNumberOfHits = cms.int32(5),
    nSigmaMinPt = cms.double(5.0),
    seedExtension = cms.int32(0),
    seedPairPenalty = cms.int32(0),
    strictSeedExtension = cms.bool(False)
)

process.HLTIter2PSetTrajectoryBuilderIT = cms.PSet(
    ComponentType = cms.string('CkfTrajectoryBuilder'),
    MeasurementTrackerName = cms.string('hltIter2ESPMeasurementTracker'),
    TTRHBuilder = cms.string('hltESPTTRHBWithTrackAngle'),
    alwaysUseInvalidHits = cms.bool(False),
    estimator = cms.string('hltESPChi2ChargeMeasurementEstimator16'),
    intermediateCleaning = cms.bool(True),
    lostHitPenalty = cms.double(30.0),
    maxCand = cms.int32(2),
    propagatorAlong = cms.string('PropagatorWithMaterialParabolicMf'),
    propagatorOpposite = cms.string('PropagatorWithMaterialParabolicMfOpposite'),
    trajectoryFilter = cms.PSet(
        refToPSet_ = cms.string('HLTIter2PSetTrajectoryFilterIT')
    ),
    updator = cms.string('hltESPKFUpdator')
)

process.HLTIter2PSetTrajectoryFilterIT = cms.PSet(
    ComponentType = cms.string('CkfBaseTrajectoryFilter'),
    chargeSignificance = cms.double(-1.0),
    constantValueForLostHitsFractionFilter = cms.double(1.0),
    extraNumberOfHitsBeforeTheFirstLoop = cms.int32(4),
    maxCCCLostHits = cms.int32(0),
    maxConsecLostHits = cms.int32(1),
    maxLostHits = cms.int32(1),
    maxLostHitsFraction = cms.double(999.0),
    maxNumberOfHits = cms.int32(100),
    minGoodStripCharge = cms.PSet(
        refToPSet_ = cms.string('HLTSiStripClusterChargeCutNone')
    ),
    minHitsMinPt = cms.int32(3),
    minNumberOfHitsForLoopers = cms.int32(13),
    minNumberOfHitsPerLoop = cms.int32(4),
    minPt = cms.double(0.3),
    minimumNumberOfHits = cms.int32(3),
    nSigmaMinPt = cms.double(5.0),
    seedExtension = cms.int32(1),
    seedPairPenalty = cms.int32(0),
    strictSeedExtension = cms.bool(False)
)

process.HLTIter3PSetTrajectoryBuilderIT = cms.PSet(
    ComponentType = cms.string('CkfTrajectoryBuilder'),
    MeasurementTrackerName = cms.string('hltIter3ESPMeasurementTracker'),
    TTRHBuilder = cms.string('hltESPTTRHBWithTrackAngle'),
    alwaysUseInvalidHits = cms.bool(False),
    estimator = cms.string('hltESPChi2ChargeMeasurementEstimator16'),
    intermediateCleaning = cms.bool(True),
    lostHitPenalty = cms.double(30.0),
    maxCand = cms.int32(1),
    propagatorAlong = cms.string('PropagatorWithMaterialParabolicMf'),
    propagatorOpposite = cms.string('PropagatorWithMaterialParabolicMfOpposite'),
    trajectoryFilter = cms.PSet(
        refToPSet_ = cms.string('HLTIter3PSetTrajectoryFilterIT')
    ),
    updator = cms.string('hltESPKFUpdator')
)

process.HLTIter3PSetTrajectoryFilterIT = cms.PSet(
    ComponentType = cms.string('CkfBaseTrajectoryFilter'),
    chargeSignificance = cms.double(-1.0),
    constantValueForLostHitsFractionFilter = cms.double(1.0),
    extraNumberOfHitsBeforeTheFirstLoop = cms.int32(4),
    maxCCCLostHits = cms.int32(9999),
    maxConsecLostHits = cms.int32(1),
    maxLostHits = cms.int32(0),
    maxLostHitsFraction = cms.double(999.0),
    maxNumberOfHits = cms.int32(100),
    minGoodStripCharge = cms.PSet(
        refToPSet_ = cms.string('HLTSiStripClusterChargeCutNone')
    ),
    minHitsMinPt = cms.int32(3),
    minNumberOfHitsForLoopers = cms.int32(13),
    minNumberOfHitsPerLoop = cms.int32(4),
    minPt = cms.double(0.3),
    minimumNumberOfHits = cms.int32(3),
    nSigmaMinPt = cms.double(5.0),
    seedExtension = cms.int32(0),
    seedPairPenalty = cms.int32(0),
    strictSeedExtension = cms.bool(False)
)

process.HLTIter4PSetTrajectoryBuilderIT = cms.PSet(
    ComponentType = cms.string('CkfTrajectoryBuilder'),
    MeasurementTrackerName = cms.string('hltIter4ESPMeasurementTracker'),
    TTRHBuilder = cms.string('hltESPTTRHBWithTrackAngle'),
    alwaysUseInvalidHits = cms.bool(False),
    estimator = cms.string('hltESPChi2ChargeMeasurementEstimator16'),
    intermediateCleaning = cms.bool(True),
    lostHitPenalty = cms.double(30.0),
    maxCand = cms.int32(1),
    minNrOfHitsForRebuild = cms.untracked.int32(4),
    propagatorAlong = cms.string('PropagatorWithMaterialParabolicMf'),
    propagatorOpposite = cms.string('PropagatorWithMaterialParabolicMfOpposite'),
    trajectoryFilter = cms.PSet(
        refToPSet_ = cms.string('HLTIter4PSetTrajectoryFilterIT')
    ),
    updator = cms.string('hltESPKFUpdator')
)

process.HLTIter4PSetTrajectoryFilterIT = cms.PSet(
    ComponentType = cms.string('CkfBaseTrajectoryFilter'),
    chargeSignificance = cms.double(-1.0),
    constantValueForLostHitsFractionFilter = cms.double(1.0),
    extraNumberOfHitsBeforeTheFirstLoop = cms.int32(4),
    maxCCCLostHits = cms.int32(9999),
    maxConsecLostHits = cms.int32(1),
    maxLostHits = cms.int32(0),
    maxLostHitsFraction = cms.double(999.0),
    maxNumberOfHits = cms.int32(100),
    minGoodStripCharge = cms.PSet(
        refToPSet_ = cms.string('HLTSiStripClusterChargeCutNone')
    ),
    minHitsMinPt = cms.int32(3),
    minNumberOfHitsForLoopers = cms.int32(13),
    minNumberOfHitsPerLoop = cms.int32(4),
    minPt = cms.double(0.3),
    minimumNumberOfHits = cms.int32(6),
    nSigmaMinPt = cms.double(5.0),
    seedExtension = cms.int32(0),
    seedPairPenalty = cms.int32(0),
    strictSeedExtension = cms.bool(False)
)

process.HLTPSetCkf3HitTrajectoryFilter = cms.PSet(
    ComponentType = cms.string('CkfBaseTrajectoryFilter'),
    chargeSignificance = cms.double(-1.0),
    constantValueForLostHitsFractionFilter = cms.double(1.0),
    extraNumberOfHitsBeforeTheFirstLoop = cms.int32(4),
    maxCCCLostHits = cms.int32(9999),
    maxConsecLostHits = cms.int32(1),
    maxLostHits = cms.int32(1),
    maxLostHitsFraction = cms.double(999.0),
    maxNumberOfHits = cms.int32(-1),
    minGoodStripCharge = cms.PSet(
        refToPSet_ = cms.string('HLTSiStripClusterChargeCutNone')
    ),
    minHitsMinPt = cms.int32(3),
    minNumberOfHitsForLoopers = cms.int32(13),
    minNumberOfHitsPerLoop = cms.int32(4),
    minPt = cms.double(0.9),
    minimumNumberOfHits = cms.int32(3),
    nSigmaMinPt = cms.double(5.0),
    seedExtension = cms.int32(0),
    seedPairPenalty = cms.int32(0),
    strictSeedExtension = cms.bool(False)
)

process.HLTPSetCkfTrajectoryFilter = cms.PSet(
    ComponentType = cms.string('CkfBaseTrajectoryFilter'),
    chargeSignificance = cms.double(-1.0),
    constantValueForLostHitsFractionFilter = cms.double(1.0),
    extraNumberOfHitsBeforeTheFirstLoop = cms.int32(4),
    maxCCCLostHits = cms.int32(9999),
    maxConsecLostHits = cms.int32(1),
    maxLostHits = cms.int32(1),
    maxLostHitsFraction = cms.double(999.0),
    maxNumberOfHits = cms.int32(-1),
    minGoodStripCharge = cms.PSet(
        refToPSet_ = cms.string('HLTSiStripClusterChargeCutNone')
    ),
    minHitsMinPt = cms.int32(3),
    minNumberOfHitsForLoopers = cms.int32(13),
    minNumberOfHitsPerLoop = cms.int32(4),
    minPt = cms.double(0.9),
    minimumNumberOfHits = cms.int32(5),
    nSigmaMinPt = cms.double(5.0),
    seedExtension = cms.int32(0),
    seedPairPenalty = cms.int32(0),
    strictSeedExtension = cms.bool(False)
)

process.HLTPSetDetachedCkfTrajectoryBuilderForHI = cms.PSet(
    ComponentType = cms.string('GroupedCkfTrajectoryBuilder'),
    MeasurementTrackerName = cms.string(''),
    TTRHBuilder = cms.string('hltESPTTRHBWithTrackAngle'),
    alwaysUseInvalidHits = cms.bool(False),
    bestHitOnly = cms.bool(True),
    estimator = cms.string('hltESPChi2MeasurementEstimator9'),
    foundHitBonus = cms.double(5.0),
    inOutTrajectoryFilter = cms.PSet(
        refToPSet_ = cms.string('HLTPSetDetachedCkfTrajectoryFilterForHI')
    ),
    intermediateCleaning = cms.bool(True),
    keepOriginalIfRebuildFails = cms.bool(False),
    lockHits = cms.bool(True),
    lostHitPenalty = cms.double(30.0),
    maxCand = cms.int32(2),
    maxDPhiForLooperReconstruction = cms.double(0.0),
    maxPtForLooperReconstruction = cms.double(0.0),
    minNrOfHitsForRebuild = cms.int32(5),
    propagatorAlong = cms.string('PropagatorWithMaterialForHI'),
    propagatorOpposite = cms.string('PropagatorWithMaterialOppositeForHI'),
    requireSeedHitsInRebuild = cms.bool(True),
    trajectoryFilter = cms.PSet(
        refToPSet_ = cms.string('HLTPSetDetachedCkfTrajectoryFilterForHI')
    ),
    updator = cms.string('hltESPKFUpdator'),
    useSameTrajFilter = cms.bool(True)
)

process.HLTPSetDetachedCkfTrajectoryBuilderForHIGlobalPt8 = cms.PSet(
    ComponentType = cms.string('GroupedCkfTrajectoryBuilder'),
    MeasurementTrackerName = cms.string(''),
    TTRHBuilder = cms.string('hltESPTTRHBWithTrackAngle'),
    alwaysUseInvalidHits = cms.bool(False),
    bestHitOnly = cms.bool(True),
    estimator = cms.string('hltESPChi2MeasurementEstimator9'),
    foundHitBonus = cms.double(5.0),
    inOutTrajectoryFilter = cms.PSet(
        refToPSet_ = cms.string('HLTPSetDetachedCkfTrajectoryFilterForHIGlobalPt8')
    ),
    intermediateCleaning = cms.bool(True),
    keepOriginalIfRebuildFails = cms.bool(False),
    lockHits = cms.bool(True),
    lostHitPenalty = cms.double(30.0),
    maxCand = cms.int32(2),
    maxDPhiForLooperReconstruction = cms.double(0.0),
    maxPtForLooperReconstruction = cms.double(0.0),
    minNrOfHitsForRebuild = cms.int32(5),
    propagatorAlong = cms.string('PropagatorWithMaterialForHI'),
    propagatorOpposite = cms.string('PropagatorWithMaterialOppositeForHI'),
    requireSeedHitsInRebuild = cms.bool(True),
    trajectoryFilter = cms.PSet(
        refToPSet_ = cms.string('HLTPSetDetachedCkfTrajectoryFilterForHIGlobalPt8')
    ),
    updator = cms.string('hltESPKFUpdator'),
    useSameTrajFilter = cms.bool(True)
)

process.HLTPSetDetachedCkfTrajectoryFilterForHI = cms.PSet(
    ComponentType = cms.string('CkfBaseTrajectoryFilter'),
    chargeSignificance = cms.double(-1.0),
    constantValueForLostHitsFractionFilter = cms.double(0.701),
    extraNumberOfHitsBeforeTheFirstLoop = cms.int32(4),
    maxCCCLostHits = cms.int32(9999),
    maxConsecLostHits = cms.int32(1),
    maxLostHits = cms.int32(1),
    maxLostHitsFraction = cms.double(999.0),
    maxNumberOfHits = cms.int32(100),
    minGoodStripCharge = cms.PSet(
        refToPSet_ = cms.string('HLTSiStripClusterChargeCutNone')
    ),
    minHitsMinPt = cms.int32(3),
    minNumberOfHitsForLoopers = cms.int32(13),
    minNumberOfHitsPerLoop = cms.int32(4),
    minPt = cms.double(0.3),
    minimumNumberOfHits = cms.int32(6),
    nSigmaMinPt = cms.double(5.0),
    seedExtension = cms.int32(0),
    seedPairPenalty = cms.int32(0),
    strictSeedExtension = cms.bool(False)
)

process.HLTPSetDetachedCkfTrajectoryFilterForHIGlobalPt8 = cms.PSet(
    ComponentType = cms.string('CkfBaseTrajectoryFilter'),
    chargeSignificance = cms.double(-1.0),
    constantValueForLostHitsFractionFilter = cms.double(0.701),
    extraNumberOfHitsBeforeTheFirstLoop = cms.int32(4),
    maxCCCLostHits = cms.int32(9999),
    maxConsecLostHits = cms.int32(1),
    maxLostHits = cms.int32(1),
    maxLostHitsFraction = cms.double(999.0),
    maxNumberOfHits = cms.int32(100),
    minGoodStripCharge = cms.PSet(
        refToPSet_ = cms.string('HLTSiStripClusterChargeCutNone')
    ),
    minHitsMinPt = cms.int32(3),
    minNumberOfHitsForLoopers = cms.int32(13),
    minNumberOfHitsPerLoop = cms.int32(4),
    minPt = cms.double(8.0),
    minimumNumberOfHits = cms.int32(6),
    nSigmaMinPt = cms.double(5.0),
    seedExtension = cms.int32(0),
    seedPairPenalty = cms.int32(0),
    strictSeedExtension = cms.bool(False)
)

process.HLTPSetDetachedStepTrajectoryBuilder = cms.PSet(
    ComponentType = cms.string('GroupedCkfTrajectoryBuilder'),
    MeasurementTrackerName = cms.string(''),
    TTRHBuilder = cms.string('hltESPTTRHBWithTrackAngle'),
    alwaysUseInvalidHits = cms.bool(False),
    bestHitOnly = cms.bool(True),
    estimator = cms.string('hltESPChi2ChargeMeasurementEstimator9'),
    foundHitBonus = cms.double(5.0),
    inOutTrajectoryFilter = cms.PSet(
        refToPSet_ = cms.string('HLTPSetDetachedStepTrajectoryFilter')
    ),
    intermediateCleaning = cms.bool(True),
    keepOriginalIfRebuildFails = cms.bool(False),
    lockHits = cms.bool(True),
    lostHitPenalty = cms.double(30.0),
    maxCand = cms.int32(3),
    maxDPhiForLooperReconstruction = cms.double(2.0),
    maxPtForLooperReconstruction = cms.double(0.7),
    minNrOfHitsForRebuild = cms.int32(5),
    propagatorAlong = cms.string('PropagatorWithMaterialParabolicMf'),
    propagatorOpposite = cms.string('PropagatorWithMaterialParabolicMfOpposite'),
    requireSeedHitsInRebuild = cms.bool(True),
    trajectoryFilter = cms.PSet(
        refToPSet_ = cms.string('HLTPSetDetachedStepTrajectoryFilter')
    ),
    updator = cms.string('hltESPKFUpdator'),
    useSameTrajFilter = cms.bool(True)
)

process.HLTPSetDetachedStepTrajectoryFilter = cms.PSet(
    ComponentType = cms.string('CompositeTrajectoryFilter'),
    filters = cms.VPSet(cms.PSet(
        refToPSet_ = cms.string('HLTPSetDetachedStepTrajectoryFilterBase')
    ))
)

process.HLTPSetDetachedStepTrajectoryFilterBase = cms.PSet(
    ComponentType = cms.string('CkfBaseTrajectoryFilter'),
    chargeSignificance = cms.double(-1.0),
    constantValueForLostHitsFractionFilter = cms.double(2.0),
    extraNumberOfHitsBeforeTheFirstLoop = cms.int32(4),
    maxCCCLostHits = cms.int32(2),
    maxConsecLostHits = cms.int32(1),
    maxLostHits = cms.int32(999),
    maxLostHitsFraction = cms.double(0.1),
    maxNumberOfHits = cms.int32(100),
    minGoodStripCharge = cms.PSet(
        refToPSet_ = cms.string('HLTSiStripClusterChargeCutLoose')
    ),
    minHitsMinPt = cms.int32(3),
    minNumberOfHitsForLoopers = cms.int32(13),
    minNumberOfHitsPerLoop = cms.int32(4),
    minPt = cms.double(0.075),
    minimumNumberOfHits = cms.int32(3),
    nSigmaMinPt = cms.double(5.0),
    seedExtension = cms.int32(0),
    seedPairPenalty = cms.int32(0),
    strictSeedExtension = cms.bool(False)
)

process.HLTPSetInitialCkfTrajectoryBuilderForHI = cms.PSet(
    ComponentType = cms.string('CkfTrajectoryBuilder'),
    MeasurementTrackerName = cms.string('hltESPMeasurementTracker'),
    TTRHBuilder = cms.string('hltESPTTRHBWithTrackAngle'),
    alwaysUseInvalidHits = cms.bool(False),
    estimator = cms.string('hltESPChi2MeasurementEstimator30'),
    intermediateCleaning = cms.bool(False),
    lostHitPenalty = cms.double(30.0),
    maxCand = cms.int32(5),
    propagatorAlong = cms.string('PropagatorWithMaterialForHI'),
    propagatorOpposite = cms.string('PropagatorWithMaterialOppositeForHI'),
    trajectoryFilter = cms.PSet(
        refToPSet_ = cms.string('HLTPSetInitialCkfTrajectoryFilterForHI')
    ),
    updator = cms.string('hltESPKFUpdator')
)

process.HLTPSetInitialCkfTrajectoryFilterForHI = cms.PSet(
    ComponentType = cms.string('CkfBaseTrajectoryFilter'),
    chargeSignificance = cms.double(-1.0),
    constantValueForLostHitsFractionFilter = cms.double(1.0),
    extraNumberOfHitsBeforeTheFirstLoop = cms.int32(4),
    maxCCCLostHits = cms.int32(9999),
    maxConsecLostHits = cms.int32(1),
    maxLostHits = cms.int32(999),
    maxLostHitsFraction = cms.double(999.0),
    maxNumberOfHits = cms.int32(100),
    minGoodStripCharge = cms.PSet(
        refToPSet_ = cms.string('HLTSiStripClusterChargeCutNone')
    ),
    minHitsMinPt = cms.int32(3),
    minNumberOfHitsForLoopers = cms.int32(13),
    minNumberOfHitsPerLoop = cms.int32(4),
    minPt = cms.double(0.9),
    minimumNumberOfHits = cms.int32(6),
    nSigmaMinPt = cms.double(5.0),
    seedExtension = cms.int32(0),
    seedPairPenalty = cms.int32(0),
    strictSeedExtension = cms.bool(False)
)

process.HLTPSetInitialStepTrajectoryBuilder = cms.PSet(
    ComponentType = cms.string('GroupedCkfTrajectoryBuilder'),
    MeasurementTrackerName = cms.string(''),
    TTRHBuilder = cms.string('hltESPTTRHBWithTrackAngle'),
    alwaysUseInvalidHits = cms.bool(True),
    bestHitOnly = cms.bool(True),
    estimator = cms.string('hltESPInitialStepChi2ChargeMeasurementEstimator30'),
    foundHitBonus = cms.double(5.0),
    inOutTrajectoryFilter = cms.PSet(
        refToPSet_ = cms.string('HLTPSetInitialStepTrajectoryFilter')
    ),
    intermediateCleaning = cms.bool(True),
    keepOriginalIfRebuildFails = cms.bool(False),
    lockHits = cms.bool(True),
    lostHitPenalty = cms.double(30.0),
    maxCand = cms.int32(3),
    maxDPhiForLooperReconstruction = cms.double(2.0),
    maxPtForLooperReconstruction = cms.double(0.7),
    minNrOfHitsForRebuild = cms.int32(5),
    propagatorAlong = cms.string('PropagatorWithMaterialParabolicMf'),
    propagatorOpposite = cms.string('PropagatorWithMaterialParabolicMfOpposite'),
    requireSeedHitsInRebuild = cms.bool(True),
    trajectoryFilter = cms.PSet(
        refToPSet_ = cms.string('HLTPSetInitialStepTrajectoryFilter')
    ),
    updator = cms.string('hltESPKFUpdator'),
    useSameTrajFilter = cms.bool(True)
)

process.HLTPSetInitialStepTrajectoryFilter = cms.PSet(
    ComponentType = cms.string('CompositeTrajectoryFilter'),
    filters = cms.VPSet(cms.PSet(
        refToPSet_ = cms.string('HLTPSetInitialStepTrajectoryFilterBase')
    ))
)

process.HLTPSetInitialStepTrajectoryFilterBase = cms.PSet(
    ComponentType = cms.string('CkfBaseTrajectoryFilter'),
    chargeSignificance = cms.double(-1.0),
    constantValueForLostHitsFractionFilter = cms.double(2.0),
    extraNumberOfHitsBeforeTheFirstLoop = cms.int32(4),
    maxCCCLostHits = cms.int32(2),
    maxConsecLostHits = cms.int32(1),
    maxLostHits = cms.int32(999),
    maxLostHitsFraction = cms.double(0.1),
    maxNumberOfHits = cms.int32(100),
    minGoodStripCharge = cms.PSet(
        refToPSet_ = cms.string('HLTSiStripClusterChargeCutLoose')
    ),
    minHitsMinPt = cms.int32(3),
    minNumberOfHitsForLoopers = cms.int32(13),
    minNumberOfHitsPerLoop = cms.int32(4),
    minPt = cms.double(0.2),
    minimumNumberOfHits = cms.int32(3),
    nSigmaMinPt = cms.double(5.0),
    seedExtension = cms.int32(0),
    seedPairPenalty = cms.int32(0),
    strictSeedExtension = cms.bool(False)
)

process.HLTPSetJetCoreStepTrajectoryBuilder = cms.PSet(
    ComponentType = cms.string('GroupedCkfTrajectoryBuilder'),
    MeasurementTrackerName = cms.string(''),
    TTRHBuilder = cms.string('hltESPTTRHBWithTrackAngle'),
    alwaysUseInvalidHits = cms.bool(True),
    bestHitOnly = cms.bool(True),
    estimator = cms.string('hltESPChi2MeasurementEstimator30'),
    foundHitBonus = cms.double(5.0),
    inOutTrajectoryFilter = cms.PSet(
        refToPSet_ = cms.string('HLTPSetJetCoreStepTrajectoryFilter')
    ),
    intermediateCleaning = cms.bool(True),
    keepOriginalIfRebuildFails = cms.bool(False),
    lockHits = cms.bool(True),
    lostHitPenalty = cms.double(30.0),
    maxCand = cms.int32(50),
    maxDPhiForLooperReconstruction = cms.double(2.0),
    maxPtForLooperReconstruction = cms.double(0.7),
    minNrOfHitsForRebuild = cms.int32(5),
    propagatorAlong = cms.string('PropagatorWithMaterialParabolicMf'),
    propagatorOpposite = cms.string('PropagatorWithMaterialParabolicMfOpposite'),
    requireSeedHitsInRebuild = cms.bool(True),
    trajectoryFilter = cms.PSet(
        refToPSet_ = cms.string('HLTPSetJetCoreStepTrajectoryFilter')
    ),
    updator = cms.string('hltESPKFUpdator'),
    useSameTrajFilter = cms.bool(True)
)

process.HLTPSetJetCoreStepTrajectoryFilter = cms.PSet(
    ComponentType = cms.string('CkfBaseTrajectoryFilter'),
    chargeSignificance = cms.double(-1.0),
    constantValueForLostHitsFractionFilter = cms.double(2.0),
    extraNumberOfHitsBeforeTheFirstLoop = cms.int32(4),
    maxCCCLostHits = cms.int32(9999),
    maxConsecLostHits = cms.int32(1),
    maxLostHits = cms.int32(999),
    maxLostHitsFraction = cms.double(0.1),
    maxNumberOfHits = cms.int32(100),
    minGoodStripCharge = cms.PSet(
        refToPSet_ = cms.string('HLTSiStripClusterChargeCutNone')
    ),
    minHitsMinPt = cms.int32(3),
    minNumberOfHitsForLoopers = cms.int32(13),
    minNumberOfHitsPerLoop = cms.int32(4),
    minPt = cms.double(0.1),
    minimumNumberOfHits = cms.int32(4),
    nSigmaMinPt = cms.double(5.0),
    pixelSeedExtension = cms.bool(False),
    seedExtension = cms.int32(0),
    seedPairPenalty = cms.int32(0),
    strictSeedExtension = cms.bool(False)
)

process.HLTPSetLowPtStepTrajectoryBuilder = cms.PSet(
    ComponentType = cms.string('GroupedCkfTrajectoryBuilder'),
    MeasurementTrackerName = cms.string(''),
    TTRHBuilder = cms.string('hltESPTTRHBWithTrackAngle'),
    alwaysUseInvalidHits = cms.bool(True),
    bestHitOnly = cms.bool(True),
    estimator = cms.string('hltESPChi2ChargeMeasurementEstimator9'),
    foundHitBonus = cms.double(5.0),
    inOutTrajectoryFilter = cms.PSet(
        refToPSet_ = cms.string('HLTPSetLowPtStepTrajectoryFilter')
    ),
    intermediateCleaning = cms.bool(True),
    keepOriginalIfRebuildFails = cms.bool(False),
    lockHits = cms.bool(True),
    lostHitPenalty = cms.double(30.0),
    maxCand = cms.int32(4),
    maxDPhiForLooperReconstruction = cms.double(2.0),
    maxPtForLooperReconstruction = cms.double(0.7),
    minNrOfHitsForRebuild = cms.int32(5),
    propagatorAlong = cms.string('PropagatorWithMaterialParabolicMf'),
    propagatorOpposite = cms.string('PropagatorWithMaterialParabolicMfOpposite'),
    requireSeedHitsInRebuild = cms.bool(True),
    trajectoryFilter = cms.PSet(
        refToPSet_ = cms.string('HLTPSetLowPtStepTrajectoryFilter')
    ),
    updator = cms.string('hltESPKFUpdator'),
    useSameTrajFilter = cms.bool(True)
)

process.HLTPSetLowPtStepTrajectoryFilter = cms.PSet(
    ComponentType = cms.string('CkfBaseTrajectoryFilter'),
    chargeSignificance = cms.double(-1.0),
    constantValueForLostHitsFractionFilter = cms.double(2.0),
    extraNumberOfHitsBeforeTheFirstLoop = cms.int32(4),
    maxCCCLostHits = cms.int32(1),
    maxConsecLostHits = cms.int32(1),
    maxLostHits = cms.int32(999),
    maxLostHitsFraction = cms.double(0.1),
    maxNumberOfHits = cms.int32(100),
    minGoodStripCharge = cms.PSet(
        refToPSet_ = cms.string('HLTSiStripClusterChargeCutLoose')
    ),
    minHitsMinPt = cms.int32(3),
    minNumberOfHitsForLoopers = cms.int32(13),
    minNumberOfHitsPerLoop = cms.int32(4),
    minPt = cms.double(0.075),
    minimumNumberOfHits = cms.int32(3),
    nSigmaMinPt = cms.double(5.0),
    seedExtension = cms.int32(0),
    seedPairPenalty = cms.int32(0),
    strictSeedExtension = cms.bool(False)
)

process.HLTPSetMixedStepTrajectoryBuilder = cms.PSet(
    ComponentType = cms.string('GroupedCkfTrajectoryBuilder'),
    MeasurementTrackerName = cms.string(''),
    TTRHBuilder = cms.string('hltESPTTRHBWithTrackAngle'),
    alwaysUseInvalidHits = cms.bool(True),
    bestHitOnly = cms.bool(True),
    estimator = cms.string('hltESPChi2ChargeTightMeasurementEstimator16'),
    foundHitBonus = cms.double(5.0),
    inOutTrajectoryFilter = cms.PSet(
        refToPSet_ = cms.string('HLTPSetMixedStepTrajectoryFilter')
    ),
    intermediateCleaning = cms.bool(True),
    keepOriginalIfRebuildFails = cms.bool(False),
    lockHits = cms.bool(True),
    lostHitPenalty = cms.double(30.0),
    maxCand = cms.int32(2),
    maxDPhiForLooperReconstruction = cms.double(2.0),
    maxPtForLooperReconstruction = cms.double(0.7),
    minNrOfHitsForRebuild = cms.int32(5),
    propagatorAlong = cms.string('PropagatorWithMaterialForMixedStep'),
    propagatorOpposite = cms.string('PropagatorWithMaterialForMixedStepOpposite'),
    requireSeedHitsInRebuild = cms.bool(True),
    trajectoryFilter = cms.PSet(
        refToPSet_ = cms.string('HLTPSetMixedStepTrajectoryFilter')
    ),
    updator = cms.string('hltESPKFUpdator'),
    useSameTrajFilter = cms.bool(True)
)

process.HLTPSetMixedStepTrajectoryFilter = cms.PSet(
    ComponentType = cms.string('CkfBaseTrajectoryFilter'),
    chargeSignificance = cms.double(-1.0),
    constantValueForLostHitsFractionFilter = cms.double(1.4),
    extraNumberOfHitsBeforeTheFirstLoop = cms.int32(4),
    maxCCCLostHits = cms.int32(9999),
    maxConsecLostHits = cms.int32(1),
    maxLostHits = cms.int32(999),
    maxLostHitsFraction = cms.double(0.1),
    maxNumberOfHits = cms.int32(100),
    minGoodStripCharge = cms.PSet(
        refToPSet_ = cms.string('HLTSiStripClusterChargeCutNone')
    ),
    minHitsMinPt = cms.int32(3),
    minNumberOfHitsForLoopers = cms.int32(13),
    minNumberOfHitsPerLoop = cms.int32(4),
    minPt = cms.double(0.1),
    minimumNumberOfHits = cms.int32(3),
    nSigmaMinPt = cms.double(5.0),
    seedExtension = cms.int32(0),
    seedPairPenalty = cms.int32(0),
    strictSeedExtension = cms.bool(False)
)

process.HLTPSetMixedStepTrajectoryFilterBase = cms.PSet(
    ComponentType = cms.string('CkfBaseTrajectoryFilter'),
    chargeSignificance = cms.double(-1.0),
    constantValueForLostHitsFractionFilter = cms.double(2.0),
    extraNumberOfHitsBeforeTheFirstLoop = cms.int32(4),
    maxCCCLostHits = cms.int32(9999),
    maxConsecLostHits = cms.int32(1),
    maxLostHits = cms.int32(0),
    maxLostHitsFraction = cms.double(0.1),
    maxNumberOfHits = cms.int32(100),
    minGoodStripCharge = cms.PSet(
        refToPSet_ = cms.string('HLTSiStripClusterChargeCutNone')
    ),
    minHitsMinPt = cms.int32(3),
    minNumberOfHitsForLoopers = cms.int32(13),
    minNumberOfHitsPerLoop = cms.int32(4),
    minPt = cms.double(0.05),
    minimumNumberOfHits = cms.int32(3),
    nSigmaMinPt = cms.double(5.0),
    seedExtension = cms.int32(0),
    seedPairPenalty = cms.int32(0),
    strictSeedExtension = cms.bool(False)
)

process.HLTPSetMuTrackJpsiEffTrajectoryBuilder = cms.PSet(
    ComponentType = cms.string('CkfTrajectoryBuilder'),
    MeasurementTrackerName = cms.string('hltESPMeasurementTracker'),
    TTRHBuilder = cms.string('hltESPTTRHBWithTrackAngle'),
    alwaysUseInvalidHits = cms.bool(False),
    estimator = cms.string('hltESPChi2ChargeMeasurementEstimator30'),
    intermediateCleaning = cms.bool(True),
    lostHitPenalty = cms.double(30.0),
    maxCand = cms.int32(1),
    propagatorAlong = cms.string('PropagatorWithMaterial'),
    propagatorOpposite = cms.string('PropagatorWithMaterialOpposite'),
    trajectoryFilter = cms.PSet(
        refToPSet_ = cms.string('HLTPSetMuTrackJpsiEffTrajectoryFilter')
    ),
    updator = cms.string('hltESPKFUpdator')
)

process.HLTPSetMuTrackJpsiEffTrajectoryFilter = cms.PSet(
    ComponentType = cms.string('CkfBaseTrajectoryFilter'),
    chargeSignificance = cms.double(-1.0),
    constantValueForLostHitsFractionFilter = cms.double(1.0),
    extraNumberOfHitsBeforeTheFirstLoop = cms.int32(4),
    maxCCCLostHits = cms.int32(9999),
    maxConsecLostHits = cms.int32(1),
    maxLostHits = cms.int32(1),
    maxLostHitsFraction = cms.double(999.0),
    maxNumberOfHits = cms.int32(9),
    minGoodStripCharge = cms.PSet(
        refToPSet_ = cms.string('HLTSiStripClusterChargeCutNone')
    ),
    minHitsMinPt = cms.int32(3),
    minNumberOfHitsForLoopers = cms.int32(13),
    minNumberOfHitsPerLoop = cms.int32(4),
    minPt = cms.double(1.0),
    minimumNumberOfHits = cms.int32(5),
    nSigmaMinPt = cms.double(5.0),
    seedExtension = cms.int32(0),
    seedPairPenalty = cms.int32(0),
    strictSeedExtension = cms.bool(False)
)

process.HLTPSetMuTrackJpsiTrajectoryBuilder = cms.PSet(
    ComponentType = cms.string('CkfTrajectoryBuilder'),
    MeasurementTrackerName = cms.string('hltESPMeasurementTracker'),
    TTRHBuilder = cms.string('hltESPTTRHBWithTrackAngle'),
    alwaysUseInvalidHits = cms.bool(False),
    estimator = cms.string('hltESPChi2ChargeMeasurementEstimator30'),
    intermediateCleaning = cms.bool(True),
    lostHitPenalty = cms.double(30.0),
    maxCand = cms.int32(1),
    propagatorAlong = cms.string('PropagatorWithMaterial'),
    propagatorOpposite = cms.string('PropagatorWithMaterialOpposite'),
    trajectoryFilter = cms.PSet(
        refToPSet_ = cms.string('HLTPSetMuTrackJpsiTrajectoryFilter')
    ),
    updator = cms.string('hltESPKFUpdator')
)

process.HLTPSetMuTrackJpsiTrajectoryFilter = cms.PSet(
    ComponentType = cms.string('CkfBaseTrajectoryFilter'),
    chargeSignificance = cms.double(-1.0),
    constantValueForLostHitsFractionFilter = cms.double(1.0),
    extraNumberOfHitsBeforeTheFirstLoop = cms.int32(4),
    maxCCCLostHits = cms.int32(9999),
    maxConsecLostHits = cms.int32(1),
    maxLostHits = cms.int32(1),
    maxLostHitsFraction = cms.double(999.0),
    maxNumberOfHits = cms.int32(8),
    minGoodStripCharge = cms.PSet(
        refToPSet_ = cms.string('HLTSiStripClusterChargeCutNone')
    ),
    minHitsMinPt = cms.int32(3),
    minNumberOfHitsForLoopers = cms.int32(13),
    minNumberOfHitsPerLoop = cms.int32(4),
    minPt = cms.double(10.0),
    minimumNumberOfHits = cms.int32(5),
    nSigmaMinPt = cms.double(5.0),
    seedExtension = cms.int32(0),
    seedPairPenalty = cms.int32(0),
    strictSeedExtension = cms.bool(False)
)

process.HLTPSetMuonCkfTrajectoryBuilder = cms.PSet(
    ComponentType = cms.string('MuonCkfTrajectoryBuilder'),
    MeasurementTrackerName = cms.string('hltESPMeasurementTracker'),
    TTRHBuilder = cms.string('hltESPTTRHBWithTrackAngle'),
    alwaysUseInvalidHits = cms.bool(True),
    deltaEta = cms.double(-1.0),
    deltaPhi = cms.double(-1.0),
    estimator = cms.string('hltESPChi2ChargeMeasurementEstimator30'),
    intermediateCleaning = cms.bool(False),
    lostHitPenalty = cms.double(30.0),
    maxCand = cms.int32(5),
    propagatorAlong = cms.string('PropagatorWithMaterial'),
    propagatorOpposite = cms.string('PropagatorWithMaterialOpposite'),
    propagatorProximity = cms.string('SteppingHelixPropagatorAny'),
    rescaleErrorIfFail = cms.double(1.0),
    trajectoryFilter = cms.PSet(
        refToPSet_ = cms.string('HLTPSetMuonCkfTrajectoryFilter')
    ),
    updator = cms.string('hltESPKFUpdator'),
    useSeedLayer = cms.bool(False)
)

process.HLTPSetMuonCkfTrajectoryBuilderSeedHit = cms.PSet(
    ComponentType = cms.string('MuonCkfTrajectoryBuilder'),
    MeasurementTrackerName = cms.string('hltESPMeasurementTracker'),
    TTRHBuilder = cms.string('hltESPTTRHBWithTrackAngle'),
    alwaysUseInvalidHits = cms.bool(True),
    deltaEta = cms.double(-1.0),
    deltaPhi = cms.double(-1.0),
    estimator = cms.string('hltESPChi2ChargeMeasurementEstimator30'),
    intermediateCleaning = cms.bool(False),
    lostHitPenalty = cms.double(30.0),
    maxCand = cms.int32(5),
    propagatorAlong = cms.string('PropagatorWithMaterial'),
    propagatorOpposite = cms.string('PropagatorWithMaterialOpposite'),
    propagatorProximity = cms.string('SteppingHelixPropagatorAny'),
    rescaleErrorIfFail = cms.double(1.0),
    trajectoryFilter = cms.PSet(
        refToPSet_ = cms.string('HLTPSetMuonCkfTrajectoryFilter')
    ),
    updator = cms.string('hltESPKFUpdator'),
    useSeedLayer = cms.bool(True)
)

process.HLTPSetMuonCkfTrajectoryFilter = cms.PSet(
    ComponentType = cms.string('CkfBaseTrajectoryFilter'),
    chargeSignificance = cms.double(-1.0),
    constantValueForLostHitsFractionFilter = cms.double(1.0),
    extraNumberOfHitsBeforeTheFirstLoop = cms.int32(4),
    maxCCCLostHits = cms.int32(9999),
    maxConsecLostHits = cms.int32(1),
    maxLostHits = cms.int32(1),
    maxLostHitsFraction = cms.double(999.0),
    maxNumberOfHits = cms.int32(-1),
    minGoodStripCharge = cms.PSet(
        refToPSet_ = cms.string('HLTSiStripClusterChargeCutNone')
    ),
    minHitsMinPt = cms.int32(3),
    minNumberOfHitsForLoopers = cms.int32(13),
    minNumberOfHitsPerLoop = cms.int32(4),
    minPt = cms.double(0.9),
    minimumNumberOfHits = cms.int32(5),
    nSigmaMinPt = cms.double(5.0),
    seedExtension = cms.int32(0),
    seedPairPenalty = cms.int32(0),
    strictSeedExtension = cms.bool(False)
)

process.HLTPSetMuonTrackingRegionBuilder8356 = cms.PSet(
    DeltaEta = cms.double(0.2),
    DeltaPhi = cms.double(0.2),
    DeltaR = cms.double(0.2),
    DeltaZ = cms.double(15.9),
    EtaR_UpperLimit_Par1 = cms.double(0.25),
    EtaR_UpperLimit_Par2 = cms.double(0.15),
    Eta_fixed = cms.bool(False),
    Eta_min = cms.double(0.1),
    MeasurementTrackerName = cms.InputTag("hltESPMeasurementTracker"),
    OnDemand = cms.int32(-1),
    PhiR_UpperLimit_Par1 = cms.double(0.6),
    PhiR_UpperLimit_Par2 = cms.double(0.2),
    Phi_fixed = cms.bool(False),
    Phi_min = cms.double(0.1),
    Pt_fixed = cms.bool(False),
    Pt_min = cms.double(1.5),
    Rescale_Dz = cms.double(3.0),
    Rescale_eta = cms.double(3.0),
    Rescale_phi = cms.double(3.0),
    UseVertex = cms.bool(False),
    Z_fixed = cms.bool(True),
    beamSpot = cms.InputTag("hltOnlineBeamSpot"),
    input = cms.InputTag("hltL2Muons","UpdatedAtVtx"),
    maxRegions = cms.int32(2),
    precise = cms.bool(True),
    vertexCollection = cms.InputTag("pixelVertices")
)

process.HLTPSetPixelLessStepTrajectoryBuilder = cms.PSet(
    ComponentType = cms.string('GroupedCkfTrajectoryBuilder'),
    MeasurementTrackerName = cms.string(''),
    TTRHBuilder = cms.string('hltESPTTRHBWithTrackAngle'),
    alwaysUseInvalidHits = cms.bool(False),
    bestHitOnly = cms.bool(True),
    estimator = cms.string('hltESPChi2ChargeTightMeasurementEstimator16'),
    foundHitBonus = cms.double(5.0),
    inOutTrajectoryFilter = cms.PSet(
        refToPSet_ = cms.string('HLTPSetPixelLessStepTrajectoryFilter')
    ),
    intermediateCleaning = cms.bool(True),
    keepOriginalIfRebuildFails = cms.bool(False),
    lockHits = cms.bool(True),
    lostHitPenalty = cms.double(30.0),
    maxCand = cms.int32(2),
    maxDPhiForLooperReconstruction = cms.double(2.0),
    maxPtForLooperReconstruction = cms.double(0.7),
    minNrOfHitsForRebuild = cms.int32(4),
    propagatorAlong = cms.string('PropagatorWithMaterialParabolicMf'),
    propagatorOpposite = cms.string('PropagatorWithMaterialParabolicMfOpposite'),
    requireSeedHitsInRebuild = cms.bool(True),
    trajectoryFilter = cms.PSet(
        refToPSet_ = cms.string('HLTPSetPixelLessStepTrajectoryFilter')
    ),
    updator = cms.string('hltESPKFUpdator'),
    useSameTrajFilter = cms.bool(True)
)

process.HLTPSetPixelLessStepTrajectoryFilter = cms.PSet(
    ComponentType = cms.string('CkfBaseTrajectoryFilter'),
    chargeSignificance = cms.double(-1.0),
    constantValueForLostHitsFractionFilter = cms.double(2.0),
    extraNumberOfHitsBeforeTheFirstLoop = cms.int32(4),
    maxCCCLostHits = cms.int32(9999),
    maxConsecLostHits = cms.int32(1),
    maxLostHits = cms.int32(0),
    maxLostHitsFraction = cms.double(0.1),
    maxNumberOfHits = cms.int32(100),
    minGoodStripCharge = cms.PSet(
        refToPSet_ = cms.string('HLTSiStripClusterChargeCutNone')
    ),
    minHitsMinPt = cms.int32(3),
    minNumberOfHitsForLoopers = cms.int32(13),
    minNumberOfHitsPerLoop = cms.int32(4),
    minPt = cms.double(0.1),
    minimumNumberOfHits = cms.int32(4),
    nSigmaMinPt = cms.double(5.0),
    seedExtension = cms.int32(0),
    seedPairPenalty = cms.int32(1),
    strictSeedExtension = cms.bool(False)
)

process.HLTPSetPixelLessStepTrajectoryFilterBase = cms.PSet(
    ComponentType = cms.string('CkfBaseTrajectoryFilter'),
    chargeSignificance = cms.double(-1.0),
    constantValueForLostHitsFractionFilter = cms.double(1.0),
    extraNumberOfHitsBeforeTheFirstLoop = cms.int32(4),
    maxCCCLostHits = cms.int32(9999),
    maxConsecLostHits = cms.int32(1),
    maxLostHits = cms.int32(0),
    maxLostHitsFraction = cms.double(0.1),
    maxNumberOfHits = cms.int32(100),
    minGoodStripCharge = cms.PSet(
        refToPSet_ = cms.string('HLTSiStripClusterChargeCutNone')
    ),
    minHitsMinPt = cms.int32(3),
    minNumberOfHitsForLoopers = cms.int32(13),
    minNumberOfHitsPerLoop = cms.int32(4),
    minPt = cms.double(0.05),
    minimumNumberOfHits = cms.int32(4),
    nSigmaMinPt = cms.double(5.0),
    seedExtension = cms.int32(0),
    seedPairPenalty = cms.int32(0),
    strictSeedExtension = cms.bool(False)
)

process.HLTPSetPixelPairCkfTrajectoryBuilderForHI = cms.PSet(
    ComponentType = cms.string('GroupedCkfTrajectoryBuilder'),
    MeasurementTrackerName = cms.string(''),
    TTRHBuilder = cms.string('hltESPTTRHBWithTrackAngle'),
    alwaysUseInvalidHits = cms.bool(True),
    bestHitOnly = cms.bool(True),
    estimator = cms.string('hltESPChi2ChargeMeasurementEstimator9ForHI'),
    foundHitBonus = cms.double(5.0),
    inOutTrajectoryFilter = cms.PSet(
        refToPSet_ = cms.string('HLTPSetPixelPairCkfTrajectoryFilterForHI')
    ),
    intermediateCleaning = cms.bool(True),
    keepOriginalIfRebuildFails = cms.bool(False),
    lockHits = cms.bool(True),
    lostHitPenalty = cms.double(30.0),
    maxCand = cms.int32(3),
    maxDPhiForLooperReconstruction = cms.double(2.0),
    maxPtForLooperReconstruction = cms.double(0.7),
    minNrOfHitsForRebuild = cms.int32(5),
    propagatorAlong = cms.string('PropagatorWithMaterialForHI'),
    propagatorOpposite = cms.string('PropagatorWithMaterialOppositeForHI'),
    requireSeedHitsInRebuild = cms.bool(True),
    trajectoryFilter = cms.PSet(
        refToPSet_ = cms.string('HLTPSetPixelPairCkfTrajectoryFilterForHI')
    ),
    updator = cms.string('hltESPKFUpdator'),
    useSameTrajFilter = cms.bool(True)
)

process.HLTPSetPixelPairCkfTrajectoryBuilderForHIGlobalPt8 = cms.PSet(
    ComponentType = cms.string('GroupedCkfTrajectoryBuilder'),
    MeasurementTrackerName = cms.string(''),
    TTRHBuilder = cms.string('hltESPTTRHBWithTrackAngle'),
    alwaysUseInvalidHits = cms.bool(True),
    bestHitOnly = cms.bool(True),
    estimator = cms.string('hltESPChi2ChargeMeasurementEstimator9ForHI'),
    foundHitBonus = cms.double(5.0),
    inOutTrajectoryFilter = cms.PSet(
        refToPSet_ = cms.string('HLTPSetPixelPairCkfTrajectoryFilterForHIGlobalPt8')
    ),
    intermediateCleaning = cms.bool(True),
    keepOriginalIfRebuildFails = cms.bool(False),
    lockHits = cms.bool(True),
    lostHitPenalty = cms.double(30.0),
    maxCand = cms.int32(3),
    maxDPhiForLooperReconstruction = cms.double(2.0),
    maxPtForLooperReconstruction = cms.double(0.7),
    minNrOfHitsForRebuild = cms.int32(5),
    propagatorAlong = cms.string('PropagatorWithMaterialForHI'),
    propagatorOpposite = cms.string('PropagatorWithMaterialOppositeForHI'),
    requireSeedHitsInRebuild = cms.bool(True),
    trajectoryFilter = cms.PSet(
        refToPSet_ = cms.string('HLTPSetPixelPairCkfTrajectoryFilterForHIGlobalPt8')
    ),
    updator = cms.string('hltESPKFUpdator'),
    useSameTrajFilter = cms.bool(True)
)

process.HLTPSetPixelPairCkfTrajectoryFilterForHI = cms.PSet(
    ComponentType = cms.string('CkfBaseTrajectoryFilter'),
    chargeSignificance = cms.double(-1.0),
    constantValueForLostHitsFractionFilter = cms.double(1.0),
    extraNumberOfHitsBeforeTheFirstLoop = cms.int32(4),
    maxCCCLostHits = cms.int32(9999),
    maxConsecLostHits = cms.int32(1),
    maxLostHits = cms.int32(100),
    maxLostHitsFraction = cms.double(999.0),
    maxNumberOfHits = cms.int32(100),
    minGoodStripCharge = cms.PSet(
        refToPSet_ = cms.string('HLTSiStripClusterChargeCutNone')
    ),
    minHitsMinPt = cms.int32(3),
    minNumberOfHitsForLoopers = cms.int32(13),
    minNumberOfHitsPerLoop = cms.int32(4),
    minPt = cms.double(1.0),
    minimumNumberOfHits = cms.int32(6),
    nSigmaMinPt = cms.double(5.0),
    seedExtension = cms.int32(0),
    seedPairPenalty = cms.int32(0),
    strictSeedExtension = cms.bool(False)
)

process.HLTPSetPixelPairCkfTrajectoryFilterForHIGlobalPt8 = cms.PSet(
    ComponentType = cms.string('CkfBaseTrajectoryFilter'),
    chargeSignificance = cms.double(-1.0),
    constantValueForLostHitsFractionFilter = cms.double(1.0),
    extraNumberOfHitsBeforeTheFirstLoop = cms.int32(4),
    maxCCCLostHits = cms.int32(9999),
    maxConsecLostHits = cms.int32(1),
    maxLostHits = cms.int32(100),
    maxLostHitsFraction = cms.double(999.0),
    maxNumberOfHits = cms.int32(100),
    minGoodStripCharge = cms.PSet(
        refToPSet_ = cms.string('HLTSiStripClusterChargeCutNone')
    ),
    minHitsMinPt = cms.int32(3),
    minNumberOfHitsForLoopers = cms.int32(13),
    minNumberOfHitsPerLoop = cms.int32(4),
    minPt = cms.double(8.0),
    minimumNumberOfHits = cms.int32(6),
    nSigmaMinPt = cms.double(5.0),
    seedExtension = cms.int32(0),
    seedPairPenalty = cms.int32(0),
    strictSeedExtension = cms.bool(False)
)

process.HLTPSetPixelPairStepTrajectoryBuilder = cms.PSet(
    ComponentType = cms.string('GroupedCkfTrajectoryBuilder'),
    MeasurementTrackerName = cms.string(''),
    TTRHBuilder = cms.string('hltESPTTRHBWithTrackAngle'),
    alwaysUseInvalidHits = cms.bool(True),
    bestHitOnly = cms.bool(True),
    estimator = cms.string('hltESPChi2ChargeMeasurementEstimator9'),
    foundHitBonus = cms.double(5.0),
    inOutTrajectoryFilter = cms.PSet(
        refToPSet_ = cms.string('HLTPSetPixelPairStepTrajectoryFilter')
    ),
    intermediateCleaning = cms.bool(True),
    keepOriginalIfRebuildFails = cms.bool(False),
    lockHits = cms.bool(True),
    lostHitPenalty = cms.double(30.0),
    maxCand = cms.int32(3),
    maxDPhiForLooperReconstruction = cms.double(2.0),
    maxPtForLooperReconstruction = cms.double(0.7),
    minNrOfHitsForRebuild = cms.int32(5),
    propagatorAlong = cms.string('PropagatorWithMaterialParabolicMf'),
    propagatorOpposite = cms.string('PropagatorWithMaterialParabolicMfOpposite'),
    requireSeedHitsInRebuild = cms.bool(True),
    trajectoryFilter = cms.PSet(
        refToPSet_ = cms.string('HLTPSetPixelPairStepTrajectoryFilter')
    ),
    updator = cms.string('hltESPKFUpdator'),
    useSameTrajFilter = cms.bool(True)
)

process.HLTPSetPixelPairStepTrajectoryFilter = cms.PSet(
    ComponentType = cms.string('CompositeTrajectoryFilter'),
    filters = cms.VPSet(cms.PSet(
        refToPSet_ = cms.string('HLTPSetPixelPairStepTrajectoryFilterBase')
    ))
)

process.HLTPSetPixelPairStepTrajectoryFilterBase = cms.PSet(
    ComponentType = cms.string('CkfBaseTrajectoryFilter'),
    chargeSignificance = cms.double(-1.0),
    constantValueForLostHitsFractionFilter = cms.double(2.0),
    extraNumberOfHitsBeforeTheFirstLoop = cms.int32(4),
    maxCCCLostHits = cms.int32(2),
    maxConsecLostHits = cms.int32(1),
    maxLostHits = cms.int32(999),
    maxLostHitsFraction = cms.double(0.1),
    maxNumberOfHits = cms.int32(100),
    minGoodStripCharge = cms.PSet(
        refToPSet_ = cms.string('HLTSiStripClusterChargeCutLoose')
    ),
    minHitsMinPt = cms.int32(3),
    minNumberOfHitsForLoopers = cms.int32(13),
    minNumberOfHitsPerLoop = cms.int32(4),
    minPt = cms.double(0.1),
    minimumNumberOfHits = cms.int32(3),
    nSigmaMinPt = cms.double(5.0),
    seedExtension = cms.int32(0),
    seedPairPenalty = cms.int32(0),
    strictSeedExtension = cms.bool(False)
)

process.HLTPSetPvClusterComparer = cms.PSet(
    track_chi2_max = cms.double(9999999.0),
    track_prob_min = cms.double(-1.0),
    track_pt_max = cms.double(10.0),
    track_pt_min = cms.double(2.5)
)

process.HLTPSetPvClusterComparerForBTag = cms.PSet(
    track_chi2_max = cms.double(20.0),
    track_prob_min = cms.double(-1.0),
    track_pt_max = cms.double(20.0),
    track_pt_min = cms.double(0.1)
)

process.HLTPSetPvClusterComparerForIT = cms.PSet(
    track_chi2_max = cms.double(20.0),
    track_prob_min = cms.double(-1.0),
    track_pt_max = cms.double(20.0),
    track_pt_min = cms.double(1.0)
)

process.HLTPSetTobTecStepInOutTrajectoryFilterBase = cms.PSet(
    ComponentType = cms.string('CkfBaseTrajectoryFilter'),
    chargeSignificance = cms.double(-1.0),
    constantValueForLostHitsFractionFilter = cms.double(2.0),
    extraNumberOfHitsBeforeTheFirstLoop = cms.int32(4),
    maxCCCLostHits = cms.int32(9999),
    maxConsecLostHits = cms.int32(1),
    maxLostHits = cms.int32(0),
    maxLostHitsFraction = cms.double(0.1),
    maxNumberOfHits = cms.int32(100),
    minGoodStripCharge = cms.PSet(
        refToPSet_ = cms.string('HLTSiStripClusterChargeCutNone')
    ),
    minHitsMinPt = cms.int32(3),
    minNumberOfHitsForLoopers = cms.int32(13),
    minNumberOfHitsPerLoop = cms.int32(4),
    minPt = cms.double(0.1),
    minimumNumberOfHits = cms.int32(4),
    nSigmaMinPt = cms.double(5.0),
    seedExtension = cms.int32(0),
    seedPairPenalty = cms.int32(1),
    strictSeedExtension = cms.bool(False)
)

process.HLTPSetTobTecStepTrajectoryBuilder = cms.PSet(
    ComponentType = cms.string('GroupedCkfTrajectoryBuilder'),
    MeasurementTrackerName = cms.string(''),
    TTRHBuilder = cms.string('hltESPTTRHBWithTrackAngle'),
    alwaysUseInvalidHits = cms.bool(False),
    bestHitOnly = cms.bool(True),
    estimator = cms.string('hltESPChi2ChargeTightMeasurementEstimator16'),
    foundHitBonus = cms.double(5.0),
    inOutTrajectoryFilter = cms.PSet(
        refToPSet_ = cms.string('HLTPSetTobTecStepInOutTrajectoryFilterBase')
    ),
    intermediateCleaning = cms.bool(True),
    keepOriginalIfRebuildFails = cms.bool(False),
    lockHits = cms.bool(True),
    lostHitPenalty = cms.double(30.0),
    maxCand = cms.int32(2),
    maxDPhiForLooperReconstruction = cms.double(2.0),
    maxPtForLooperReconstruction = cms.double(0.7),
    minNrOfHitsForRebuild = cms.int32(4),
    propagatorAlong = cms.string('PropagatorWithMaterialParabolicMf'),
    propagatorOpposite = cms.string('PropagatorWithMaterialParabolicMfOpposite'),
    requireSeedHitsInRebuild = cms.bool(True),
    trajectoryFilter = cms.PSet(
        refToPSet_ = cms.string('HLTPSetTobTecStepTrajectoryFilterBase')
    ),
    updator = cms.string('hltESPKFUpdator'),
    useSameTrajFilter = cms.bool(False)
)

process.HLTPSetTobTecStepTrajectoryFilterBase = cms.PSet(
    ComponentType = cms.string('CkfBaseTrajectoryFilter'),
    chargeSignificance = cms.double(-1.0),
    constantValueForLostHitsFractionFilter = cms.double(2.0),
    extraNumberOfHitsBeforeTheFirstLoop = cms.int32(4),
    maxCCCLostHits = cms.int32(9999),
    maxConsecLostHits = cms.int32(1),
    maxLostHits = cms.int32(0),
    maxLostHitsFraction = cms.double(0.1),
    maxNumberOfHits = cms.int32(100),
    minGoodStripCharge = cms.PSet(
        refToPSet_ = cms.string('HLTSiStripClusterChargeCutNone')
    ),
    minHitsMinPt = cms.int32(3),
    minNumberOfHitsForLoopers = cms.int32(13),
    minNumberOfHitsPerLoop = cms.int32(4),
    minPt = cms.double(0.1),
    minimumNumberOfHits = cms.int32(5),
    nSigmaMinPt = cms.double(5.0),
    seedExtension = cms.int32(0),
    seedPairPenalty = cms.int32(1),
    strictSeedExtension = cms.bool(False)
)

process.HLTPSetTrajectoryBuilderForElectrons = cms.PSet(
    ComponentType = cms.string('CkfTrajectoryBuilder'),
    MeasurementTrackerName = cms.string('hltESPMeasurementTracker'),
    TTRHBuilder = cms.string('hltESPTTRHBWithTrackAngle'),
    alwaysUseInvalidHits = cms.bool(True),
    estimator = cms.string('hltESPChi2ChargeMeasurementEstimator30'),
    intermediateCleaning = cms.bool(False),
    lostHitPenalty = cms.double(90.0),
    maxCand = cms.int32(5),
    propagatorAlong = cms.string('hltESPFwdElectronPropagator'),
    propagatorOpposite = cms.string('hltESPBwdElectronPropagator'),
    trajectoryFilter = cms.PSet(
        refToPSet_ = cms.string('HLTPSetTrajectoryFilterForElectrons')
    ),
    updator = cms.string('hltESPKFUpdator')
)

process.HLTPSetTrajectoryBuilderForGsfElectrons = cms.PSet(
    ComponentType = cms.string('CkfTrajectoryBuilder'),
    MeasurementTrackerName = cms.string('hltESPMeasurementTracker'),
    TTRHBuilder = cms.string('hltESPTTRHBWithTrackAngle'),
    alwaysUseInvalidHits = cms.bool(True),
    estimator = cms.string('hltESPChi2ChargeMeasurementEstimator2000'),
    intermediateCleaning = cms.bool(False),
    lostHitPenalty = cms.double(90.0),
    maxCand = cms.int32(5),
    propagatorAlong = cms.string('hltESPFwdElectronPropagator'),
    propagatorOpposite = cms.string('hltESPBwdElectronPropagator'),
    trajectoryFilter = cms.PSet(
        refToPSet_ = cms.string('HLTPSetTrajectoryFilterForElectrons')
    ),
    updator = cms.string('hltESPKFUpdator')
)

process.HLTPSetTrajectoryFilterForElectrons = cms.PSet(
    ComponentType = cms.string('CkfBaseTrajectoryFilter'),
    chargeSignificance = cms.double(-1.0),
    constantValueForLostHitsFractionFilter = cms.double(1.0),
    extraNumberOfHitsBeforeTheFirstLoop = cms.int32(4),
    maxCCCLostHits = cms.int32(9999),
    maxConsecLostHits = cms.int32(1),
    maxLostHits = cms.int32(1),
    maxLostHitsFraction = cms.double(999.0),
    maxNumberOfHits = cms.int32(-1),
    minGoodStripCharge = cms.PSet(
        refToPSet_ = cms.string('HLTSiStripClusterChargeCutNone')
    ),
    minHitsMinPt = cms.int32(-1),
    minNumberOfHitsForLoopers = cms.int32(13),
    minNumberOfHitsPerLoop = cms.int32(4),
    minPt = cms.double(2.0),
    minimumNumberOfHits = cms.int32(5),
    nSigmaMinPt = cms.double(5.0),
    seedExtension = cms.int32(0),
    seedPairPenalty = cms.int32(0),
    strictSeedExtension = cms.bool(False)
)

process.HLTPSetTrajectoryFilterIT = cms.PSet(
    ComponentType = cms.string('CkfBaseTrajectoryFilter'),
    chargeSignificance = cms.double(-1.0),
    constantValueForLostHitsFractionFilter = cms.double(1.0),
    extraNumberOfHitsBeforeTheFirstLoop = cms.int32(4),
    maxCCCLostHits = cms.int32(9999),
    maxConsecLostHits = cms.int32(1),
    maxLostHits = cms.int32(1),
    maxLostHitsFraction = cms.double(999.0),
    maxNumberOfHits = cms.int32(100),
    minGoodStripCharge = cms.PSet(
        refToPSet_ = cms.string('HLTSiStripClusterChargeCutNone')
    ),
    minHitsMinPt = cms.int32(3),
    minNumberOfHitsForLoopers = cms.int32(13),
    minNumberOfHitsPerLoop = cms.int32(4),
    minPt = cms.double(0.3),
    minimumNumberOfHits = cms.int32(3),
    nSigmaMinPt = cms.double(5.0),
    seedExtension = cms.int32(0),
    seedPairPenalty = cms.int32(0),
    strictSeedExtension = cms.bool(False)
)

process.HLTPSetTrajectoryFilterL3 = cms.PSet(
    ComponentType = cms.string('CkfBaseTrajectoryFilter'),
    chargeSignificance = cms.double(-1.0),
    constantValueForLostHitsFractionFilter = cms.double(1.0),
    extraNumberOfHitsBeforeTheFirstLoop = cms.int32(4),
    maxCCCLostHits = cms.int32(9999),
    maxConsecLostHits = cms.int32(1),
    maxLostHits = cms.int32(1),
    maxLostHitsFraction = cms.double(999.0),
    maxNumberOfHits = cms.int32(1000000000),
    minGoodStripCharge = cms.PSet(
        refToPSet_ = cms.string('HLTSiStripClusterChargeCutNone')
    ),
    minHitsMinPt = cms.int32(3),
    minNumberOfHitsForLoopers = cms.int32(13),
    minNumberOfHitsPerLoop = cms.int32(4),
    minPt = cms.double(0.5),
    minimumNumberOfHits = cms.int32(5),
    nSigmaMinPt = cms.double(5.0),
    seedExtension = cms.int32(0),
    seedPairPenalty = cms.int32(0),
    strictSeedExtension = cms.bool(False)
)

process.HLTPSetbJetRegionalTrajectoryFilter = cms.PSet(
    ComponentType = cms.string('CkfBaseTrajectoryFilter'),
    chargeSignificance = cms.double(-1.0),
    constantValueForLostHitsFractionFilter = cms.double(1.0),
    extraNumberOfHitsBeforeTheFirstLoop = cms.int32(4),
    maxCCCLostHits = cms.int32(9999),
    maxConsecLostHits = cms.int32(1),
    maxLostHits = cms.int32(1),
    maxLostHitsFraction = cms.double(999.0),
    maxNumberOfHits = cms.int32(8),
    minGoodStripCharge = cms.PSet(
        refToPSet_ = cms.string('HLTSiStripClusterChargeCutNone')
    ),
    minHitsMinPt = cms.int32(3),
    minNumberOfHitsForLoopers = cms.int32(13),
    minNumberOfHitsPerLoop = cms.int32(4),
    minPt = cms.double(1.0),
    minimumNumberOfHits = cms.int32(5),
    nSigmaMinPt = cms.double(5.0),
    seedExtension = cms.int32(0),
    seedPairPenalty = cms.int32(0),
    strictSeedExtension = cms.bool(False)
)

process.HLTSeedFromConsecutiveHitsCreator = cms.PSet(
    ComponentName = cms.string('SeedFromConsecutiveHitsCreator'),
    MinOneOverPtError = cms.double(1.0),
    OriginTransverseErrorMultiplier = cms.double(1.0),
    SeedMomentumForBOFF = cms.double(5.0),
    TTRHBuilder = cms.string('hltESPTTRHBWithTrackAngle'),
    forceKinematicWithRegionDirection = cms.bool(False),
    magneticField = cms.string(''),
    propagator = cms.string('PropagatorWithMaterial')
)

process.HLTSeedFromConsecutiveHitsCreatorIT = cms.PSet(
    ComponentName = cms.string('SeedFromConsecutiveHitsCreator'),
    MinOneOverPtError = cms.double(1.0),
    OriginTransverseErrorMultiplier = cms.double(1.0),
    SeedMomentumForBOFF = cms.double(5.0),
    TTRHBuilder = cms.string('hltESPTTRHBWithTrackAngle'),
    forceKinematicWithRegionDirection = cms.bool(False),
    magneticField = cms.string('ParabolicMf'),
    propagator = cms.string('PropagatorWithMaterialParabolicMf')
)

process.HLTSeedFromConsecutiveHitsTripletOnlyCreator = cms.PSet(
    ComponentName = cms.string('SeedFromConsecutiveHitsTripletOnlyCreator'),
    MinOneOverPtError = cms.double(1.0),
    OriginTransverseErrorMultiplier = cms.double(1.0),
    SeedMomentumForBOFF = cms.double(5.0),
    TTRHBuilder = cms.string('hltESPTTRHBWithTrackAngle'),
    forceKinematicWithRegionDirection = cms.bool(False),
    magneticField = cms.string('ParabolicMf'),
    propagator = cms.string('PropagatorWithMaterialParabolicMf')
)

process.HLTSeedFromProtoTracks = cms.PSet(
    ComponentName = cms.string('SeedFromConsecutiveHitsCreator'),
    MinOneOverPtError = cms.double(1.0),
    OriginTransverseErrorMultiplier = cms.double(1.0),
    SeedMomentumForBOFF = cms.double(5.0),
    TTRHBuilder = cms.string('hltESPTTRHBuilderPixelOnly'),
    forceKinematicWithRegionDirection = cms.bool(False),
    magneticField = cms.string('ParabolicMf'),
    propagator = cms.string('PropagatorWithMaterialParabolicMf')
)

process.HLTSiStripClusterChargeCutForHI = cms.PSet(
    value = cms.double(2069.0)
)

process.HLTSiStripClusterChargeCutLoose = cms.PSet(
    value = cms.double(1620.0)
)

process.HLTSiStripClusterChargeCutNone = cms.PSet(
    value = cms.double(-1.0)
)

process.HLTSiStripClusterChargeCutTight = cms.PSet(
    value = cms.double(1945.0)
)

process.HLTSiStripClusterChargeCutTiny = cms.PSet(
    value = cms.double(800.0)
)

process.datasets = cms.PSet(
    AlCaElectron = cms.vstring('AlCa_DoubleEle_CaloIdL_TrackIdL_IsoVL_DZ_v6', 
        'AlCa_DoubleEle_CaloIdL_TrackIdL_IsoVL_v6', 
        'AlCa_SingleEle_WPVeryLoose_Gsf_v7'),
    AlCaLumiPixels = cms.vstring('AlCa_LumiPixels_Random_v2', 
        'AlCa_LumiPixels_ZeroBias_v5'),
    AlCaP0 = cms.vstring('AlCa_EcalEtaEBonly_v8', 
        'AlCa_EcalEtaEEonly_v8', 
        'AlCa_EcalPi0EBonly_v8', 
        'AlCa_EcalPi0EEonly_v8'),
    AlCaPhiSym = cms.vstring('AlCa_EcalPhiSym_v6'),
    BTagCSV = cms.vstring('HLT_DoubleJet90_Double30_DoubleBTagCSV_p087_v5', 
        'HLT_DoubleJet90_Double30_TripleBTagCSV_p087_v5', 
        'HLT_DoubleJetsC100_DoubleBTagCSV_p014_DoublePFJetsC100MaxDeta1p6_v5', 
        'HLT_DoubleJetsC100_DoubleBTagCSV_p026_DoublePFJetsC160_v5', 
        'HLT_DoubleJetsC100_SingleBTagCSV_p014_SinglePFJetC350_v5', 
        'HLT_DoubleJetsC100_SingleBTagCSV_p014_v3', 
        'HLT_DoubleJetsC100_SingleBTagCSV_p026_SinglePFJetC350_v5', 
        'HLT_DoubleJetsC100_SingleBTagCSV_p026_v3', 
        'HLT_DoubleJetsC112_DoubleBTagCSV_p014_DoublePFJetsC112MaxDeta1p6_v5', 
        'HLT_DoubleJetsC112_DoubleBTagCSV_p026_DoublePFJetsC172_v5', 
        'HLT_QuadJet45_DoubleBTagCSV_p087_v6', 
        'HLT_QuadJet45_TripleBTagCSV_p087_v6', 
        'HLT_QuadPFJet_BTagCSV_p016_VBF_Mqq460_v5', 
        'HLT_QuadPFJet_BTagCSV_p016_VBF_Mqq500_v5', 
        'HLT_QuadPFJet_BTagCSV_p016_p11_VBF_Mqq200_v5', 
        'HLT_QuadPFJet_BTagCSV_p016_p11_VBF_Mqq240_v5', 
        'HLT_Rsq0p02_MR400_TriPFJet80_60_40_DoubleBTagCSV_p063_Mbb60_200_v3', 
        'HLT_Rsq0p02_MR450_TriPFJet80_60_40_DoubleBTagCSV_p063_Mbb60_200_v3', 
        'HLT_Rsq0p02_MR500_TriPFJet80_60_40_DoubleBTagCSV_p063_Mbb60_200_v2', 
        'HLT_Rsq0p02_MR550_TriPFJet80_60_40_DoubleBTagCSV_p063_Mbb60_200_v2'),
    BTagMu = cms.vstring('HLT_BTagMu_AK8Jet300_Mu5_v4', 
        'HLT_BTagMu_DiJet110_Mu5_v5', 
        'HLT_BTagMu_DiJet170_Mu5_v4', 
        'HLT_BTagMu_DiJet20_Mu5_v5', 
        'HLT_BTagMu_DiJet40_Mu5_v5', 
        'HLT_BTagMu_DiJet70_Mu5_v5', 
        'HLT_BTagMu_Jet300_Mu5_v5'),
    Charmonium = cms.vstring('HLT_Dimuon0_Jpsi_Muon_v5', 
        'HLT_Dimuon0er16_Jpsi_NoOS_NoVertexing_v4', 
        'HLT_Dimuon13_PsiPrime_v6', 
        'HLT_Dimuon16_Jpsi_v6', 
        'HLT_Dimuon20_Jpsi_v6', 
        'HLT_Dimuon6_Jpsi_NoVertexing_v4', 
        'HLT_Dimuon8_PsiPrime_Barrel_v6', 
        'HLT_DoubleMu4_3_Bs_v7', 
        'HLT_DoubleMu4_3_Jpsi_Displaced_v7', 
        'HLT_DoubleMu4_JpsiTrk_Displaced_v7', 
        'HLT_DoubleMu4_PsiPrimeTrk_Displaced_v7', 
        'HLT_Mu7p5_L2Mu2_Jpsi_v4', 
        'HLT_Mu7p5_Track2_Jpsi_v4', 
        'HLT_Mu7p5_Track3p5_Jpsi_v4', 
        'HLT_Mu7p5_Track7_Jpsi_v4', 
        'HLT_QuadMuon0_Dimuon0_Jpsi_v4'),
    Commissioning = cms.vstring('HLT_DiSC30_18_EIso_AND_HE_Mass70_v6', 
        'HLT_HcalIsolatedbunch_v2', 
        'HLT_IsoTrackHB_v3', 
        'HLT_IsoTrackHE_v3'),
    DisplacedJet = cms.vstring('HLT_HT200_v4', 
        'HLT_HT250_DisplacedDijet40_DisplacedTrack_v5', 
        'HLT_HT275_v4', 
        'HLT_HT325_v4', 
        'HLT_HT350_DisplacedDijet40_DisplacedTrack_v5', 
        'HLT_HT350_DisplacedDijet40_Inclusive_v4', 
        'HLT_HT350_DisplacedDijet80_DisplacedTrack_v5', 
        'HLT_HT350_DisplacedDijet80_Tight_DisplacedTrack_v5', 
        'HLT_HT425_v4', 
        'HLT_HT550_DisplacedDijet80_Inclusive_v2', 
        'HLT_HT575_v4', 
        'HLT_HT650_DisplacedDijet80_Inclusive_v5', 
        'HLT_HT750_DisplacedDijet80_Inclusive_v5', 
        'HLT_VBF_DisplacedJet40_DisplacedTrack_2TrackIP2DSig5_v5', 
        'HLT_VBF_DisplacedJet40_DisplacedTrack_v5', 
        'HLT_VBF_DisplacedJet40_TightID_DisplacedTrack_v5', 
        'HLT_VBF_DisplacedJet40_TightID_Hadronic_v5', 
        'HLT_VBF_DisplacedJet40_VTightID_DisplacedTrack_v5', 
        'HLT_VBF_DisplacedJet40_VTightID_Hadronic_v5', 
        'HLT_VBF_DisplacedJet40_VVTightID_DisplacedTrack_v5', 
        'HLT_VBF_DisplacedJet40_VVTightID_Hadronic_v5'),
    DoubleEG = cms.vstring('HLT_Diphoton30EB_18EB_R9Id_OR_IsoCaloId_AND_HE_R9Id_DoublePixelVeto_Mass55_v7', 
        'HLT_Diphoton30PV_18PV_R9Id_AND_IsoCaloId_AND_HE_R9Id_DoublePixelVeto_Mass55_v7', 
        'HLT_Diphoton30_18_R9Id_OR_IsoCaloId_AND_HE_R9Id_DoublePixelSeedMatch_Mass70_v7', 
        'HLT_Diphoton30_18_R9Id_OR_IsoCaloId_AND_HE_R9Id_Mass90_v7', 
        'HLT_Diphoton30_18_Solid_R9Id_AND_IsoCaloId_AND_HE_R9Id_Mass55_v7', 
        'HLT_DoubleEle24_22_eta2p1_WPLoose_Gsf_v8', 
        'HLT_DoubleEle33_CaloIdL_GsfTrkIdVL_MW_v10', 
        'HLT_DoubleEle33_CaloIdL_MW_v8', 
        'HLT_DoubleEle33_CaloIdL_v7', 
        'HLT_DoubleEle37_Ele27_CaloIdL_GsfTrkIdVL_v7', 
        'HLT_DoubleEle8_CaloIdM_TrackIdM_Mass8_PFHT300_v10', 
        'HLT_DoublePhoton60_v7', 
        'HLT_DoublePhoton85_v8', 
        'HLT_ECALHT800_v6', 
        'HLT_Ele10_CaloIdM_TrackIdM_CentralPFJet30_BTagCSV_p13_v7', 
        'HLT_Ele12_CaloIdL_TrackIdL_IsoVL_PFJet30_v9', 
        'HLT_Ele12_CaloIdL_TrackIdL_IsoVL_v8', 
        'HLT_Ele12_CaloIdM_TrackIdM_PFJet30_v9', 
        'HLT_Ele16_Ele12_Ele8_CaloIdL_TrackIdL_v8', 
        'HLT_Ele17_CaloIdL_GsfTrkIdVL_v6', 
        'HLT_Ele17_CaloIdL_TrackIdL_IsoVL_PFJet30_v7', 
        'HLT_Ele17_CaloIdL_TrackIdL_IsoVL_v7', 
        'HLT_Ele17_CaloIdM_TrackIdM_PFJet30_v7', 
        'HLT_Ele17_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v9', 
        'HLT_Ele17_Ele12_CaloIdL_TrackIdL_IsoVL_v9', 
        'HLT_Ele23_CaloIdL_TrackIdL_IsoVL_PFJet30_v9', 
        'HLT_Ele23_CaloIdL_TrackIdL_IsoVL_v8', 
        'HLT_Ele23_CaloIdM_TrackIdM_PFJet30_v9', 
        'HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_L1JetTauSeeded_v3', 
        'HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v9', 
        'HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_v9', 
        'HLT_Ele27_HighEta_Ele20_Mass55_v8', 
        'HLT_Ele8_CaloIdL_TrackIdL_IsoVL_PFJet30_v7', 
        'HLT_Ele8_CaloIdM_TrackIdM_PFJet30_v9', 
        'HLT_Photon26_R9Id85_OR_CaloId24b40e_Iso50T80L_Photon16_AND_HE10_R9Id65_Eta2_Mass60_v9', 
        'HLT_Photon36_R9Id85_OR_CaloId24b40e_Iso50T80L_Photon22_AND_HE10_R9Id65_Eta2_Mass15_v9', 
        'HLT_Photon42_R9Id85_OR_CaloId24b40e_Iso50T80L_Photon25_AND_HE10_R9Id65_Eta2_Mass15_v9'),
    DoubleMuon = cms.vstring('HLT_DoubleMu0_v2', 
        'HLT_DoubleMu18NoFiltersNoVtx_v5', 
        'HLT_DoubleMu23NoFiltersNoVtxDisplaced_v5', 
        'HLT_DoubleMu28NoFiltersNoVtxDisplaced_v5', 
        'HLT_DoubleMu33NoFiltersNoVtx_v5', 
        'HLT_DoubleMu38NoFiltersNoVtx_v5', 
        'HLT_DoubleMu8_Mass8_PFHT300_v9', 
        'HLT_L2DoubleMu23_NoVertex_v6', 
        'HLT_L2DoubleMu28_NoVertex_2Cha_Angle2p5_Mass10_v6', 
        'HLT_L2DoubleMu38_NoVertex_2Cha_Angle2p5_Mass10_v6', 
        'HLT_Mu10_CentralPFJet30_BTagCSV_p13_v5', 
        'HLT_Mu17_Mu8_DZ_v7', 
        'HLT_Mu17_Mu8_SameSign_DZ_v6', 
        'HLT_Mu17_Mu8_SameSign_v5', 
        'HLT_Mu17_Mu8_v5', 
        'HLT_Mu17_TkMu8_DZ_v6', 
        'HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v7', 
        'HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_v6', 
        'HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ_v6', 
        'HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_v5', 
        'HLT_Mu17_TrkIsoVVL_v4', 
        'HLT_Mu17_v4', 
        'HLT_Mu20_Mu10_DZ_v6', 
        'HLT_Mu20_Mu10_SameSign_DZ_v6', 
        'HLT_Mu20_Mu10_SameSign_v4', 
        'HLT_Mu20_Mu10_v5', 
        'HLT_Mu27_TkMu8_v5', 
        'HLT_Mu30_TkMu11_v5', 
        'HLT_Mu3_PFJet40_v6', 
        'HLT_Mu40_TkMu11_v5', 
        'HLT_Mu8_TrkIsoVVL_v5', 
        'HLT_Mu8_v5', 
        'HLT_TkMu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ_v3', 
        'HLT_TkMu17_TrkIsoVVL_TkMu8_TrkIsoVVL_v3', 
        'HLT_TripleMu_12_10_5_v4', 
        'HLT_TripleMu_5_3_3_DZ_Mass3p8_v1', 
        'HLT_TrkMu15_DoubleTrkMu5NoFiltersNoVtx_v6', 
        'HLT_TrkMu17_DoubleTrkMu8NoFiltersNoVtx_v6'),
    DoubleMuonLowMass = cms.vstring('HLT_DoubleMu3_Trk_Tau3mu_v4', 
        'HLT_DoubleMu4_LowMassNonResonantTrk_Displaced_v7'),
    EcalLaser = cms.vstring('HLT_EcalCalibration_v3'),
    EmptyBX = cms.vstring('HLT_L1BptxMinus_v2', 
        'HLT_L1BptxPlus_v2', 
        'HLT_L1NotBptxOR_v2'),
    EventDisplay = cms.vstring('HLT_AK4PFJet100_v7', 
        'HLT_DoubleEle33_CaloIdL_GsfTrkIdVL_MW_v10', 
        'HLT_DoublePhoton85_v8', 
        'HLT_HISinglePhoton60_v4', 
        'HLT_Mu40_TkMu11_v5', 
        'HLT_PFJet500_v9', 
        'HLT_PFMET170_HBHECleaned_v9'),
    ExpressPhysics = cms.vstring('HLT_Ele17_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v9', 
        'HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v9', 
        'HLT_Ele250_CaloIdVT_GsfTrkIdT_v6', 
        'HLT_Ele300_CaloIdVT_GsfTrkIdT_v6', 
        'HLT_HT2000_v5', 
        'HLT_HT2500_v5', 
        'HLT_IsoMu20_v6', 
        'HLT_IsoMu24_v4', 
        'HLT_L1FatEvents_v2', 
        'HLT_MET600_v5', 
        'HLT_MET700_v5', 
        'HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v7', 
        'HLT_Mu300_v3', 
        'HLT_Mu350_v3', 
        'HLT_PFMET500_v7', 
        'HLT_PFMET600_v7', 
        'HLT_Photon500_v6', 
        'HLT_Photon600_v6', 
        'HLT_Physics_v5', 
        'HLT_Random_v2', 
        'HLT_ZeroBias_FirstCollisionAfterAbortGap_copy_v1', 
        'HLT_ZeroBias_FirstCollisionAfterAbortGap_v3', 
        'HLT_ZeroBias_IsolatedBunches_v3', 
        'HLT_ZeroBias_v4'),
    FSQJets = cms.vstring('HLT_DiPFJet15_FBEta3_NoCaloMatched_v6', 
        'HLT_DiPFJet15_NoCaloMatched_v5', 
        'HLT_DiPFJet25_FBEta3_NoCaloMatched_v6', 
        'HLT_DiPFJet25_NoCaloMatched_v5', 
        'HLT_DiPFJetAve15_HFJEC_v6', 
        'HLT_DiPFJetAve25_HFJEC_v6', 
        'HLT_DiPFJetAve35_HFJEC_v6', 
        'HLT_PFJet15_NoCaloMatched_v7', 
        'HLT_PFJet25_NoCaloMatched_v5'),
    HINCaloJets = cms.vstring('HLT_AK4CaloJet100_v4', 
        'HLT_AK4CaloJet30_v5', 
        'HLT_AK4CaloJet40_v4', 
        'HLT_AK4CaloJet50_v4', 
        'HLT_AK4CaloJet80_v4'),
    HINPFJets = cms.vstring('HLT_AK4PFJet100_v7', 
        'HLT_AK4PFJet30_v7', 
        'HLT_AK4PFJet50_v7', 
        'HLT_AK4PFJet80_v7'),
    HINPhoton = cms.vstring('HLT_HISinglePhoton10_v4', 
        'HLT_HISinglePhoton15_v4', 
        'HLT_HISinglePhoton20_v4', 
        'HLT_HISinglePhoton40_v4', 
        'HLT_HISinglePhoton60_v4'),
    HLTMonitor = cms.vstring('HLT_DiPFJetAve40_v8', 
        'HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v9', 
        'HLT_Ele27_WPTight_Gsf_v7', 
        'HLT_Ele27_eta2p1_WPLoose_Gsf_v8', 
        'HLT_PFHT350_v8', 
        'HLT_PFMET120_PFMHT120_IDTight_v8', 
        'HLT_QuadPFJet_BTagCSV_p016_VBF_Mqq460_v5', 
        'HLT_QuadPFJet_BTagCSV_p016_VBF_Mqq500_v5', 
        'HLT_QuadPFJet_BTagCSV_p016_p11_VBF_Mqq200_v5', 
        'HLT_QuadPFJet_BTagCSV_p016_p11_VBF_Mqq240_v5', 
        'HLT_QuadPFJet_VBF_v8'),
    HLTPhysics = cms.vstring('HLT_L1FatEvents_v2', 
        'HLT_Physics_v5'),
    HLTPhysics0 = cms.vstring('HLT_L1FatEvents_part0_v1'),
    HLTPhysics1 = cms.vstring('HLT_L1FatEvents_part1_v1'),
    HLTPhysics2 = cms.vstring('HLT_L1FatEvents_part2_v1'),
    HLTPhysics3 = cms.vstring('HLT_L1FatEvents_part3_v1'),
    HTMHT = cms.vstring('HLT_DiPFJet40_DEta3p5_MJJ600_PFMETNoMu140_v6', 
        'HLT_DiPFJet40_DEta3p5_MJJ600_PFMETNoMu80_v5', 
        'HLT_PFHT200_DiPFJetAve90_PFAlphaT0p63_v8', 
        'HLT_PFHT200_PFAlphaT0p51_v8', 
        'HLT_PFHT250_DiPFJetAve90_PFAlphaT0p58_v8', 
        'HLT_PFHT300_DiPFJetAve90_PFAlphaT0p54_v8', 
        'HLT_PFHT300_PFMET110_v6', 
        'HLT_PFHT350_DiPFJetAve90_PFAlphaT0p53_v8', 
        'HLT_PFHT400_DiPFJetAve90_PFAlphaT0p52_v8', 
        'HLT_Rsq0p25_v6', 
        'HLT_Rsq0p30_v6', 
        'HLT_RsqMR270_Rsq0p09_MR200_4jet_v6', 
        'HLT_RsqMR270_Rsq0p09_MR200_v6'),
    HcalHPDNoise = cms.vstring('HLT_GlobalRunHPDNoise_v4'),
    HcalNZS = cms.vstring('HLT_HcalNZS_v10', 
        'HLT_HcalPhiSym_v11'),
    HighMultiplicityEOF = cms.vstring('HLT_FullTracks_Multiplicity100_v5', 
        'HLT_FullTracks_Multiplicity130_v5', 
        'HLT_FullTracks_Multiplicity150_v5', 
        'HLT_FullTracks_Multiplicity80_v4'),
    JetHT = cms.vstring('HLT_AK8DiPFJet250_200_TrimMass30_BTagCSV_p20_v5', 
        'HLT_AK8DiPFJet250_200_TrimMass30_v5', 
        'HLT_AK8DiPFJet280_200_TrimMass30_BTagCSV_p087_v1', 
        'HLT_AK8DiPFJet280_200_TrimMass30_BTagCSV_p20_v5', 
        'HLT_AK8DiPFJet280_200_TrimMass30_v5', 
        'HLT_AK8DiPFJet300_200_TrimMass30_BTagCSV_p087_v1', 
        'HLT_AK8DiPFJet300_200_TrimMass30_BTagCSV_p20_v1', 
        'HLT_AK8DiPFJet300_200_TrimMass30_v1', 
        'HLT_AK8PFHT600_TrimR0p1PT0p03Mass50_BTagCSV_p20_v6', 
        'HLT_AK8PFHT650_TrimR0p1PT0p03Mass50_v7', 
        'HLT_AK8PFHT700_TrimR0p1PT0p03Mass50_v8', 
        'HLT_AK8PFHT750_TrimMass50_v1', 
        'HLT_AK8PFHT800_TrimMass50_v1', 
        'HLT_AK8PFJet140_v4', 
        'HLT_AK8PFJet200_v4', 
        'HLT_AK8PFJet260_v5', 
        'HLT_AK8PFJet320_v5', 
        'HLT_AK8PFJet360_TrimMass30_v7', 
        'HLT_AK8PFJet400_TrimMass30_v1', 
        'HLT_AK8PFJet400_v5', 
        'HLT_AK8PFJet40_v5', 
        'HLT_AK8PFJet450_v5', 
        'HLT_AK8PFJet500_v5', 
        'HLT_AK8PFJet60_v4', 
        'HLT_AK8PFJet80_v4', 
        'HLT_CaloJet500_NoJetID_v5', 
        'HLT_DiCentralPFJet170_CFMax0p1_v5', 
        'HLT_DiCentralPFJet170_v5', 
        'HLT_DiCentralPFJet330_CFMax0p5_v5', 
        'HLT_DiCentralPFJet430_v5', 
        'HLT_DiJetVBFMu_PassThrough_v1', 
        'HLT_DiJetVBF_PassThrough_v1', 
        'HLT_DiPFJetAve100_HFJEC_v8', 
        'HLT_DiPFJetAve140_v7', 
        'HLT_DiPFJetAve160_HFJEC_v8', 
        'HLT_DiPFJetAve200_v7', 
        'HLT_DiPFJetAve220_HFJEC_v9', 
        'HLT_DiPFJetAve260_v8', 
        'HLT_DiPFJetAve300_HFJEC_v9', 
        'HLT_DiPFJetAve320_v8', 
        'HLT_DiPFJetAve400_v8', 
        'HLT_DiPFJetAve40_v8', 
        'HLT_DiPFJetAve500_v8', 
        'HLT_DiPFJetAve60_HFJEC_v8', 
        'HLT_DiPFJetAve60_v8', 
        'HLT_DiPFJetAve80_HFJEC_v8', 
        'HLT_DiPFJetAve80_v7', 
        'HLT_HT2000_v5', 
        'HLT_HT2500_v5', 
        'HLT_L1_TripleJet_VBF_v5', 
        'HLT_PFHT125_v5', 
        'HLT_PFHT200_v6', 
        'HLT_PFHT250_v6', 
        'HLT_PFHT300_v7', 
        'HLT_PFHT350_v8', 
        'HLT_PFHT400_SixJet30_DoubleBTagCSV_p056_v6', 
        'HLT_PFHT400_SixJet30_v8', 
        'HLT_PFHT400_v7', 
        'HLT_PFHT450_SixJet40_BTagCSV_p056_v6', 
        'HLT_PFHT450_SixJet40_v8', 
        'HLT_PFHT475_v7', 
        'HLT_PFHT550_4JetPt50_v6', 
        'HLT_PFHT600_v8', 
        'HLT_PFHT650_4JetPt50_v6', 
        'HLT_PFHT650_WideJetMJJ900DEtaJJ1p5_v8', 
        'HLT_PFHT650_WideJetMJJ950DEtaJJ1p5_v8', 
        'HLT_PFHT650_v8', 
        'HLT_PFHT750_4JetPt70_v2', 
        'HLT_PFHT750_4JetPt80_v2', 
        'HLT_PFHT800_4JetPt50_v2', 
        'HLT_PFHT850_4JetPt50_v2', 
        'HLT_PFHT900_v6', 
        'HLT_PFJet140_v8', 
        'HLT_PFJet200_v8', 
        'HLT_PFJet260_v9', 
        'HLT_PFJet320_v9', 
        'HLT_PFJet400_v9', 
        'HLT_PFJet40_v9', 
        'HLT_PFJet450_v9', 
        'HLT_PFJet500_v9', 
        'HLT_PFJet60_v9', 
        'HLT_PFJet80_v8', 
        'HLT_QuadPFJet_VBF_v8', 
        'HLT_SingleCentralPFJet170_CFMax0p1_v5'),
    L1Accept = cms.vstring('DST_Physics_v5'),
    L1MinimumBias = cms.vstring('HLT_L1MinimumBiasHF_AND_v2'),
    L1MinimumBias0 = cms.vstring('HLT_L1MinimumBiasHF_OR_part0_v2'),
    L1MinimumBias1 = cms.vstring('HLT_L1MinimumBiasHF_OR_part1_v2'),
    L1MinimumBias2 = cms.vstring('HLT_L1MinimumBiasHF_OR_part2_v2'),
    L1MinimumBias3 = cms.vstring('HLT_L1MinimumBiasHF_OR_part3_v2'),
    L1MinimumBias4 = cms.vstring('HLT_L1MinimumBiasHF_OR_part4_v2'),
    L1MinimumBias5 = cms.vstring('HLT_L1MinimumBiasHF_OR_part5_v2'),
    L1MinimumBias6 = cms.vstring('HLT_L1MinimumBiasHF_OR_part6_v2'),
    L1MinimumBias7 = cms.vstring('HLT_L1MinimumBiasHF_OR_part7_v2'),
    L1MinimumBias8 = cms.vstring('HLT_L1MinimumBiasHF_OR_part8_v2'),
    L1MinimumBias9 = cms.vstring('HLT_L1MinimumBiasHF_OR_part9_v2'),
    MET = cms.vstring('HLT_CaloMHTNoPU90_PFMET90_PFMHT90_IDTight_BTagCSV_p067_v7', 
        'HLT_CaloMHTNoPU90_PFMET90_PFMHT90_IDTight_v8', 
        'HLT_DoubleMu3_PFMET50_v6', 
        'HLT_MET200_v5', 
        'HLT_MET250_v5', 
        'HLT_MET300_v5', 
        'HLT_MET600_v5', 
        'HLT_MET60_IsoTrk35_Loose_v3', 
        'HLT_MET700_v5', 
        'HLT_MET75_IsoTrk50_v6', 
        'HLT_MET90_IsoTrk50_v6', 
        'HLT_MonoCentralPFJet80_PFMETNoMu110_PFMHTNoMu110_IDTight_v8', 
        'HLT_MonoCentralPFJet80_PFMETNoMu120_PFMHTNoMu120_IDTight_v8', 
        'HLT_Mu6_PFHT200_PFMET100_v5', 
        'HLT_PFMET110_PFMHT110_IDTight_v8', 
        'HLT_PFMET120_PFMHT120_IDTight_v8', 
        'HLT_PFMET170_BeamHaloCleaned_v7', 
        'HLT_PFMET170_HBHECleaned_v9', 
        'HLT_PFMET170_HBHE_BeamHaloCleaned_v5', 
        'HLT_PFMET170_NotCleaned_v8', 
        'HLT_PFMET300_v7', 
        'HLT_PFMET400_v7', 
        'HLT_PFMET500_v7', 
        'HLT_PFMET600_v7', 
        'HLT_PFMETNoMu110_PFMHTNoMu110_IDTight_v8', 
        'HLT_PFMETNoMu120_PFMHTNoMu120_IDTight_v8', 
        'HLT_PFMETTypeOne190_HBHE_BeamHaloCleaned_v5'),
    MonteCarlo = cms.vstring('MC_AK4CaloJets_v3', 
        'MC_AK4PFJets_v6', 
        'MC_AK8CaloHT_v3', 
        'MC_AK8PFHT_v6', 
        'MC_AK8PFJets_v6', 
        'MC_AK8TrimPFJets_v6', 
        'MC_CaloHT_v3', 
        'MC_CaloMET_JetIdCleaned_v3', 
        'MC_CaloMET_v3', 
        'MC_CaloMHT_v3', 
        'MC_Diphoton10_10_R9Id_OR_IsoCaloId_AND_HE_R9Id_Mass10_v6', 
        'MC_DoubleEle5_CaloIdL_GsfTrkIdVL_MW_v6', 
        'MC_DoubleGlbTrkMu_TrkIsoVVL_DZ_v4', 
        'MC_DoubleL1Tau_MediumIsoPFTau32_Trk1_eta2p1_Reg_v6', 
        'MC_DoubleMuNoFiltersNoVtx_v3', 
        'MC_DoubleMu_TrkIsoVVL_DZ_v4', 
        'MC_Ele15_Ele10_CaloIdL_TrackIdL_IsoVL_DZ_v7', 
        'MC_Ele5_WPLoose_Gsf_v8', 
        'MC_IsoMu_v7', 
        'MC_IsoTkMu15_v6', 
        'MC_LooseIsoPFTau20_v5', 
        'MC_LooseIsoPFTau50_Trk30_eta2p1_v4', 
        'MC_PFHT_v6', 
        'MC_PFMET_v6', 
        'MC_PFMHT_v6', 
        'MC_ReducedIterativeTracking_v3'),
    MuOnia = cms.vstring('HLT_Dimuon0_Phi_Barrel_v6', 
        'HLT_Dimuon0_Upsilon_Muon_v5', 
        'HLT_Dimuon13_Upsilon_v6', 
        'HLT_Dimuon8_Upsilon_Barrel_v6', 
        'HLT_Mu25_TkMu0_dEta18_Onia_v6', 
        'HLT_Mu7p5_L2Mu2_Upsilon_v4', 
        'HLT_Mu7p5_Track2_Upsilon_v4', 
        'HLT_Mu7p5_Track3p5_Upsilon_v4', 
        'HLT_Mu7p5_Track7_Upsilon_v4', 
        'HLT_QuadMuon0_Dimuon0_Upsilon_v4'),
    MuonEG = cms.vstring('HLT_DiMu9_Ele9_CaloIdL_TrackIdL_v8', 
        'HLT_Mu12_Photon25_CaloIdL_L1ISO_v8', 
        'HLT_Mu12_Photon25_CaloIdL_L1OR_v8', 
        'HLT_Mu12_Photon25_CaloIdL_v8', 
        'HLT_Mu12_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ_v4', 
        'HLT_Mu12_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_v3', 
        'HLT_Mu17_Photon30_CaloIdL_L1ISO_v9', 
        'HLT_Mu17_Photon35_CaloIdL_L1ISO_v9', 
        'HLT_Mu17_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_v9', 
        'HLT_Mu23NoFiltersNoVtx_Photon23_CaloIdL_v7', 
        'HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v4', 
        'HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_v9', 
        'HLT_Mu23_TrkIsoVVL_Ele8_CaloIdL_TrackIdL_IsoVL_DZ_v3', 
        'HLT_Mu27_Ele37_CaloIdL_GsfTrkIdVL_v6', 
        'HLT_Mu28NoFiltersNoVtxDisplaced_Photon28_CaloIdL_v7', 
        'HLT_Mu33NoFiltersNoVtxDisplaced_Photon33_CaloIdL_v7', 
        'HLT_Mu33_Ele33_CaloIdL_GsfTrkIdVL_v3', 
        'HLT_Mu37_Ele27_CaloIdL_GsfTrkIdVL_v6', 
        'HLT_Mu38NoFiltersNoVtx_Photon38_CaloIdL_v7', 
        'HLT_Mu42NoFiltersNoVtx_Photon42_CaloIdL_v7', 
        'HLT_Mu8_DiEle12_CaloIdL_TrackIdL_v9', 
        'HLT_Mu8_Ele8_CaloIdM_TrackIdM_Mass8_PFHT300_v10', 
        'HLT_Mu8_TrkIsoVVL_Ele17_CaloIdL_TrackIdL_IsoVL_v9', 
        'HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ_v4'),
    NoBPTX = cms.vstring('HLT_JetE30_NoBPTX3BX_v4', 
        'HLT_JetE30_NoBPTX_v4', 
        'HLT_JetE50_NoBPTX3BX_v4', 
        'HLT_JetE70_NoBPTX3BX_v4', 
        'HLT_L2Mu10_NoVertex_NoBPTX3BX_v2', 
        'HLT_L2Mu10_NoVertex_NoBPTX_v3', 
        'HLT_L2Mu40_NoVertex_3Sta_NoBPTX3BX_v2', 
        'HLT_L2Mu45_NoVertex_3Sta_NoBPTX3BX_v1'),
    OnlineMonitor = cms.vstring( ('DST_CaloJet40_BTagScouting_v7', 
        'DST_CaloJet40_CaloBTagScouting_v6', 
        'DST_CaloJet40_CaloScouting_PFScouting_v7', 
        'DST_DoubleMu3_Mass10_BTagScouting_v8', 
        'DST_DoubleMu3_Mass10_CaloScouting_PFScouting_v7', 
        'DST_HT250_CaloBTagScouting_v3', 
        'DST_HT250_CaloScouting_v5', 
        'DST_HT410_BTagScouting_v7', 
        'DST_HT410_PFScouting_v7', 
        'DST_HT450_BTagScouting_v7', 
        'DST_HT450_PFScouting_v7', 
        'DST_L1DoubleMu_BTagScouting_v8', 
        'DST_L1DoubleMu_CaloScouting_PFScouting_v7', 
        'DST_L1HTT_BTagScouting_v7', 
        'DST_L1HTT_CaloBTagScouting_v6', 
        'DST_L1HTT_CaloScouting_PFScouting_v7', 
        'DST_ZeroBias_BTagScouting_v7', 
        'DST_ZeroBias_CaloScouting_PFScouting_v6', 
        'HLT_AK4CaloJet100_v4', 
        'HLT_AK4CaloJet30_v5', 
        'HLT_AK4CaloJet40_v4', 
        'HLT_AK4CaloJet50_v4', 
        'HLT_AK4CaloJet80_v4', 
        'HLT_AK4PFJet100_v7', 
        'HLT_AK4PFJet30_v7', 
        'HLT_AK4PFJet50_v7', 
        'HLT_AK4PFJet80_v7', 
        'HLT_AK8DiPFJet250_200_TrimMass30_BTagCSV_p20_v5', 
        'HLT_AK8DiPFJet250_200_TrimMass30_v5', 
        'HLT_AK8DiPFJet280_200_TrimMass30_BTagCSV_p087_v1', 
        'HLT_AK8DiPFJet280_200_TrimMass30_BTagCSV_p20_v5', 
        'HLT_AK8DiPFJet280_200_TrimMass30_v5', 
        'HLT_AK8DiPFJet300_200_TrimMass30_BTagCSV_p087_v1', 
        'HLT_AK8DiPFJet300_200_TrimMass30_BTagCSV_p20_v1', 
        'HLT_AK8DiPFJet300_200_TrimMass30_v1', 
        'HLT_AK8PFHT600_TrimR0p1PT0p03Mass50_BTagCSV_p20_v6', 
        'HLT_AK8PFHT650_TrimR0p1PT0p03Mass50_v7', 
        'HLT_AK8PFHT700_TrimR0p1PT0p03Mass50_v8', 
        'HLT_AK8PFHT750_TrimMass50_v1', 
        'HLT_AK8PFHT800_TrimMass50_v1', 
        'HLT_AK8PFJet140_v4', 
        'HLT_AK8PFJet200_v4', 
        'HLT_AK8PFJet260_v5', 
        'HLT_AK8PFJet320_v5', 
        'HLT_AK8PFJet360_TrimMass30_v7', 
        'HLT_AK8PFJet400_TrimMass30_v1', 
        'HLT_AK8PFJet400_v5', 
        'HLT_AK8PFJet40_v5', 
        'HLT_AK8PFJet450_v5', 
        'HLT_AK8PFJet500_v5', 
        'HLT_AK8PFJet60_v4', 
        'HLT_AK8PFJet80_v4', 
        'HLT_BTagMu_AK8Jet300_Mu5_v4', 
        'HLT_BTagMu_DiJet110_Mu5_v5', 
        'HLT_BTagMu_DiJet170_Mu5_v4', 
        'HLT_BTagMu_DiJet20_Mu5_v5', 
        'HLT_BTagMu_DiJet40_Mu5_v5', 
        'HLT_BTagMu_DiJet70_Mu5_v5', 
        'HLT_BTagMu_Jet300_Mu5_v5', 
        'HLT_CaloJet500_NoJetID_v5', 
        'HLT_CaloMHTNoPU90_PFMET90_PFMHT90_IDTight_BTagCSV_p067_v7', 
        'HLT_CaloMHTNoPU90_PFMET90_PFMHT90_IDTight_v8', 
        'HLT_DiCentralPFJet170_CFMax0p1_v5', 
        'HLT_DiCentralPFJet170_v5', 
        'HLT_DiCentralPFJet330_CFMax0p5_v5', 
        'HLT_DiCentralPFJet430_v5', 
        'HLT_DiJetVBFMu_PassThrough_v1', 
        'HLT_DiJetVBF_PassThrough_v1', 
        'HLT_DiMu9_Ele9_CaloIdL_TrackIdL_v8', 
        'HLT_DiPFJet15_FBEta3_NoCaloMatched_v6', 
        'HLT_DiPFJet15_NoCaloMatched_v5', 
        'HLT_DiPFJet25_FBEta3_NoCaloMatched_v6', 
        'HLT_DiPFJet25_NoCaloMatched_v5', 
        'HLT_DiPFJet40_DEta3p5_MJJ600_PFMETNoMu140_v6', 
        'HLT_DiPFJet40_DEta3p5_MJJ600_PFMETNoMu80_v5', 
        'HLT_DiPFJetAve100_HFJEC_v8', 
        'HLT_DiPFJetAve140_v7', 
        'HLT_DiPFJetAve15_HFJEC_v6', 
        'HLT_DiPFJetAve160_HFJEC_v8', 
        'HLT_DiPFJetAve200_v7', 
        'HLT_DiPFJetAve220_HFJEC_v9', 
        'HLT_DiPFJetAve25_HFJEC_v6', 
        'HLT_DiPFJetAve260_v8', 
        'HLT_DiPFJetAve300_HFJEC_v9', 
        'HLT_DiPFJetAve320_v8', 
        'HLT_DiPFJetAve35_HFJEC_v6', 
        'HLT_DiPFJetAve400_v8', 
        'HLT_DiPFJetAve40_v8', 
        'HLT_DiPFJetAve500_v8', 
        'HLT_DiPFJetAve60_HFJEC_v8', 
        'HLT_DiPFJetAve60_v8', 
        'HLT_DiPFJetAve80_HFJEC_v8', 
        'HLT_DiPFJetAve80_v7', 
        'HLT_DiSC30_18_EIso_AND_HE_Mass70_v6', 
        'HLT_Dimuon0_Jpsi_Muon_v5', 
        'HLT_Dimuon0_Phi_Barrel_v6', 
        'HLT_Dimuon0_Upsilon_Muon_v5', 
        'HLT_Dimuon0er16_Jpsi_NoOS_NoVertexing_v4', 
        'HLT_Dimuon13_PsiPrime_v6', 
        'HLT_Dimuon13_Upsilon_v6', 
        'HLT_Dimuon16_Jpsi_v6', 
        'HLT_Dimuon20_Jpsi_v6', 
        'HLT_Dimuon6_Jpsi_NoVertexing_v4', 
        'HLT_Dimuon8_PsiPrime_Barrel_v6', 
        'HLT_Dimuon8_Upsilon_Barrel_v6', 
        'HLT_Diphoton30EB_18EB_R9Id_OR_IsoCaloId_AND_HE_R9Id_DoublePixelVeto_Mass55_v7', 
        'HLT_Diphoton30PV_18PV_R9Id_AND_IsoCaloId_AND_HE_R9Id_DoublePixelVeto_Mass55_v7', 
        'HLT_Diphoton30_18_R9Id_OR_IsoCaloId_AND_HE_R9Id_DoublePixelSeedMatch_Mass70_v7', 
        'HLT_Diphoton30_18_R9Id_OR_IsoCaloId_AND_HE_R9Id_Mass90_v7', 
        'HLT_Diphoton30_18_Solid_R9Id_AND_IsoCaloId_AND_HE_R9Id_Mass55_v7', 
        'HLT_DoubleEle24_22_eta2p1_WPLoose_Gsf_v8', 
        'HLT_DoubleEle33_CaloIdL_GsfTrkIdVL_MW_v10', 
        'HLT_DoubleEle33_CaloIdL_MW_v8', 
        'HLT_DoubleEle33_CaloIdL_v7', 
        'HLT_DoubleEle37_Ele27_CaloIdL_GsfTrkIdVL_v7', 
        'HLT_DoubleEle8_CaloIdM_TrackIdM_Mass8_PFHT300_v10', 
        'HLT_DoubleIsoMu17_eta2p1_noDzCut_v5', 
        'HLT_DoubleJet90_Double30_DoubleBTagCSV_p087_v5', 
        'HLT_DoubleJet90_Double30_TripleBTagCSV_p087_v5', 
        'HLT_DoubleJetsC100_DoubleBTagCSV_p014_DoublePFJetsC100MaxDeta1p6_v5', 
        'HLT_DoubleJetsC100_DoubleBTagCSV_p026_DoublePFJetsC160_v5', 
        'HLT_DoubleJetsC100_SingleBTagCSV_p014_SinglePFJetC350_v5', 
        'HLT_DoubleJetsC100_SingleBTagCSV_p014_v3', 
        'HLT_DoubleJetsC100_SingleBTagCSV_p026_SinglePFJetC350_v5', 
        'HLT_DoubleJetsC100_SingleBTagCSV_p026_v3', 
        'HLT_DoubleJetsC112_DoubleBTagCSV_p014_DoublePFJetsC112MaxDeta1p6_v5', 
        'HLT_DoubleJetsC112_DoubleBTagCSV_p026_DoublePFJetsC172_v5', 
        'HLT_DoubleMediumCombinedIsoPFTau35_Trk1_eta2p1_Reg_v3', 
        'HLT_DoubleMediumCombinedIsoPFTau40_Trk1_eta2p1_Reg_v2', 
        'HLT_DoubleMediumCombinedIsoPFTau40_Trk1_eta2p1_v2', 
        'HLT_DoubleMu0_v2', 
        'HLT_DoubleMu18NoFiltersNoVtx_v5', 
        'HLT_DoubleMu23NoFiltersNoVtxDisplaced_v5', 
        'HLT_DoubleMu28NoFiltersNoVtxDisplaced_v5', 
        'HLT_DoubleMu33NoFiltersNoVtx_v5', 
        'HLT_DoubleMu38NoFiltersNoVtx_v5', 
        'HLT_DoubleMu3_PFMET50_v6', 
        'HLT_DoubleMu3_Trk_Tau3mu_v4', 
        'HLT_DoubleMu4_3_Bs_v7', 
        'HLT_DoubleMu4_3_Jpsi_Displaced_v7', 
        'HLT_DoubleMu4_JpsiTrk_Displaced_v7', 
        'HLT_DoubleMu4_LowMassNonResonantTrk_Displaced_v7', 
        'HLT_DoubleMu4_PsiPrimeTrk_Displaced_v7', 
        'HLT_DoubleMu8_Mass8_PFHT300_v9', 
        'HLT_DoublePhoton60_v7', 
        'HLT_DoublePhoton85_v8', 
        'HLT_DoubleTightCombinedIsoPFTau35_Trk1_eta2p1_Reg_v3', 
        'HLT_DoubleTightCombinedIsoPFTau40_Trk1_eta2p1_Reg_v2', 
        'HLT_DoubleTightCombinedIsoPFTau40_Trk1_eta2p1_v2', 
        'HLT_ECALHT800_v6', 
        'HLT_Ele105_CaloIdVT_GsfTrkIdT_v8', 
        'HLT_Ele10_CaloIdM_TrackIdM_CentralPFJet30_BTagCSV_p13_v7', 
        'HLT_Ele115_CaloIdVT_GsfTrkIdT_v7', 
        'HLT_Ele12_CaloIdL_TrackIdL_IsoVL_PFJet30_v9', 
        'HLT_Ele12_CaloIdL_TrackIdL_IsoVL_v8', 
        'HLT_Ele12_CaloIdM_TrackIdM_PFJet30_v9', 
        'HLT_Ele145_CaloIdVT_GsfTrkIdT_v1', 
        'HLT_Ele15_IsoVVVL_BTagCSV_p067_PFHT400_v7', 
        'HLT_Ele15_IsoVVVL_PFHT400_PFMET50_v6', 
        'HLT_Ele15_IsoVVVL_PFHT400_v6', 
        'HLT_Ele15_IsoVVVL_PFHT600_v9', 
        'HLT_Ele16_Ele12_Ele8_CaloIdL_TrackIdL_v8', 
        'HLT_Ele17_CaloIdL_GsfTrkIdVL_v6', 
        'HLT_Ele17_CaloIdL_TrackIdL_IsoVL_PFJet30_v7', 
        'HLT_Ele17_CaloIdL_TrackIdL_IsoVL_v7', 
        'HLT_Ele17_CaloIdM_TrackIdM_PFJet30_v7', 
        'HLT_Ele17_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v9', 
        'HLT_Ele17_Ele12_CaloIdL_TrackIdL_IsoVL_v9', 
        'HLT_Ele200_CaloIdVT_GsfTrkIdT_v1', 
        'HLT_Ele20_eta2p1_WPLoose_Gsf_LooseIsoPFTau28_v3', 
        'HLT_Ele22_eta2p1_WPLoose_Gsf_LooseIsoPFTau29_v3', 
        'HLT_Ele22_eta2p1_WPLoose_Gsf_v9', 
        'HLT_Ele23_CaloIdL_TrackIdL_IsoVL_PFJet30_v9', 
        'HLT_Ele23_CaloIdL_TrackIdL_IsoVL_v8', 
        'HLT_Ele23_CaloIdM_TrackIdM_PFJet30_v9', 
        'HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_L1JetTauSeeded_v3', 
        'HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v9', 
        'HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_v9', 
        'HLT_Ele24_eta2p1_WPLoose_Gsf_LooseIsoPFTau30_v4', 
        'HLT_Ele250_CaloIdVT_GsfTrkIdT_v6', 
        'HLT_Ele25_WPTight_Gsf_v7', 
        'HLT_Ele25_eta2p1_WPTight_Gsf_v7', 
        'HLT_Ele27_HighEta_Ele20_Mass55_v8', 
        'HLT_Ele27_WPLoose_Gsf_WHbbBoost_v9', 
        'HLT_Ele27_WPTight_Gsf_L1JetTauSeeded_v4', 
        'HLT_Ele27_WPTight_Gsf_v7', 
        'HLT_Ele27_eta2p1_WPLoose_Gsf_HT200_v10', 
        'HLT_Ele27_eta2p1_WPLoose_Gsf_v8', 
        'HLT_Ele27_eta2p1_WPTight_Gsf_v8', 
        'HLT_Ele300_CaloIdVT_GsfTrkIdT_v6', 
        'HLT_Ele30_WPTight_Gsf_v1', 
        'HLT_Ele30_eta2p1_WPTight_Gsf_v1', 
        'HLT_Ele32_WPTight_Gsf_v1', 
        'HLT_Ele32_eta2p1_WPTight_Gsf_v8', 
        'HLT_Ele36_eta2p1_WPLoose_Gsf_LooseIsoPFTau20_SingleL1_v3', 
        'HLT_Ele45_CaloIdVT_GsfTrkIdT_PFJet200_PFJet50_v9', 
        'HLT_Ele50_CaloIdVT_GsfTrkIdT_PFJet165_v7', 
        'HLT_Ele50_IsoVVVL_PFHT400_v6', 
        'HLT_Ele8_CaloIdL_TrackIdL_IsoVL_PFJet30_v7', 
        'HLT_Ele8_CaloIdM_TrackIdM_PFJet30_v9', 
        'HLT_FullTracks_Multiplicity100_v5', 
        'HLT_FullTracks_Multiplicity130_v5', 
        'HLT_FullTracks_Multiplicity150_v5', 
        'HLT_FullTracks_Multiplicity80_v4', 
        'HLT_GlobalRunHPDNoise_v4', 
        'HLT_HISinglePhoton10_v4', 
        'HLT_HISinglePhoton15_v4', 
        'HLT_HISinglePhoton20_v4', 
        'HLT_HISinglePhoton40_v4', 
        'HLT_HISinglePhoton60_v4', 
        'HLT_HT2000_v5', 
        'HLT_HT200_v4', 
        'HLT_HT2500_v5', 
        'HLT_HT250_DisplacedDijet40_DisplacedTrack_v5', 
        'HLT_HT275_v4', 
        'HLT_HT325_v4', 
        'HLT_HT350_DisplacedDijet40_DisplacedTrack_v5', 
        'HLT_HT350_DisplacedDijet40_Inclusive_v4', 
        'HLT_HT350_DisplacedDijet80_DisplacedTrack_v5', 
        'HLT_HT350_DisplacedDijet80_Tight_DisplacedTrack_v5', 
        'HLT_HT425_v4', 
        'HLT_HT430to450_v4', 
        'HLT_HT450to470_v4', 
        'HLT_HT470to500_v4', 
        'HLT_HT500to550_v4', 
        'HLT_HT550_DisplacedDijet80_Inclusive_v2', 
        'HLT_HT550to650_v4', 
        'HLT_HT575_v4', 
        'HLT_HT650_DisplacedDijet80_Inclusive_v5', 
        'HLT_HT650_v5', 
        'HLT_HT750_DisplacedDijet80_Inclusive_v5', 
        'HLT_HcalIsolatedbunch_v2', 
        'HLT_HcalNZS_v10', 
        'HLT_HcalPhiSym_v11', 
        'HLT_IsoMu16_eta2p1_MET30_LooseIsoPFTau50_Trk30_eta2p1_v5', 
        'HLT_IsoMu16_eta2p1_MET30_v4', 
        'HLT_IsoMu19_eta2p1_LooseCombinedIsoPFTau20_v1', 
        'HLT_IsoMu19_eta2p1_LooseIsoPFTau20_SingleL1_v5', 
        'HLT_IsoMu19_eta2p1_LooseIsoPFTau20_v5', 
        'HLT_IsoMu19_eta2p1_MediumCombinedIsoPFTau32_Trk1_eta2p1_Reg_v1', 
        'HLT_IsoMu19_eta2p1_MediumIsoPFTau32_Trk1_eta2p1_Reg_v5', 
        'HLT_IsoMu19_eta2p1_TightCombinedIsoPFTau32_Trk1_eta2p1_Reg_v1', 
        'HLT_IsoMu20_v6', 
        'HLT_IsoMu21_eta2p1_LooseIsoPFTau20_SingleL1_v5', 
        'HLT_IsoMu21_eta2p1_LooseIsoPFTau50_Trk30_eta2p1_SingleL1_v4', 
        'HLT_IsoMu21_eta2p1_MediumCombinedIsoPFTau32_Trk1_eta2p1_Reg_v1', 
        'HLT_IsoMu21_eta2p1_MediumIsoPFTau32_Trk1_eta2p1_Reg_v5', 
        'HLT_IsoMu21_eta2p1_TightCombinedIsoPFTau32_Trk1_eta2p1_Reg_v1', 
        'HLT_IsoMu22_eta2p1_v4', 
        'HLT_IsoMu22_v5', 
        'HLT_IsoMu24_eta2p1_v3', 
        'HLT_IsoMu24_v4', 
        'HLT_IsoMu27_v7', 
        'HLT_IsoTkMu20_v7', 
        'HLT_IsoTkMu22_eta2p1_v4', 
        'HLT_IsoTkMu22_v5', 
        'HLT_IsoTkMu24_eta2p1_v3', 
        'HLT_IsoTkMu24_v4', 
        'HLT_IsoTkMu27_v7', 
        'HLT_IsoTrackHB_v3', 
        'HLT_IsoTrackHE_v3', 
        'HLT_JetE30_NoBPTX3BX_v4', 
        'HLT_JetE30_NoBPTX_v4', 
        'HLT_JetE50_NoBPTX3BX_v4', 
        'HLT_JetE70_NoBPTX3BX_v4', 
        'HLT_L1BptxMinus_v2', 
        'HLT_L1BptxPlus_v2', 
        'HLT_L1FatEvents_v2', 
        'HLT_L1MinimumBiasHF_AND_v2', 
        'HLT_L1MinimumBiasHF_OR_part0_v2', 
        'HLT_L1MinimumBiasHF_OR_part1_v2', 
        'HLT_L1MinimumBiasHF_OR_part2_v2', 
        'HLT_L1MinimumBiasHF_OR_part3_v2', 
        'HLT_L1MinimumBiasHF_OR_part4_v2', 
        'HLT_L1MinimumBiasHF_OR_part5_v2', 
        'HLT_L1MinimumBiasHF_OR_part6_v2', 
        'HLT_L1MinimumBiasHF_OR_part7_v2', 
        'HLT_L1MinimumBiasHF_OR_part8_v2', 
        'HLT_L1MinimumBiasHF_OR_part9_v2', 
        'HLT_L1NotBptxOR_v2', 
        'HLT_L1SingleMu18_v1', 
        'HLT_L1_TripleJet_VBF_v5', 
        'HLT_L2DoubleMu23_NoVertex_v6', 
        'HLT_L2DoubleMu28_NoVertex_2Cha_Angle2p5_Mass10_v6', 
        'HLT_L2DoubleMu38_NoVertex_2Cha_Angle2p5_Mass10_v6', 
        'HLT_L2Mu10_NoVertex_NoBPTX3BX_v2', 
        'HLT_L2Mu10_NoVertex_NoBPTX_v3', 
        'HLT_L2Mu10_v3', 
        'HLT_L2Mu40_NoVertex_3Sta_NoBPTX3BX_v2', 
        'HLT_L2Mu45_NoVertex_3Sta_NoBPTX3BX_v1', 
        'HLT_LooseIsoPFTau50_Trk30_eta2p1_MET110_v6', 
        'HLT_LooseIsoPFTau50_Trk30_eta2p1_MET120_v6', 
        'HLT_LooseIsoPFTau50_Trk30_eta2p1_MET90_v6', 
        'HLT_LooseIsoPFTau50_Trk30_eta2p1_v7', 
        'HLT_MET200_v5', 
        'HLT_MET250_v5', 
        'HLT_MET300_v5', 
        'HLT_MET600_v5', 
        'HLT_MET60_IsoTrk35_Loose_v3', 
        'HLT_MET700_v5', 
        'HLT_MET75_IsoTrk50_v6', 
        'HLT_MET90_IsoTrk50_v6', 
        'HLT_MonoCentralPFJet80_PFMETNoMu110_PFMHTNoMu110_IDTight_v8', 
        'HLT_MonoCentralPFJet80_PFMETNoMu120_PFMHTNoMu120_IDTight_v8', 
        'HLT_Mu10_CentralPFJet30_BTagCSV_p13_v5', 
        'HLT_Mu10_TrkIsoVVL_DiPFJet40_DEta3p5_MJJ750_HTT350_PFMETNoMu60_v5', 
        'HLT_Mu12_Photon25_CaloIdL_L1ISO_v8', 
        'HLT_Mu12_Photon25_CaloIdL_L1OR_v8', 
        'HLT_Mu12_Photon25_CaloIdL_v8', 
        'HLT_Mu12_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ_v4', 
        'HLT_Mu12_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_v3', 
        'HLT_Mu15_IsoVVVL_BTagCSV_p067_PFHT400_v6', 
        'HLT_Mu15_IsoVVVL_PFHT400_PFMET50_v5', 
        'HLT_Mu15_IsoVVVL_PFHT400_v5', 
        'HLT_Mu15_IsoVVVL_PFHT600_v8', 
        'HLT_Mu17_Mu8_DZ_v7', 
        'HLT_Mu17_Mu8_SameSign_DZ_v6', 
        'HLT_Mu17_Mu8_SameSign_v5', 
        'HLT_Mu17_Mu8_v5', 
        'HLT_Mu17_Photon30_CaloIdL_L1ISO_v9', 
        'HLT_Mu17_Photon35_CaloIdL_L1ISO_v9', 
        'HLT_Mu17_TkMu8_DZ_v6', 
        'HLT_Mu17_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_v9', 
        'HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v7', 
        'HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_v6', 
        'HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ_v6', 
        'HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_v5', 
        'HLT_Mu17_TrkIsoVVL_v4', 
        'HLT_Mu17_v4', 
        'HLT_Mu20_Mu10_DZ_v6', 
        'HLT_Mu20_Mu10_SameSign_DZ_v6', 
        'HLT_Mu20_Mu10_SameSign_v4', 
        'HLT_Mu20_Mu10_v5', 
        'HLT_Mu20_v4', 
        'HLT_Mu23NoFiltersNoVtx_Photon23_CaloIdL_v7', 
        'HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v4', 
        'HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_v9', 
        'HLT_Mu23_TrkIsoVVL_Ele8_CaloIdL_TrackIdL_IsoVL_DZ_v3', 
        'HLT_Mu24_eta2p1_v5', 
        'HLT_Mu25_TkMu0_dEta18_Onia_v6', 
        'HLT_Mu27_Ele37_CaloIdL_GsfTrkIdVL_v6', 
        'HLT_Mu27_TkMu8_v5', 
        'HLT_Mu27_v5', 
        'HLT_Mu28NoFiltersNoVtxDisplaced_Photon28_CaloIdL_v7', 
        'HLT_Mu28NoFiltersNoVtx_CentralCaloJet40_v5', 
        'HLT_Mu28NoFiltersNoVtx_DisplacedJet40_Loose_v5', 
        'HLT_Mu300_v3', 
        'HLT_Mu30_TkMu11_v5', 
        'HLT_Mu30_eta2p1_PFJet150_PFJet50_v5', 
        'HLT_Mu33NoFiltersNoVtxDisplaced_Photon33_CaloIdL_v7', 
        'HLT_Mu33_Ele33_CaloIdL_GsfTrkIdVL_v3', 
        'HLT_Mu350_v3', 
        'HLT_Mu37_Ele27_CaloIdL_GsfTrkIdVL_v6', 
        'HLT_Mu38NoFiltersNoVtxDisplaced_DisplacedJet60_Loose_v5', 
        'HLT_Mu38NoFiltersNoVtxDisplaced_DisplacedJet60_Tight_v5', 
        'HLT_Mu38NoFiltersNoVtx_DisplacedJet60_Loose_v5', 
        'HLT_Mu38NoFiltersNoVtx_Photon38_CaloIdL_v7', 
        'HLT_Mu3_PFJet40_v6', 
        'HLT_Mu40_TkMu11_v5', 
        'HLT_Mu40_eta2p1_PFJet200_PFJet50_v7', 
        'HLT_Mu42NoFiltersNoVtx_Photon42_CaloIdL_v7', 
        'HLT_Mu45_eta2p1_v5', 
        'HLT_Mu50_IsoVVVL_PFHT400_v5', 
        'HLT_Mu50_v5', 
        'HLT_Mu55_v4', 
        'HLT_Mu6_PFHT200_PFMET100_v5', 
        'HLT_Mu7p5_L2Mu2_Jpsi_v4', 
        'HLT_Mu7p5_L2Mu2_Upsilon_v4', 
        'HLT_Mu7p5_Track2_Jpsi_v4', 
        'HLT_Mu7p5_Track2_Upsilon_v4', 
        'HLT_Mu7p5_Track3p5_Jpsi_v4', 
        'HLT_Mu7p5_Track3p5_Upsilon_v4', 
        'HLT_Mu7p5_Track7_Jpsi_v4', 
        'HLT_Mu7p5_Track7_Upsilon_v4', 
        'HLT_Mu8_DiEle12_CaloIdL_TrackIdL_v9', 
        'HLT_Mu8_Ele8_CaloIdM_TrackIdM_Mass8_PFHT300_v10', 
        'HLT_Mu8_TrkIsoVVL_DiPFJet40_DEta3p5_MJJ750_HTT300_PFMETNoMu60_v4', 
        'HLT_Mu8_TrkIsoVVL_Ele17_CaloIdL_TrackIdL_IsoVL_v9', 
        'HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ_v4', 
        'HLT_Mu8_TrkIsoVVL_v5', 
        'HLT_Mu8_v5', 
        'HLT_PFHT125_v5', 
        'HLT_PFHT200_DiPFJetAve90_PFAlphaT0p63_v8', 
        'HLT_PFHT200_PFAlphaT0p51_v8', 
        'HLT_PFHT200_v6', 
        'HLT_PFHT250_DiPFJetAve90_PFAlphaT0p58_v8', 
        'HLT_PFHT250_v6', 
        'HLT_PFHT300_DiPFJetAve90_PFAlphaT0p54_v8', 
        'HLT_PFHT300_PFMET110_v6', 
        'HLT_PFHT300_v7', 
        'HLT_PFHT350_DiPFJetAve90_PFAlphaT0p53_v8', 
        'HLT_PFHT350_v8', 
        'HLT_PFHT400_DiPFJetAve90_PFAlphaT0p52_v8', 
        'HLT_PFHT400_SixJet30_DoubleBTagCSV_p056_v6', 
        'HLT_PFHT400_SixJet30_v8', 
        'HLT_PFHT400_v7', 
        'HLT_PFHT450_SixJet40_BTagCSV_p056_v6', 
        'HLT_PFHT450_SixJet40_v8', 
        'HLT_PFHT475_v7', 
        'HLT_PFHT550_4JetPt50_v6', 
        'HLT_PFHT600_v8', 
        'HLT_PFHT650_4JetPt50_v6', 
        'HLT_PFHT650_WideJetMJJ900DEtaJJ1p5_v8', 
        'HLT_PFHT650_WideJetMJJ950DEtaJJ1p5_v8', 
        'HLT_PFHT650_v8', 
        'HLT_PFHT750_4JetPt70_v2', 
        'HLT_PFHT750_4JetPt80_v2', 
        'HLT_PFHT800_4JetPt50_v2', 
        'HLT_PFHT850_4JetPt50_v2', 
        'HLT_PFHT900_v6', 
        'HLT_PFJet140_v8', 
        'HLT_PFJet15_NoCaloMatched_v7', 
        'HLT_PFJet200_v8', 
        'HLT_PFJet25_NoCaloMatched_v5', 
        'HLT_PFJet260_v9', 
        'HLT_PFJet320_v9', 
        'HLT_PFJet400_v9', 
        'HLT_PFJet40_v9', 
        'HLT_PFJet450_v9', 
        'HLT_PFJet500_v9', 
        'HLT_PFJet60_v9', 
        'HLT_PFJet80_v8', 
        'HLT_PFMET110_PFMHT110_IDTight_v8', 
        'HLT_PFMET120_PFMHT120_IDTight_v8', 
        'HLT_PFMET170_BeamHaloCleaned_v7', 
        'HLT_PFMET170_HBHECleaned_v9', 
        'HLT_PFMET170_HBHE_BeamHaloCleaned_v5', 
        'HLT_PFMET170_NotCleaned_v8', 
        'HLT_PFMET300_v7', 
        'HLT_PFMET400_v7', 
        'HLT_PFMET500_v7', 
        'HLT_PFMET600_v7', 
        'HLT_PFMETNoMu110_PFMHTNoMu110_IDTight_v8', 
        'HLT_PFMETNoMu120_PFMHTNoMu120_IDTight_v8', 
        'HLT_PFMETTypeOne190_HBHE_BeamHaloCleaned_v5', 
        'HLT_PFTau120_eta2p1_v5', 
        'HLT_PFTau140_eta2p1_v5', 
        'HLT_Photon120_R9Id90_HE10_Iso40_EBOnly_PFMET40_v9', 
        'HLT_Photon120_R9Id90_HE10_Iso40_EBOnly_VBF_v7', 
        'HLT_Photon120_R9Id90_HE10_IsoM_v8', 
        'HLT_Photon120_v7', 
        'HLT_Photon135_PFMET100_v8', 
        'HLT_Photon165_HE10_v8', 
        'HLT_Photon165_R9Id90_HE10_IsoM_v9', 
        'HLT_Photon175_v8', 
        'HLT_Photon22_R9Id90_HE10_Iso40_EBOnly_PFMET40_v9', 
        'HLT_Photon22_R9Id90_HE10_Iso40_EBOnly_VBF_v7', 
        'HLT_Photon22_R9Id90_HE10_IsoM_v7', 
        'HLT_Photon22_v6', 
        'HLT_Photon250_NoHE_v7', 
        'HLT_Photon26_R9Id85_OR_CaloId24b40e_Iso50T80L_Photon16_AND_HE10_R9Id65_Eta2_Mass60_v9', 
        'HLT_Photon300_NoHE_v7', 
        'HLT_Photon30_R9Id90_HE10_IsoM_v8', 
        'HLT_Photon30_v7', 
        'HLT_Photon36_R9Id85_OR_CaloId24b40e_Iso50T80L_Photon22_AND_HE10_R9Id65_Eta2_Mass15_v9', 
        'HLT_Photon36_R9Id90_HE10_Iso40_EBOnly_PFMET40_v9', 
        'HLT_Photon36_R9Id90_HE10_Iso40_EBOnly_VBF_v8', 
        'HLT_Photon36_R9Id90_HE10_IsoM_v8', 
        'HLT_Photon36_v7', 
        'HLT_Photon42_R9Id85_OR_CaloId24b40e_Iso50T80L_Photon25_AND_HE10_R9Id65_Eta2_Mass15_v9', 
        'HLT_Photon500_v6', 
        'HLT_Photon50_R9Id90_HE10_Iso40_EBOnly_PFMET40_v9', 
        'HLT_Photon50_R9Id90_HE10_Iso40_EBOnly_VBF_v7', 
        'HLT_Photon50_R9Id90_HE10_IsoM_v8', 
        'HLT_Photon50_v7', 
        'HLT_Photon600_v6', 
        'HLT_Photon75_R9Id90_HE10_Iso40_EBOnly_PFMET40_v9', 
        'HLT_Photon75_R9Id90_HE10_Iso40_EBOnly_VBF_v7', 
        'HLT_Photon75_R9Id90_HE10_IsoM_v8', 
        'HLT_Photon75_v7', 
        'HLT_Photon90_CaloIdL_PFHT600_v8', 
        'HLT_Photon90_R9Id90_HE10_Iso40_EBOnly_PFMET40_v9', 
        'HLT_Photon90_R9Id90_HE10_Iso40_EBOnly_VBF_v7', 
        'HLT_Photon90_R9Id90_HE10_IsoM_v8', 
        'HLT_Photon90_v7', 
        'HLT_Physics_v5', 
        'HLT_QuadJet45_DoubleBTagCSV_p087_v6', 
        'HLT_QuadJet45_TripleBTagCSV_p087_v6', 
        'HLT_QuadMuon0_Dimuon0_Jpsi_v4', 
        'HLT_QuadMuon0_Dimuon0_Upsilon_v4', 
        'HLT_QuadPFJet_BTagCSV_p016_VBF_Mqq460_v5', 
        'HLT_QuadPFJet_BTagCSV_p016_VBF_Mqq500_v5', 
        'HLT_QuadPFJet_BTagCSV_p016_p11_VBF_Mqq200_v5', 
        'HLT_QuadPFJet_BTagCSV_p016_p11_VBF_Mqq240_v5', 
        'HLT_QuadPFJet_VBF_v8', 
        'HLT_Random_v2', 
        'HLT_Rsq0p02_MR400_TriPFJet80_60_40_DoubleBTagCSV_p063_Mbb60_200_v3', 
        'HLT_Rsq0p02_MR450_TriPFJet80_60_40_DoubleBTagCSV_p063_Mbb60_200_v3', 
        'HLT_Rsq0p02_MR500_TriPFJet80_60_40_DoubleBTagCSV_p063_Mbb60_200_v2', 
        'HLT_Rsq0p02_MR550_TriPFJet80_60_40_DoubleBTagCSV_p063_Mbb60_200_v2', 
        'HLT_Rsq0p25_v6', 
        'HLT_Rsq0p30_v6', 
        'HLT_RsqMR270_Rsq0p09_MR200_4jet_v6', 
        'HLT_RsqMR270_Rsq0p09_MR200_v6', 
        'HLT_SingleCentralPFJet170_CFMax0p1_v5', 
        'HLT_TkMu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ_v3', 
        'HLT_TkMu17_TrkIsoVVL_TkMu8_TrkIsoVVL_v3', 
        'HLT_TkMu17_v1', 
        'HLT_TkMu20_v4', 
        'HLT_TkMu24_eta2p1_v5', 
        'HLT_TkMu27_v5', 
        'HLT_TkMu50_v3', 
        'HLT_TripleMu_12_10_5_v4', 
        'HLT_TripleMu_5_3_3_DZ_Mass3p8_v1', 
        'HLT_TrkMu15_DoubleTrkMu5NoFiltersNoVtx_v6', 
        'HLT_TrkMu17_DoubleTrkMu8NoFiltersNoVtx_v6', 
        'HLT_VBF_DisplacedJet40_DisplacedTrack_2TrackIP2DSig5_v5', 
        'HLT_VBF_DisplacedJet40_DisplacedTrack_v5', 
        'HLT_VBF_DisplacedJet40_TightID_DisplacedTrack_v5', 
        'HLT_VBF_DisplacedJet40_TightID_Hadronic_v5', 
        'HLT_VBF_DisplacedJet40_VTightID_DisplacedTrack_v5', 
        'HLT_VBF_DisplacedJet40_VTightID_Hadronic_v5', 
        'HLT_VBF_DisplacedJet40_VVTightID_DisplacedTrack_v5', 
        'HLT_VBF_DisplacedJet40_VVTightID_Hadronic_v5', 
        'HLT_VLooseIsoPFTau120_Trk50_eta2p1_v5', 
        'HLT_VLooseIsoPFTau140_Trk50_eta2p1_v5', 
        'HLT_ZeroBias_FirstBXAfterTrain_v1', 
        'HLT_ZeroBias_FirstCollisionAfterAbortGap_copy_v1', 
        'HLT_ZeroBias_FirstCollisionAfterAbortGap_v3', 
        'HLT_ZeroBias_FirstCollisionInTrain_v1', 
        'HLT_ZeroBias_IsolatedBunches_v3', 
        'HLT_ZeroBias_v4' ) ),
    ParkingHT430to450 = cms.vstring('HLT_HT430to450_v4'),
    ParkingHT450to470 = cms.vstring('HLT_HT450to470_v4'),
    ParkingHT470to500 = cms.vstring('HLT_HT470to500_v4'),
    ParkingHT500to550 = cms.vstring('HLT_HT500to550_v4'),
    ParkingHT550to650 = cms.vstring('HLT_HT550to650_v4'),
    ParkingHT650 = cms.vstring('HLT_HT650_v5'),
    ParkingScoutingMonitor = cms.vstring('DST_CaloJet40_BTagScouting_v7', 
        'DST_CaloJet40_CaloBTagScouting_v6', 
        'DST_CaloJet40_CaloScouting_PFScouting_v7', 
        'DST_DoubleMu3_Mass10_BTagScouting_v8', 
        'DST_DoubleMu3_Mass10_CaloScouting_PFScouting_v7', 
        'DST_HT250_CaloBTagScouting_v3', 
        'DST_HT250_CaloScouting_v5', 
        'DST_HT410_BTagScouting_v7', 
        'DST_HT410_PFScouting_v7', 
        'DST_HT450_BTagScouting_v7', 
        'DST_HT450_PFScouting_v7', 
        'DST_L1DoubleMu_BTagScouting_v8', 
        'DST_L1DoubleMu_CaloScouting_PFScouting_v7', 
        'DST_L1HTT_BTagScouting_v7', 
        'DST_L1HTT_CaloBTagScouting_v6', 
        'DST_L1HTT_CaloScouting_PFScouting_v7', 
        'DST_ZeroBias_BTagScouting_v7', 
        'DST_ZeroBias_CaloScouting_PFScouting_v6', 
        'HLT_HT430to450_v4', 
        'HLT_HT450to470_v4', 
        'HLT_HT470to500_v4', 
        'HLT_HT500to550_v4', 
        'HLT_HT550to650_v4', 
        'HLT_HT650_v5'),
    RPCMonitor = cms.vstring('AlCa_RPCMuonNoHits_v10', 
        'AlCa_RPCMuonNoTriggers_v10', 
        'AlCa_RPCMuonNormalisation_v10'),
    ScoutingCaloCommissioning = cms.vstring('DST_CaloJet40_CaloBTagScouting_v6', 
        'DST_CaloJet40_CaloScouting_PFScouting_v7', 
        'DST_L1HTT_CaloBTagScouting_v6', 
        'DST_L1HTT_CaloScouting_PFScouting_v7'),
    ScoutingCaloHT = cms.vstring('DST_HT250_CaloBTagScouting_v3', 
        'DST_HT250_CaloScouting_v5'),
    ScoutingPFCommissioning = cms.vstring('DST_CaloJet40_BTagScouting_v7', 
        'DST_CaloJet40_CaloScouting_PFScouting_v7', 
        'DST_L1DoubleMu_BTagScouting_v8', 
        'DST_L1DoubleMu_CaloScouting_PFScouting_v7', 
        'DST_L1HTT_BTagScouting_v7', 
        'DST_L1HTT_CaloScouting_PFScouting_v7', 
        'DST_ZeroBias_BTagScouting_v7', 
        'DST_ZeroBias_CaloScouting_PFScouting_v6'),
    ScoutingPFHT = cms.vstring('DST_HT410_BTagScouting_v7', 
        'DST_HT410_PFScouting_v7', 
        'DST_HT450_BTagScouting_v7', 
        'DST_HT450_PFScouting_v7'),
    ScoutingPFMuons = cms.vstring('DST_DoubleMu3_Mass10_BTagScouting_v8', 
        'DST_DoubleMu3_Mass10_CaloScouting_PFScouting_v7'),
    SingleElectron = cms.vstring('HLT_Ele105_CaloIdVT_GsfTrkIdT_v8', 
        'HLT_Ele115_CaloIdVT_GsfTrkIdT_v7', 
        'HLT_Ele145_CaloIdVT_GsfTrkIdT_v1', 
        'HLT_Ele15_IsoVVVL_BTagCSV_p067_PFHT400_v7', 
        'HLT_Ele15_IsoVVVL_PFHT400_PFMET50_v6', 
        'HLT_Ele15_IsoVVVL_PFHT400_v6', 
        'HLT_Ele15_IsoVVVL_PFHT600_v9', 
        'HLT_Ele200_CaloIdVT_GsfTrkIdT_v1', 
        'HLT_Ele20_eta2p1_WPLoose_Gsf_LooseIsoPFTau28_v3', 
        'HLT_Ele22_eta2p1_WPLoose_Gsf_LooseIsoPFTau29_v3', 
        'HLT_Ele22_eta2p1_WPLoose_Gsf_v9', 
        'HLT_Ele24_eta2p1_WPLoose_Gsf_LooseIsoPFTau30_v4', 
        'HLT_Ele250_CaloIdVT_GsfTrkIdT_v6', 
        'HLT_Ele25_WPTight_Gsf_v7', 
        'HLT_Ele25_eta2p1_WPTight_Gsf_v7', 
        'HLT_Ele27_WPLoose_Gsf_WHbbBoost_v9', 
        'HLT_Ele27_WPTight_Gsf_L1JetTauSeeded_v4', 
        'HLT_Ele27_WPTight_Gsf_v7', 
        'HLT_Ele27_eta2p1_WPLoose_Gsf_HT200_v10', 
        'HLT_Ele27_eta2p1_WPLoose_Gsf_v8', 
        'HLT_Ele27_eta2p1_WPTight_Gsf_v8', 
        'HLT_Ele300_CaloIdVT_GsfTrkIdT_v6', 
        'HLT_Ele30_WPTight_Gsf_v1', 
        'HLT_Ele30_eta2p1_WPTight_Gsf_v1', 
        'HLT_Ele32_WPTight_Gsf_v1', 
        'HLT_Ele32_eta2p1_WPTight_Gsf_v8', 
        'HLT_Ele36_eta2p1_WPLoose_Gsf_LooseIsoPFTau20_SingleL1_v3', 
        'HLT_Ele45_CaloIdVT_GsfTrkIdT_PFJet200_PFJet50_v9', 
        'HLT_Ele50_CaloIdVT_GsfTrkIdT_PFJet165_v7', 
        'HLT_Ele50_IsoVVVL_PFHT400_v6'),
    SingleMuon = cms.vstring('HLT_DoubleIsoMu17_eta2p1_noDzCut_v5', 
        'HLT_IsoMu16_eta2p1_MET30_LooseIsoPFTau50_Trk30_eta2p1_v5', 
        'HLT_IsoMu16_eta2p1_MET30_v4', 
        'HLT_IsoMu19_eta2p1_LooseIsoPFTau20_SingleL1_v5', 
        'HLT_IsoMu19_eta2p1_LooseIsoPFTau20_v5', 
        'HLT_IsoMu19_eta2p1_MediumIsoPFTau32_Trk1_eta2p1_Reg_v5', 
        'HLT_IsoMu20_v6', 
        'HLT_IsoMu21_eta2p1_LooseIsoPFTau20_SingleL1_v5', 
        'HLT_IsoMu21_eta2p1_LooseIsoPFTau50_Trk30_eta2p1_SingleL1_v4', 
        'HLT_IsoMu21_eta2p1_MediumIsoPFTau32_Trk1_eta2p1_Reg_v5', 
        'HLT_IsoMu22_eta2p1_v4', 
        'HLT_IsoMu22_v5', 
        'HLT_IsoMu24_eta2p1_v3', 
        'HLT_IsoMu24_v4', 
        'HLT_IsoMu27_v7', 
        'HLT_IsoTkMu20_v7', 
        'HLT_IsoTkMu22_eta2p1_v4', 
        'HLT_IsoTkMu22_v5', 
        'HLT_IsoTkMu24_eta2p1_v3', 
        'HLT_IsoTkMu24_v4', 
        'HLT_IsoTkMu27_v7', 
        'HLT_L1SingleMu18_v1', 
        'HLT_L2Mu10_v3', 
        'HLT_Mu10_TrkIsoVVL_DiPFJet40_DEta3p5_MJJ750_HTT350_PFMETNoMu60_v5', 
        'HLT_Mu15_IsoVVVL_BTagCSV_p067_PFHT400_v6', 
        'HLT_Mu15_IsoVVVL_PFHT400_PFMET50_v5', 
        'HLT_Mu15_IsoVVVL_PFHT400_v5', 
        'HLT_Mu15_IsoVVVL_PFHT600_v8', 
        'HLT_Mu20_v4', 
        'HLT_Mu24_eta2p1_v5', 
        'HLT_Mu27_v5', 
        'HLT_Mu28NoFiltersNoVtx_CentralCaloJet40_v5', 
        'HLT_Mu28NoFiltersNoVtx_DisplacedJet40_Loose_v5', 
        'HLT_Mu300_v3', 
        'HLT_Mu30_eta2p1_PFJet150_PFJet50_v5', 
        'HLT_Mu350_v3', 
        'HLT_Mu38NoFiltersNoVtxDisplaced_DisplacedJet60_Loose_v5', 
        'HLT_Mu38NoFiltersNoVtxDisplaced_DisplacedJet60_Tight_v5', 
        'HLT_Mu38NoFiltersNoVtx_DisplacedJet60_Loose_v5', 
        'HLT_Mu40_eta2p1_PFJet200_PFJet50_v7', 
        'HLT_Mu45_eta2p1_v5', 
        'HLT_Mu50_IsoVVVL_PFHT400_v5', 
        'HLT_Mu50_v5', 
        'HLT_Mu55_v4', 
        'HLT_Mu8_TrkIsoVVL_DiPFJet40_DEta3p5_MJJ750_HTT300_PFMETNoMu60_v4', 
        'HLT_TkMu17_v1', 
        'HLT_TkMu20_v4', 
        'HLT_TkMu24_eta2p1_v5', 
        'HLT_TkMu27_v5', 
        'HLT_TkMu50_v3'),
    SinglePhoton = cms.vstring('HLT_Photon120_R9Id90_HE10_Iso40_EBOnly_PFMET40_v9', 
        'HLT_Photon120_R9Id90_HE10_Iso40_EBOnly_VBF_v7', 
        'HLT_Photon120_R9Id90_HE10_IsoM_v8', 
        'HLT_Photon120_v7', 
        'HLT_Photon135_PFMET100_v8', 
        'HLT_Photon165_HE10_v8', 
        'HLT_Photon165_R9Id90_HE10_IsoM_v9', 
        'HLT_Photon175_v8', 
        'HLT_Photon22_R9Id90_HE10_Iso40_EBOnly_PFMET40_v9', 
        'HLT_Photon22_R9Id90_HE10_Iso40_EBOnly_VBF_v7', 
        'HLT_Photon22_R9Id90_HE10_IsoM_v7', 
        'HLT_Photon22_v6', 
        'HLT_Photon250_NoHE_v7', 
        'HLT_Photon300_NoHE_v7', 
        'HLT_Photon30_R9Id90_HE10_IsoM_v8', 
        'HLT_Photon30_v7', 
        'HLT_Photon36_R9Id90_HE10_Iso40_EBOnly_PFMET40_v9', 
        'HLT_Photon36_R9Id90_HE10_Iso40_EBOnly_VBF_v8', 
        'HLT_Photon36_R9Id90_HE10_IsoM_v8', 
        'HLT_Photon36_v7', 
        'HLT_Photon500_v6', 
        'HLT_Photon50_R9Id90_HE10_Iso40_EBOnly_PFMET40_v9', 
        'HLT_Photon50_R9Id90_HE10_Iso40_EBOnly_VBF_v7', 
        'HLT_Photon50_R9Id90_HE10_IsoM_v8', 
        'HLT_Photon50_v7', 
        'HLT_Photon600_v6', 
        'HLT_Photon75_R9Id90_HE10_Iso40_EBOnly_PFMET40_v9', 
        'HLT_Photon75_R9Id90_HE10_Iso40_EBOnly_VBF_v7', 
        'HLT_Photon75_R9Id90_HE10_IsoM_v8', 
        'HLT_Photon75_v7', 
        'HLT_Photon90_CaloIdL_PFHT600_v8', 
        'HLT_Photon90_R9Id90_HE10_Iso40_EBOnly_PFMET40_v9', 
        'HLT_Photon90_R9Id90_HE10_Iso40_EBOnly_VBF_v7', 
        'HLT_Photon90_R9Id90_HE10_IsoM_v8', 
        'HLT_Photon90_v7'),
    Tau = cms.vstring('HLT_DoubleMediumCombinedIsoPFTau35_Trk1_eta2p1_Reg_v3', 
        'HLT_DoubleMediumCombinedIsoPFTau40_Trk1_eta2p1_Reg_v2', 
        'HLT_DoubleMediumCombinedIsoPFTau40_Trk1_eta2p1_v2', 
        'HLT_DoubleTightCombinedIsoPFTau35_Trk1_eta2p1_Reg_v3', 
        'HLT_DoubleTightCombinedIsoPFTau40_Trk1_eta2p1_Reg_v2', 
        'HLT_DoubleTightCombinedIsoPFTau40_Trk1_eta2p1_v2', 
        'HLT_IsoMu19_eta2p1_LooseCombinedIsoPFTau20_v1', 
        'HLT_IsoMu19_eta2p1_MediumCombinedIsoPFTau32_Trk1_eta2p1_Reg_v1', 
        'HLT_IsoMu19_eta2p1_TightCombinedIsoPFTau32_Trk1_eta2p1_Reg_v1', 
        'HLT_IsoMu21_eta2p1_MediumCombinedIsoPFTau32_Trk1_eta2p1_Reg_v1', 
        'HLT_IsoMu21_eta2p1_TightCombinedIsoPFTau32_Trk1_eta2p1_Reg_v1', 
        'HLT_LooseIsoPFTau50_Trk30_eta2p1_MET110_v6', 
        'HLT_LooseIsoPFTau50_Trk30_eta2p1_MET120_v6', 
        'HLT_LooseIsoPFTau50_Trk30_eta2p1_MET90_v6', 
        'HLT_LooseIsoPFTau50_Trk30_eta2p1_v7', 
        'HLT_PFTau120_eta2p1_v5', 
        'HLT_PFTau140_eta2p1_v5', 
        'HLT_VLooseIsoPFTau120_Trk50_eta2p1_v5', 
        'HLT_VLooseIsoPFTau140_Trk50_eta2p1_v5'),
    TestEnablesEcalHcal = cms.vstring('HLT_EcalCalibration_v3', 
        'HLT_HcalCalibration_v4'),
    TestEnablesEcalHcalDQM = cms.vstring('HLT_EcalCalibration_v3', 
        'HLT_HcalCalibration_v4'),
    ZeroBias = cms.vstring('HLT_Random_v2', 
        'HLT_ZeroBias_FirstBXAfterTrain_v1', 
        'HLT_ZeroBias_FirstCollisionAfterAbortGap_copy_v1', 
        'HLT_ZeroBias_FirstCollisionAfterAbortGap_v3', 
        'HLT_ZeroBias_FirstCollisionInTrain_v1', 
        'HLT_ZeroBias_IsolatedBunches_v3', 
        'HLT_ZeroBias_v4')
)

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

process.options = cms.untracked.PSet(
    SkipEvent = cms.untracked.vstring('ProductNotFound')
)

process.streams = cms.PSet(
    ALCAELECTRON = cms.vstring('AlCaElectron'),
    ALCALUMIPIXELS = cms.vstring('AlCaLumiPixels'),
    ALCAP0 = cms.vstring('AlCaP0'),
    ALCAPHISYM = cms.vstring('AlCaPhiSym'),
    Calibration = cms.vstring('TestEnablesEcalHcal'),
    DQM = cms.vstring('OnlineMonitor'),
    DQMCalibration = cms.vstring('TestEnablesEcalHcalDQM'),
    DQMEventDisplay = cms.vstring('EventDisplay'),
    EcalCalibration = cms.vstring('EcalLaser'),
    Express = cms.vstring('ExpressPhysics'),
    HLTMonitor = cms.vstring('HLTMonitor'),
    NanoDST = cms.vstring('L1Accept'),
    Parking = cms.vstring('ParkingHT430to450', 
        'ParkingHT450to470', 
        'ParkingHT470to500', 
        'ParkingHT500to550', 
        'ParkingHT550to650', 
        'ParkingHT650'),
    ParkingHLTPhysics = cms.vstring('HLTPhysics0', 
        'HLTPhysics1', 
        'HLTPhysics2', 
        'HLTPhysics3'),
    PhysicsCommissioning = cms.vstring('Commissioning', 
        'HLTPhysics', 
        'HcalHPDNoise', 
        'HcalNZS', 
        'MonteCarlo', 
        'NoBPTX', 
        'ZeroBias'),
    PhysicsEGamma = cms.vstring('DoubleEG', 
        'SingleElectron', 
        'SinglePhoton'),
    PhysicsEndOfFill = cms.vstring('EmptyBX', 
        'FSQJets', 
        'HINCaloJets', 
        'HINPFJets', 
        'HINPhoton', 
        'HighMultiplicityEOF', 
        'L1MinimumBias'),
    PhysicsHadronsTaus = cms.vstring('BTagCSV', 
        'BTagMu', 
        'DisplacedJet', 
        'HTMHT', 
        'JetHT', 
        'MET', 
        'Tau'),
    PhysicsMinimumBias0 = cms.vstring('L1MinimumBias0', 
        'L1MinimumBias1', 
        'L1MinimumBias2'),
    PhysicsMinimumBias1 = cms.vstring('L1MinimumBias3', 
        'L1MinimumBias4', 
        'L1MinimumBias5'),
    PhysicsMinimumBias2 = cms.vstring('L1MinimumBias6', 
        'L1MinimumBias7', 
        'L1MinimumBias8', 
        'L1MinimumBias9'),
    PhysicsMuons = cms.vstring('Charmonium', 
        'DoubleMuon', 
        'DoubleMuonLowMass', 
        'MuOnia', 
        'MuonEG', 
        'SingleMuon'),
    PhysicsParkingScoutingMonitor = cms.vstring('ParkingScoutingMonitor'),
    RPCMON = cms.vstring('RPCMonitor'),
    ScoutingCalo = cms.vstring('ScoutingCaloCommissioning', 
        'ScoutingCaloHT'),
    ScoutingPF = cms.vstring('ScoutingPFCommissioning', 
        'ScoutingPFHT', 
        'ScoutingPFMuons')
)

process.transferSystem = cms.PSet(
    default = cms.PSet(
        default = cms.vstring('Tier0'),
        emulator = cms.vstring('Lustre'),
        streamLookArea = cms.PSet(

        ),
        test = cms.vstring('Lustre')
    ),
    destinations = cms.vstring('Tier0', 
        'DQM', 
        'ECAL', 
        'EventDisplay', 
        'Lustre', 
        'None'),
    streamA = cms.PSet(
        default = cms.vstring('Tier0'),
        emulator = cms.vstring('Lustre'),
        test = cms.vstring('Lustre')
    ),
    streamCalibration = cms.PSet(
        default = cms.vstring('Tier0'),
        emulator = cms.vstring('None'),
        test = cms.vstring('Lustre')
    ),
    streamDQM = cms.PSet(
        default = cms.vstring('DQM'),
        emulator = cms.vstring('None'),
        test = cms.vstring('DQM', 
            'Lustre')
    ),
    streamDQMCalibration = cms.PSet(
        default = cms.vstring('DQM'),
        emulator = cms.vstring('None'),
        test = cms.vstring('DQM', 
            'Lustre')
    ),
    streamEcalCalibration = cms.PSet(
        default = cms.vstring('ECAL'),
        emulator = cms.vstring('None'),
        test = cms.vstring('ECAL')
    ),
    streamEventDisplay = cms.PSet(
        default = cms.vstring('EventDisplay', 
            'Tier0'),
        emulator = cms.vstring('None'),
        test = cms.vstring('EventDisplay', 
            'Lustre')
    ),
    streamExpressCosmics = cms.PSet(
        default = cms.vstring('Tier0'),
        emulator = cms.vstring('Lustre'),
        test = cms.vstring('Lustre')
    ),
    streamLookArea = cms.PSet(
        default = cms.vstring('DQM'),
        emulator = cms.vstring('None'),
        test = cms.vstring('DQM', 
            'Lustre')
    ),
    streamNanoDST = cms.PSet(
        default = cms.vstring('Tier0'),
        emulator = cms.vstring('None'),
        test = cms.vstring('Lustre')
    ),
    streamRPCMON = cms.PSet(
        default = cms.vstring('Tier0'),
        emulator = cms.vstring('None'),
        test = cms.vstring('Lustre')
    ),
    streamTrackerCalibration = cms.PSet(
        default = cms.vstring('Tier0'),
        emulator = cms.vstring('None'),
        test = cms.vstring('Lustre')
    ),
    transferModes = cms.vstring('default', 
        'test', 
        'emulator')
)

process.hltAK4CaloAbsoluteCorrector = cms.EDProducer("LXXXCorrectorProducer",
    algorithm = cms.string('AK4CaloHLT'),
    level = cms.string('L3Absolute')
)


process.hltAK4CaloCorrector = cms.EDProducer("ChainedJetCorrectorProducer",
    correctors = cms.VInputTag("hltAK4CaloFastJetCorrector", "hltAK4CaloRelativeCorrector", "hltAK4CaloAbsoluteCorrector", "hltAK4CaloResidualCorrector")
)


process.hltAK4CaloFastJetCorrector = cms.EDProducer("L1FastjetCorrectorProducer",
    algorithm = cms.string('AK4CaloHLT'),
    level = cms.string('L1FastJet'),
    srcRho = cms.InputTag("hltFixedGridRhoFastjetAllCalo")
)


process.hltAK4CaloJets = cms.EDProducer("FastjetJetProducer",
    Active_Area_Repeats = cms.int32(5),
    DxyTrVtxMax = cms.double(0.0),
    DzTrVtxMax = cms.double(0.0),
    GhostArea = cms.double(0.01),
    Ghost_EtaMax = cms.double(6.0),
    MaxVtxZ = cms.double(15.0),
    MinVtxNdof = cms.int32(5),
    Rho_EtaMax = cms.double(4.4),
    UseOnlyOnePV = cms.bool(False),
    UseOnlyVertexTracks = cms.bool(False),
    dRMax = cms.double(-1.0),
    dRMin = cms.double(-1.0),
    doAreaDiskApprox = cms.bool(True),
    doAreaFastjet = cms.bool(False),
    doOutputJets = cms.bool(True),
    doPUOffsetCorr = cms.bool(False),
    doPVCorrection = cms.bool(False),
    doRhoFastjet = cms.bool(False),
    inputEMin = cms.double(0.0),
    inputEtMin = cms.double(0.3),
    jetAlgorithm = cms.string('AntiKt'),
    jetPtMin = cms.double(1.0),
    jetType = cms.string('CaloJet'),
    maxBadEcalCells = cms.uint32(9999999),
    maxBadHcalCells = cms.uint32(9999999),
    maxDepth = cms.int32(-1),
    maxProblematicEcalCells = cms.uint32(9999999),
    maxProblematicHcalCells = cms.uint32(9999999),
    maxRecoveredEcalCells = cms.uint32(9999999),
    maxRecoveredHcalCells = cms.uint32(9999999),
    minSeed = cms.uint32(14327),
    muCut = cms.double(-1.0),
    muMax = cms.double(-1.0),
    muMin = cms.double(-1.0),
    nFilt = cms.int32(-1),
    nSigmaPU = cms.double(1.0),
    puPtMin = cms.double(10.0),
    rFilt = cms.double(-1.0),
    rParam = cms.double(0.4),
    radiusPU = cms.double(0.4),
    rcut_factor = cms.double(-1.0),
    src = cms.InputTag("hltTowerMakerForAll"),
    srcPVs = cms.InputTag("NotUsed"),
    subjetPtMin = cms.double(-1.0),
    subtractorName = cms.string(''),
    sumRecHits = cms.bool(False),
    trimPtFracMin = cms.double(-1.0),
    useCMSBoostedTauSeedingAlgorithm = cms.bool(False),
    useDeterministicSeed = cms.bool(True),
    useFiltering = cms.bool(False),
    useMassDropTagger = cms.bool(False),
    usePruning = cms.bool(False),
    useTrimming = cms.bool(False),
    voronoiRfact = cms.double(0.9),
    yCut = cms.double(-1.0),
    yMax = cms.double(-1.0),
    yMin = cms.double(-1.0),
    zcut = cms.double(-1.0)
)


process.hltAK4CaloJetsCorrected = cms.EDProducer("CorrectedCaloJetProducer",
    correctors = cms.VInputTag("hltAK4CaloCorrector"),
    src = cms.InputTag("hltAK4CaloJets")
)


process.hltAK4CaloJetsCorrectedIDPassed = cms.EDProducer("CorrectedCaloJetProducer",
    correctors = cms.VInputTag("hltAK4CaloCorrector"),
    src = cms.InputTag("hltAK4CaloJetsIDPassed")
)


process.hltAK4CaloJetsIDPassed = cms.EDProducer("HLTCaloJetIDProducer",
    JetIDParams = cms.PSet(
        ebRecHitsColl = cms.InputTag("hltEcalRecHit","EcalRecHitsEB"),
        eeRecHitsColl = cms.InputTag("hltEcalRecHit","EcalRecHitsEE"),
        hbheRecHitsColl = cms.InputTag("hltHbhereco"),
        hfRecHitsColl = cms.InputTag("hltHfreco"),
        hoRecHitsColl = cms.InputTag("hltHoreco"),
        useRecHits = cms.bool(True)
    ),
    jetsInput = cms.InputTag("hltAK4CaloJets"),
    max_EMF = cms.double(999.0),
    min_EMF = cms.double(1e-06),
    min_N90 = cms.int32(-2),
    min_N90hits = cms.int32(2)
)


process.hltAK4CaloJetsPF = cms.EDProducer("FastjetJetProducer",
    Active_Area_Repeats = cms.int32(5),
    DxyTrVtxMax = cms.double(0.0),
    DzTrVtxMax = cms.double(0.0),
    GhostArea = cms.double(0.01),
    Ghost_EtaMax = cms.double(6.0),
    MaxVtxZ = cms.double(15.0),
    MinVtxNdof = cms.int32(5),
    Rho_EtaMax = cms.double(4.4),
    UseOnlyOnePV = cms.bool(False),
    UseOnlyVertexTracks = cms.bool(False),
    dRMax = cms.double(-1.0),
    dRMin = cms.double(-1.0),
    doAreaDiskApprox = cms.bool(False),
    doAreaFastjet = cms.bool(False),
    doOutputJets = cms.bool(True),
    doPUOffsetCorr = cms.bool(False),
    doPVCorrection = cms.bool(False),
    doRhoFastjet = cms.bool(False),
    inputEMin = cms.double(0.0),
    inputEtMin = cms.double(0.3),
    jetAlgorithm = cms.string('AntiKt'),
    jetPtMin = cms.double(1.0),
    jetType = cms.string('CaloJet'),
    maxBadEcalCells = cms.uint32(9999999),
    maxBadHcalCells = cms.uint32(9999999),
    maxDepth = cms.int32(-1),
    maxProblematicEcalCells = cms.uint32(9999999),
    maxProblematicHcalCells = cms.uint32(9999999),
    maxRecoveredEcalCells = cms.uint32(9999999),
    maxRecoveredHcalCells = cms.uint32(9999999),
    minSeed = cms.uint32(0),
    muCut = cms.double(-1.0),
    muMax = cms.double(-1.0),
    muMin = cms.double(-1.0),
    nFilt = cms.int32(-1),
    nSigmaPU = cms.double(1.0),
    puPtMin = cms.double(10.0),
    rFilt = cms.double(-1.0),
    rParam = cms.double(0.4),
    radiusPU = cms.double(0.4),
    rcut_factor = cms.double(-1.0),
    src = cms.InputTag("hltTowerMakerForPF"),
    srcPVs = cms.InputTag("NotUsed"),
    subjetPtMin = cms.double(-1.0),
    subtractorName = cms.string(''),
    sumRecHits = cms.bool(False),
    trimPtFracMin = cms.double(-1.0),
    useCMSBoostedTauSeedingAlgorithm = cms.bool(False),
    useDeterministicSeed = cms.bool(True),
    useFiltering = cms.bool(False),
    useMassDropTagger = cms.bool(False),
    usePruning = cms.bool(False),
    useTrimming = cms.bool(False),
    voronoiRfact = cms.double(-9.0),
    yCut = cms.double(-1.0),
    yMax = cms.double(-1.0),
    yMin = cms.double(-1.0),
    zcut = cms.double(-1.0)
)


process.hltAK4CaloRelativeCorrector = cms.EDProducer("LXXXCorrectorProducer",
    algorithm = cms.string('AK4CaloHLT'),
    level = cms.string('L2Relative')
)


process.hltAK4CaloResidualCorrector = cms.EDProducer("LXXXCorrectorProducer",
    algorithm = cms.string('AK4CaloHLT'),
    level = cms.string('L2L3Residual')
)


process.hltAK4Iter0TrackJets4Iter1 = cms.EDProducer("FastjetJetProducer",
    Active_Area_Repeats = cms.int32(5),
    DxyTrVtxMax = cms.double(0.2),
    DzTrVtxMax = cms.double(0.5),
    GhostArea = cms.double(0.01),
    Ghost_EtaMax = cms.double(6.0),
    MaxVtxZ = cms.double(30.0),
    MinVtxNdof = cms.int32(0),
    Rho_EtaMax = cms.double(4.4),
    UseOnlyOnePV = cms.bool(True),
    UseOnlyVertexTracks = cms.bool(False),
    dRMax = cms.double(-1.0),
    dRMin = cms.double(-1.0),
    doAreaDiskApprox = cms.bool(False),
    doAreaFastjet = cms.bool(False),
    doOutputJets = cms.bool(True),
    doPUOffsetCorr = cms.bool(False),
    doPVCorrection = cms.bool(False),
    doRhoFastjet = cms.bool(False),
    inputEMin = cms.double(0.0),
    inputEtMin = cms.double(0.1),
    jetAlgorithm = cms.string('AntiKt'),
    jetPtMin = cms.double(1.0),
    jetType = cms.string('TrackJet'),
    maxBadEcalCells = cms.uint32(9999999),
    maxBadHcalCells = cms.uint32(9999999),
    maxDepth = cms.int32(-1),
    maxProblematicEcalCells = cms.uint32(9999999),
    maxProblematicHcalCells = cms.uint32(9999999),
    maxRecoveredEcalCells = cms.uint32(9999999),
    maxRecoveredHcalCells = cms.uint32(9999999),
    minSeed = cms.uint32(14327),
    muCut = cms.double(-1.0),
    muMax = cms.double(-1.0),
    muMin = cms.double(-1.0),
    nFilt = cms.int32(-1),
    nSigmaPU = cms.double(1.0),
    puPtMin = cms.double(0.0),
    rFilt = cms.double(-1.0),
    rParam = cms.double(0.4),
    radiusPU = cms.double(0.4),
    rcut_factor = cms.double(-1.0),
    src = cms.InputTag("hltTrackIter0RefsForJets4Iter1"),
    srcPVs = cms.InputTag("hltTrimmedPixelVertices"),
    subjetPtMin = cms.double(-1.0),
    subtractorName = cms.string(''),
    sumRecHits = cms.bool(False),
    trimPtFracMin = cms.double(-1.0),
    useCMSBoostedTauSeedingAlgorithm = cms.bool(False),
    useDeterministicSeed = cms.bool(True),
    useFiltering = cms.bool(False),
    useMassDropTagger = cms.bool(False),
    usePruning = cms.bool(False),
    useTrimming = cms.bool(False),
    voronoiRfact = cms.double(0.9),
    yCut = cms.double(-1.0),
    yMax = cms.double(-1.0),
    yMin = cms.double(-1.0),
    zcut = cms.double(-1.0)
)


process.hltAK4Iter1TrackJets4Iter2 = cms.EDProducer("FastjetJetProducer",
    Active_Area_Repeats = cms.int32(5),
    DxyTrVtxMax = cms.double(0.2),
    DzTrVtxMax = cms.double(0.5),
    GhostArea = cms.double(0.01),
    Ghost_EtaMax = cms.double(6.0),
    MaxVtxZ = cms.double(30.0),
    MinVtxNdof = cms.int32(0),
    Rho_EtaMax = cms.double(4.4),
    UseOnlyOnePV = cms.bool(True),
    UseOnlyVertexTracks = cms.bool(False),
    dRMax = cms.double(-1.0),
    dRMin = cms.double(-1.0),
    doAreaDiskApprox = cms.bool(False),
    doAreaFastjet = cms.bool(False),
    doOutputJets = cms.bool(True),
    doPUOffsetCorr = cms.bool(False),
    doPVCorrection = cms.bool(False),
    doRhoFastjet = cms.bool(False),
    inputEMin = cms.double(0.0),
    inputEtMin = cms.double(0.1),
    jetAlgorithm = cms.string('AntiKt'),
    jetPtMin = cms.double(7.5),
    jetType = cms.string('TrackJet'),
    maxBadEcalCells = cms.uint32(9999999),
    maxBadHcalCells = cms.uint32(9999999),
    maxDepth = cms.int32(-1),
    maxProblematicEcalCells = cms.uint32(9999999),
    maxProblematicHcalCells = cms.uint32(9999999),
    maxRecoveredEcalCells = cms.uint32(9999999),
    maxRecoveredHcalCells = cms.uint32(9999999),
    minSeed = cms.uint32(14327),
    muCut = cms.double(-1.0),
    muMax = cms.double(-1.0),
    muMin = cms.double(-1.0),
    nFilt = cms.int32(-1),
    nSigmaPU = cms.double(1.0),
    puPtMin = cms.double(0.0),
    rFilt = cms.double(-1.0),
    rParam = cms.double(0.4),
    radiusPU = cms.double(0.4),
    rcut_factor = cms.double(-1.0),
    src = cms.InputTag("hltIter1TrackRefsForJets4Iter2"),
    srcPVs = cms.InputTag("hltTrimmedPixelVertices"),
    subjetPtMin = cms.double(-1.0),
    subtractorName = cms.string(''),
    sumRecHits = cms.bool(False),
    trimPtFracMin = cms.double(-1.0),
    useCMSBoostedTauSeedingAlgorithm = cms.bool(False),
    useDeterministicSeed = cms.bool(True),
    useFiltering = cms.bool(False),
    useMassDropTagger = cms.bool(False),
    usePruning = cms.bool(False),
    useTrimming = cms.bool(False),
    voronoiRfact = cms.double(0.9),
    yCut = cms.double(-1.0),
    yMax = cms.double(-1.0),
    yMin = cms.double(-1.0),
    zcut = cms.double(-1.0)
)


process.hltAK4PFAbsoluteCorrector = cms.EDProducer("LXXXCorrectorProducer",
    algorithm = cms.string('AK4PFHLT'),
    level = cms.string('L3Absolute')
)


process.hltAK4PFCorrector = cms.EDProducer("ChainedJetCorrectorProducer",
    correctors = cms.VInputTag("hltAK4PFFastJetCorrector", "hltAK4PFRelativeCorrector", "hltAK4PFAbsoluteCorrector", "hltAK4PFResidualCorrector")
)


process.hltAK4PFFastJetCorrector = cms.EDProducer("L1FastjetCorrectorProducer",
    algorithm = cms.string('AK4PFHLT'),
    level = cms.string('L1FastJet'),
    srcRho = cms.InputTag("hltFixedGridRhoFastjetAll")
)


process.hltAK4PFJets = cms.EDProducer("FastjetJetProducer",
    Active_Area_Repeats = cms.int32(5),
    DxyTrVtxMax = cms.double(0.0),
    DzTrVtxMax = cms.double(0.0),
    GhostArea = cms.double(0.01),
    Ghost_EtaMax = cms.double(6.0),
    MaxVtxZ = cms.double(15.0),
    MinVtxNdof = cms.int32(0),
    Rho_EtaMax = cms.double(4.4),
    UseOnlyOnePV = cms.bool(False),
    UseOnlyVertexTracks = cms.bool(False),
    dRMax = cms.double(-1.0),
    dRMin = cms.double(-1.0),
    doAreaDiskApprox = cms.bool(True),
    doAreaFastjet = cms.bool(False),
    doOutputJets = cms.bool(True),
    doPUOffsetCorr = cms.bool(False),
    doPVCorrection = cms.bool(False),
    doRhoFastjet = cms.bool(False),
    inputEMin = cms.double(0.0),
    inputEtMin = cms.double(0.0),
    jetAlgorithm = cms.string('AntiKt'),
    jetPtMin = cms.double(0.0),
    jetType = cms.string('PFJet'),
    maxBadEcalCells = cms.uint32(9999999),
    maxBadHcalCells = cms.uint32(9999999),
    maxDepth = cms.int32(-1),
    maxProblematicEcalCells = cms.uint32(9999999),
    maxProblematicHcalCells = cms.uint32(9999999),
    maxRecoveredEcalCells = cms.uint32(9999999),
    maxRecoveredHcalCells = cms.uint32(9999999),
    minSeed = cms.uint32(0),
    muCut = cms.double(-1.0),
    muMax = cms.double(-1.0),
    muMin = cms.double(-1.0),
    nFilt = cms.int32(-1),
    nSigmaPU = cms.double(1.0),
    puPtMin = cms.double(10.0),
    rFilt = cms.double(-1.0),
    rParam = cms.double(0.4),
    radiusPU = cms.double(0.4),
    rcut_factor = cms.double(-1.0),
    src = cms.InputTag("hltParticleFlow"),
    srcPVs = cms.InputTag("hltPixelVertices"),
    subjetPtMin = cms.double(-1.0),
    subtractorName = cms.string(''),
    sumRecHits = cms.bool(False),
    trimPtFracMin = cms.double(-1.0),
    useCMSBoostedTauSeedingAlgorithm = cms.bool(False),
    useDeterministicSeed = cms.bool(True),
    useFiltering = cms.bool(False),
    useMassDropTagger = cms.bool(False),
    usePruning = cms.bool(False),
    useTrimming = cms.bool(False),
    voronoiRfact = cms.double(-9.0),
    yCut = cms.double(-1.0),
    yMax = cms.double(-1.0),
    yMin = cms.double(-1.0),
    zcut = cms.double(-1.0)
)


process.hltAK4PFJetsCorrected = cms.EDProducer("CorrectedPFJetProducer",
    correctors = cms.VInputTag("hltAK4PFCorrector"),
    src = cms.InputTag("hltAK4PFJets")
)


process.hltAK4PFJetsLooseID = cms.EDProducer("HLTPFJetIDProducer",
    CEF = cms.double(0.99),
    CHF = cms.double(0.0),
    NCH = cms.int32(0),
    NEF = cms.double(0.99),
    NHF = cms.double(0.99),
    NTOT = cms.int32(1),
    jetsInput = cms.InputTag("hltAK4PFJets"),
    maxCF = cms.double(99.0),
    maxEta = cms.double(1e+99),
    minPt = cms.double(20.0)
)


process.hltAK4PFJetsLooseIDCorrected = cms.EDProducer("CorrectedPFJetProducer",
    correctors = cms.VInputTag("hltAK4PFCorrector"),
    src = cms.InputTag("hltAK4PFJetsLooseID")
)


process.hltAK4PFJetsTightID = cms.EDProducer("HLTPFJetIDProducer",
    CEF = cms.double(0.99),
    CHF = cms.double(0.0),
    NCH = cms.int32(0),
    NEF = cms.double(0.99),
    NHF = cms.double(0.9),
    NTOT = cms.int32(1),
    jetsInput = cms.InputTag("hltAK4PFJets"),
    maxCF = cms.double(99.0),
    maxEta = cms.double(1e+99),
    minPt = cms.double(20.0)
)


process.hltAK4PFJetsTightIDCorrected = cms.EDProducer("CorrectedPFJetProducer",
    correctors = cms.VInputTag("hltAK4PFCorrector"),
    src = cms.InputTag("hltAK4PFJetsTightID")
)


process.hltAK4PFRelativeCorrector = cms.EDProducer("LXXXCorrectorProducer",
    algorithm = cms.string('AK4PFHLT'),
    level = cms.string('L2Relative')
)


process.hltAK4PFResidualCorrector = cms.EDProducer("LXXXCorrectorProducer",
    algorithm = cms.string('AK4PFHLT'),
    level = cms.string('L2L3Residual')
)


process.hltCaloJetsCorrectedMatchedToL1 = cms.EDProducer("HLTCaloJetL1TMatchProducer",
    DeltaR = cms.double(0.5),
    L1Jets = cms.InputTag("hltCaloStage2Digis","Jet"),
    jetsInput = cms.InputTag("hltAK4CaloJetsCorrectedIDPassed")
)


process.hltCaloStage2Digis = cms.EDProducer("L1TRawToDigi",
    CTP7 = cms.untracked.bool(False),
    FWId = cms.uint32(0),
    FWOverride = cms.bool(False),
    FedIds = cms.vint32(1360, 1366),
    InputLabel = cms.InputTag("rawDataCollector"),
    MTF7 = cms.untracked.bool(False),
    MinFeds = cms.uint32(0),
    Setup = cms.string('stage2::CaloSetup'),
    debug = cms.untracked.bool(False),
    lenAMC13Header = cms.untracked.int32(8),
    lenAMC13Trailer = cms.untracked.int32(8),
    lenAMCHeader = cms.untracked.int32(8),
    lenAMCTrailer = cms.untracked.int32(0),
    lenSlinkHeader = cms.untracked.int32(8),
    lenSlinkTrailer = cms.untracked.int32(8)
)


process.hltCsc2DRecHits = cms.EDProducer("CSCRecHitDProducer",
    CSCDebug = cms.untracked.bool(False),
    CSCNoOfTimeBinsForDynamicPedestal = cms.int32(2),
    CSCStripClusterChargeCut = cms.double(25.0),
    CSCStripClusterSize = cms.untracked.int32(3),
    CSCStripPeakThreshold = cms.double(10.0),
    CSCStripxtalksOffset = cms.double(0.03),
    CSCUseCalibrations = cms.bool(True),
    CSCUseGasGainCorrections = cms.bool(False),
    CSCUseStaticPedestals = cms.bool(False),
    CSCUseTimingCorrections = cms.bool(True),
    CSCWireClusterDeltaT = cms.int32(1),
    CSCstripWireDeltaTime = cms.int32(8),
    ConstSyst_ME12 = cms.double(0.0),
    ConstSyst_ME13 = cms.double(0.0),
    ConstSyst_ME1a = cms.double(0.022),
    ConstSyst_ME1b = cms.double(0.007),
    ConstSyst_ME21 = cms.double(0.0),
    ConstSyst_ME22 = cms.double(0.0),
    ConstSyst_ME31 = cms.double(0.0),
    ConstSyst_ME32 = cms.double(0.0),
    ConstSyst_ME41 = cms.double(0.0),
    NoiseLevel_ME12 = cms.double(9.0),
    NoiseLevel_ME13 = cms.double(8.0),
    NoiseLevel_ME1a = cms.double(7.0),
    NoiseLevel_ME1b = cms.double(8.0),
    NoiseLevel_ME21 = cms.double(9.0),
    NoiseLevel_ME22 = cms.double(9.0),
    NoiseLevel_ME31 = cms.double(9.0),
    NoiseLevel_ME32 = cms.double(9.0),
    NoiseLevel_ME41 = cms.double(9.0),
    UseAverageTime = cms.bool(False),
    UseFivePoleFit = cms.bool(True),
    UseParabolaFit = cms.bool(False),
    XTasymmetry_ME12 = cms.double(0.0),
    XTasymmetry_ME13 = cms.double(0.0),
    XTasymmetry_ME1a = cms.double(0.0),
    XTasymmetry_ME1b = cms.double(0.0),
    XTasymmetry_ME21 = cms.double(0.0),
    XTasymmetry_ME22 = cms.double(0.0),
    XTasymmetry_ME31 = cms.double(0.0),
    XTasymmetry_ME32 = cms.double(0.0),
    XTasymmetry_ME41 = cms.double(0.0),
    readBadChambers = cms.bool(True),
    readBadChannels = cms.bool(False),
    stripDigiTag = cms.InputTag("hltMuonCSCDigis","MuonCSCStripDigi"),
    wireDigiTag = cms.InputTag("hltMuonCSCDigis","MuonCSCWireDigi")
)


process.hltCscSegments = cms.EDProducer("CSCSegmentProducer",
    algo_psets = cms.VPSet(cms.PSet(
        algo_name = cms.string('CSCSegAlgoST'),
        algo_psets = cms.VPSet(cms.PSet(
            BPMinImprovement = cms.double(10000.0),
            BrutePruning = cms.bool(True),
            CSCDebug = cms.untracked.bool(False),
            CorrectTheErrors = cms.bool(True),
            Covariance = cms.double(0.0),
            ForceCovariance = cms.bool(False),
            ForceCovarianceAll = cms.bool(False),
            NormChi2Cut2D = cms.double(20.0),
            NormChi2Cut3D = cms.double(10.0),
            Pruning = cms.bool(True),
            SeedBig = cms.double(0.0015),
            SeedSmall = cms.double(0.0002),
            curvePenalty = cms.double(2.0),
            curvePenaltyThreshold = cms.double(0.85),
            dPhiFineMax = cms.double(0.025),
            dRPhiFineMax = cms.double(8.0),
            dXclusBoxMax = cms.double(4.0),
            dYclusBoxMax = cms.double(8.0),
            hitDropLimit4Hits = cms.double(0.6),
            hitDropLimit5Hits = cms.double(0.8),
            hitDropLimit6Hits = cms.double(0.3333),
            maxDPhi = cms.double(999.0),
            maxDTheta = cms.double(999.0),
            maxRatioResidualPrune = cms.double(3.0),
            maxRecHitsInCluster = cms.int32(20),
            minHitsPerSegment = cms.int32(3),
            onlyBestSegment = cms.bool(False),
            preClustering = cms.bool(True),
            preClusteringUseChaining = cms.bool(True),
            prePrun = cms.bool(True),
            prePrunLimit = cms.double(3.17),
            tanPhiMax = cms.double(0.5),
            tanThetaMax = cms.double(1.2),
            useShowering = cms.bool(False),
            yweightPenalty = cms.double(1.5),
            yweightPenaltyThreshold = cms.double(1.0)
        ), 
            cms.PSet(
                BPMinImprovement = cms.double(10000.0),
                BrutePruning = cms.bool(True),
                CSCDebug = cms.untracked.bool(False),
                CorrectTheErrors = cms.bool(True),
                Covariance = cms.double(0.0),
                ForceCovariance = cms.bool(False),
                ForceCovarianceAll = cms.bool(False),
                NormChi2Cut2D = cms.double(20.0),
                NormChi2Cut3D = cms.double(10.0),
                Pruning = cms.bool(True),
                SeedBig = cms.double(0.0015),
                SeedSmall = cms.double(0.0002),
                curvePenalty = cms.double(2.0),
                curvePenaltyThreshold = cms.double(0.85),
                dPhiFineMax = cms.double(0.025),
                dRPhiFineMax = cms.double(8.0),
                dXclusBoxMax = cms.double(4.0),
                dYclusBoxMax = cms.double(8.0),
                hitDropLimit4Hits = cms.double(0.6),
                hitDropLimit5Hits = cms.double(0.8),
                hitDropLimit6Hits = cms.double(0.3333),
                maxDPhi = cms.double(999.0),
                maxDTheta = cms.double(999.0),
                maxRatioResidualPrune = cms.double(3.0),
                maxRecHitsInCluster = cms.int32(24),
                minHitsPerSegment = cms.int32(3),
                onlyBestSegment = cms.bool(False),
                preClustering = cms.bool(True),
                preClusteringUseChaining = cms.bool(True),
                prePrun = cms.bool(True),
                prePrunLimit = cms.double(3.17),
                tanPhiMax = cms.double(0.5),
                tanThetaMax = cms.double(1.2),
                useShowering = cms.bool(False),
                yweightPenalty = cms.double(1.5),
                yweightPenaltyThreshold = cms.double(1.0)
            )),
        chamber_types = cms.vstring('ME1/a', 
            'ME1/b', 
            'ME1/2', 
            'ME1/3', 
            'ME2/1', 
            'ME2/2', 
            'ME3/1', 
            'ME3/2', 
            'ME4/1', 
            'ME4/2'),
        parameters_per_chamber_type = cms.vint32(2, 1, 1, 1, 1, 
            1, 1, 1, 1, 1)
    )),
    algo_type = cms.int32(1),
    inputObjects = cms.InputTag("hltCsc2DRecHits")
)


process.hltDt1DRecHits = cms.EDProducer("DTRecHitProducer",
    debug = cms.untracked.bool(False),
    dtDigiLabel = cms.InputTag("hltMuonDTDigis"),
    recAlgo = cms.string('DTLinearDriftFromDBAlgo'),
    recAlgoConfig = cms.PSet(
        debug = cms.untracked.bool(False),
        doVdriftCorr = cms.bool(True),
        maxTime = cms.double(420.0),
        minTime = cms.double(-3.0),
        stepTwoFromDigi = cms.bool(False),
        tTrigMode = cms.string('DTTTrigSyncFromDB'),
        tTrigModeConfig = cms.PSet(
            debug = cms.untracked.bool(False),
            doT0Correction = cms.bool(True),
            doTOFCorrection = cms.bool(True),
            doWirePropCorrection = cms.bool(True),
            tTrigLabel = cms.string(''),
            tofCorrType = cms.int32(0),
            vPropWire = cms.double(24.4),
            wirePropCorrType = cms.int32(0)
        ),
        useUncertDB = cms.bool(True)
    )
)


process.hltDt4DSegments = cms.EDProducer("DTRecSegment4DProducer",
    Reco4DAlgoConfig = cms.PSet(
        AllDTRecHits = cms.bool(True),
        Reco2DAlgoConfig = cms.PSet(
            AlphaMaxPhi = cms.double(1.0),
            AlphaMaxTheta = cms.double(0.9),
            MaxAllowedHits = cms.uint32(50),
            debug = cms.untracked.bool(False),
            hit_afterT0_resolution = cms.double(0.03),
            nSharedHitsMax = cms.int32(2),
            nUnSharedHitsMin = cms.int32(2),
            performT0SegCorrection = cms.bool(False),
            performT0_vdriftSegCorrection = cms.bool(False),
            perform_delta_rejecting = cms.bool(False),
            recAlgo = cms.string('DTLinearDriftFromDBAlgo'),
            recAlgoConfig = cms.PSet(
                debug = cms.untracked.bool(False),
                doVdriftCorr = cms.bool(True),
                maxTime = cms.double(420.0),
                minTime = cms.double(-3.0),
                stepTwoFromDigi = cms.bool(False),
                tTrigMode = cms.string('DTTTrigSyncFromDB'),
                tTrigModeConfig = cms.PSet(
                    debug = cms.untracked.bool(False),
                    doT0Correction = cms.bool(True),
                    doTOFCorrection = cms.bool(True),
                    doWirePropCorrection = cms.bool(True),
                    tTrigLabel = cms.string(''),
                    tofCorrType = cms.int32(0),
                    vPropWire = cms.double(24.4),
                    wirePropCorrType = cms.int32(0)
                ),
                useUncertDB = cms.bool(True)
            ),
            segmCleanerMode = cms.int32(2)
        ),
        Reco2DAlgoName = cms.string('DTCombinatorialPatternReco'),
        debug = cms.untracked.bool(False),
        hit_afterT0_resolution = cms.double(0.03),
        nSharedHitsMax = cms.int32(2),
        nUnSharedHitsMin = cms.int32(2),
        performT0SegCorrection = cms.bool(False),
        performT0_vdriftSegCorrection = cms.bool(False),
        perform_delta_rejecting = cms.bool(False),
        recAlgo = cms.string('DTLinearDriftFromDBAlgo'),
        recAlgoConfig = cms.PSet(
            debug = cms.untracked.bool(False),
            doVdriftCorr = cms.bool(True),
            maxTime = cms.double(420.0),
            minTime = cms.double(-3.0),
            stepTwoFromDigi = cms.bool(False),
            tTrigMode = cms.string('DTTTrigSyncFromDB'),
            tTrigModeConfig = cms.PSet(
                debug = cms.untracked.bool(False),
                doT0Correction = cms.bool(True),
                doTOFCorrection = cms.bool(True),
                doWirePropCorrection = cms.bool(True),
                tTrigLabel = cms.string(''),
                tofCorrType = cms.int32(0),
                vPropWire = cms.double(24.4),
                wirePropCorrType = cms.int32(0)
            ),
            useUncertDB = cms.bool(True)
        ),
        segmCleanerMode = cms.int32(2)
    ),
    Reco4DAlgoName = cms.string('DTCombinatorialPatternReco4D'),
    debug = cms.untracked.bool(False),
    recHits1DLabel = cms.InputTag("hltDt1DRecHits"),
    recHits2DLabel = cms.InputTag("dt2DSegments")
)


process.hltEcalDetIdToBeRecovered = cms.EDProducer("EcalDetIdToBeRecoveredProducer",
    ebDetIdToBeRecovered = cms.string('ebDetId'),
    ebFEToBeRecovered = cms.string('ebFE'),
    ebIntegrityChIdErrors = cms.InputTag("hltEcalDigis","EcalIntegrityChIdErrors"),
    ebIntegrityGainErrors = cms.InputTag("hltEcalDigis","EcalIntegrityGainErrors"),
    ebIntegrityGainSwitchErrors = cms.InputTag("hltEcalDigis","EcalIntegrityGainSwitchErrors"),
    ebSrFlagCollection = cms.InputTag("hltEcalDigis"),
    eeDetIdToBeRecovered = cms.string('eeDetId'),
    eeFEToBeRecovered = cms.string('eeFE'),
    eeIntegrityChIdErrors = cms.InputTag("hltEcalDigis","EcalIntegrityChIdErrors"),
    eeIntegrityGainErrors = cms.InputTag("hltEcalDigis","EcalIntegrityGainErrors"),
    eeIntegrityGainSwitchErrors = cms.InputTag("hltEcalDigis","EcalIntegrityGainSwitchErrors"),
    eeSrFlagCollection = cms.InputTag("hltEcalDigis"),
    integrityBlockSizeErrors = cms.InputTag("hltEcalDigis","EcalIntegrityBlockSizeErrors"),
    integrityTTIdErrors = cms.InputTag("hltEcalDigis","EcalIntegrityTTIdErrors")
)


process.hltEcalDigis = cms.EDProducer("EcalRawToDigi",
    DoRegional = cms.bool(False),
    FEDs = cms.vint32(601, 602, 603, 604, 605, 
        606, 607, 608, 609, 610, 
        611, 612, 613, 614, 615, 
        616, 617, 618, 619, 620, 
        621, 622, 623, 624, 625, 
        626, 627, 628, 629, 630, 
        631, 632, 633, 634, 635, 
        636, 637, 638, 639, 640, 
        641, 642, 643, 644, 645, 
        646, 647, 648, 649, 650, 
        651, 652, 653, 654),
    FedLabel = cms.InputTag("listfeds"),
    InputLabel = cms.InputTag("rawDataCollector"),
    eventPut = cms.bool(True),
    feIdCheck = cms.bool(True),
    feUnpacking = cms.bool(True),
    forceToKeepFRData = cms.bool(False),
    headerUnpacking = cms.bool(True),
    memUnpacking = cms.bool(True),
    numbTriggerTSamples = cms.int32(1),
    numbXtalTSamples = cms.int32(10),
    orderedDCCIdList = cms.vint32(1, 2, 3, 4, 5, 
        6, 7, 8, 9, 10, 
        11, 12, 13, 14, 15, 
        16, 17, 18, 19, 20, 
        21, 22, 23, 24, 25, 
        26, 27, 28, 29, 30, 
        31, 32, 33, 34, 35, 
        36, 37, 38, 39, 40, 
        41, 42, 43, 44, 45, 
        46, 47, 48, 49, 50, 
        51, 52, 53, 54),
    orderedFedList = cms.vint32(601, 602, 603, 604, 605, 
        606, 607, 608, 609, 610, 
        611, 612, 613, 614, 615, 
        616, 617, 618, 619, 620, 
        621, 622, 623, 624, 625, 
        626, 627, 628, 629, 630, 
        631, 632, 633, 634, 635, 
        636, 637, 638, 639, 640, 
        641, 642, 643, 644, 645, 
        646, 647, 648, 649, 650, 
        651, 652, 653, 654),
    silentMode = cms.untracked.bool(True),
    srpUnpacking = cms.bool(True),
    syncCheck = cms.bool(True),
    tccUnpacking = cms.bool(True)
)


process.hltEcalPreshowerDigis = cms.EDProducer("ESRawToDigi",
    ESdigiCollection = cms.string(''),
    InstanceES = cms.string(''),
    LookupTable = cms.FileInPath('EventFilter/ESDigiToRaw/data/ES_lookup_table.dat'),
    debugMode = cms.untracked.bool(False),
    sourceTag = cms.InputTag("rawDataCollector")
)


process.hltEcalPreshowerRecHit = cms.EDProducer("ESRecHitProducer",
    ESRecoAlgo = cms.int32(0),
    ESdigiCollection = cms.InputTag("hltEcalPreshowerDigis"),
    ESrechitCollection = cms.string('EcalRecHitsES'),
    algo = cms.string('ESRecHitWorker')
)


process.hltEcalRecHit = cms.EDProducer("EcalRecHitProducer",
    ChannelStatusToBeExcluded = cms.vstring(),
    EBLaserMAX = cms.double(3.0),
    EBLaserMIN = cms.double(0.5),
    EBrechitCollection = cms.string('EcalRecHitsEB'),
    EBuncalibRecHitCollection = cms.InputTag("hltEcalUncalibRecHit","EcalUncalibRecHitsEB"),
    EELaserMAX = cms.double(8.0),
    EELaserMIN = cms.double(0.5),
    EErechitCollection = cms.string('EcalRecHitsEE'),
    EEuncalibRecHitCollection = cms.InputTag("hltEcalUncalibRecHit","EcalUncalibRecHitsEE"),
    algo = cms.string('EcalRecHitWorkerSimple'),
    algoRecover = cms.string('EcalRecHitWorkerRecover'),
    cleaningConfig = cms.PSet(
        cThreshold_barrel = cms.double(4.0),
        cThreshold_double = cms.double(10.0),
        cThreshold_endcap = cms.double(15.0),
        e4e1Threshold_barrel = cms.double(0.08),
        e4e1Threshold_endcap = cms.double(0.3),
        e4e1_a_barrel = cms.double(0.04),
        e4e1_a_endcap = cms.double(0.02),
        e4e1_b_barrel = cms.double(-0.024),
        e4e1_b_endcap = cms.double(-0.0125),
        e6e2thresh = cms.double(0.04),
        ignoreOutOfTimeThresh = cms.double(1000000000.0),
        tightenCrack_e1_double = cms.double(2.0),
        tightenCrack_e1_single = cms.double(2.0),
        tightenCrack_e4e1_single = cms.double(3.0),
        tightenCrack_e6e2_double = cms.double(3.0)
    ),
    dbStatusToBeExcludedEB = cms.vint32(14, 78, 142),
    dbStatusToBeExcludedEE = cms.vint32(14, 78, 142),
    ebDetIdToBeRecovered = cms.InputTag("hltEcalDetIdToBeRecovered","ebDetId"),
    ebFEToBeRecovered = cms.InputTag("hltEcalDetIdToBeRecovered","ebFE"),
    eeDetIdToBeRecovered = cms.InputTag("hltEcalDetIdToBeRecovered","eeDetId"),
    eeFEToBeRecovered = cms.InputTag("hltEcalDetIdToBeRecovered","eeFE"),
    flagsMapDBReco = cms.PSet(
        kDead = cms.vstring('kNoDataNoTP'),
        kGood = cms.vstring('kOk', 
            'kDAC', 
            'kNoLaser', 
            'kNoisy'),
        kNeighboursRecovered = cms.vstring('kFixedG0', 
            'kNonRespondingIsolated', 
            'kDeadVFE'),
        kNoisy = cms.vstring('kNNoisy', 
            'kFixedG6', 
            'kFixedG1'),
        kTowerRecovered = cms.vstring('kDeadFE')
    ),
    killDeadChannels = cms.bool(True),
    laserCorrection = cms.bool(True),
    logWarningEtThreshold_EB_FE = cms.double(50.0),
    logWarningEtThreshold_EE_FE = cms.double(50.0),
    recoverEBFE = cms.bool(True),
    recoverEBIsolatedChannels = cms.bool(False),
    recoverEBVFE = cms.bool(False),
    recoverEEFE = cms.bool(True),
    recoverEEIsolatedChannels = cms.bool(False),
    recoverEEVFE = cms.bool(False),
    singleChannelRecoveryMethod = cms.string('NeuralNetworks'),
    singleChannelRecoveryThreshold = cms.double(8.0),
    skipTimeCalib = cms.bool(True),
    triggerPrimitiveDigiCollection = cms.InputTag("hltEcalDigis","EcalTriggerPrimitives")
)


process.hltEcalUncalibRecHit = cms.EDProducer("EcalUncalibRecHitProducer",
    EBdigiCollection = cms.InputTag("hltEcalDigis","ebDigis"),
    EBhitCollection = cms.string('EcalUncalibRecHitsEB'),
    EEdigiCollection = cms.InputTag("hltEcalDigis","eeDigis"),
    EEhitCollection = cms.string('EcalUncalibRecHitsEE'),
    algo = cms.string('EcalUncalibRecHitWorkerMultiFit'),
    algoPSet = cms.PSet(
        EBamplitudeFitParameters = cms.vdouble(1.138, 1.652),
        EBtimeConstantTerm = cms.double(0.6),
        EBtimeFitLimits_Lower = cms.double(0.2),
        EBtimeFitLimits_Upper = cms.double(1.4),
        EBtimeFitParameters = cms.vdouble(-2.015452, 3.130702, -12.3473, 41.88921, -82.83944, 
            91.01147, -50.35761, 11.05621),
        EBtimeNconst = cms.double(28.5),
        EEamplitudeFitParameters = cms.vdouble(1.89, 1.4),
        EEtimeConstantTerm = cms.double(1.0),
        EEtimeFitLimits_Lower = cms.double(0.2),
        EEtimeFitLimits_Upper = cms.double(1.4),
        EEtimeFitParameters = cms.vdouble(-2.390548, 3.553628, -17.62341, 67.67538, -133.213, 
            140.7432, -75.41106, 16.20277),
        EEtimeNconst = cms.double(31.8),
        activeBXs = cms.vint32(-5, -4, -3, -2, -1, 
            0, 1, 2),
        ampErrorCalculation = cms.bool(False),
        amplitudeThresholdEB = cms.double(10.0),
        amplitudeThresholdEE = cms.double(10.0),
        chi2ThreshEB_ = cms.double(65.0),
        chi2ThreshEE_ = cms.double(50.0),
        doPrefitEB = cms.bool(True),
        doPrefitEE = cms.bool(True),
        ebSpikeThreshold = cms.double(1.042),
        kPoorRecoFlagEB = cms.bool(True),
        kPoorRecoFlagEE = cms.bool(False),
        outOfTimeThresholdGain12mEB = cms.double(5.0),
        outOfTimeThresholdGain12mEE = cms.double(1000.0),
        outOfTimeThresholdGain12pEB = cms.double(5.0),
        outOfTimeThresholdGain12pEE = cms.double(1000.0),
        outOfTimeThresholdGain61mEB = cms.double(5.0),
        outOfTimeThresholdGain61mEE = cms.double(1000.0),
        outOfTimeThresholdGain61pEB = cms.double(5.0),
        outOfTimeThresholdGain61pEE = cms.double(1000.0),
        prefitMaxChiSqEB = cms.double(15.0),
        prefitMaxChiSqEE = cms.double(10.0),
        timealgo = cms.string('None'),
        useLumiInfoRunHeader = cms.bool(False)
    )
)


process.hltFEDSelector = cms.EDProducer("EvFFEDSelector",
    fedList = cms.vuint32(1023, 1024),
    inputTag = cms.InputTag("rawDataCollector")
)


process.hltFixedGridRhoFastjetAll = cms.EDProducer("FixedGridRhoProducerFastjet",
    gridSpacing = cms.double(0.55),
    maxRapidity = cms.double(5.0),
    pfCandidatesTag = cms.InputTag("hltParticleFlow")
)


process.hltFixedGridRhoFastjetAllCalo = cms.EDProducer("FixedGridRhoProducerFastjet",
    gridSpacing = cms.double(0.55),
    maxRapidity = cms.double(5.0),
    pfCandidatesTag = cms.InputTag("hltTowerMakerForAll")
)


process.hltGmtStage2Digis = cms.EDProducer("L1TRawToDigi",
    CTP7 = cms.untracked.bool(False),
    FWId = cms.uint32(0),
    FWOverride = cms.bool(False),
    FedIds = cms.vint32(1402),
    InputLabel = cms.InputTag("rawDataCollector"),
    MTF7 = cms.untracked.bool(False),
    MinFeds = cms.uint32(0),
    Setup = cms.string('stage2::GMTSetup'),
    debug = cms.untracked.bool(False),
    lenAMC13Header = cms.untracked.int32(8),
    lenAMC13Trailer = cms.untracked.int32(8),
    lenAMCHeader = cms.untracked.int32(8),
    lenAMCTrailer = cms.untracked.int32(0),
    lenSlinkHeader = cms.untracked.int32(8),
    lenSlinkTrailer = cms.untracked.int32(8)
)


process.hltGtStage2Digis = cms.EDProducer("L1TRawToDigi",
    CTP7 = cms.untracked.bool(False),
    FWId = cms.uint32(0),
    FWOverride = cms.bool(False),
    FedIds = cms.vint32(1404),
    InputLabel = cms.InputTag("rawDataCollector"),
    MTF7 = cms.untracked.bool(False),
    MinFeds = cms.uint32(0),
    Setup = cms.string('stage2::GTSetup'),
    debug = cms.untracked.bool(False),
    lenAMC13Header = cms.untracked.int32(8),
    lenAMC13Trailer = cms.untracked.int32(8),
    lenAMCHeader = cms.untracked.int32(8),
    lenAMCTrailer = cms.untracked.int32(0),
    lenSlinkHeader = cms.untracked.int32(8),
    lenSlinkTrailer = cms.untracked.int32(8)
)


process.hltGtStage2ObjectMap = cms.EDProducer("L1TGlobalProducer",
    AlgorithmTriggersUnmasked = cms.bool(True),
    AlgorithmTriggersUnprescaled = cms.bool(True),
    AlternativeNrBxBoardDaq = cms.uint32(0),
    BstLengthBytes = cms.int32(-1),
    EGammaInputTag = cms.InputTag("hltCaloStage2Digis","EGamma"),
    EmulateBxInEvent = cms.int32(1),
    EtSumInputTag = cms.InputTag("hltCaloStage2Digis","EtSum"),
    ExtInputTag = cms.InputTag("hltGtStage2Digis"),
    JetInputTag = cms.InputTag("hltCaloStage2Digis","Jet"),
    L1DataBxInEvent = cms.int32(5),
    MuonInputTag = cms.InputTag("hltGmtStage2Digis","Muon"),
    PrescaleCSVFile = cms.string('prescale_L1TGlobal.csv'),
    PrescaleSet = cms.uint32(1),
    PrintL1Menu = cms.untracked.bool(False),
    ProduceL1GtDaqRecord = cms.bool(True),
    ProduceL1GtObjectMapRecord = cms.bool(True),
    TauInputTag = cms.InputTag("hltCaloStage2Digis","Tau"),
    TriggerMenuLuminosity = cms.string('startup'),
    Verbosity = cms.untracked.int32(0)
)


process.hltHbhereco = cms.EDProducer("HcalHitReconstructor",
    HFInWindowStat = cms.PSet(

    ),
    PETstat = cms.PSet(

    ),
    S8S1stat = cms.PSet(

    ),
    S9S1stat = cms.PSet(

    ),
    Subdetector = cms.string('HBHE'),
    applyPedConstraint = cms.bool(True),
    applyPulseJitter = cms.bool(False),
    applyTimeConstraint = cms.bool(True),
    applyTimeSlew = cms.bool(True),
    applyUnconstrainedFit = cms.bool(False),
    chargeMax = cms.double(6.0),
    correctForPhaseContainment = cms.bool(True),
    correctForTimeslew = cms.bool(True),
    correctTiming = cms.bool(False),
    correctionPhaseNS = cms.double(13.0),
    dataOOTCorrectionCategory = cms.string('Data'),
    dataOOTCorrectionName = cms.string(''),
    digiLabel = cms.InputTag("hltHcalDigis"),
    digiTimeFromDB = cms.bool(True),
    digistat = cms.PSet(

    ),
    dropZSmarkedPassed = cms.bool(True),
    firstAuxTS = cms.int32(4),
    firstSample = cms.int32(4),
    fitTimes = cms.int32(1),
    flagParameters = cms.PSet(
        hitEnergyMinimum = cms.double(1.0),
        hitMultiplicityThreshold = cms.int32(17),
        nominalPedestal = cms.double(3.0),
        pulseShapeParameterSets = cms.VPSet(cms.PSet(
            pulseShapeParameters = cms.vdouble(0.0, 100.0, -50.0, 0.0, -15.0, 
                0.15)
        ), 
            cms.PSet(
                pulseShapeParameters = cms.vdouble(100.0, 2000.0, -50.0, 0.0, -5.0, 
                    0.05)
            ), 
            cms.PSet(
                pulseShapeParameters = cms.vdouble(2000.0, 1000000.0, -50.0, 0.0, 95.0, 
                    0.0)
            ), 
            cms.PSet(
                pulseShapeParameters = cms.vdouble(-1000000.0, 1000000.0, 45.0, 0.1, 1000000.0, 
                    0.0)
            ))
    ),
    hfTimingTrustParameters = cms.PSet(

    ),
    hscpParameters = cms.PSet(
        TimingEnergyThreshold = cms.double(30.0),
        fracLeaderMax = cms.double(0.7),
        fracLeaderMin = cms.double(0.4),
        outerMax = cms.double(0.1),
        outerMin = cms.double(0.0),
        r1Max = cms.double(1.0),
        r1Min = cms.double(0.15),
        r2Max = cms.double(0.5),
        r2Min = cms.double(0.1),
        slopeMax = cms.double(-0.6),
        slopeMin = cms.double(-1.5)
    ),
    mcOOTCorrectionCategory = cms.string('MC'),
    mcOOTCorrectionName = cms.string(''),
    meanPed = cms.double(0.0),
    meanTime = cms.double(-2.5),
    noise = cms.double(1.0),
    pedSigma = cms.double(0.5),
    pedestalSubtractionType = cms.int32(1),
    pedestalUpperLimit = cms.double(2.7),
    puCorrMethod = cms.int32(3),
    pulseJitter = cms.double(1.0),
    pulseShapeParameters = cms.PSet(
        LeftSlopeCut = cms.vdouble(),
        LeftSlopeThreshold = cms.vdouble(),
        LinearCut = cms.vdouble(),
        LinearThreshold = cms.vdouble(),
        MinimumChargeThreshold = cms.double(20.0),
        MinimumTS4TS5Threshold = cms.double(100.0),
        R45MinusOneRange = cms.double(0.2),
        R45PlusOneRange = cms.double(0.2),
        RMS8MaxCut = cms.vdouble(),
        RMS8MaxThreshold = cms.vdouble(),
        RightSlopeCut = cms.vdouble(),
        RightSlopeSmallCut = cms.vdouble(),
        RightSlopeSmallThreshold = cms.vdouble(),
        RightSlopeThreshold = cms.vdouble(),
        TS3TS4ChargeThreshold = cms.double(70.0),
        TS3TS4UpperChargeThreshold = cms.double(20.0),
        TS4TS5ChargeThreshold = cms.double(70.0),
        TS4TS5LowerCut = cms.vdouble(-1.0, -0.7, -0.5, -0.4, -0.3, 
            0.1),
        TS4TS5LowerThreshold = cms.vdouble(100.0, 120.0, 160.0, 200.0, 300.0, 
            500.0),
        TS4TS5UpperCut = cms.vdouble(1.0, 0.8, 0.75, 0.72),
        TS4TS5UpperThreshold = cms.vdouble(70.0, 90.0, 100.0, 400.0),
        TS5TS6ChargeThreshold = cms.double(70.0),
        TS5TS6UpperChargeThreshold = cms.double(20.0),
        TriangleIgnoreSlow = cms.bool(False),
        TrianglePeakTS = cms.uint32(0),
        UseDualFit = cms.bool(False)
    ),
    recoParamsFromDB = cms.bool(True),
    respCorrM3 = cms.double(1.0),
    samplesToAdd = cms.int32(4),
    saturationParameters = cms.PSet(
        maxADCvalue = cms.int32(127)
    ),
    setHSCPFlags = cms.bool(False),
    setNegativeFlags = cms.bool(False),
    setNoiseFlags = cms.bool(False),
    setPulseShapeFlags = cms.bool(True),
    setSaturationFlags = cms.bool(False),
    setTimingShapedCutsFlags = cms.bool(False),
    setTimingTrustFlags = cms.bool(False),
    timeMax = cms.double(10.0),
    timeMin = cms.double(-15.0),
    timeSigma = cms.double(5.0),
    timeSlewPars = cms.vdouble(12.2999, -2.19142, 0, 12.2999, -2.19142, 
        0, 12.2999, -2.19142, 0),
    timeSlewParsType = cms.int32(3),
    timingshapedcutsParameters = cms.PSet(
        ignorehighest = cms.bool(False),
        ignorelowest = cms.bool(True),
        tfilterEnvelope = cms.vdouble(4.0, 12.04, 13.0, 10.56, 23.5, 
            8.82, 37.0, 7.38, 56.0, 6.3, 
            81.0, 5.64, 114.5, 5.44, 175.5, 
            5.38, 350.5, 5.14),
        win_gain = cms.double(1.0),
        win_offset = cms.double(0.0)
    ),
    ts345chi2 = cms.double(100.0),
    ts3chi2 = cms.double(5.0),
    ts4Max = cms.double(500.0),
    ts4Min = cms.double(5.0),
    ts4chi2 = cms.double(15.0),
    tsFromDB = cms.bool(True),
    useLeakCorrection = cms.bool(False)
)


process.hltHcalDigis = cms.EDProducer("HcalRawToDigi",
    ComplainEmptyData = cms.untracked.bool(False),
    ElectronicsMap = cms.string(''),
    ExpectedOrbitMessageTime = cms.untracked.int32(-1),
    FEDs = cms.untracked.vint32(),
    FilterDataQuality = cms.bool(True),
    HcalFirstFED = cms.untracked.int32(700),
    InputLabel = cms.InputTag("rawDataCollector"),
    UnpackCalib = cms.untracked.bool(True),
    UnpackTTP = cms.untracked.bool(False),
    UnpackUMNio = cms.untracked.bool(True),
    UnpackZDC = cms.untracked.bool(True),
    UnpackerMode = cms.untracked.int32(0),
    firstSample = cms.int32(0),
    lastSample = cms.int32(9),
    silent = cms.untracked.bool(True)
)


process.hltHfreco = cms.EDProducer("HcalHitReconstructor",
    HFInWindowStat = cms.PSet(
        hflongEthresh = cms.double(40.0),
        hflongMaxWindowTime = cms.vdouble(10.0),
        hflongMinWindowTime = cms.vdouble(-10.0),
        hfshortEthresh = cms.double(40.0),
        hfshortMaxWindowTime = cms.vdouble(10.0),
        hfshortMinWindowTime = cms.vdouble(-12.0)
    ),
    PETstat = cms.PSet(
        HcalAcceptSeverityLevel = cms.int32(9),
        flagsToSkip = cms.int32(0),
        longETParams = cms.vdouble(0.0, 0.0, 0.0, 0.0, 0.0, 
            0.0, 0.0, 0.0, 0.0, 0.0, 
            0.0, 0.0, 0.0),
        longEnergyParams = cms.vdouble(43.5, 45.7, 48.32, 51.36, 54.82, 
            58.7, 63.0, 67.72, 72.86, 78.42, 
            84.4, 90.8, 97.62),
        long_R = cms.vdouble(0.98),
        long_R_29 = cms.vdouble(0.8),
        shortETParams = cms.vdouble(0.0, 0.0, 0.0, 0.0, 0.0, 
            0.0, 0.0, 0.0, 0.0, 0.0, 
            0.0, 0.0, 0.0),
        shortEnergyParams = cms.vdouble(35.1773, 35.37, 35.7933, 36.4472, 37.3317, 
            38.4468, 39.7925, 41.3688, 43.1757, 45.2132, 
            47.4813, 49.98, 52.7093),
        short_R = cms.vdouble(0.8),
        short_R_29 = cms.vdouble(0.8)
    ),
    S8S1stat = cms.PSet(
        HcalAcceptSeverityLevel = cms.int32(9),
        flagsToSkip = cms.int32(16),
        isS8S1 = cms.bool(True),
        longETParams = cms.vdouble(0.0, 0.0, 0.0, 0.0, 0.0, 
            0.0, 0.0, 0.0, 0.0, 0.0, 
            0.0, 0.0, 0.0),
        longEnergyParams = cms.vdouble(40.0, 100.0, 100.0, 100.0, 100.0, 
            100.0, 100.0, 100.0, 100.0, 100.0, 
            100.0, 100.0, 100.0),
        long_optimumSlope = cms.vdouble(0.3, 0.1, 0.1, 0.1, 0.1, 
            0.1, 0.1, 0.1, 0.1, 0.1, 
            0.1, 0.1, 0.1),
        shortETParams = cms.vdouble(0.0, 0.0, 0.0, 0.0, 0.0, 
            0.0, 0.0, 0.0, 0.0, 0.0, 
            0.0, 0.0, 0.0),
        shortEnergyParams = cms.vdouble(40.0, 100.0, 100.0, 100.0, 100.0, 
            100.0, 100.0, 100.0, 100.0, 100.0, 
            100.0, 100.0, 100.0),
        short_optimumSlope = cms.vdouble(0.3, 0.1, 0.1, 0.1, 0.1, 
            0.1, 0.1, 0.1, 0.1, 0.1, 
            0.1, 0.1, 0.1)
    ),
    S9S1stat = cms.PSet(
        HcalAcceptSeverityLevel = cms.int32(9),
        flagsToSkip = cms.int32(24),
        isS8S1 = cms.bool(False),
        longETParams = cms.vdouble(0.0, 0.0, 0.0, 0.0, 0.0, 
            0.0, 0.0, 0.0, 0.0, 0.0, 
            0.0, 0.0, 0.0),
        longEnergyParams = cms.vdouble(43.5, 45.7, 48.32, 51.36, 54.82, 
            58.7, 63.0, 67.72, 72.86, 78.42, 
            84.4, 90.8, 97.62),
        long_optimumSlope = cms.vdouble(-99999.0, 0.0164905, 0.0238698, 0.0321383, 0.041296, 
            0.0513428, 0.0622789, 0.0741041, 0.0868186, 0.100422, 
            0.135313, 0.136289, 0.0589927),
        shortETParams = cms.vdouble(0.0, 0.0, 0.0, 0.0, 0.0, 
            0.0, 0.0, 0.0, 0.0, 0.0, 
            0.0, 0.0, 0.0),
        shortEnergyParams = cms.vdouble(35.1773, 35.37, 35.7933, 36.4472, 37.3317, 
            38.4468, 39.7925, 41.3688, 43.1757, 45.2132, 
            47.4813, 49.98, 52.7093),
        short_optimumSlope = cms.vdouble(-99999.0, 0.0164905, 0.0238698, 0.0321383, 0.041296, 
            0.0513428, 0.0622789, 0.0741041, 0.0868186, 0.100422, 
            0.135313, 0.136289, 0.0589927)
    ),
    Subdetector = cms.string('HF'),
    applyPedConstraint = cms.bool(True),
    applyPulseJitter = cms.bool(False),
    applyTimeConstraint = cms.bool(True),
    applyTimeSlew = cms.bool(True),
    applyUnconstrainedFit = cms.bool(False),
    chargeMax = cms.double(6.0),
    correctForPhaseContainment = cms.bool(False),
    correctForTimeslew = cms.bool(False),
    correctTiming = cms.bool(False),
    correctionPhaseNS = cms.double(13.0),
    dataOOTCorrectionCategory = cms.string('Data'),
    dataOOTCorrectionName = cms.string(''),
    digiLabel = cms.InputTag("hltHcalDigis"),
    digiTimeFromDB = cms.bool(True),
    digistat = cms.PSet(
        HFdigiflagCoef = cms.vdouble(0.93, -0.012667, -0.38275),
        HFdigiflagExpectedPeak = cms.int32(2),
        HFdigiflagFirstSample = cms.int32(1),
        HFdigiflagMinEthreshold = cms.double(40.0),
        HFdigiflagSamplesToAdd = cms.int32(3)
    ),
    dropZSmarkedPassed = cms.bool(True),
    firstAuxTS = cms.int32(1),
    firstSample = cms.int32(2),
    fitTimes = cms.int32(1),
    flagParameters = cms.PSet(

    ),
    hfTimingTrustParameters = cms.PSet(
        hfTimingTrustLevel1 = cms.int32(1),
        hfTimingTrustLevel2 = cms.int32(4)
    ),
    hscpParameters = cms.PSet(

    ),
    mcOOTCorrectionCategory = cms.string('MC'),
    mcOOTCorrectionName = cms.string(''),
    meanPed = cms.double(0.0),
    meanTime = cms.double(-2.5),
    noise = cms.double(1.0),
    pedSigma = cms.double(0.5),
    pedestalSubtractionType = cms.int32(1),
    pedestalUpperLimit = cms.double(2.7),
    puCorrMethod = cms.int32(0),
    pulseJitter = cms.double(1.0),
    pulseShapeParameters = cms.PSet(

    ),
    recoParamsFromDB = cms.bool(True),
    respCorrM3 = cms.double(0.95),
    samplesToAdd = cms.int32(2),
    saturationParameters = cms.PSet(
        maxADCvalue = cms.int32(127)
    ),
    setHSCPFlags = cms.bool(False),
    setNegativeFlags = cms.bool(False),
    setNoiseFlags = cms.bool(True),
    setPulseShapeFlags = cms.bool(False),
    setSaturationFlags = cms.bool(False),
    setTimingShapedCutsFlags = cms.bool(False),
    setTimingTrustFlags = cms.bool(False),
    timeMax = cms.double(10.0),
    timeMin = cms.double(-15.0),
    timeSigma = cms.double(5.0),
    timeSlewPars = cms.vdouble(12.2999, -2.19142, 0.0, 12.2999, -2.19142, 
        0.0, 12.2999, -2.19142, 0.0),
    timeSlewParsType = cms.int32(3),
    timingshapedcutsParameters = cms.PSet(

    ),
    ts345chi2 = cms.double(100.0),
    ts3chi2 = cms.double(5.0),
    ts4Max = cms.double(500.0),
    ts4Min = cms.double(5.0),
    ts4chi2 = cms.double(15.0),
    tsFromDB = cms.bool(True),
    useLeakCorrection = cms.bool(False)
)


process.hltHoreco = cms.EDProducer("HcalHitReconstructor",
    HFInWindowStat = cms.PSet(

    ),
    PETstat = cms.PSet(

    ),
    S8S1stat = cms.PSet(

    ),
    S9S1stat = cms.PSet(

    ),
    Subdetector = cms.string('HO'),
    applyPedConstraint = cms.bool(True),
    applyPulseJitter = cms.bool(False),
    applyTimeConstraint = cms.bool(True),
    applyTimeSlew = cms.bool(True),
    applyUnconstrainedFit = cms.bool(False),
    chargeMax = cms.double(6.0),
    correctForPhaseContainment = cms.bool(True),
    correctForTimeslew = cms.bool(True),
    correctTiming = cms.bool(False),
    correctionPhaseNS = cms.double(13.0),
    dataOOTCorrectionCategory = cms.string('Data'),
    dataOOTCorrectionName = cms.string(''),
    digiLabel = cms.InputTag("hltHcalDigis"),
    digiTimeFromDB = cms.bool(True),
    digistat = cms.PSet(

    ),
    dropZSmarkedPassed = cms.bool(True),
    firstAuxTS = cms.int32(4),
    firstSample = cms.int32(4),
    fitTimes = cms.int32(1),
    flagParameters = cms.PSet(

    ),
    hfTimingTrustParameters = cms.PSet(

    ),
    hscpParameters = cms.PSet(

    ),
    mcOOTCorrectionCategory = cms.string('MC'),
    mcOOTCorrectionName = cms.string(''),
    meanPed = cms.double(0.0),
    meanTime = cms.double(-2.5),
    noise = cms.double(1.0),
    pedSigma = cms.double(0.5),
    pedestalSubtractionType = cms.int32(1),
    pedestalUpperLimit = cms.double(2.7),
    puCorrMethod = cms.int32(0),
    pulseJitter = cms.double(1.0),
    pulseShapeParameters = cms.PSet(

    ),
    recoParamsFromDB = cms.bool(True),
    respCorrM3 = cms.double(0.95),
    samplesToAdd = cms.int32(4),
    saturationParameters = cms.PSet(
        maxADCvalue = cms.int32(127)
    ),
    setHSCPFlags = cms.bool(False),
    setNegativeFlags = cms.bool(False),
    setNoiseFlags = cms.bool(False),
    setPulseShapeFlags = cms.bool(False),
    setSaturationFlags = cms.bool(False),
    setTimingShapedCutsFlags = cms.bool(False),
    setTimingTrustFlags = cms.bool(False),
    timeMax = cms.double(10.0),
    timeMin = cms.double(-15.0),
    timeSigma = cms.double(5.0),
    timeSlewPars = cms.vdouble(12.2999, -2.19142, 0.0, 12.2999, -2.19142, 
        0.0, 12.2999, -2.19142, 0.0),
    timeSlewParsType = cms.int32(3),
    timingshapedcutsParameters = cms.PSet(

    ),
    ts345chi2 = cms.double(100.0),
    ts3chi2 = cms.double(5.0),
    ts4Max = cms.double(500.0),
    ts4Min = cms.double(5.0),
    ts4chi2 = cms.double(15.0),
    tsFromDB = cms.bool(True),
    useLeakCorrection = cms.bool(False)
)


process.hltIter0PFLowPixelSeedsFromPixelTracks = cms.EDProducer("SeedGeneratorFromProtoTracksEDProducer",
    InputCollection = cms.InputTag("hltPixelTracks"),
    InputVertexCollection = cms.InputTag("hltTrimmedPixelVertices"),
    SeedCreatorPSet = cms.PSet(
        refToPSet_ = cms.string('HLTSeedFromProtoTracks')
    ),
    TTRHBuilder = cms.string('hltESPTTRHBuilderPixelOnly'),
    originHalfLength = cms.double(0.3),
    originRadius = cms.double(0.1),
    useEventsWithNoVertex = cms.bool(True),
    usePV = cms.bool(False),
    useProtoTrackKinematics = cms.bool(False)
)


process.hltIter0PFlowCkfTrackCandidates = cms.EDProducer("CkfTrackCandidateMaker",
    MeasurementTrackerEvent = cms.InputTag("hltSiStripClusters"),
    NavigationSchool = cms.string('SimpleNavigationSchool'),
    RedundantSeedCleaner = cms.string('CachingSeedCleanerBySharedInput'),
    SimpleMagneticField = cms.string('ParabolicMf'),
    TrajectoryBuilder = cms.string(''),
    TrajectoryBuilderPSet = cms.PSet(
        refToPSet_ = cms.string('HLTIter0PSetTrajectoryBuilderIT')
    ),
    TrajectoryCleaner = cms.string('hltESPTrajectoryCleanerBySharedHits'),
    TransientInitialStateEstimatorParameters = cms.PSet(
        numberMeasurementsForFit = cms.int32(4),
        propagatorAlongTISE = cms.string('PropagatorWithMaterialParabolicMf'),
        propagatorOppositeTISE = cms.string('PropagatorWithMaterialParabolicMfOpposite')
    ),
    cleanTrajectoryAfterInOut = cms.bool(False),
    doSeedingRegionRebuilding = cms.bool(False),
    maxNSeeds = cms.uint32(100000),
    maxSeedsBeforeCleaning = cms.uint32(1000),
    src = cms.InputTag("hltIter0PFLowPixelSeedsFromPixelTracks"),
    useHitsSplitting = cms.bool(False)
)


process.hltIter0PFlowCtfWithMaterialTracks = cms.EDProducer("TrackProducer",
    AlgorithmName = cms.string('hltIter0'),
    Fitter = cms.string('hltESPFittingSmootherIT'),
    GeometricInnerState = cms.bool(True),
    MeasurementTracker = cms.string(''),
    MeasurementTrackerEvent = cms.InputTag("hltSiStripClusters"),
    NavigationSchool = cms.string(''),
    Propagator = cms.string('hltESPRungeKuttaTrackerPropagator'),
    SimpleMagneticField = cms.string('ParabolicMf'),
    TTRHBuilder = cms.string('hltESPTTRHBWithTrackAngle'),
    TrajectoryInEvent = cms.bool(True),
    alias = cms.untracked.string('ctfWithMaterialTracks'),
    beamSpot = cms.InputTag("hltOnlineBeamSpot"),
    clusterRemovalInfo = cms.InputTag(""),
    src = cms.InputTag("hltIter0PFlowCkfTrackCandidates"),
    useHitsSplitting = cms.bool(False),
    useSimpleMF = cms.bool(True)
)


process.hltIter0PFlowTrackCutClassifier = cms.EDProducer("TrackCutClassifier",
    GBRForestFileName = cms.string(''),
    GBRForestLabel = cms.string(''),
    beamspot = cms.InputTag("hltOnlineBeamSpot"),
    mva = cms.PSet(
        dr_par = cms.PSet(
            d0err = cms.vdouble(0.003, 0.003, 0.003),
            d0err_par = cms.vdouble(0.001, 0.001, 0.001),
            dr_exp = cms.vint32(4, 4, 4),
            dr_par1 = cms.vdouble(0.4, 0.4, 0.4),
            dr_par2 = cms.vdouble(0.3, 0.3, 0.3)
        ),
        dz_par = cms.PSet(
            dz_exp = cms.vint32(4, 4, 4),
            dz_par1 = cms.vdouble(0.4, 0.4, 0.4),
            dz_par2 = cms.vdouble(0.35, 0.35, 0.35)
        ),
        maxChi2 = cms.vdouble(9999.0, 25.0, 16.0),
        maxChi2n = cms.vdouble(1.2, 1.0, 0.7),
        maxDr = cms.vdouble(0.5, 0.03, 3.40282346639e+38),
        maxDz = cms.vdouble(0.5, 0.2, 3.40282346639e+38),
        maxDzWrtBS = cms.vdouble(3.40282346639e+38, 24.0, 15.0),
        maxLostLayers = cms.vint32(1, 1, 1),
        min3DLayers = cms.vint32(0, 0, 0),
        minLayers = cms.vint32(3, 3, 3),
        minNVtxTrk = cms.int32(3),
        minNdof = cms.vdouble(1e-05, 1e-05, 1e-05),
        minPixelHits = cms.vint32(0, 0, 0)
    ),
    qualityCuts = cms.vdouble(-0.7, 0.1, 0.7),
    src = cms.InputTag("hltIter0PFlowCtfWithMaterialTracks"),
    vertices = cms.InputTag("hltTrimmedPixelVertices")
)


process.hltIter0PFlowTrackSelectionHighPurity = cms.EDProducer("TrackCollectionFilterCloner",
    cloner = cms.untracked.PSet(
        copyExtras = cms.untracked.bool(False),
        copyTrajectories = cms.untracked.bool(False)
    ),
    minQuality = cms.string('highPurity'),
    originalMVAVals = cms.InputTag("hltIter0PFlowTrackCutClassifier","MVAValues"),
    originalQualVals = cms.InputTag("hltIter0PFlowTrackCutClassifier","QualityMasks"),
    originalSource = cms.InputTag("hltIter0PFlowCtfWithMaterialTracks")
)


process.hltIter0TrackAndTauJets4Iter1 = cms.EDProducer("TauJetSelectorForHLTTrackSeeding",
    etaMaxCaloJet = cms.double(2.7),
    etaMinCaloJet = cms.double(-2.7),
    fractionMaxChargedPUInCaloCone = cms.double(0.3),
    fractionMinCaloInTauCone = cms.double(0.7),
    inputCaloJetTag = cms.InputTag("hltAK4CaloJetsPFEt5"),
    inputTrackJetTag = cms.InputTag("hltAK4Iter0TrackJets4Iter1"),
    inputTrackTag = cms.InputTag("hltIter0PFlowTrackSelectionHighPurity"),
    isolationConeSize = cms.double(0.5),
    nTrkMaxInCaloCone = cms.int32(0),
    ptMinCaloJet = cms.double(5.0),
    ptTrkMaxInCaloCone = cms.double(1.0),
    tauConeSize = cms.double(0.2)
)


process.hltIter1ClustersRefRemoval = cms.EDProducer("TrackClusterRemover",
    TrackQuality = cms.string('highPurity'),
    maxChi2 = cms.double(9.0),
    minNumberOfLayersWithMeasBeforeFiltering = cms.int32(0),
    oldClusterRemovalInfo = cms.InputTag(""),
    overrideTrkQuals = cms.InputTag(""),
    pixelClusters = cms.InputTag("hltSiPixelClusters"),
    stripClusters = cms.InputTag("hltSiStripRawToClustersFacility"),
    trackClassifier = cms.InputTag("","QualityMasks"),
    trajectories = cms.InputTag("hltIter0PFlowTrackSelectionHighPurity")
)


process.hltIter1MaskedMeasurementTrackerEvent = cms.EDProducer("MaskedMeasurementTrackerEventProducer",
    OnDemand = cms.bool(False),
    clustersToSkip = cms.InputTag("hltIter1ClustersRefRemoval"),
    src = cms.InputTag("hltSiStripClusters")
)


process.hltIter1Merged = cms.EDProducer("TrackListMerger",
    Epsilon = cms.double(-0.001),
    FoundHitBonus = cms.double(5.0),
    LostHitPenalty = cms.double(20.0),
    MaxNormalizedChisq = cms.double(1000.0),
    MinFound = cms.int32(3),
    MinPT = cms.double(0.05),
    ShareFrac = cms.double(0.19),
    TrackProducers = cms.VInputTag("hltIter0PFlowTrackSelectionHighPurity", "hltIter1PFlowTrackSelectionHighPurity"),
    allowFirstHitShare = cms.bool(True),
    copyExtras = cms.untracked.bool(True),
    copyMVA = cms.bool(False),
    hasSelector = cms.vint32(0, 0),
    indivShareFrac = cms.vdouble(1.0, 1.0),
    newQuality = cms.string('confirmed'),
    selectedTrackQuals = cms.VInputTag("hltIter0PFlowTrackSelectionHighPurity", "hltIter1PFlowTrackSelectionHighPurity"),
    setsToMerge = cms.VPSet(cms.PSet(
        pQual = cms.bool(False),
        tLists = cms.vint32(0, 1)
    )),
    writeOnlyTrkQuals = cms.bool(False)
)


process.hltIter1PFlowCkfTrackCandidates = cms.EDProducer("CkfTrackCandidateMaker",
    MeasurementTrackerEvent = cms.InputTag("hltIter1MaskedMeasurementTrackerEvent"),
    NavigationSchool = cms.string('SimpleNavigationSchool'),
    RedundantSeedCleaner = cms.string('CachingSeedCleanerBySharedInput'),
    SimpleMagneticField = cms.string('ParabolicMf'),
    TrajectoryBuilder = cms.string(''),
    TrajectoryBuilderPSet = cms.PSet(
        refToPSet_ = cms.string('HLTIter1PSetTrajectoryBuilderIT')
    ),
    TrajectoryCleaner = cms.string('hltESPTrajectoryCleanerBySharedHits'),
    TransientInitialStateEstimatorParameters = cms.PSet(
        numberMeasurementsForFit = cms.int32(4),
        propagatorAlongTISE = cms.string('PropagatorWithMaterialParabolicMf'),
        propagatorOppositeTISE = cms.string('PropagatorWithMaterialParabolicMfOpposite')
    ),
    cleanTrajectoryAfterInOut = cms.bool(False),
    doSeedingRegionRebuilding = cms.bool(False),
    maxNSeeds = cms.uint32(100000),
    maxSeedsBeforeCleaning = cms.uint32(1000),
    src = cms.InputTag("hltIter1PFlowPixelSeeds"),
    useHitsSplitting = cms.bool(False)
)


process.hltIter1PFlowCtfWithMaterialTracks = cms.EDProducer("TrackProducer",
    AlgorithmName = cms.string('hltIter1'),
    Fitter = cms.string('hltESPFittingSmootherIT'),
    GeometricInnerState = cms.bool(True),
    MeasurementTracker = cms.string(''),
    MeasurementTrackerEvent = cms.InputTag("hltIter1MaskedMeasurementTrackerEvent"),
    NavigationSchool = cms.string(''),
    Propagator = cms.string('hltESPRungeKuttaTrackerPropagator'),
    SimpleMagneticField = cms.string('ParabolicMf'),
    TTRHBuilder = cms.string('hltESPTTRHBWithTrackAngle'),
    TrajectoryInEvent = cms.bool(True),
    alias = cms.untracked.string('ctfWithMaterialTracks'),
    beamSpot = cms.InputTag("hltOnlineBeamSpot"),
    clusterRemovalInfo = cms.InputTag(""),
    src = cms.InputTag("hltIter1PFlowCkfTrackCandidates"),
    useHitsSplitting = cms.bool(False),
    useSimpleMF = cms.bool(True)
)


process.hltIter1PFlowPixelSeeds = cms.EDProducer("SeedGeneratorFromRegionHitsEDProducer",
    ClusterCheckPSet = cms.PSet(
        ClusterCollectionLabel = cms.InputTag("hltSiStripClusters"),
        MaxNumberOfCosmicClusters = cms.uint32(50000),
        MaxNumberOfPixelClusters = cms.uint32(10000),
        PixelClusterCollectionLabel = cms.InputTag("hltSiPixelClusters"),
        doClusterCheck = cms.bool(False)
    ),
    OrderedHitsFactoryPSet = cms.PSet(
        ComponentName = cms.string('StandardHitTripletGenerator'),
        GeneratorPSet = cms.PSet(
            ComponentName = cms.string('PixelTripletHLTGenerator'),
            SeedComparitorPSet = cms.PSet(
                ComponentName = cms.string('none')
            ),
            extraHitRPhitolerance = cms.double(0.032),
            extraHitRZtolerance = cms.double(0.037),
            maxElement = cms.uint32(100000),
            phiPreFiltering = cms.double(0.3),
            useBending = cms.bool(True),
            useFixedPreFiltering = cms.bool(False),
            useMultScattering = cms.bool(True)
        ),
        SeedingLayers = cms.InputTag("hltIter1PixelLayerTriplets"),
        maxElement = cms.uint32(0)
    ),
    RegionFactoryPSet = cms.PSet(
        ComponentName = cms.string('CandidateSeededTrackingRegionsProducer'),
        RegionPSet = cms.PSet(
            beamSpot = cms.InputTag("hltOnlineBeamSpot"),
            deltaEta = cms.double(1.0),
            deltaPhi = cms.double(1.0),
            input = cms.InputTag("hltIter0TrackAndTauJets4Iter1"),
            maxNRegions = cms.int32(100),
            maxNVertices = cms.int32(10),
            measurementTrackerName = cms.InputTag("hltIter1MaskedMeasurementTrackerEvent"),
            mode = cms.string('VerticesFixed'),
            nSigmaZBeamSpot = cms.double(3.0),
            originRadius = cms.double(0.05),
            precise = cms.bool(True),
            ptMin = cms.double(0.5),
            searchOpt = cms.bool(True),
            useFakeVertices = cms.bool(False),
            useMultipleScattering = cms.bool(False),
            vertexCollection = cms.InputTag("hltTrimmedPixelVertices"),
            whereToUseMeasurementTracker = cms.string('ForSiStrips'),
            zErrorBeamSpot = cms.double(15.0),
            zErrorVetex = cms.double(0.1)
        )
    ),
    SeedComparitorPSet = cms.PSet(
        ComponentName = cms.string('none')
    ),
    SeedCreatorPSet = cms.PSet(
        refToPSet_ = cms.string('HLTSeedFromConsecutiveHitsTripletOnlyCreator')
    )
)


process.hltIter1PFlowTrackCutClassifierDetached = cms.EDProducer("TrackCutClassifier",
    GBRForestFileName = cms.string(''),
    GBRForestLabel = cms.string(''),
    beamspot = cms.InputTag("hltOnlineBeamSpot"),
    mva = cms.PSet(
        dr_par = cms.PSet(
            d0err = cms.vdouble(0.003, 0.003, 0.003),
            d0err_par = cms.vdouble(0.001, 0.001, 0.001),
            dr_exp = cms.vint32(4, 4, 4),
            dr_par1 = cms.vdouble(1.0, 1.0, 1.0),
            dr_par2 = cms.vdouble(1.0, 1.0, 1.0)
        ),
        dz_par = cms.PSet(
            dz_exp = cms.vint32(4, 4, 4),
            dz_par1 = cms.vdouble(1.0, 1.0, 1.0),
            dz_par2 = cms.vdouble(1.0, 1.0, 1.0)
        ),
        maxChi2 = cms.vdouble(9999.0, 25.0, 16.0),
        maxChi2n = cms.vdouble(1.0, 0.7, 0.4),
        maxDr = cms.vdouble(3.40282346639e+38, 1.0, 3.40282346639e+38),
        maxDz = cms.vdouble(3.40282346639e+38, 1.0, 3.40282346639e+38),
        maxDzWrtBS = cms.vdouble(3.40282346639e+38, 24.0, 15.0),
        maxLostLayers = cms.vint32(99, 3, 3),
        min3DLayers = cms.vint32(1, 2, 3),
        minLayers = cms.vint32(5, 5, 5),
        minNVtxTrk = cms.int32(2),
        minNdof = cms.vdouble(-1.0, -1.0, -1.0),
        minPixelHits = cms.vint32(0, 0, 1)
    ),
    qualityCuts = cms.vdouble(-0.7, 0.1, 0.7),
    src = cms.InputTag("hltIter1PFlowCtfWithMaterialTracks"),
    vertices = cms.InputTag("hltTrimmedPixelVertices")
)


process.hltIter1PFlowTrackCutClassifierMerged = cms.EDProducer("ClassifierMerger",
    inputClassifiers = cms.vstring('hltIter1PFlowTrackCutClassifierPrompt', 
        'hltIter1PFlowTrackCutClassifierDetached')
)


process.hltIter1PFlowTrackCutClassifierPrompt = cms.EDProducer("TrackCutClassifier",
    GBRForestFileName = cms.string(''),
    GBRForestLabel = cms.string(''),
    beamspot = cms.InputTag("hltOnlineBeamSpot"),
    mva = cms.PSet(
        dr_par = cms.PSet(
            d0err = cms.vdouble(0.003, 0.003, 0.003),
            d0err_par = cms.vdouble(0.001, 0.001, 0.001),
            dr_exp = cms.vint32(3, 3, 3),
            dr_par1 = cms.vdouble(3.40282346639e+38, 1.0, 0.9),
            dr_par2 = cms.vdouble(3.40282346639e+38, 1.0, 0.85)
        ),
        dz_par = cms.PSet(
            dz_exp = cms.vint32(3, 3, 3),
            dz_par1 = cms.vdouble(3.40282346639e+38, 1.0, 0.9),
            dz_par2 = cms.vdouble(3.40282346639e+38, 1.0, 0.8)
        ),
        maxChi2 = cms.vdouble(9999.0, 25.0, 16.0),
        maxChi2n = cms.vdouble(1.2, 1.0, 0.7),
        maxDr = cms.vdouble(3.40282346639e+38, 1.0, 3.40282346639e+38),
        maxDz = cms.vdouble(3.40282346639e+38, 1.0, 3.40282346639e+38),
        maxDzWrtBS = cms.vdouble(3.40282346639e+38, 24.0, 15.0),
        maxLostLayers = cms.vint32(1, 1, 1),
        min3DLayers = cms.vint32(0, 0, 0),
        minLayers = cms.vint32(3, 3, 3),
        minNVtxTrk = cms.int32(3),
        minNdof = cms.vdouble(1e-05, 1e-05, 1e-05),
        minPixelHits = cms.vint32(0, 0, 2)
    ),
    qualityCuts = cms.vdouble(-0.7, 0.1, 0.7),
    src = cms.InputTag("hltIter1PFlowCtfWithMaterialTracks"),
    vertices = cms.InputTag("hltTrimmedPixelVertices")
)


process.hltIter1PFlowTrackSelectionHighPurity = cms.EDProducer("TrackCollectionFilterCloner",
    cloner = cms.untracked.PSet(
        copyExtras = cms.untracked.bool(False),
        copyTrajectories = cms.untracked.bool(False)
    ),
    minQuality = cms.string('highPurity'),
    originalMVAVals = cms.InputTag("hltIter1PFlowTrackCutClassifierMerged","MVAValues"),
    originalQualVals = cms.InputTag("hltIter1PFlowTrackCutClassifierMerged","QualityMasks"),
    originalSource = cms.InputTag("hltIter1PFlowCtfWithMaterialTracks")
)


process.hltIter1PixelLayerTriplets = cms.EDProducer("SeedingLayersEDProducer",
    BPix = cms.PSet(
        HitProducer = cms.string('hltSiPixelRecHits'),
        TTRHBuilder = cms.string('hltESPTTRHBuilderPixelOnly'),
        hitErrorRPhi = cms.double(0.0027),
        hitErrorRZ = cms.double(0.006),
        skipClusters = cms.InputTag("hltIter1ClustersRefRemoval"),
        useErrorsFromParam = cms.bool(True)
    ),
    FPix = cms.PSet(
        HitProducer = cms.string('hltSiPixelRecHits'),
        TTRHBuilder = cms.string('hltESPTTRHBuilderPixelOnly'),
        hitErrorRPhi = cms.double(0.0051),
        hitErrorRZ = cms.double(0.0036),
        skipClusters = cms.InputTag("hltIter1ClustersRefRemoval"),
        useErrorsFromParam = cms.bool(True)
    ),
    MTEC = cms.PSet(

    ),
    MTIB = cms.PSet(

    ),
    MTID = cms.PSet(

    ),
    MTOB = cms.PSet(

    ),
    TEC = cms.PSet(

    ),
    TIB = cms.PSet(

    ),
    TID = cms.PSet(

    ),
    TOB = cms.PSet(

    ),
    layerList = cms.vstring('BPix1+BPix2+BPix3', 
        'BPix1+BPix2+FPix1_pos', 
        'BPix1+BPix2+FPix1_neg', 
        'BPix1+FPix1_pos+FPix2_pos', 
        'BPix1+FPix1_neg+FPix2_neg')
)


process.hltIter1TrackAndTauJets4Iter2 = cms.EDProducer("TauJetSelectorForHLTTrackSeeding",
    etaMaxCaloJet = cms.double(2.7),
    etaMinCaloJet = cms.double(-2.7),
    fractionMaxChargedPUInCaloCone = cms.double(0.3),
    fractionMinCaloInTauCone = cms.double(0.7),
    inputCaloJetTag = cms.InputTag("hltAK4CaloJetsPFEt5"),
    inputTrackJetTag = cms.InputTag("hltAK4Iter1TrackJets4Iter2"),
    inputTrackTag = cms.InputTag("hltIter1Merged"),
    isolationConeSize = cms.double(0.5),
    nTrkMaxInCaloCone = cms.int32(0),
    ptMinCaloJet = cms.double(5.0),
    ptTrkMaxInCaloCone = cms.double(1.4),
    tauConeSize = cms.double(0.2)
)


process.hltIter1TrackRefsForJets4Iter2 = cms.EDProducer("ChargedRefCandidateProducer",
    particleType = cms.string('pi+'),
    src = cms.InputTag("hltIter1Merged")
)


process.hltIter2ClustersRefRemoval = cms.EDProducer("TrackClusterRemover",
    TrackQuality = cms.string('highPurity'),
    maxChi2 = cms.double(16.0),
    minNumberOfLayersWithMeasBeforeFiltering = cms.int32(0),
    oldClusterRemovalInfo = cms.InputTag("hltIter1ClustersRefRemoval"),
    overrideTrkQuals = cms.InputTag(""),
    pixelClusters = cms.InputTag("hltSiPixelClusters"),
    stripClusters = cms.InputTag("hltSiStripRawToClustersFacility"),
    trackClassifier = cms.InputTag("","QualityMasks"),
    trajectories = cms.InputTag("hltIter1PFlowTrackSelectionHighPurity")
)


process.hltIter2MaskedMeasurementTrackerEvent = cms.EDProducer("MaskedMeasurementTrackerEventProducer",
    OnDemand = cms.bool(False),
    clustersToSkip = cms.InputTag("hltIter2ClustersRefRemoval"),
    src = cms.InputTag("hltSiStripClusters")
)


process.hltIter2Merged = cms.EDProducer("TrackListMerger",
    Epsilon = cms.double(-0.001),
    FoundHitBonus = cms.double(5.0),
    LostHitPenalty = cms.double(20.0),
    MaxNormalizedChisq = cms.double(1000.0),
    MinFound = cms.int32(3),
    MinPT = cms.double(0.05),
    ShareFrac = cms.double(0.19),
    TrackProducers = cms.VInputTag("hltIter1Merged", "hltIter2PFlowTrackSelectionHighPurity"),
    allowFirstHitShare = cms.bool(True),
    copyExtras = cms.untracked.bool(True),
    copyMVA = cms.bool(False),
    hasSelector = cms.vint32(0, 0),
    indivShareFrac = cms.vdouble(1.0, 1.0),
    newQuality = cms.string('confirmed'),
    selectedTrackQuals = cms.VInputTag("hltIter1Merged", "hltIter2PFlowTrackSelectionHighPurity"),
    setsToMerge = cms.VPSet(cms.PSet(
        pQual = cms.bool(False),
        tLists = cms.vint32(0, 1)
    )),
    writeOnlyTrkQuals = cms.bool(False)
)


process.hltIter2PFlowCkfTrackCandidates = cms.EDProducer("CkfTrackCandidateMaker",
    MeasurementTrackerEvent = cms.InputTag("hltIter2MaskedMeasurementTrackerEvent"),
    NavigationSchool = cms.string('SimpleNavigationSchool'),
    RedundantSeedCleaner = cms.string('CachingSeedCleanerBySharedInput'),
    SimpleMagneticField = cms.string('ParabolicMf'),
    TrajectoryBuilder = cms.string(''),
    TrajectoryBuilderPSet = cms.PSet(
        refToPSet_ = cms.string('HLTIter2PSetTrajectoryBuilderIT')
    ),
    TrajectoryCleaner = cms.string('hltESPTrajectoryCleanerBySharedHits'),
    TransientInitialStateEstimatorParameters = cms.PSet(
        numberMeasurementsForFit = cms.int32(4),
        propagatorAlongTISE = cms.string('PropagatorWithMaterialParabolicMf'),
        propagatorOppositeTISE = cms.string('PropagatorWithMaterialParabolicMfOpposite')
    ),
    cleanTrajectoryAfterInOut = cms.bool(False),
    doSeedingRegionRebuilding = cms.bool(False),
    maxNSeeds = cms.uint32(100000),
    maxSeedsBeforeCleaning = cms.uint32(1000),
    src = cms.InputTag("hltIter2PFlowPixelSeeds"),
    useHitsSplitting = cms.bool(False)
)


process.hltIter2PFlowCtfWithMaterialTracks = cms.EDProducer("TrackProducer",
    AlgorithmName = cms.string('hltIter2'),
    Fitter = cms.string('hltESPFittingSmootherIT'),
    GeometricInnerState = cms.bool(True),
    MeasurementTracker = cms.string(''),
    MeasurementTrackerEvent = cms.InputTag("hltIter2MaskedMeasurementTrackerEvent"),
    NavigationSchool = cms.string(''),
    Propagator = cms.string('hltESPRungeKuttaTrackerPropagator'),
    SimpleMagneticField = cms.string('ParabolicMf'),
    TTRHBuilder = cms.string('hltESPTTRHBWithTrackAngle'),
    TrajectoryInEvent = cms.bool(True),
    alias = cms.untracked.string('ctfWithMaterialTracks'),
    beamSpot = cms.InputTag("hltOnlineBeamSpot"),
    clusterRemovalInfo = cms.InputTag(""),
    src = cms.InputTag("hltIter2PFlowCkfTrackCandidates"),
    useHitsSplitting = cms.bool(False),
    useSimpleMF = cms.bool(True)
)


process.hltIter2PFlowPixelSeeds = cms.EDProducer("SeedGeneratorFromRegionHitsEDProducer",
    ClusterCheckPSet = cms.PSet(
        ClusterCollectionLabel = cms.InputTag("hltSiStripClusters"),
        MaxNumberOfCosmicClusters = cms.uint32(50000),
        MaxNumberOfPixelClusters = cms.uint32(10000),
        PixelClusterCollectionLabel = cms.InputTag("hltSiPixelClusters"),
        doClusterCheck = cms.bool(False)
    ),
    OrderedHitsFactoryPSet = cms.PSet(
        ComponentName = cms.string('StandardHitPairGenerator'),
        GeneratorPSet = cms.PSet(
            SeedComparitorPSet = cms.PSet(
                ComponentName = cms.string('none')
            ),
            maxElement = cms.uint32(100000)
        ),
        SeedingLayers = cms.InputTag("hltIter2PixelLayerPairs"),
        maxElement = cms.uint32(0)
    ),
    RegionFactoryPSet = cms.PSet(
        ComponentName = cms.string('CandidateSeededTrackingRegionsProducer'),
        RegionPSet = cms.PSet(
            beamSpot = cms.InputTag("hltOnlineBeamSpot"),
            deltaEta = cms.double(0.8),
            deltaPhi = cms.double(0.8),
            input = cms.InputTag("hltIter1TrackAndTauJets4Iter2"),
            maxNRegions = cms.int32(100),
            maxNVertices = cms.int32(10),
            measurementTrackerName = cms.InputTag("hltIter2MaskedMeasurementTrackerEvent"),
            mode = cms.string('VerticesFixed'),
            nSigmaZBeamSpot = cms.double(3.0),
            originRadius = cms.double(0.025),
            originZPos = cms.double(0.0),
            precise = cms.bool(True),
            ptMin = cms.double(1.2),
            searchOpt = cms.bool(True),
            useFakeVertices = cms.bool(False),
            useMultipleScattering = cms.bool(False),
            vertexCollection = cms.InputTag("hltTrimmedPixelVertices"),
            whereToUseMeasurementTracker = cms.string('ForSiStrips'),
            zErrorBeamSpot = cms.double(15.0),
            zErrorVetex = cms.double(0.05)
        )
    ),
    SeedComparitorPSet = cms.PSet(
        ComponentName = cms.string('none')
    ),
    SeedCreatorPSet = cms.PSet(
        refToPSet_ = cms.string('HLTSeedFromConsecutiveHitsCreatorIT')
    )
)


process.hltIter2PFlowTrackCutClassifier = cms.EDProducer("TrackCutClassifier",
    GBRForestFileName = cms.string(''),
    GBRForestLabel = cms.string(''),
    beamspot = cms.InputTag("hltOnlineBeamSpot"),
    mva = cms.PSet(
        dr_par = cms.PSet(
            d0err = cms.vdouble(0.003, 0.003, 0.003),
            d0err_par = cms.vdouble(0.001, 0.001, 0.001),
            dr_exp = cms.vint32(4, 4, 4),
            dr_par1 = cms.vdouble(3.40282346639e+38, 0.4, 0.4),
            dr_par2 = cms.vdouble(3.40282346639e+38, 0.3, 0.3)
        ),
        dz_par = cms.PSet(
            dz_exp = cms.vint32(4, 4, 4),
            dz_par1 = cms.vdouble(3.40282346639e+38, 0.4, 0.4),
            dz_par2 = cms.vdouble(3.40282346639e+38, 0.35, 0.35)
        ),
        maxChi2 = cms.vdouble(9999.0, 25.0, 16.0),
        maxChi2n = cms.vdouble(1.2, 1.0, 0.7),
        maxDr = cms.vdouble(0.5, 0.03, 3.40282346639e+38),
        maxDz = cms.vdouble(0.5, 0.2, 3.40282346639e+38),
        maxDzWrtBS = cms.vdouble(3.40282346639e+38, 24.0, 15.0),
        maxLostLayers = cms.vint32(1, 1, 1),
        min3DLayers = cms.vint32(0, 0, 0),
        minLayers = cms.vint32(3, 3, 3),
        minNVtxTrk = cms.int32(3),
        minNdof = cms.vdouble(1e-05, 1e-05, 1e-05),
        minPixelHits = cms.vint32(0, 0, 0)
    ),
    qualityCuts = cms.vdouble(-0.7, 0.1, 0.7),
    src = cms.InputTag("hltIter2PFlowCtfWithMaterialTracks"),
    vertices = cms.InputTag("hltTrimmedPixelVertices")
)


process.hltIter2PFlowTrackSelectionHighPurity = cms.EDProducer("TrackCollectionFilterCloner",
    cloner = cms.untracked.PSet(
        copyExtras = cms.untracked.bool(False),
        copyTrajectories = cms.untracked.bool(False)
    ),
    minQuality = cms.string('highPurity'),
    originalMVAVals = cms.InputTag("hltIter2PFlowTrackCutClassifier","MVAValues"),
    originalQualVals = cms.InputTag("hltIter2PFlowTrackCutClassifier","QualityMasks"),
    originalSource = cms.InputTag("hltIter2PFlowCtfWithMaterialTracks")
)


process.hltIter2PixelLayerPairs = cms.EDProducer("SeedingLayersEDProducer",
    BPix = cms.PSet(
        HitProducer = cms.string('hltSiPixelRecHits'),
        TTRHBuilder = cms.string('hltESPTTRHBuilderPixelOnly'),
        hitErrorRPhi = cms.double(0.0027),
        hitErrorRZ = cms.double(0.006),
        skipClusters = cms.InputTag("hltIter2ClustersRefRemoval"),
        useErrorsFromParam = cms.bool(True)
    ),
    FPix = cms.PSet(
        HitProducer = cms.string('hltSiPixelRecHits'),
        TTRHBuilder = cms.string('hltESPTTRHBuilderPixelOnly'),
        hitErrorRPhi = cms.double(0.0051),
        hitErrorRZ = cms.double(0.0036),
        skipClusters = cms.InputTag("hltIter2ClustersRefRemoval"),
        useErrorsFromParam = cms.bool(True)
    ),
    MTEC = cms.PSet(

    ),
    MTIB = cms.PSet(

    ),
    MTID = cms.PSet(

    ),
    MTOB = cms.PSet(

    ),
    TEC = cms.PSet(

    ),
    TIB = cms.PSet(

    ),
    TID = cms.PSet(

    ),
    TOB = cms.PSet(

    ),
    layerList = cms.vstring('BPix1+BPix2', 
        'BPix1+BPix3', 
        'BPix2+BPix3', 
        'BPix1+FPix1_pos', 
        'BPix1+FPix1_neg', 
        'BPix1+FPix2_pos', 
        'BPix1+FPix2_neg', 
        'BPix2+FPix1_pos', 
        'BPix2+FPix1_neg', 
        'BPix2+FPix2_pos', 
        'BPix2+FPix2_neg', 
        'FPix1_pos+FPix2_pos', 
        'FPix1_neg+FPix2_neg')
)


process.hltL2MuonCandidates = cms.EDProducer("L2MuonCandidateProducer",
    InputObjects = cms.InputTag("hltL2Muons","UpdatedAtVtx")
)


process.hltL2MuonSeeds = cms.EDProducer("L2MuonSeedGeneratorFromL1T",
    CentralBxOnly = cms.bool(True),
    EtaMatchingBins = cms.vdouble(0.0, 2.5),
    GMTReadoutCollection = cms.InputTag(""),
    InputObjects = cms.InputTag("hltGmtStage2Digis","Muon"),
    L1MaxEta = cms.double(2.5),
    L1MinPt = cms.double(0.0),
    L1MinQuality = cms.uint32(7),
    MatchDR = cms.vdouble(0.3),
    OfflineSeedLabel = cms.untracked.InputTag("hltL2OfflineMuonSeeds"),
    Propagator = cms.string('SteppingHelixPropagatorAny'),
    ServiceParameters = cms.PSet(
        Propagators = cms.untracked.vstring('SteppingHelixPropagatorAny'),
        RPCLayers = cms.bool(True),
        UseMuonNavigation = cms.untracked.bool(True)
    ),
    UseOfflineSeed = cms.untracked.bool(True),
    UseUnassociatedL1 = cms.bool(False)
)


process.hltL2Muons = cms.EDProducer("L2MuonProducer",
    DoSeedRefit = cms.bool(False),
    InputObjects = cms.InputTag("hltL2MuonSeeds"),
    L2TrajBuilderParameters = cms.PSet(
        BWFilterParameters = cms.PSet(
            BWSeedType = cms.string('fromGenerator'),
            CSCRecSegmentLabel = cms.InputTag("hltCscSegments"),
            DTRecSegmentLabel = cms.InputTag("hltDt4DSegments"),
            EnableCSCMeasurement = cms.bool(True),
            EnableDTMeasurement = cms.bool(True),
            EnableRPCMeasurement = cms.bool(True),
            FitDirection = cms.string('outsideIn'),
            MaxChi2 = cms.double(100.0),
            MuonTrajectoryUpdatorParameters = cms.PSet(
                ExcludeRPCFromFit = cms.bool(False),
                Granularity = cms.int32(0),
                MaxChi2 = cms.double(25.0),
                RescaleError = cms.bool(False),
                RescaleErrorFactor = cms.double(100.0),
                UseInvalidHits = cms.bool(True)
            ),
            NumberOfSigma = cms.double(3.0),
            Propagator = cms.string('hltESPFastSteppingHelixPropagatorAny'),
            RPCRecSegmentLabel = cms.InputTag("hltRpcRecHits")
        ),
        DoBackwardFilter = cms.bool(True),
        DoRefit = cms.bool(False),
        DoSeedRefit = cms.bool(False),
        FilterParameters = cms.PSet(
            CSCRecSegmentLabel = cms.InputTag("hltCscSegments"),
            DTRecSegmentLabel = cms.InputTag("hltDt4DSegments"),
            EnableCSCMeasurement = cms.bool(True),
            EnableDTMeasurement = cms.bool(True),
            EnableRPCMeasurement = cms.bool(True),
            FitDirection = cms.string('insideOut'),
            MaxChi2 = cms.double(1000.0),
            MuonTrajectoryUpdatorParameters = cms.PSet(
                ExcludeRPCFromFit = cms.bool(False),
                Granularity = cms.int32(0),
                MaxChi2 = cms.double(25.0),
                RescaleError = cms.bool(False),
                RescaleErrorFactor = cms.double(100.0),
                UseInvalidHits = cms.bool(True)
            ),
            NumberOfSigma = cms.double(3.0),
            Propagator = cms.string('hltESPFastSteppingHelixPropagatorAny'),
            RPCRecSegmentLabel = cms.InputTag("hltRpcRecHits")
        ),
        NavigationType = cms.string('Standard'),
        SeedPosition = cms.string('in'),
        SeedPropagator = cms.string('hltESPFastSteppingHelixPropagatorAny'),
        SeedTransformerParameters = cms.PSet(
            Fitter = cms.string('hltESPKFFittingSmootherForL2Muon'),
            MuonRecHitBuilder = cms.string('hltESPMuonTransientTrackingRecHitBuilder'),
            NMinRecHits = cms.uint32(2),
            Propagator = cms.string('hltESPFastSteppingHelixPropagatorAny'),
            RescaleError = cms.double(100.0),
            UseSubRecHits = cms.bool(False)
        )
    ),
    MuonTrajectoryBuilder = cms.string('Exhaustive'),
    SeedTransformerParameters = cms.PSet(
        Fitter = cms.string('hltESPKFFittingSmootherForL2Muon'),
        MuonRecHitBuilder = cms.string('hltESPMuonTransientTrackingRecHitBuilder'),
        NMinRecHits = cms.uint32(2),
        Propagator = cms.string('hltESPFastSteppingHelixPropagatorAny'),
        RescaleError = cms.double(100.0),
        UseSubRecHits = cms.bool(False)
    ),
    ServiceParameters = cms.PSet(
        Propagators = cms.untracked.vstring('hltESPFastSteppingHelixPropagatorAny', 
            'hltESPFastSteppingHelixPropagatorOpposite'),
        RPCLayers = cms.bool(True),
        UseMuonNavigation = cms.untracked.bool(True)
    ),
    TrackLoaderParameters = cms.PSet(
        DoSmoothing = cms.bool(False),
        MuonUpdatorAtVertexParameters = cms.PSet(
            BeamSpotPosition = cms.vdouble(0.0, 0.0, 0.0),
            BeamSpotPositionErrors = cms.vdouble(0.1, 0.1, 5.3),
            MaxChi2 = cms.double(1000000.0),
            Propagator = cms.string('hltESPFastSteppingHelixPropagatorOpposite')
        ),
        Smoother = cms.string('hltESPKFTrajectorySmootherForMuonTrackLoader'),
        TTRHBuilder = cms.string('hltESPTTRHBWithTrackAngle'),
        VertexConstraint = cms.bool(True),
        beamSpot = cms.InputTag("hltOnlineBeamSpot")
    )
)


process.hltL2OfflineMuonSeeds = cms.EDProducer("MuonSeedGenerator",
    CSCRecSegmentLabel = cms.InputTag("hltCscSegments"),
    CSC_01 = cms.vdouble(0.166, 0.0, 0.0, 0.031, 0.0, 
        0.0),
    CSC_01_1_scale = cms.vdouble(-1.915329, 0.0),
    CSC_02 = cms.vdouble(0.612, -0.207, 0.0, 0.067, -0.001, 
        0.0),
    CSC_03 = cms.vdouble(0.787, -0.338, 0.029, 0.101, -0.008, 
        0.0),
    CSC_12 = cms.vdouble(-0.161, 0.254, -0.047, 0.042, -0.007, 
        0.0),
    CSC_12_1_scale = cms.vdouble(-6.434242, 0.0),
    CSC_12_2_scale = cms.vdouble(-1.63622, 0.0),
    CSC_12_3_scale = cms.vdouble(-1.63622, 0.0),
    CSC_13 = cms.vdouble(0.901, -1.302, 0.533, 0.045, 0.005, 
        0.0),
    CSC_13_2_scale = cms.vdouble(-6.077936, 0.0),
    CSC_13_3_scale = cms.vdouble(-1.701268, 0.0),
    CSC_14 = cms.vdouble(0.606, -0.181, -0.002, 0.111, -0.003, 
        0.0),
    CSC_14_3_scale = cms.vdouble(-1.969563, 0.0),
    CSC_23 = cms.vdouble(-0.081, 0.113, -0.029, 0.015, 0.008, 
        0.0),
    CSC_23_1_scale = cms.vdouble(-19.084285, 0.0),
    CSC_23_2_scale = cms.vdouble(-6.079917, 0.0),
    CSC_24 = cms.vdouble(0.004, 0.021, -0.002, 0.053, 0.0, 
        0.0),
    CSC_24_1_scale = cms.vdouble(-6.055701, 0.0),
    CSC_34 = cms.vdouble(0.062, -0.067, 0.019, 0.021, 0.003, 
        0.0),
    CSC_34_1_scale = cms.vdouble(-11.520507, 0.0),
    DTRecSegmentLabel = cms.InputTag("hltDt4DSegments"),
    DT_12 = cms.vdouble(0.183, 0.054, -0.087, 0.028, 0.002, 
        0.0),
    DT_12_1_scale = cms.vdouble(-3.692398, 0.0),
    DT_12_2_scale = cms.vdouble(-3.518165, 0.0),
    DT_13 = cms.vdouble(0.315, 0.068, -0.127, 0.051, -0.002, 
        0.0),
    DT_13_1_scale = cms.vdouble(-4.520923, 0.0),
    DT_13_2_scale = cms.vdouble(-4.257687, 0.0),
    DT_14 = cms.vdouble(0.359, 0.052, -0.107, 0.072, -0.004, 
        0.0),
    DT_14_1_scale = cms.vdouble(-5.644816, 0.0),
    DT_14_2_scale = cms.vdouble(-4.808546, 0.0),
    DT_23 = cms.vdouble(0.13, 0.023, -0.057, 0.028, 0.004, 
        0.0),
    DT_23_1_scale = cms.vdouble(-5.320346, 0.0),
    DT_23_2_scale = cms.vdouble(-5.117625, 0.0),
    DT_24 = cms.vdouble(0.176, 0.014, -0.051, 0.051, 0.003, 
        0.0),
    DT_24_1_scale = cms.vdouble(-7.490909, 0.0),
    DT_24_2_scale = cms.vdouble(-6.63094, 0.0),
    DT_34 = cms.vdouble(0.044, 0.004, -0.013, 0.029, 0.003, 
        0.0),
    DT_34_1_scale = cms.vdouble(-13.783765, 0.0),
    DT_34_2_scale = cms.vdouble(-11.901897, 0.0),
    EnableCSCMeasurement = cms.bool(True),
    EnableDTMeasurement = cms.bool(True),
    OL_1213 = cms.vdouble(0.96, -0.737, 0.0, 0.052, 0.0, 
        0.0),
    OL_1213_0_scale = cms.vdouble(-4.488158, 0.0),
    OL_1222 = cms.vdouble(0.848, -0.591, 0.0, 0.062, 0.0, 
        0.0),
    OL_1222_0_scale = cms.vdouble(-5.810449, 0.0),
    OL_1232 = cms.vdouble(0.184, 0.0, 0.0, 0.066, 0.0, 
        0.0),
    OL_1232_0_scale = cms.vdouble(-5.964634, 0.0),
    OL_2213 = cms.vdouble(0.117, 0.0, 0.0, 0.044, 0.0, 
        0.0),
    OL_2213_0_scale = cms.vdouble(-7.239789, 0.0),
    OL_2222 = cms.vdouble(0.107, 0.0, 0.0, 0.04, 0.0, 
        0.0),
    OL_2222_0_scale = cms.vdouble(-7.667231, 0.0),
    SMB_10 = cms.vdouble(1.387, -0.038, 0.0, 0.19, 0.0, 
        0.0),
    SMB_10_0_scale = cms.vdouble(2.448566, 0.0),
    SMB_11 = cms.vdouble(1.247, 0.72, -0.802, 0.229, -0.075, 
        0.0),
    SMB_11_0_scale = cms.vdouble(2.56363, 0.0),
    SMB_12 = cms.vdouble(2.128, -0.956, 0.0, 0.199, 0.0, 
        0.0),
    SMB_12_0_scale = cms.vdouble(2.283221, 0.0),
    SMB_20 = cms.vdouble(1.011, -0.052, 0.0, 0.188, 0.0, 
        0.0),
    SMB_20_0_scale = cms.vdouble(1.486168, 0.0),
    SMB_21 = cms.vdouble(1.043, -0.124, 0.0, 0.183, 0.0, 
        0.0),
    SMB_21_0_scale = cms.vdouble(1.58384, 0.0),
    SMB_22 = cms.vdouble(1.474, -0.758, 0.0, 0.185, 0.0, 
        0.0),
    SMB_22_0_scale = cms.vdouble(1.346681, 0.0),
    SMB_30 = cms.vdouble(0.505, -0.022, 0.0, 0.215, 0.0, 
        0.0),
    SMB_30_0_scale = cms.vdouble(-3.629838, 0.0),
    SMB_31 = cms.vdouble(0.549, -0.145, 0.0, 0.207, 0.0, 
        0.0),
    SMB_31_0_scale = cms.vdouble(-3.323768, 0.0),
    SMB_32 = cms.vdouble(0.67, -0.327, 0.0, 0.22, 0.0, 
        0.0),
    SMB_32_0_scale = cms.vdouble(-3.054156, 0.0),
    SME_11 = cms.vdouble(3.295, -1.527, 0.112, 0.378, 0.02, 
        0.0),
    SME_11_0_scale = cms.vdouble(1.325085, 0.0),
    SME_12 = cms.vdouble(0.102, 0.599, 0.0, 0.38, 0.0, 
        0.0),
    SME_12_0_scale = cms.vdouble(2.279181, 0.0),
    SME_13 = cms.vdouble(-1.286, 1.711, 0.0, 0.356, 0.0, 
        0.0),
    SME_13_0_scale = cms.vdouble(0.104905, 0.0),
    SME_21 = cms.vdouble(-0.529, 1.194, -0.358, 0.472, 0.086, 
        0.0),
    SME_21_0_scale = cms.vdouble(-0.040862, 0.0),
    SME_22 = cms.vdouble(-1.207, 1.491, -0.251, 0.189, 0.243, 
        0.0),
    SME_22_0_scale = cms.vdouble(-3.457901, 0.0),
    SME_31 = cms.vdouble(-1.594, 1.482, -0.317, 0.487, 0.097, 
        0.0),
    SME_32 = cms.vdouble(-0.901, 1.333, -0.47, 0.41, 0.073, 
        0.0),
    SME_41 = cms.vdouble(-0.003, 0.005, 0.005, 0.608, 0.076, 
        0.0),
    SME_42 = cms.vdouble(-0.003, 0.005, 0.005, 0.608, 0.076, 
        0.0),
    beamSpotTag = cms.InputTag("hltOnlineBeamSpot"),
    crackEtas = cms.vdouble(0.2, 1.6, 1.7),
    crackWindow = cms.double(0.04),
    deltaEtaCrackSearchWindow = cms.double(0.25),
    deltaEtaSearchWindow = cms.double(0.2),
    deltaPhiSearchWindow = cms.double(0.25),
    scaleDT = cms.bool(True)
)


process.hltL3MuonCandidates = cms.EDProducer("L3MuonCandidateProducer",
    InputLinksObjects = cms.InputTag("hltL3MuonsLinksCombination"),
    InputObjects = cms.InputTag("hltL3Muons"),
    MuonPtOption = cms.string('Tracker')
)


process.hltL3Muons = cms.EDProducer("L3TrackCombiner",
    labels = cms.VInputTag("hltL3MuonsOIState", "hltL3MuonsOIHit", "hltL3MuonsIOHit")
)


process.hltL3MuonsIOHit = cms.EDProducer("L3MuonProducer",
    L3TrajBuilderParameters = cms.PSet(
        GlbRefitterParameters = cms.PSet(
            CSCRecSegmentLabel = cms.InputTag("hltCscSegments"),
            Chi2CutCSC = cms.double(150.0),
            Chi2CutDT = cms.double(10.0),
            Chi2CutRPC = cms.double(1.0),
            DTRecSegmentLabel = cms.InputTag("hltDt4DSegments"),
            DYTthrs = cms.vint32(30, 15),
            DoPredictionsOnly = cms.bool(False),
            Fitter = cms.string('hltESPL3MuKFTrajectoryFitter'),
            HitThreshold = cms.int32(1),
            MuonHitsOption = cms.int32(1),
            MuonRecHitBuilder = cms.string('hltESPMuonTransientTrackingRecHitBuilder'),
            PropDirForCosmics = cms.bool(False),
            Propagator = cms.string('hltESPSmartPropagatorAny'),
            RefitDirection = cms.string('insideOut'),
            RefitRPCHits = cms.bool(True),
            SkipStation = cms.int32(-1),
            TrackerRecHitBuilder = cms.string('hltESPTTRHBWithTrackAngle'),
            TrackerSkipSection = cms.int32(-1),
            TrackerSkipSystem = cms.int32(-1)
        ),
        GlobalMuonTrackMatcher = cms.PSet(
            Chi2Cut_1 = cms.double(50.0),
            Chi2Cut_2 = cms.double(50.0),
            Chi2Cut_3 = cms.double(200.0),
            DeltaDCut_1 = cms.double(40.0),
            DeltaDCut_2 = cms.double(10.0),
            DeltaDCut_3 = cms.double(15.0),
            DeltaRCut_1 = cms.double(0.1),
            DeltaRCut_2 = cms.double(0.2),
            DeltaRCut_3 = cms.double(1.0),
            Eta_threshold = cms.double(1.2),
            LocChi2Cut = cms.double(0.001),
            MinP = cms.double(2.5),
            MinPt = cms.double(1.0),
            Propagator = cms.string('hltESPSmartPropagator'),
            Pt_threshold1 = cms.double(0.0),
            Pt_threshold2 = cms.double(999999999.0),
            Quality_1 = cms.double(20.0),
            Quality_2 = cms.double(15.0),
            Quality_3 = cms.double(7.0)
        ),
        MuonRecHitBuilder = cms.string('hltESPMuonTransientTrackingRecHitBuilder'),
        MuonTrackingRegionBuilder = cms.PSet(
            refToPSet_ = cms.string('HLTPSetMuonTrackingRegionBuilder8356')
        ),
        PCut = cms.double(2.5),
        PtCut = cms.double(1.0),
        RefitRPCHits = cms.bool(True),
        ScaleTECxFactor = cms.double(-1.0),
        ScaleTECyFactor = cms.double(-1.0),
        TrackTransformer = cms.PSet(
            DoPredictionsOnly = cms.bool(False),
            Fitter = cms.string('hltESPL3MuKFTrajectoryFitter'),
            MuonRecHitBuilder = cms.string('hltESPMuonTransientTrackingRecHitBuilder'),
            Propagator = cms.string('hltESPSmartPropagatorAny'),
            RefitDirection = cms.string('insideOut'),
            RefitRPCHits = cms.bool(True),
            Smoother = cms.string('hltESPKFTrajectorySmootherForMuonTrackLoader'),
            TrackerRecHitBuilder = cms.string('hltESPTTRHBWithTrackAngle')
        ),
        TrackerPropagator = cms.string('SteppingHelixPropagatorAny'),
        TrackerRecHitBuilder = cms.string('hltESPTTRHBWithTrackAngle'),
        tkTrajBeamSpot = cms.InputTag("hltOnlineBeamSpot"),
        tkTrajLabel = cms.InputTag("hltL3TkTracksFromL2IOHit"),
        tkTrajMaxChi2 = cms.double(9999.0),
        tkTrajMaxDXYBeamSpot = cms.double(0.2),
        tkTrajUseVertex = cms.bool(False),
        tkTrajVertex = cms.InputTag("pixelVertices")
    ),
    MuonCollectionLabel = cms.InputTag("hltL2Muons","UpdatedAtVtx"),
    ServiceParameters = cms.PSet(
        Propagators = cms.untracked.vstring('hltESPSmartPropagatorAny', 
            'SteppingHelixPropagatorAny', 
            'hltESPSmartPropagator', 
            'hltESPSteppingHelixPropagatorOpposite'),
        RPCLayers = cms.bool(True),
        UseMuonNavigation = cms.untracked.bool(True)
    ),
    TrackLoaderParameters = cms.PSet(
        DoSmoothing = cms.bool(True),
        MuonSeededTracksInstance = cms.untracked.string('L2Seeded'),
        MuonUpdatorAtVertexParameters = cms.PSet(
            BeamSpotPositionErrors = cms.vdouble(0.1, 0.1, 5.3),
            MaxChi2 = cms.double(1000000.0),
            Propagator = cms.string('hltESPSteppingHelixPropagatorOpposite')
        ),
        PutTkTrackIntoEvent = cms.untracked.bool(False),
        SmoothTkTrack = cms.untracked.bool(False),
        Smoother = cms.string('hltESPKFTrajectorySmootherForMuonTrackLoader'),
        TTRHBuilder = cms.string('hltESPTTRHBWithTrackAngle'),
        VertexConstraint = cms.bool(False),
        beamSpot = cms.InputTag("hltOnlineBeamSpot")
    )
)


process.hltL3MuonsLinksCombination = cms.EDProducer("L3TrackLinksCombiner",
    labels = cms.VInputTag("hltL3MuonsOIState", "hltL3MuonsOIHit", "hltL3MuonsIOHit")
)


process.hltL3MuonsOIHit = cms.EDProducer("L3MuonProducer",
    L3TrajBuilderParameters = cms.PSet(
        GlbRefitterParameters = cms.PSet(
            CSCRecSegmentLabel = cms.InputTag("hltCscSegments"),
            Chi2CutCSC = cms.double(150.0),
            Chi2CutDT = cms.double(10.0),
            Chi2CutRPC = cms.double(1.0),
            DTRecSegmentLabel = cms.InputTag("hltDt4DSegments"),
            DYTthrs = cms.vint32(30, 15),
            DoPredictionsOnly = cms.bool(False),
            Fitter = cms.string('hltESPL3MuKFTrajectoryFitter'),
            HitThreshold = cms.int32(1),
            MuonHitsOption = cms.int32(1),
            MuonRecHitBuilder = cms.string('hltESPMuonTransientTrackingRecHitBuilder'),
            PropDirForCosmics = cms.bool(False),
            Propagator = cms.string('hltESPSmartPropagatorAny'),
            RefitDirection = cms.string('insideOut'),
            RefitRPCHits = cms.bool(True),
            SkipStation = cms.int32(-1),
            TrackerRecHitBuilder = cms.string('hltESPTTRHBWithTrackAngle'),
            TrackerSkipSection = cms.int32(-1),
            TrackerSkipSystem = cms.int32(-1)
        ),
        GlobalMuonTrackMatcher = cms.PSet(
            Chi2Cut_1 = cms.double(50.0),
            Chi2Cut_2 = cms.double(50.0),
            Chi2Cut_3 = cms.double(200.0),
            DeltaDCut_1 = cms.double(40.0),
            DeltaDCut_2 = cms.double(10.0),
            DeltaDCut_3 = cms.double(15.0),
            DeltaRCut_1 = cms.double(0.1),
            DeltaRCut_2 = cms.double(0.2),
            DeltaRCut_3 = cms.double(1.0),
            Eta_threshold = cms.double(1.2),
            LocChi2Cut = cms.double(0.001),
            MinP = cms.double(2.5),
            MinPt = cms.double(1.0),
            Propagator = cms.string('hltESPSmartPropagator'),
            Pt_threshold1 = cms.double(0.0),
            Pt_threshold2 = cms.double(999999999.0),
            Quality_1 = cms.double(20.0),
            Quality_2 = cms.double(15.0),
            Quality_3 = cms.double(7.0)
        ),
        MuonRecHitBuilder = cms.string('hltESPMuonTransientTrackingRecHitBuilder'),
        MuonTrackingRegionBuilder = cms.PSet(
            refToPSet_ = cms.string('HLTPSetMuonTrackingRegionBuilder8356')
        ),
        PCut = cms.double(2.5),
        PtCut = cms.double(1.0),
        RefitRPCHits = cms.bool(True),
        ScaleTECxFactor = cms.double(-1.0),
        ScaleTECyFactor = cms.double(-1.0),
        TrackTransformer = cms.PSet(
            DoPredictionsOnly = cms.bool(False),
            Fitter = cms.string('hltESPL3MuKFTrajectoryFitter'),
            MuonRecHitBuilder = cms.string('hltESPMuonTransientTrackingRecHitBuilder'),
            Propagator = cms.string('hltESPSmartPropagatorAny'),
            RefitDirection = cms.string('insideOut'),
            RefitRPCHits = cms.bool(True),
            Smoother = cms.string('hltESPKFTrajectorySmootherForMuonTrackLoader'),
            TrackerRecHitBuilder = cms.string('hltESPTTRHBWithTrackAngle')
        ),
        TrackerPropagator = cms.string('SteppingHelixPropagatorAny'),
        TrackerRecHitBuilder = cms.string('hltESPTTRHBWithTrackAngle'),
        tkTrajBeamSpot = cms.InputTag("hltOnlineBeamSpot"),
        tkTrajLabel = cms.InputTag("hltL3TkTracksFromL2OIHit"),
        tkTrajMaxChi2 = cms.double(9999.0),
        tkTrajMaxDXYBeamSpot = cms.double(0.2),
        tkTrajUseVertex = cms.bool(False),
        tkTrajVertex = cms.InputTag("pixelVertices")
    ),
    MuonCollectionLabel = cms.InputTag("hltL2Muons","UpdatedAtVtx"),
    ServiceParameters = cms.PSet(
        Propagators = cms.untracked.vstring('hltESPSmartPropagatorAny', 
            'SteppingHelixPropagatorAny', 
            'hltESPSmartPropagator', 
            'hltESPSteppingHelixPropagatorOpposite'),
        RPCLayers = cms.bool(True),
        UseMuonNavigation = cms.untracked.bool(True)
    ),
    TrackLoaderParameters = cms.PSet(
        DoSmoothing = cms.bool(True),
        MuonSeededTracksInstance = cms.untracked.string('L2Seeded'),
        MuonUpdatorAtVertexParameters = cms.PSet(
            BeamSpotPositionErrors = cms.vdouble(0.1, 0.1, 5.3),
            MaxChi2 = cms.double(1000000.0),
            Propagator = cms.string('hltESPSteppingHelixPropagatorOpposite')
        ),
        PutTkTrackIntoEvent = cms.untracked.bool(False),
        SmoothTkTrack = cms.untracked.bool(False),
        Smoother = cms.string('hltESPKFTrajectorySmootherForMuonTrackLoader'),
        TTRHBuilder = cms.string('hltESPTTRHBWithTrackAngle'),
        VertexConstraint = cms.bool(False),
        beamSpot = cms.InputTag("hltOnlineBeamSpot")
    )
)


process.hltL3MuonsOIState = cms.EDProducer("L3MuonProducer",
    L3TrajBuilderParameters = cms.PSet(
        GlbRefitterParameters = cms.PSet(
            CSCRecSegmentLabel = cms.InputTag("hltCscSegments"),
            Chi2CutCSC = cms.double(150.0),
            Chi2CutDT = cms.double(10.0),
            Chi2CutRPC = cms.double(1.0),
            DTRecSegmentLabel = cms.InputTag("hltDt4DSegments"),
            DYTthrs = cms.vint32(30, 15),
            DoPredictionsOnly = cms.bool(False),
            Fitter = cms.string('hltESPL3MuKFTrajectoryFitter'),
            HitThreshold = cms.int32(1),
            MuonHitsOption = cms.int32(1),
            MuonRecHitBuilder = cms.string('hltESPMuonTransientTrackingRecHitBuilder'),
            PropDirForCosmics = cms.bool(False),
            Propagator = cms.string('hltESPSmartPropagatorAny'),
            RefitDirection = cms.string('insideOut'),
            RefitRPCHits = cms.bool(True),
            SkipStation = cms.int32(-1),
            TrackerRecHitBuilder = cms.string('hltESPTTRHBWithTrackAngle'),
            TrackerSkipSection = cms.int32(-1),
            TrackerSkipSystem = cms.int32(-1)
        ),
        GlobalMuonTrackMatcher = cms.PSet(
            Chi2Cut_1 = cms.double(50.0),
            Chi2Cut_2 = cms.double(50.0),
            Chi2Cut_3 = cms.double(200.0),
            DeltaDCut_1 = cms.double(40.0),
            DeltaDCut_2 = cms.double(10.0),
            DeltaDCut_3 = cms.double(15.0),
            DeltaRCut_1 = cms.double(0.1),
            DeltaRCut_2 = cms.double(0.2),
            DeltaRCut_3 = cms.double(1.0),
            Eta_threshold = cms.double(1.2),
            LocChi2Cut = cms.double(0.001),
            MinP = cms.double(2.5),
            MinPt = cms.double(1.0),
            Propagator = cms.string('hltESPSmartPropagator'),
            Pt_threshold1 = cms.double(0.0),
            Pt_threshold2 = cms.double(999999999.0),
            Quality_1 = cms.double(20.0),
            Quality_2 = cms.double(15.0),
            Quality_3 = cms.double(7.0)
        ),
        MuonRecHitBuilder = cms.string('hltESPMuonTransientTrackingRecHitBuilder'),
        MuonTrackingRegionBuilder = cms.PSet(
            refToPSet_ = cms.string('HLTPSetMuonTrackingRegionBuilder8356')
        ),
        PCut = cms.double(2.5),
        PtCut = cms.double(1.0),
        RefitRPCHits = cms.bool(True),
        ScaleTECxFactor = cms.double(-1.0),
        ScaleTECyFactor = cms.double(-1.0),
        TrackTransformer = cms.PSet(
            DoPredictionsOnly = cms.bool(False),
            Fitter = cms.string('hltESPL3MuKFTrajectoryFitter'),
            MuonRecHitBuilder = cms.string('hltESPMuonTransientTrackingRecHitBuilder'),
            Propagator = cms.string('hltESPSmartPropagatorAny'),
            RefitDirection = cms.string('insideOut'),
            RefitRPCHits = cms.bool(True),
            Smoother = cms.string('hltESPKFTrajectorySmootherForMuonTrackLoader'),
            TrackerRecHitBuilder = cms.string('hltESPTTRHBWithTrackAngle')
        ),
        TrackerPropagator = cms.string('SteppingHelixPropagatorAny'),
        TrackerRecHitBuilder = cms.string('hltESPTTRHBWithTrackAngle'),
        tkTrajBeamSpot = cms.InputTag("hltOnlineBeamSpot"),
        tkTrajLabel = cms.InputTag("hltL3TkTracksFromL2OIState"),
        tkTrajMaxChi2 = cms.double(9999.0),
        tkTrajMaxDXYBeamSpot = cms.double(0.2),
        tkTrajUseVertex = cms.bool(False),
        tkTrajVertex = cms.InputTag("pixelVertices")
    ),
    MuonCollectionLabel = cms.InputTag("hltL2Muons","UpdatedAtVtx"),
    ServiceParameters = cms.PSet(
        Propagators = cms.untracked.vstring('hltESPSmartPropagatorAny', 
            'SteppingHelixPropagatorAny', 
            'hltESPSmartPropagator', 
            'hltESPSteppingHelixPropagatorOpposite'),
        RPCLayers = cms.bool(True),
        UseMuonNavigation = cms.untracked.bool(True)
    ),
    TrackLoaderParameters = cms.PSet(
        DoSmoothing = cms.bool(True),
        MuonSeededTracksInstance = cms.untracked.string('L2Seeded'),
        MuonUpdatorAtVertexParameters = cms.PSet(
            BeamSpotPositionErrors = cms.vdouble(0.1, 0.1, 5.3),
            MaxChi2 = cms.double(1000000.0),
            Propagator = cms.string('hltESPSteppingHelixPropagatorOpposite')
        ),
        PutTkTrackIntoEvent = cms.untracked.bool(False),
        SmoothTkTrack = cms.untracked.bool(False),
        Smoother = cms.string('hltESPKFTrajectorySmootherForMuonTrackLoader'),
        TTRHBuilder = cms.string('hltESPTTRHBWithTrackAngle'),
        VertexConstraint = cms.bool(False),
        beamSpot = cms.InputTag("hltOnlineBeamSpot")
    )
)


process.hltL3TkFromL2OICombination = cms.EDProducer("L3TrackCombiner",
    labels = cms.VInputTag("hltL3MuonsOIState", "hltL3MuonsOIHit")
)


process.hltL3TkTracksFromL2 = cms.EDProducer("TrackListMerger",
    Epsilon = cms.double(-0.001),
    FoundHitBonus = cms.double(100.0),
    LostHitPenalty = cms.double(0.0),
    MaxNormalizedChisq = cms.double(1000.0),
    MinFound = cms.int32(3),
    MinPT = cms.double(0.05),
    ShareFrac = cms.double(0.19),
    TrackProducers = cms.VInputTag("hltL3TkTracksMergeStep1", "hltL3TkTracksFromL2IOHit"),
    allowFirstHitShare = cms.bool(True),
    copyExtras = cms.untracked.bool(True),
    copyMVA = cms.bool(False),
    hasSelector = cms.vint32(0, 0),
    indivShareFrac = cms.vdouble(1.0, 1.0),
    newQuality = cms.string('confirmed'),
    selectedTrackQuals = cms.VInputTag("hltL3TkTracksMergeStep1", "hltL3TkTracksFromL2IOHit"),
    setsToMerge = cms.VPSet(cms.PSet(
        pQual = cms.bool(False),
        tLists = cms.vint32(0, 1)
    )),
    writeOnlyTrkQuals = cms.bool(False)
)


process.hltL3TkTracksFromL2IOHit = cms.EDProducer("TrackProducer",
    AlgorithmName = cms.string('hltIterX'),
    Fitter = cms.string('hltESPKFFittingSmoother'),
    GeometricInnerState = cms.bool(True),
    MeasurementTracker = cms.string(''),
    MeasurementTrackerEvent = cms.InputTag("hltSiStripClusters"),
    NavigationSchool = cms.string(''),
    Propagator = cms.string('PropagatorWithMaterial'),
    SimpleMagneticField = cms.string(''),
    TTRHBuilder = cms.string('hltESPTTRHBWithTrackAngle'),
    TrajectoryInEvent = cms.bool(True),
    alias = cms.untracked.string(''),
    beamSpot = cms.InputTag("hltOnlineBeamSpot"),
    clusterRemovalInfo = cms.InputTag(""),
    src = cms.InputTag("hltL3TrackCandidateFromL2IOHit"),
    useHitsSplitting = cms.bool(False),
    useSimpleMF = cms.bool(False)
)


process.hltL3TkTracksFromL2OIHit = cms.EDProducer("TrackProducer",
    AlgorithmName = cms.string('hltIterX'),
    Fitter = cms.string('hltESPKFFittingSmoother'),
    GeometricInnerState = cms.bool(True),
    MeasurementTracker = cms.string(''),
    MeasurementTrackerEvent = cms.InputTag("hltSiStripClusters"),
    NavigationSchool = cms.string(''),
    Propagator = cms.string('PropagatorWithMaterial'),
    SimpleMagneticField = cms.string(''),
    TTRHBuilder = cms.string('hltESPTTRHBWithTrackAngle'),
    TrajectoryInEvent = cms.bool(True),
    alias = cms.untracked.string(''),
    beamSpot = cms.InputTag("hltOnlineBeamSpot"),
    clusterRemovalInfo = cms.InputTag(""),
    src = cms.InputTag("hltL3TrackCandidateFromL2OIHit"),
    useHitsSplitting = cms.bool(False),
    useSimpleMF = cms.bool(False)
)


process.hltL3TkTracksFromL2OIState = cms.EDProducer("TrackProducer",
    AlgorithmName = cms.string('hltIterX'),
    Fitter = cms.string('hltESPKFFittingSmoother'),
    GeometricInnerState = cms.bool(True),
    MeasurementTracker = cms.string(''),
    MeasurementTrackerEvent = cms.InputTag("hltSiStripClusters"),
    NavigationSchool = cms.string(''),
    Propagator = cms.string('PropagatorWithMaterial'),
    SimpleMagneticField = cms.string(''),
    TTRHBuilder = cms.string('hltESPTTRHBWithTrackAngle'),
    TrajectoryInEvent = cms.bool(True),
    alias = cms.untracked.string(''),
    beamSpot = cms.InputTag("hltOnlineBeamSpot"),
    clusterRemovalInfo = cms.InputTag(""),
    src = cms.InputTag("hltL3TrackCandidateFromL2OIState"),
    useHitsSplitting = cms.bool(False),
    useSimpleMF = cms.bool(False)
)


process.hltL3TkTracksMergeStep1 = cms.EDProducer("TrackListMerger",
    Epsilon = cms.double(-0.001),
    FoundHitBonus = cms.double(100.0),
    LostHitPenalty = cms.double(0.0),
    MaxNormalizedChisq = cms.double(1000.0),
    MinFound = cms.int32(3),
    MinPT = cms.double(0.05),
    ShareFrac = cms.double(0.19),
    TrackProducers = cms.VInputTag("hltL3TkTracksFromL2OIState", "hltL3TkTracksFromL2OIHit"),
    allowFirstHitShare = cms.bool(True),
    copyExtras = cms.untracked.bool(True),
    copyMVA = cms.bool(False),
    hasSelector = cms.vint32(0, 0),
    indivShareFrac = cms.vdouble(1.0, 1.0),
    newQuality = cms.string('confirmed'),
    selectedTrackQuals = cms.VInputTag("hltL3TkTracksFromL2OIState", "hltL3TkTracksFromL2OIHit"),
    setsToMerge = cms.VPSet(cms.PSet(
        pQual = cms.bool(False),
        tLists = cms.vint32(0, 1)
    )),
    writeOnlyTrkQuals = cms.bool(False)
)


process.hltL3TrackCandidateFromL2 = cms.EDProducer("L3TrackCandCombiner",
    labels = cms.VInputTag("hltL3TrackCandidateFromL2IOHit", "hltL3TrackCandidateFromL2OIHit", "hltL3TrackCandidateFromL2OIState")
)


process.hltL3TrackCandidateFromL2IOHit = cms.EDProducer("CkfTrajectoryMaker",
    MeasurementTrackerEvent = cms.InputTag("hltSiStripClusters"),
    NavigationSchool = cms.string('SimpleNavigationSchool'),
    RedundantSeedCleaner = cms.string('CachingSeedCleanerBySharedInput'),
    TrajectoryBuilder = cms.string(''),
    TrajectoryBuilderPSet = cms.PSet(
        refToPSet_ = cms.string('HLTPSetMuonCkfTrajectoryBuilder')
    ),
    TrajectoryCleaner = cms.string('hltESPTrajectoryCleanerBySharedHits'),
    TransientInitialStateEstimatorParameters = cms.PSet(
        numberMeasurementsForFit = cms.int32(4),
        propagatorAlongTISE = cms.string('PropagatorWithMaterial'),
        propagatorOppositeTISE = cms.string('PropagatorWithMaterialOpposite')
    ),
    cleanTrajectoryAfterInOut = cms.bool(False),
    doSeedingRegionRebuilding = cms.bool(False),
    maxNSeeds = cms.uint32(100000),
    reverseTrajectories = cms.bool(False),
    src = cms.InputTag("hltL3TrajSeedIOHit"),
    trackCandidateAlso = cms.bool(True),
    useHitsSplitting = cms.bool(False)
)


process.hltL3TrackCandidateFromL2OIHit = cms.EDProducer("CkfTrajectoryMaker",
    MeasurementTrackerEvent = cms.InputTag("hltSiStripClusters"),
    NavigationSchool = cms.string('SimpleNavigationSchool'),
    RedundantSeedCleaner = cms.string('CachingSeedCleanerBySharedInput'),
    TrajectoryBuilder = cms.string(''),
    TrajectoryBuilderPSet = cms.PSet(
        refToPSet_ = cms.string('HLTPSetMuonCkfTrajectoryBuilder')
    ),
    TrajectoryCleaner = cms.string('hltESPTrajectoryCleanerBySharedHits'),
    TransientInitialStateEstimatorParameters = cms.PSet(
        numberMeasurementsForFit = cms.int32(4),
        propagatorAlongTISE = cms.string('PropagatorWithMaterial'),
        propagatorOppositeTISE = cms.string('PropagatorWithMaterialOpposite')
    ),
    cleanTrajectoryAfterInOut = cms.bool(False),
    doSeedingRegionRebuilding = cms.bool(False),
    maxNSeeds = cms.uint32(100000),
    reverseTrajectories = cms.bool(True),
    src = cms.InputTag("hltL3TrajSeedOIHit"),
    trackCandidateAlso = cms.bool(True),
    useHitsSplitting = cms.bool(False)
)


process.hltL3TrackCandidateFromL2OIState = cms.EDProducer("CkfTrajectoryMaker",
    MeasurementTrackerEvent = cms.InputTag("hltSiStripClusters"),
    NavigationSchool = cms.string('SimpleNavigationSchool'),
    RedundantSeedCleaner = cms.string('CachingSeedCleanerBySharedInput'),
    TrajectoryBuilder = cms.string(''),
    TrajectoryBuilderPSet = cms.PSet(
        refToPSet_ = cms.string('HLTPSetMuonCkfTrajectoryBuilder')
    ),
    TrajectoryCleaner = cms.string('hltESPTrajectoryCleanerBySharedHits'),
    TransientInitialStateEstimatorParameters = cms.PSet(
        numberMeasurementsForFit = cms.int32(4),
        propagatorAlongTISE = cms.string('PropagatorWithMaterial'),
        propagatorOppositeTISE = cms.string('PropagatorWithMaterialOpposite')
    ),
    cleanTrajectoryAfterInOut = cms.bool(False),
    doSeedingRegionRebuilding = cms.bool(False),
    maxNSeeds = cms.uint32(100000),
    reverseTrajectories = cms.bool(True),
    src = cms.InputTag("hltL3TrajSeedOIState"),
    trackCandidateAlso = cms.bool(True),
    useHitsSplitting = cms.bool(False)
)


process.hltL3TrajSeedIOHit = cms.EDProducer("TSGFromL2Muon",
    MuonCollectionLabel = cms.InputTag("hltL2Muons","UpdatedAtVtx"),
    MuonTrackingRegionBuilder = cms.PSet(
        refToPSet_ = cms.string('HLTPSetMuonTrackingRegionBuilder8356')
    ),
    PCut = cms.double(2.5),
    PtCut = cms.double(1.0),
    ServiceParameters = cms.PSet(
        Propagators = cms.untracked.vstring('PropagatorWithMaterial'),
        RPCLayers = cms.bool(True),
        UseMuonNavigation = cms.untracked.bool(True)
    ),
    TkSeedGenerator = cms.PSet(
        ComponentName = cms.string('DualByL2TSG'),
        L3TkCollectionA = cms.InputTag("hltL3TkFromL2OICombination"),
        PSetNames = cms.vstring('skipTSG', 
            'iterativeTSG'),
        iterativeTSG = cms.PSet(
            ComponentName = cms.string('CombinedTSG'),
            PSetNames = cms.vstring('firstTSG', 
                'secondTSG'),
            firstTSG = cms.PSet(
                ComponentName = cms.string('TSGFromOrderedHits'),
                OrderedHitsFactoryPSet = cms.PSet(
                    ComponentName = cms.string('StandardHitTripletGenerator'),
                    GeneratorPSet = cms.PSet(
                        ComponentName = cms.string('PixelTripletHLTGenerator'),
                        SeedComparitorPSet = cms.PSet(
                            ComponentName = cms.string('none')
                        ),
                        extraHitRPhitolerance = cms.double(0.06),
                        extraHitRZtolerance = cms.double(0.06),
                        maxElement = cms.uint32(0),
                        phiPreFiltering = cms.double(0.3),
                        useBending = cms.bool(True),
                        useFixedPreFiltering = cms.bool(False),
                        useMultScattering = cms.bool(True)
                    ),
                    SeedingLayers = cms.InputTag("hltPixelLayerTriplets")
                ),
                SeedCreatorPSet = cms.PSet(
                    refToPSet_ = cms.string('HLTSeedFromConsecutiveHitsCreator')
                ),
                TTRHBuilder = cms.string('hltESPTTRHBWithTrackAngle')
            ),
            secondTSG = cms.PSet(
                ComponentName = cms.string('TSGFromOrderedHits'),
                OrderedHitsFactoryPSet = cms.PSet(
                    ComponentName = cms.string('StandardHitPairGenerator'),
                    SeedingLayers = cms.InputTag("hltPixelLayerPairs"),
                    maxElement = cms.uint32(0),
                    useOnDemandTracker = cms.untracked.int32(0)
                ),
                SeedCreatorPSet = cms.PSet(
                    refToPSet_ = cms.string('HLTSeedFromConsecutiveHitsCreator')
                ),
                TTRHBuilder = cms.string('hltESPTTRHBWithTrackAngle')
            ),
            thirdTSG = cms.PSet(
                ComponentName = cms.string('DualByEtaTSG'),
                PSetNames = cms.vstring('endcapTSG', 
                    'barrelTSG'),
                SeedCreatorPSet = cms.PSet(
                    refToPSet_ = cms.string('HLTSeedFromConsecutiveHitsCreator')
                ),
                barrelTSG = cms.PSet(

                ),
                endcapTSG = cms.PSet(
                    ComponentName = cms.string('TSGFromOrderedHits'),
                    OrderedHitsFactoryPSet = cms.PSet(
                        ComponentName = cms.string('StandardHitPairGenerator'),
                        SeedingLayers = cms.InputTag("hltMixedLayerPairs"),
                        maxElement = cms.uint32(0),
                        useOnDemandTracker = cms.untracked.int32(0)
                    ),
                    TTRHBuilder = cms.string('hltESPTTRHBWithTrackAngle')
                ),
                etaSeparation = cms.double(2.0)
            )
        ),
        skipTSG = cms.PSet(

        )
    ),
    TrackerSeedCleaner = cms.PSet(
        TTRHBuilder = cms.string('hltESPTTRHBWithTrackAngle'),
        beamSpot = cms.InputTag("hltOnlineBeamSpot"),
        cleanerFromSharedHits = cms.bool(True),
        directionCleaner = cms.bool(True),
        ptCleaner = cms.bool(True)
    )
)


process.hltL3TrajSeedOIHit = cms.EDProducer("TSGFromL2Muon",
    MuonCollectionLabel = cms.InputTag("hltL2Muons","UpdatedAtVtx"),
    MuonTrackingRegionBuilder = cms.PSet(

    ),
    PCut = cms.double(2.5),
    PtCut = cms.double(1.0),
    ServiceParameters = cms.PSet(
        Propagators = cms.untracked.vstring('PropagatorWithMaterial', 
            'hltESPSmartPropagatorAnyOpposite'),
        RPCLayers = cms.bool(True),
        UseMuonNavigation = cms.untracked.bool(True)
    ),
    TkSeedGenerator = cms.PSet(
        ComponentName = cms.string('DualByL2TSG'),
        L3TkCollectionA = cms.InputTag("hltL3MuonsOIState"),
        PSetNames = cms.vstring('skipTSG', 
            'iterativeTSG'),
        iterativeTSG = cms.PSet(
            ComponentName = cms.string('TSGFromPropagation'),
            ErrorRescaling = cms.double(3.0),
            MaxChi2 = cms.double(40.0),
            MeasurementTrackerEvent = cms.InputTag("hltSiStripClusters"),
            MeasurementTrackerName = cms.string('hltESPMeasurementTracker'),
            Propagator = cms.string('hltESPSmartPropagatorAnyOpposite'),
            ResetMethod = cms.string('matrix'),
            SelectState = cms.bool(False),
            SigmaZ = cms.double(25.0),
            UpdateState = cms.bool(True),
            UseVertexState = cms.bool(True),
            beamSpot = cms.InputTag("unused"),
            errorMatrixPset = cms.PSet(
                action = cms.string('use'),
                atIP = cms.bool(True),
                errorMatrixValuesPSet = cms.PSet(
                    pf3_V11 = cms.PSet(
                        action = cms.string('scale'),
                        values = cms.vdouble(3.0, 3.0, 3.0, 5.0, 4.0, 
                            5.0, 10.0, 7.0, 10.0, 10.0, 
                            10.0, 10.0)
                    ),
                    pf3_V12 = cms.PSet(
                        action = cms.string('scale'),
                        values = cms.vdouble(1.0, 1.0, 1.0, 1.0, 1.0, 
                            1.0, 1.0, 1.0, 1.0, 1.0, 
                            1.0, 1.0)
                    ),
                    pf3_V13 = cms.PSet(
                        action = cms.string('scale'),
                        values = cms.vdouble(1.0, 1.0, 1.0, 1.0, 1.0, 
                            1.0, 1.0, 1.0, 1.0, 1.0, 
                            1.0, 1.0)
                    ),
                    pf3_V14 = cms.PSet(
                        action = cms.string('scale'),
                        values = cms.vdouble(1.0, 1.0, 1.0, 1.0, 1.0, 
                            1.0, 1.0, 1.0, 1.0, 1.0, 
                            1.0, 1.0)
                    ),
                    pf3_V15 = cms.PSet(
                        action = cms.string('scale'),
                        values = cms.vdouble(1.0, 1.0, 1.0, 1.0, 1.0, 
                            1.0, 1.0, 1.0, 1.0, 1.0, 
                            1.0, 1.0)
                    ),
                    pf3_V22 = cms.PSet(
                        action = cms.string('scale'),
                        values = cms.vdouble(3.0, 3.0, 3.0, 5.0, 4.0, 
                            5.0, 10.0, 7.0, 10.0, 10.0, 
                            10.0, 10.0)
                    ),
                    pf3_V23 = cms.PSet(
                        action = cms.string('scale'),
                        values = cms.vdouble(1.0, 1.0, 1.0, 1.0, 1.0, 
                            1.0, 1.0, 1.0, 1.0, 1.0, 
                            1.0, 1.0)
                    ),
                    pf3_V24 = cms.PSet(
                        action = cms.string('scale'),
                        values = cms.vdouble(1.0, 1.0, 1.0, 1.0, 1.0, 
                            1.0, 1.0, 1.0, 1.0, 1.0, 
                            1.0, 1.0)
                    ),
                    pf3_V25 = cms.PSet(
                        action = cms.string('scale'),
                        values = cms.vdouble(1.0, 1.0, 1.0, 1.0, 1.0, 
                            1.0, 1.0, 1.0, 1.0, 1.0, 
                            1.0, 1.0)
                    ),
                    pf3_V33 = cms.PSet(
                        action = cms.string('scale'),
                        values = cms.vdouble(3.0, 3.0, 3.0, 5.0, 4.0, 
                            5.0, 10.0, 7.0, 10.0, 10.0, 
                            10.0, 10.0)
                    ),
                    pf3_V34 = cms.PSet(
                        action = cms.string('scale'),
                        values = cms.vdouble(1.0, 1.0, 1.0, 1.0, 1.0, 
                            1.0, 1.0, 1.0, 1.0, 1.0, 
                            1.0, 1.0)
                    ),
                    pf3_V35 = cms.PSet(
                        action = cms.string('scale'),
                        values = cms.vdouble(1.0, 1.0, 1.0, 1.0, 1.0, 
                            1.0, 1.0, 1.0, 1.0, 1.0, 
                            1.0, 1.0)
                    ),
                    pf3_V44 = cms.PSet(
                        action = cms.string('scale'),
                        values = cms.vdouble(3.0, 3.0, 3.0, 5.0, 4.0, 
                            5.0, 10.0, 7.0, 10.0, 10.0, 
                            10.0, 10.0)
                    ),
                    pf3_V45 = cms.PSet(
                        action = cms.string('scale'),
                        values = cms.vdouble(1.0, 1.0, 1.0, 1.0, 1.0, 
                            1.0, 1.0, 1.0, 1.0, 1.0, 
                            1.0, 1.0)
                    ),
                    pf3_V55 = cms.PSet(
                        action = cms.string('scale'),
                        values = cms.vdouble(3.0, 3.0, 3.0, 5.0, 4.0, 
                            5.0, 10.0, 7.0, 10.0, 10.0, 
                            10.0, 10.0)
                    ),
                    xAxis = cms.vdouble(0.0, 13.0, 30.0, 70.0, 1000.0),
                    yAxis = cms.vdouble(0.0, 1.0, 1.4, 10.0),
                    zAxis = cms.vdouble(-3.14159, 3.14159)
                )
            )
        ),
        skipTSG = cms.PSet(

        )
    ),
    TrackerSeedCleaner = cms.PSet(
        TTRHBuilder = cms.string('hltESPTTRHBWithTrackAngle'),
        beamSpot = cms.InputTag("hltOnlineBeamSpot"),
        cleanerFromSharedHits = cms.bool(True),
        directionCleaner = cms.bool(True),
        ptCleaner = cms.bool(True)
    )
)


process.hltL3TrajSeedOIState = cms.EDProducer("TSGFromL2Muon",
    MuonCollectionLabel = cms.InputTag("hltL2Muons","UpdatedAtVtx"),
    MuonTrackingRegionBuilder = cms.PSet(

    ),
    PCut = cms.double(2.5),
    PtCut = cms.double(1.0),
    ServiceParameters = cms.PSet(
        Propagators = cms.untracked.vstring('hltESPSteppingHelixPropagatorOpposite', 
            'hltESPSteppingHelixPropagatorAlong'),
        RPCLayers = cms.bool(True),
        UseMuonNavigation = cms.untracked.bool(True)
    ),
    TkSeedGenerator = cms.PSet(
        ComponentName = cms.string('TSGForRoadSearch'),
        MeasurementTrackerEvent = cms.InputTag("hltSiStripClusters"),
        copyMuonRecHit = cms.bool(False),
        errorMatrixPset = cms.PSet(
            action = cms.string('use'),
            atIP = cms.bool(True),
            errorMatrixValuesPSet = cms.PSet(
                pf3_V11 = cms.PSet(
                    action = cms.string('scale'),
                    values = cms.vdouble(3.0, 3.0, 3.0, 5.0, 4.0, 
                        5.0, 10.0, 7.0, 10.0, 10.0, 
                        10.0, 10.0)
                ),
                pf3_V12 = cms.PSet(
                    action = cms.string('scale'),
                    values = cms.vdouble(1.0, 1.0, 1.0, 1.0, 1.0, 
                        1.0, 1.0, 1.0, 1.0, 1.0, 
                        1.0, 1.0)
                ),
                pf3_V13 = cms.PSet(
                    action = cms.string('scale'),
                    values = cms.vdouble(1.0, 1.0, 1.0, 1.0, 1.0, 
                        1.0, 1.0, 1.0, 1.0, 1.0, 
                        1.0, 1.0)
                ),
                pf3_V14 = cms.PSet(
                    action = cms.string('scale'),
                    values = cms.vdouble(1.0, 1.0, 1.0, 1.0, 1.0, 
                        1.0, 1.0, 1.0, 1.0, 1.0, 
                        1.0, 1.0)
                ),
                pf3_V15 = cms.PSet(
                    action = cms.string('scale'),
                    values = cms.vdouble(1.0, 1.0, 1.0, 1.0, 1.0, 
                        1.0, 1.0, 1.0, 1.0, 1.0, 
                        1.0, 1.0)
                ),
                pf3_V22 = cms.PSet(
                    action = cms.string('scale'),
                    values = cms.vdouble(3.0, 3.0, 3.0, 5.0, 4.0, 
                        5.0, 10.0, 7.0, 10.0, 10.0, 
                        10.0, 10.0)
                ),
                pf3_V23 = cms.PSet(
                    action = cms.string('scale'),
                    values = cms.vdouble(1.0, 1.0, 1.0, 1.0, 1.0, 
                        1.0, 1.0, 1.0, 1.0, 1.0, 
                        1.0, 1.0)
                ),
                pf3_V24 = cms.PSet(
                    action = cms.string('scale'),
                    values = cms.vdouble(1.0, 1.0, 1.0, 1.0, 1.0, 
                        1.0, 1.0, 1.0, 1.0, 1.0, 
                        1.0, 1.0)
                ),
                pf3_V25 = cms.PSet(
                    action = cms.string('scale'),
                    values = cms.vdouble(1.0, 1.0, 1.0, 1.0, 1.0, 
                        1.0, 1.0, 1.0, 1.0, 1.0, 
                        1.0, 1.0)
                ),
                pf3_V33 = cms.PSet(
                    action = cms.string('scale'),
                    values = cms.vdouble(3.0, 3.0, 3.0, 5.0, 4.0, 
                        5.0, 10.0, 7.0, 10.0, 10.0, 
                        10.0, 10.0)
                ),
                pf3_V34 = cms.PSet(
                    action = cms.string('scale'),
                    values = cms.vdouble(1.0, 1.0, 1.0, 1.0, 1.0, 
                        1.0, 1.0, 1.0, 1.0, 1.0, 
                        1.0, 1.0)
                ),
                pf3_V35 = cms.PSet(
                    action = cms.string('scale'),
                    values = cms.vdouble(1.0, 1.0, 1.0, 1.0, 1.0, 
                        1.0, 1.0, 1.0, 1.0, 1.0, 
                        1.0, 1.0)
                ),
                pf3_V44 = cms.PSet(
                    action = cms.string('scale'),
                    values = cms.vdouble(3.0, 3.0, 3.0, 5.0, 4.0, 
                        5.0, 10.0, 7.0, 10.0, 10.0, 
                        10.0, 10.0)
                ),
                pf3_V45 = cms.PSet(
                    action = cms.string('scale'),
                    values = cms.vdouble(1.0, 1.0, 1.0, 1.0, 1.0, 
                        1.0, 1.0, 1.0, 1.0, 1.0, 
                        1.0, 1.0)
                ),
                pf3_V55 = cms.PSet(
                    action = cms.string('scale'),
                    values = cms.vdouble(3.0, 3.0, 3.0, 5.0, 4.0, 
                        5.0, 10.0, 7.0, 10.0, 10.0, 
                        10.0, 10.0)
                ),
                xAxis = cms.vdouble(0.0, 13.0, 30.0, 70.0, 1000.0),
                yAxis = cms.vdouble(0.0, 1.0, 1.4, 10.0),
                zAxis = cms.vdouble(-3.14159, 3.14159)
            )
        ),
        manySeeds = cms.bool(False),
        maxChi2 = cms.double(40.0),
        option = cms.uint32(3),
        propagatorCompatibleName = cms.string('hltESPSteppingHelixPropagatorOpposite'),
        propagatorName = cms.string('hltESPSteppingHelixPropagatorAlong')
    ),
    TrackerSeedCleaner = cms.PSet(

    )
)


process.hltL3TrajectorySeed = cms.EDProducer("L3MuonTrajectorySeedCombiner",
    labels = cms.VInputTag("hltL3TrajSeedIOHit", "hltL3TrajSeedOIState", "hltL3TrajSeedOIHit")
)


process.hltLightPFTracks = cms.EDProducer("LightPFTrackProducer",
    TkColList = cms.VInputTag("hltPFMuonMerging"),
    TrackQuality = cms.string('none'),
    UseQuality = cms.bool(False)
)


process.hltMixedLayerPairs = cms.EDProducer("SeedingLayersEDProducer",
    BPix = cms.PSet(
        HitProducer = cms.string('hltSiPixelRecHits'),
        TTRHBuilder = cms.string('hltESPTTRHBuilderPixelOnly'),
        hitErrorRPhi = cms.double(0.0027),
        hitErrorRZ = cms.double(0.006),
        useErrorsFromParam = cms.bool(True)
    ),
    FPix = cms.PSet(
        HitProducer = cms.string('hltSiPixelRecHits'),
        TTRHBuilder = cms.string('hltESPTTRHBuilderPixelOnly'),
        hitErrorRPhi = cms.double(0.0051),
        hitErrorRZ = cms.double(0.0036),
        useErrorsFromParam = cms.bool(True)
    ),
    MTEC = cms.PSet(

    ),
    MTIB = cms.PSet(

    ),
    MTID = cms.PSet(

    ),
    MTOB = cms.PSet(

    ),
    TEC = cms.PSet(
        TTRHBuilder = cms.string('hltESPTTRHBWithTrackAngle'),
        clusterChargeCut = cms.PSet(
            refToPSet_ = cms.string('HLTSiStripClusterChargeCutNone')
        ),
        maxRing = cms.int32(1),
        minRing = cms.int32(1),
        useRingSlector = cms.bool(True)
    ),
    TIB = cms.PSet(

    ),
    TID = cms.PSet(

    ),
    TOB = cms.PSet(

    ),
    layerList = cms.vstring('BPix1+BPix2', 
        'BPix1+BPix3', 
        'BPix2+BPix3', 
        'BPix1+FPix1_pos', 
        'BPix1+FPix1_neg', 
        'BPix1+FPix2_pos', 
        'BPix1+FPix2_neg', 
        'BPix2+FPix1_pos', 
        'BPix2+FPix1_neg', 
        'BPix2+FPix2_pos', 
        'BPix2+FPix2_neg', 
        'FPix1_pos+FPix2_pos', 
        'FPix1_neg+FPix2_neg', 
        'FPix2_pos+TEC1_pos', 
        'FPix2_pos+TEC2_pos', 
        'TEC1_pos+TEC2_pos', 
        'TEC2_pos+TEC3_pos', 
        'FPix2_neg+TEC1_neg', 
        'FPix2_neg+TEC2_neg', 
        'TEC1_neg+TEC2_neg', 
        'TEC2_neg+TEC3_neg')
)


process.hltMuonCSCDigis = cms.EDProducer("CSCDCCUnpacker",
    Debug = cms.untracked.bool(False),
    ErrorMask = cms.uint32(0),
    ExaminerMask = cms.uint32(535557110),
    FormatedEventDump = cms.untracked.bool(False),
    InputObjects = cms.InputTag("rawDataCollector"),
    PrintEventNumber = cms.untracked.bool(False),
    SuppressZeroLCT = cms.untracked.bool(True),
    UnpackStatusDigis = cms.bool(False),
    UseExaminer = cms.bool(True),
    UseFormatStatus = cms.bool(True),
    UseSelectiveUnpacking = cms.bool(True),
    VisualFEDInspect = cms.untracked.bool(False),
    VisualFEDShort = cms.untracked.bool(False),
    runDQM = cms.untracked.bool(False)
)


process.hltMuonDTDigis = cms.EDProducer("DTUnpackingModule",
    dataType = cms.string('DDU'),
    dqmOnly = cms.bool(False),
    inputLabel = cms.InputTag("rawDataCollector"),
    maxFEDid = cms.untracked.int32(779),
    minFEDid = cms.untracked.int32(770),
    readOutParameters = cms.PSet(
        debug = cms.untracked.bool(False),
        localDAQ = cms.untracked.bool(False),
        performDataIntegrityMonitor = cms.untracked.bool(False),
        rosParameters = cms.PSet(
            debug = cms.untracked.bool(False),
            localDAQ = cms.untracked.bool(False),
            performDataIntegrityMonitor = cms.untracked.bool(False),
            readDDUIDfromDDU = cms.untracked.bool(True),
            readingDDU = cms.untracked.bool(True),
            writeSC = cms.untracked.bool(True)
        )
    ),
    useStandardFEDid = cms.bool(True)
)


process.hltMuonLinks = cms.EDProducer("MuonLinksProducerForHLT",
    InclusiveTrackerTrackCollection = cms.InputTag("hltPFMuonMerging"),
    LinkCollection = cms.InputTag("hltL3MuonsLinksCombination"),
    pMin = cms.double(2.5),
    ptMin = cms.double(2.5),
    shareHitFraction = cms.double(0.8)
)


process.hltMuonRPCDigis = cms.EDProducer("RPCUnpackingModule",
    InputLabel = cms.InputTag("rawDataCollector"),
    doSynchro = cms.bool(False)
)


process.hltMuons = cms.EDProducer("MuonIdProducer",
    CaloExtractorPSet = cms.PSet(
        CenterConeOnCalIntersection = cms.bool(False),
        ComponentName = cms.string('CaloExtractorByAssociator'),
        DR_Max = cms.double(1.0),
        DR_Veto_E = cms.double(0.07),
        DR_Veto_H = cms.double(0.1),
        DR_Veto_HO = cms.double(0.1),
        DepositInstanceLabels = cms.vstring('ecal', 
            'hcal', 
            'ho'),
        DepositLabel = cms.untracked.string('Cal'),
        NoiseTow_EB = cms.double(0.04),
        NoiseTow_EE = cms.double(0.15),
        Noise_EB = cms.double(0.025),
        Noise_EE = cms.double(0.1),
        Noise_HB = cms.double(0.2),
        Noise_HE = cms.double(0.2),
        Noise_HO = cms.double(0.2),
        PrintTimeReport = cms.untracked.bool(False),
        PropagatorName = cms.string('hltESPFastSteppingHelixPropagatorAny'),
        ServiceParameters = cms.PSet(
            Propagators = cms.untracked.vstring('hltESPFastSteppingHelixPropagatorAny'),
            RPCLayers = cms.bool(False),
            UseMuonNavigation = cms.untracked.bool(False)
        ),
        Threshold_E = cms.double(0.2),
        Threshold_H = cms.double(0.5),
        Threshold_HO = cms.double(0.5),
        TrackAssociatorParameters = cms.PSet(
            CSCSegmentCollectionLabel = cms.InputTag("hltCscSegments"),
            CaloTowerCollectionLabel = cms.InputTag("hltTowerMakerForPF"),
            DTRecSegment4DCollectionLabel = cms.InputTag("hltDt4DSegments"),
            EBRecHitCollectionLabel = cms.InputTag("hltEcalRecHit","EcalRecHitsEB"),
            EERecHitCollectionLabel = cms.InputTag("hltEcalRecHit","EcalRecHitsEE"),
            HBHERecHitCollectionLabel = cms.InputTag("hltHbhereco"),
            HORecHitCollectionLabel = cms.InputTag("hltHoreco"),
            accountForTrajectoryChangeCalo = cms.bool(False),
            dREcal = cms.double(1.0),
            dREcalPreselection = cms.double(1.0),
            dRHcal = cms.double(1.0),
            dRHcalPreselection = cms.double(1.0),
            dRMuon = cms.double(9999.0),
            dRMuonPreselection = cms.double(0.2),
            dRPreshowerPreselection = cms.double(0.2),
            muonMaxDistanceSigmaX = cms.double(0.0),
            muonMaxDistanceSigmaY = cms.double(0.0),
            muonMaxDistanceX = cms.double(5.0),
            muonMaxDistanceY = cms.double(5.0),
            propagateAllDirections = cms.bool(True),
            trajectoryUncertaintyTolerance = cms.double(-1.0),
            truthMatch = cms.bool(False),
            useCalo = cms.bool(True),
            useEcal = cms.bool(False),
            useHO = cms.bool(False),
            useHcal = cms.bool(False),
            useMuon = cms.bool(False),
            usePreshower = cms.bool(False)
        ),
        UseRecHitsFlag = cms.bool(False)
    ),
    JetExtractorPSet = cms.PSet(
        ComponentName = cms.string('JetExtractor'),
        DR_Max = cms.double(1.0),
        DR_Veto = cms.double(0.1),
        ExcludeMuonVeto = cms.bool(True),
        JetCollectionLabel = cms.InputTag("hltAK4CaloJetsPFEt5"),
        PrintTimeReport = cms.untracked.bool(False),
        PropagatorName = cms.string('hltESPFastSteppingHelixPropagatorAny'),
        ServiceParameters = cms.PSet(
            Propagators = cms.untracked.vstring('hltESPFastSteppingHelixPropagatorAny'),
            RPCLayers = cms.bool(False),
            UseMuonNavigation = cms.untracked.bool(False)
        ),
        Threshold = cms.double(5.0),
        TrackAssociatorParameters = cms.PSet(
            CSCSegmentCollectionLabel = cms.InputTag("hltCscSegments"),
            CaloTowerCollectionLabel = cms.InputTag("hltTowerMakerForPF"),
            DTRecSegment4DCollectionLabel = cms.InputTag("hltDt4DSegments"),
            EBRecHitCollectionLabel = cms.InputTag("hltEcalRecHit","EcalRecHitsEB"),
            EERecHitCollectionLabel = cms.InputTag("hltEcalRecHit","EcalRecHitsEE"),
            HBHERecHitCollectionLabel = cms.InputTag("hltHbhereco"),
            HORecHitCollectionLabel = cms.InputTag("hltHoreco"),
            accountForTrajectoryChangeCalo = cms.bool(False),
            dREcal = cms.double(0.5),
            dREcalPreselection = cms.double(0.5),
            dRHcal = cms.double(0.5),
            dRHcalPreselection = cms.double(0.5),
            dRMuon = cms.double(9999.0),
            dRMuonPreselection = cms.double(0.2),
            dRPreshowerPreselection = cms.double(0.2),
            muonMaxDistanceSigmaX = cms.double(0.0),
            muonMaxDistanceSigmaY = cms.double(0.0),
            muonMaxDistanceX = cms.double(5.0),
            muonMaxDistanceY = cms.double(5.0),
            propagateAllDirections = cms.bool(True),
            trajectoryUncertaintyTolerance = cms.double(-1.0),
            truthMatch = cms.bool(False),
            useCalo = cms.bool(True),
            useEcal = cms.bool(False),
            useHO = cms.bool(False),
            useHcal = cms.bool(False),
            useMuon = cms.bool(False),
            usePreshower = cms.bool(False)
        )
    ),
    MuonCaloCompatibility = cms.PSet(
        MuonTemplateFileName = cms.FileInPath('RecoMuon/MuonIdentification/data/MuID_templates_muons_lowPt_3_1_norm.root'),
        PionTemplateFileName = cms.FileInPath('RecoMuon/MuonIdentification/data/MuID_templates_pions_lowPt_3_1_norm.root'),
        allSiPMHO = cms.bool(False),
        delta_eta = cms.double(0.02),
        delta_phi = cms.double(0.02)
    ),
    TimingFillerParameters = cms.PSet(
        CSCTimingParameters = cms.PSet(
            CSCStripError = cms.double(7.0),
            CSCStripTimeOffset = cms.double(0.0),
            CSCTimeOffset = cms.double(0.0),
            CSCWireError = cms.double(8.6),
            CSCWireTimeOffset = cms.double(0.0),
            CSCsegments = cms.InputTag("hltCscSegments"),
            MatchParameters = cms.PSet(
                CSCsegments = cms.InputTag("hltCscSegments"),
                DTradius = cms.double(0.01),
                DTsegments = cms.InputTag("hltDt4DSegments"),
                TightMatchCSC = cms.bool(True),
                TightMatchDT = cms.bool(False)
            ),
            PruneCut = cms.double(100.0),
            ServiceParameters = cms.PSet(
                Propagators = cms.untracked.vstring('hltESPFastSteppingHelixPropagatorAny'),
                RPCLayers = cms.bool(True)
            ),
            UseStripTime = cms.bool(True),
            UseWireTime = cms.bool(True),
            debug = cms.bool(False)
        ),
        DTTimingParameters = cms.PSet(
            DTTimeOffset = cms.double(2.7),
            DTsegments = cms.InputTag("hltDt4DSegments"),
            DoWireCorr = cms.bool(False),
            DropTheta = cms.bool(True),
            HitError = cms.double(6.0),
            HitsMin = cms.int32(5),
            MatchParameters = cms.PSet(
                CSCsegments = cms.InputTag("hltCscSegments"),
                DTradius = cms.double(0.01),
                DTsegments = cms.InputTag("hltDt4DSegments"),
                TightMatchCSC = cms.bool(True),
                TightMatchDT = cms.bool(False)
            ),
            PruneCut = cms.double(10000.0),
            RequireBothProjections = cms.bool(False),
            ServiceParameters = cms.PSet(
                Propagators = cms.untracked.vstring('hltESPFastSteppingHelixPropagatorAny'),
                RPCLayers = cms.bool(True)
            ),
            UseSegmentT0 = cms.bool(False),
            debug = cms.bool(False)
        ),
        EcalEnergyCut = cms.double(0.4),
        ErrorCSC = cms.double(7.4),
        ErrorDT = cms.double(6.0),
        ErrorEB = cms.double(2.085),
        ErrorEE = cms.double(6.95),
        UseCSC = cms.bool(True),
        UseDT = cms.bool(True),
        UseECAL = cms.bool(True)
    ),
    TrackAssociatorParameters = cms.PSet(
        CSCSegmentCollectionLabel = cms.InputTag("hltCscSegments"),
        CaloTowerCollectionLabel = cms.InputTag("hltTowerMakerForPF"),
        DTRecSegment4DCollectionLabel = cms.InputTag("hltDt4DSegments"),
        EBRecHitCollectionLabel = cms.InputTag("hltEcalRecHit","EcalRecHitsEB"),
        EERecHitCollectionLabel = cms.InputTag("hltEcalRecHit","EcalRecHitsEE"),
        HBHERecHitCollectionLabel = cms.InputTag("hltHbhereco"),
        HORecHitCollectionLabel = cms.InputTag("hltHoreco"),
        accountForTrajectoryChangeCalo = cms.bool(False),
        dREcal = cms.double(9999.0),
        dREcalPreselection = cms.double(0.05),
        dRHcal = cms.double(9999.0),
        dRHcalPreselection = cms.double(0.2),
        dRMuon = cms.double(9999.0),
        dRMuonPreselection = cms.double(0.2),
        dRPreshowerPreselection = cms.double(0.2),
        muonMaxDistanceSigmaX = cms.double(0.0),
        muonMaxDistanceSigmaY = cms.double(0.0),
        muonMaxDistanceX = cms.double(5.0),
        muonMaxDistanceY = cms.double(5.0),
        propagateAllDirections = cms.bool(True),
        trajectoryUncertaintyTolerance = cms.double(-1.0),
        truthMatch = cms.bool(False),
        useCalo = cms.bool(False),
        useEcal = cms.bool(True),
        useHO = cms.bool(True),
        useHcal = cms.bool(True),
        useMuon = cms.bool(True),
        usePreshower = cms.bool(False)
    ),
    TrackExtractorPSet = cms.PSet(
        BeamSpotLabel = cms.InputTag("hltOnlineBeamSpot"),
        BeamlineOption = cms.string('BeamSpotFromEvent'),
        Chi2Ndof_Max = cms.double(1e+64),
        Chi2Prob_Min = cms.double(-1.0),
        ComponentName = cms.string('TrackExtractor'),
        DR_Max = cms.double(1.0),
        DR_Veto = cms.double(0.01),
        DepositLabel = cms.untracked.string(''),
        Diff_r = cms.double(0.1),
        Diff_z = cms.double(0.2),
        NHits_Min = cms.uint32(0),
        Pt_Min = cms.double(-1.0),
        inputTrackCollection = cms.InputTag("hltPFMuonMerging")
    ),
    TrackerKinkFinderParameters = cms.PSet(
        diagonalOnly = cms.bool(False),
        usePosition = cms.bool(False)
    ),
    addExtraSoftMuons = cms.bool(False),
    arbitrationCleanerOptions = cms.PSet(
        ClusterDPhi = cms.double(0.6),
        ClusterDTheta = cms.double(0.02),
        Clustering = cms.bool(True),
        ME1a = cms.bool(True),
        Overlap = cms.bool(True),
        OverlapDPhi = cms.double(0.0786),
        OverlapDTheta = cms.double(0.02)
    ),
    debugWithTruthMatching = cms.bool(False),
    ecalDepositName = cms.string('ecal'),
    fillCaloCompatibility = cms.bool(True),
    fillEnergy = cms.bool(True),
    fillGlobalTrackQuality = cms.bool(False),
    fillGlobalTrackRefits = cms.bool(False),
    fillIsolation = cms.bool(True),
    fillMatching = cms.bool(True),
    fillTrackerKink = cms.bool(False),
    globalTrackQualityInputTag = cms.InputTag("glbTrackQual"),
    hcalDepositName = cms.string('hcal'),
    hoDepositName = cms.string('ho'),
    inputCollectionLabels = cms.VInputTag("hltPFMuonMerging", "hltMuonLinks", "hltL2Muons"),
    inputCollectionTypes = cms.vstring('inner tracks', 
        'links', 
        'outer tracks'),
    jetDepositName = cms.string('jets'),
    maxAbsDx = cms.double(3.0),
    maxAbsDy = cms.double(9999.0),
    maxAbsEta = cms.double(3.0),
    maxAbsPullX = cms.double(4.0),
    maxAbsPullY = cms.double(9999.0),
    minCaloCompatibility = cms.double(0.6),
    minNumberOfMatches = cms.int32(1),
    minP = cms.double(10.0),
    minPCaloMuon = cms.double(1000000000.0),
    minPt = cms.double(10.0),
    ptThresholdToFillCandidateP4WithGlobalFit = cms.double(200.0),
    runArbitrationCleaner = cms.bool(False),
    sigmaThresholdToFillCandidateP4WithGlobalFit = cms.double(2.0),
    trackDepositName = cms.string('tracker'),
    writeIsoDeposits = cms.bool(False)
)


process.hltOnlineBeamSpot = cms.EDProducer("BeamSpotOnlineProducer",
    changeToCMSCoordinates = cms.bool(False),
    gtEvmLabel = cms.InputTag(""),
    maxRadius = cms.double(2.0),
    maxZ = cms.double(40.0),
    setSigmaZ = cms.double(0.0),
    src = cms.InputTag("hltScalersRawToDigi")
)


process.hltPFMuonMerging = cms.EDProducer("TrackListMerger",
    Epsilon = cms.double(-0.001),
    FoundHitBonus = cms.double(5.0),
    LostHitPenalty = cms.double(20.0),
    MaxNormalizedChisq = cms.double(1000.0),
    MinFound = cms.int32(3),
    MinPT = cms.double(0.05),
    ShareFrac = cms.double(0.19),
    TrackProducers = cms.VInputTag("hltL3TkTracksFromL2", "hltIter2Merged"),
    allowFirstHitShare = cms.bool(True),
    copyExtras = cms.untracked.bool(True),
    copyMVA = cms.bool(False),
    hasSelector = cms.vint32(0, 0),
    indivShareFrac = cms.vdouble(1.0, 1.0),
    newQuality = cms.string('confirmed'),
    selectedTrackQuals = cms.VInputTag("hltL3TkTracksFromL2", "hltIter2Merged"),
    setsToMerge = cms.VPSet(cms.PSet(
        pQual = cms.bool(False),
        tLists = cms.vint32(0, 1)
    )),
    writeOnlyTrkQuals = cms.bool(False)
)


process.hltParticleFlow = cms.EDProducer("PFProducer",
    GedElectronValueMap = cms.InputTag("gedGsfElectronsTmp"),
    GedPhotonValueMap = cms.InputTag("tmpGedPhotons","valMapPFEgammaCandToPhoton"),
    PFEGammaCandidates = cms.InputTag("particleFlowEGamma"),
    X0_Map = cms.string('RecoParticleFlow/PFProducer/data/allX0histos.root'),
    algoType = cms.uint32(0),
    blocks = cms.InputTag("hltParticleFlowBlock"),
    calibHF_a_EMHAD = cms.vdouble(1.42215, 1.00496, 0.68961, 0.81656, 0.98504, 
        0.98504, 1.00802, 1.0593, 1.4576, 1.4576),
    calibHF_a_EMonly = cms.vdouble(0.96945, 0.96701, 0.76309, 0.82268, 0.87583, 
        0.89718, 0.98674, 1.4681, 1.458, 1.458),
    calibHF_b_EMHAD = cms.vdouble(1.27541, 0.85361, 0.86333, 0.89091, 0.94348, 
        0.94348, 0.9437, 1.0034, 1.0444, 1.0444),
    calibHF_b_HADonly = cms.vdouble(1.27541, 0.85361, 0.86333, 0.89091, 0.94348, 
        0.94348, 0.9437, 1.0034, 1.0444, 1.0444),
    calibHF_eta_step = cms.vdouble(0.0, 2.9, 3.0, 3.2, 4.2, 
        4.4, 4.6, 4.8, 5.2, 5.4),
    calibHF_use = cms.bool(False),
    calibPFSCEle_Fbrem_barrel = cms.vdouble(0.6, 6.0, -0.0255975, 0.0576727, 0.975442, 
        -0.000546394, 1.26147, 25.0, -0.02025, 0.04537, 
        0.9728, -0.0008962, 1.172),
    calibPFSCEle_Fbrem_endcap = cms.vdouble(0.9, 6.5, -0.0692932, 0.101776, 0.995338, 
        -0.00236548, 0.874998, 1.653, -0.0750184, 0.147, 
        0.923165, 0.000474665, 1.10782),
    calibPFSCEle_barrel = cms.vdouble(1.004, -1.536, 22.88, -1.467, 0.3555, 
        0.6227, 14.65, 2051.0, 25.0, 0.9932, 
        -0.5444, 0.0, 0.5438, 0.7109, 7.645, 
        0.2904, 0.0),
    calibPFSCEle_endcap = cms.vdouble(1.153, -16.5975, 5.668, -0.1772, 16.22, 
        7.326, 0.0483, -4.068, 9.406),
    calibrationsLabel = cms.string('HLT'),
    cleanedHF = cms.VInputTag("hltParticleFlowRecHitHF:Cleaned", "hltParticleFlowClusterHF:Cleaned"),
    coneEcalIsoForEgammaSC = cms.double(0.3),
    coneTrackIsoForEgammaSC = cms.double(0.3),
    cosmicRejectionDistance = cms.double(1.0),
    debug = cms.untracked.bool(False),
    dptRel_DispVtx = cms.double(10.0),
    dzPV = cms.double(0.2),
    egammaElectrons = cms.InputTag(""),
    electron_iso_combIso_barrel = cms.double(10.0),
    electron_iso_combIso_endcap = cms.double(10.0),
    electron_iso_mva_barrel = cms.double(-0.1875),
    electron_iso_mva_endcap = cms.double(-0.1075),
    electron_iso_pt = cms.double(10.0),
    electron_missinghits = cms.uint32(1),
    electron_noniso_mvaCut = cms.double(-0.1),
    electron_protectionsForJetMET = cms.PSet(
        maxDPhiIN = cms.double(0.1),
        maxE = cms.double(50.0),
        maxEcalEOverPRes = cms.double(0.2),
        maxEcalEOverP_1 = cms.double(0.5),
        maxEcalEOverP_2 = cms.double(0.2),
        maxEeleOverPout = cms.double(0.2),
        maxEeleOverPoutRes = cms.double(0.5),
        maxEleHcalEOverEcalE = cms.double(0.1),
        maxHcalE = cms.double(10.0),
        maxHcalEOverEcalE = cms.double(0.1),
        maxHcalEOverP = cms.double(1.0),
        maxNtracks = cms.double(3.0),
        maxTrackPOverEele = cms.double(1.0)
    ),
    eventFactorForCosmics = cms.double(10.0),
    eventFractionForCleaning = cms.double(0.5),
    eventFractionForRejection = cms.double(0.8),
    factors_45 = cms.vdouble(10.0, 100.0),
    iCfgCandConnector = cms.PSet(
        bCalibPrimary = cms.bool(False),
        bCalibSecondary = cms.bool(False),
        bCorrect = cms.bool(False),
        nuclCalibFactors = cms.vdouble(0.8, 0.15, 0.5, 0.5, 0.05)
    ),
    isolatedElectronID_mvaWeightFile = cms.string('RecoEgamma/ElectronIdentification/data/TMVA_BDTSimpleCat_17Feb2011.weights.xml'),
    maxDPtOPt = cms.double(1.0),
    maxDeltaPhiPt = cms.double(7.0),
    maxSignificance = cms.double(2.5),
    metFactorForCleaning = cms.double(4.0),
    metFactorForFakes = cms.double(4.0),
    metFactorForHighEta = cms.double(25.0),
    metFactorForRejection = cms.double(4.0),
    metSignificanceForCleaning = cms.double(3.0),
    metSignificanceForRejection = cms.double(4.0),
    minDeltaMet = cms.double(0.4),
    minEnergyForPunchThrough = cms.double(100.0),
    minHFCleaningPt = cms.double(5.0),
    minMomentumForPunchThrough = cms.double(100.0),
    minPixelHits = cms.int32(1),
    minPtForPostCleaning = cms.double(20.0),
    minSignificance = cms.double(2.5),
    minSignificanceReduction = cms.double(1.4),
    minTrackerHits = cms.int32(8),
    muon_ECAL = cms.vdouble(0.5, 0.5),
    muon_HCAL = cms.vdouble(3.0, 3.0),
    muon_HO = cms.vdouble(0.9, 0.9),
    muons = cms.InputTag("hltMuons"),
    nTrackIsoForEgammaSC = cms.uint32(2),
    nsigma_TRACK = cms.double(1.0),
    pf_GlobC_mvaWeightFile = cms.string('RecoParticleFlow/PFProducer/data/TMVARegression_BDTG_PFGlobalCorr_14Dec2011.root'),
    pf_Res_mvaWeightFile = cms.string('RecoParticleFlow/PFProducer/data/TMVARegression_BDTG_PFRes_14Dec2011.root'),
    pf_convID_mvaWeightFile = cms.string('RecoParticleFlow/PFProducer/data/MVAnalysis_BDT.weights_pfConversionAug0411.txt'),
    pf_conv_mvaCut = cms.double(0.0),
    pf_electronID_crackCorrection = cms.bool(False),
    pf_electronID_mvaWeightFile = cms.string('RecoParticleFlow/PFProducer/data/MVAnalysis_BDT.weights_PfElectrons23Jan_IntToFloat.txt'),
    pf_electron_mvaCut = cms.double(-0.1),
    pf_electron_output_col = cms.string('electrons'),
    pf_locC_mvaWeightFile = cms.string('RecoParticleFlow/PFProducer/data/TMVARegression_BDTG_PFClusterLCorr_14Dec2011.root'),
    pf_nsigma_ECAL = cms.double(0.0),
    pf_nsigma_HCAL = cms.double(1.0),
    photon_HoE = cms.double(0.05),
    photon_MinEt = cms.double(10.0),
    photon_SigmaiEtaiEta_barrel = cms.double(0.0125),
    photon_SigmaiEtaiEta_endcap = cms.double(0.034),
    photon_combIso = cms.double(10.0),
    photon_protectionsForJetMET = cms.PSet(
        sumPtTrackIso = cms.double(2.0),
        sumPtTrackIsoSlope = cms.double(0.001)
    ),
    postHFCleaning = cms.bool(False),
    postMuonCleaning = cms.bool(True),
    ptErrorScale = cms.double(8.0),
    ptFactorForHighEta = cms.double(2.0),
    pt_Error = cms.double(1.0),
    punchThroughFactor = cms.double(3.0),
    punchThroughMETFactor = cms.double(4.0),
    rejectTracks_Bad = cms.bool(False),
    rejectTracks_Step45 = cms.bool(False),
    sumEtEcalIsoForEgammaSC_barrel = cms.double(1.0),
    sumEtEcalIsoForEgammaSC_endcap = cms.double(2.0),
    sumPtTrackIsoForEgammaSC_barrel = cms.double(4.0),
    sumPtTrackIsoForEgammaSC_endcap = cms.double(4.0),
    sumPtTrackIsoForPhoton = cms.double(-1.0),
    sumPtTrackIsoSlopeForPhoton = cms.double(-1.0),
    trackQuality = cms.string('highPurity'),
    useCalibrationsFromDB = cms.bool(True),
    useEGammaElectrons = cms.bool(False),
    useEGammaFilters = cms.bool(False),
    useEGammaSupercluster = cms.bool(False),
    useHO = cms.bool(False),
    usePFConversions = cms.bool(False),
    usePFDecays = cms.bool(False),
    usePFElectrons = cms.bool(False),
    usePFNuclearInteractions = cms.bool(False),
    usePFPhotons = cms.bool(False),
    usePFSCEleCalib = cms.bool(True),
    usePhotonReg = cms.bool(False),
    useProtectionsForJetMET = cms.bool(True),
    useRegressionFromDB = cms.bool(False),
    useVerticesForNeutral = cms.bool(True),
    verbose = cms.untracked.bool(False),
    vertexCollection = cms.InputTag("hltPixelVertices")
)


process.hltParticleFlowBlock = cms.EDProducer("PFBlockProducer",
    debug = cms.untracked.bool(False),
    elementImporters = cms.VPSet(cms.PSet(
        DPtOverPtCuts_byTrackAlgo = cms.vdouble(0.5, 0.5, 0.5, 0.5, 0.5, 
            0.5),
        NHitCuts_byTrackAlgo = cms.vuint32(3, 3, 3, 3, 3, 
            3),
        importerName = cms.string('GeneralTracksImporter'),
        muonSrc = cms.InputTag("hltMuons"),
        source = cms.InputTag("hltLightPFTracks"),
        useIterativeTracking = cms.bool(False)
    ), 
        cms.PSet(
            BCtoPFCMap = cms.InputTag(""),
            importerName = cms.string('ECALClusterImporter'),
            source = cms.InputTag("hltParticleFlowClusterECALUnseeded")
        ), 
        cms.PSet(
            importerName = cms.string('GenericClusterImporter'),
            source = cms.InputTag("hltParticleFlowClusterHCAL")
        ), 
        cms.PSet(
            importerName = cms.string('GenericClusterImporter'),
            source = cms.InputTag("hltParticleFlowClusterHF")
        ), 
        cms.PSet(
            importerName = cms.string('GenericClusterImporter'),
            source = cms.InputTag("hltParticleFlowClusterPSUnseeded")
        )),
    linkDefinitions = cms.VPSet(cms.PSet(
        linkType = cms.string('PS1:ECAL'),
        linkerName = cms.string('PreshowerAndECALLinker'),
        useKDTree = cms.bool(True)
    ), 
        cms.PSet(
            linkType = cms.string('PS2:ECAL'),
            linkerName = cms.string('PreshowerAndECALLinker'),
            useKDTree = cms.bool(True)
        ), 
        cms.PSet(
            linkType = cms.string('TRACK:ECAL'),
            linkerName = cms.string('TrackAndECALLinker'),
            useKDTree = cms.bool(True)
        ), 
        cms.PSet(
            linkType = cms.string('TRACK:HCAL'),
            linkerName = cms.string('TrackAndHCALLinker'),
            useKDTree = cms.bool(True)
        ), 
        cms.PSet(
            linkType = cms.string('ECAL:HCAL'),
            linkerName = cms.string('ECALAndHCALLinker'),
            useKDTree = cms.bool(False)
        ), 
        cms.PSet(
            linkType = cms.string('HFEM:HFHAD'),
            linkerName = cms.string('HFEMAndHFHADLinker'),
            useKDTree = cms.bool(False)
        )),
    verbose = cms.untracked.bool(False)
)


process.hltParticleFlowClusterECALUncorrectedUnseeded = cms.EDProducer("PFClusterProducer",
    energyCorrector = cms.PSet(

    ),
    initialClusteringStep = cms.PSet(
        algoName = cms.string('Basic2DGenericTopoClusterizer'),
        thresholdsByDetector = cms.VPSet(cms.PSet(
            detector = cms.string('ECAL_BARREL'),
            gatheringThreshold = cms.double(0.08),
            gatheringThresholdPt = cms.double(0.0)
        ), 
            cms.PSet(
                detector = cms.string('ECAL_ENDCAP'),
                gatheringThreshold = cms.double(0.3),
                gatheringThresholdPt = cms.double(0.0)
            )),
        useCornerCells = cms.bool(True)
    ),
    pfClusterBuilder = cms.PSet(
        algoName = cms.string('Basic2DGenericPFlowClusterizer'),
        allCellsPositionCalc = cms.PSet(
            algoName = cms.string('Basic2DGenericPFlowPositionCalc'),
            logWeightDenominator = cms.double(0.08),
            minAllowedNormalization = cms.double(1e-09),
            minFractionInCalc = cms.double(1e-09),
            posCalcNCrystals = cms.int32(-1),
            timeResolutionCalcBarrel = cms.PSet(
                constantTerm = cms.double(0.428192),
                constantTermLowE = cms.double(0.0),
                corrTermLowE = cms.double(0.0510871),
                noiseTerm = cms.double(1.10889),
                noiseTermLowE = cms.double(1.31883),
                threshHighE = cms.double(5.0),
                threshLowE = cms.double(0.5)
            ),
            timeResolutionCalcEndcap = cms.PSet(
                constantTerm = cms.double(0.0),
                constantTermLowE = cms.double(0.0),
                corrTermLowE = cms.double(0.0),
                noiseTerm = cms.double(5.72489999999),
                noiseTermLowE = cms.double(6.92683000001),
                threshHighE = cms.double(10.0),
                threshLowE = cms.double(1.0)
            )
        ),
        excludeOtherSeeds = cms.bool(True),
        maxIterations = cms.uint32(50),
        minFracTot = cms.double(1e-20),
        minFractionToKeep = cms.double(1e-07),
        positionCalc = cms.PSet(
            algoName = cms.string('Basic2DGenericPFlowPositionCalc'),
            logWeightDenominator = cms.double(0.08),
            minAllowedNormalization = cms.double(1e-09),
            minFractionInCalc = cms.double(1e-09),
            posCalcNCrystals = cms.int32(9),
            timeResolutionCalcBarrel = cms.PSet(
                constantTerm = cms.double(0.428192),
                constantTermLowE = cms.double(0.0),
                corrTermLowE = cms.double(0.0510871),
                noiseTerm = cms.double(1.10889),
                noiseTermLowE = cms.double(1.31883),
                threshHighE = cms.double(5.0),
                threshLowE = cms.double(0.5)
            ),
            timeResolutionCalcEndcap = cms.PSet(
                constantTerm = cms.double(0.0),
                constantTermLowE = cms.double(0.0),
                corrTermLowE = cms.double(0.0),
                noiseTerm = cms.double(5.72489999999),
                noiseTermLowE = cms.double(6.92683000001),
                threshHighE = cms.double(10.0),
                threshLowE = cms.double(1.0)
            )
        ),
        positionCalcForConvergence = cms.PSet(
            T0_EB = cms.double(7.4),
            T0_EE = cms.double(3.1),
            T0_ES = cms.double(1.2),
            W0 = cms.double(4.2),
            X0 = cms.double(0.89),
            algoName = cms.string('ECAL2DPositionCalcWithDepthCorr'),
            minAllowedNormalization = cms.double(0.0),
            minFractionInCalc = cms.double(0.0)
        ),
        recHitEnergyNorms = cms.VPSet(cms.PSet(
            detector = cms.string('ECAL_BARREL'),
            recHitEnergyNorm = cms.double(0.08)
        ), 
            cms.PSet(
                detector = cms.string('ECAL_ENDCAP'),
                recHitEnergyNorm = cms.double(0.3)
            )),
        showerSigma = cms.double(1.5),
        stoppingTolerance = cms.double(1e-08)
    ),
    positionReCalc = cms.PSet(
        T0_EB = cms.double(7.4),
        T0_EE = cms.double(3.1),
        T0_ES = cms.double(1.2),
        W0 = cms.double(4.2),
        X0 = cms.double(0.89),
        algoName = cms.string('ECAL2DPositionCalcWithDepthCorr'),
        minAllowedNormalization = cms.double(0.0),
        minFractionInCalc = cms.double(0.0)
    ),
    recHitCleaners = cms.VPSet(cms.PSet(
        algoName = cms.string('SpikeAndDoubleSpikeCleaner'),
        cleaningByDetector = cms.VPSet(cms.PSet(
            detector = cms.string('ECAL_BARREL'),
            doubleSpikeS6S2 = cms.double(0.04),
            doubleSpikeThresh = cms.double(10.0),
            energyThresholdModifier = cms.double(2.0),
            fractionThresholdModifier = cms.double(3.0),
            minS4S1_a = cms.double(0.04),
            minS4S1_b = cms.double(-0.024),
            singleSpikeThresh = cms.double(4.0)
        ), 
            cms.PSet(
                detector = cms.string('ECAL_ENDCAP'),
                doubleSpikeS6S2 = cms.double(-1.0),
                doubleSpikeThresh = cms.double(1000000000.0),
                energyThresholdModifier = cms.double(2.0),
                fractionThresholdModifier = cms.double(3.0),
                minS4S1_a = cms.double(0.02),
                minS4S1_b = cms.double(-0.0125),
                singleSpikeThresh = cms.double(15.0)
            ))
    )),
    recHitsSource = cms.InputTag("hltParticleFlowRecHitECALUnseeded"),
    seedFinder = cms.PSet(
        algoName = cms.string('LocalMaximumSeedFinder'),
        nNeighbours = cms.int32(8),
        thresholdsByDetector = cms.VPSet(cms.PSet(
            detector = cms.string('ECAL_ENDCAP'),
            seedingThreshold = cms.double(0.6),
            seedingThresholdPt = cms.double(0.15)
        ), 
            cms.PSet(
                detector = cms.string('ECAL_BARREL'),
                seedingThreshold = cms.double(0.23),
                seedingThresholdPt = cms.double(0.0)
            ))
    )
)


process.hltParticleFlowClusterECALUnseeded = cms.EDProducer("CorrectedECALPFClusterProducer",
    energyCorrector = cms.PSet(
        algoName = cms.string('PFClusterEMEnergyCorrector'),
        applyCrackCorrections = cms.bool(False)
    ),
    inputECAL = cms.InputTag("hltParticleFlowClusterECALUncorrectedUnseeded"),
    inputPS = cms.InputTag("hltParticleFlowClusterPSUnseeded"),
    minimumPSEnergy = cms.double(0.0)
)


process.hltParticleFlowClusterHBHE = cms.EDProducer("PFClusterProducer",
    energyCorrector = cms.PSet(

    ),
    initialClusteringStep = cms.PSet(
        algoName = cms.string('Basic2DGenericTopoClusterizer'),
        thresholdsByDetector = cms.VPSet(cms.PSet(
            detector = cms.string('HCAL_BARREL1'),
            gatheringThreshold = cms.double(0.8),
            gatheringThresholdPt = cms.double(0.0)
        ), 
            cms.PSet(
                detector = cms.string('HCAL_ENDCAP'),
                gatheringThreshold = cms.double(0.8),
                gatheringThresholdPt = cms.double(0.0)
            )),
        useCornerCells = cms.bool(True)
    ),
    pfClusterBuilder = cms.PSet(
        algoName = cms.string('Basic2DGenericPFlowClusterizer'),
        allCellsPositionCalc = cms.PSet(
            algoName = cms.string('Basic2DGenericPFlowPositionCalc'),
            logWeightDenominator = cms.double(0.8),
            minAllowedNormalization = cms.double(1e-09),
            minFractionInCalc = cms.double(1e-09),
            posCalcNCrystals = cms.int32(-1)
        ),
        clusterTimeResFromSeed = cms.bool(False),
        excludeOtherSeeds = cms.bool(True),
        maxIterations = cms.uint32(50),
        maxNSigmaTime = cms.double(10.0),
        minChi2Prob = cms.double(0.0),
        minFracTot = cms.double(1e-20),
        minFractionToKeep = cms.double(1e-07),
        positionCalc = cms.PSet(
            algoName = cms.string('Basic2DGenericPFlowPositionCalc'),
            logWeightDenominator = cms.double(0.8),
            minAllowedNormalization = cms.double(1e-09),
            minFractionInCalc = cms.double(1e-09),
            posCalcNCrystals = cms.int32(5)
        ),
        recHitEnergyNorms = cms.VPSet(cms.PSet(
            detector = cms.string('HCAL_BARREL1'),
            recHitEnergyNorm = cms.double(0.8)
        ), 
            cms.PSet(
                detector = cms.string('HCAL_ENDCAP'),
                recHitEnergyNorm = cms.double(0.8)
            )),
        showerSigma = cms.double(10.0),
        stoppingTolerance = cms.double(1e-08),
        timeResolutionCalcBarrel = cms.PSet(
            constantTerm = cms.double(2.82),
            constantTermLowE = cms.double(4.24),
            corrTermLowE = cms.double(0.0),
            noiseTerm = cms.double(21.86),
            noiseTermLowE = cms.double(8.0),
            threshHighE = cms.double(15.0),
            threshLowE = cms.double(6.0)
        ),
        timeResolutionCalcEndcap = cms.PSet(
            constantTerm = cms.double(2.82),
            constantTermLowE = cms.double(4.24),
            corrTermLowE = cms.double(0.0),
            noiseTerm = cms.double(21.86),
            noiseTermLowE = cms.double(8.0),
            threshHighE = cms.double(15.0),
            threshLowE = cms.double(6.0)
        ),
        timeSigmaEB = cms.double(10.0),
        timeSigmaEE = cms.double(10.0)
    ),
    positionReCalc = cms.PSet(

    ),
    recHitCleaners = cms.VPSet(),
    recHitsSource = cms.InputTag("hltParticleFlowRecHitHBHE"),
    seedFinder = cms.PSet(
        algoName = cms.string('LocalMaximumSeedFinder'),
        nNeighbours = cms.int32(4),
        thresholdsByDetector = cms.VPSet(cms.PSet(
            detector = cms.string('HCAL_BARREL1'),
            seedingThreshold = cms.double(1.0),
            seedingThresholdPt = cms.double(0.0)
        ), 
            cms.PSet(
                detector = cms.string('HCAL_ENDCAP'),
                seedingThreshold = cms.double(1.1),
                seedingThresholdPt = cms.double(0.0)
            ))
    )
)


process.hltParticleFlowClusterHCAL = cms.EDProducer("PFMultiDepthClusterProducer",
    clustersSource = cms.InputTag("hltParticleFlowClusterHBHE"),
    energyCorrector = cms.PSet(

    ),
    pfClusterBuilder = cms.PSet(
        algoName = cms.string('PFMultiDepthClusterizer'),
        allCellsPositionCalc = cms.PSet(
            algoName = cms.string('Basic2DGenericPFlowPositionCalc'),
            logWeightDenominator = cms.double(0.8),
            minAllowedNormalization = cms.double(1e-09),
            minFractionInCalc = cms.double(1e-09),
            posCalcNCrystals = cms.int32(-1)
        ),
        minFractionToKeep = cms.double(1e-07),
        nSigmaEta = cms.double(2.0),
        nSigmaPhi = cms.double(2.0)
    ),
    positionReCalc = cms.PSet(

    )
)


process.hltParticleFlowClusterHF = cms.EDProducer("PFClusterProducer",
    energyCorrector = cms.PSet(

    ),
    initialClusteringStep = cms.PSet(
        algoName = cms.string('Basic2DGenericTopoClusterizer'),
        thresholdsByDetector = cms.VPSet(cms.PSet(
            detector = cms.string('HF_EM'),
            gatheringThreshold = cms.double(0.8),
            gatheringThresholdPt = cms.double(0.0)
        ), 
            cms.PSet(
                detector = cms.string('HF_HAD'),
                gatheringThreshold = cms.double(0.8),
                gatheringThresholdPt = cms.double(0.0)
            )),
        useCornerCells = cms.bool(False)
    ),
    pfClusterBuilder = cms.PSet(
        algoName = cms.string('Basic2DGenericPFlowClusterizer'),
        allCellsPositionCalc = cms.PSet(
            algoName = cms.string('Basic2DGenericPFlowPositionCalc'),
            logWeightDenominator = cms.double(0.8),
            minAllowedNormalization = cms.double(1e-09),
            minFractionInCalc = cms.double(1e-09),
            posCalcNCrystals = cms.int32(-1)
        ),
        excludeOtherSeeds = cms.bool(True),
        maxIterations = cms.uint32(50),
        minFracTot = cms.double(1e-20),
        minFractionToKeep = cms.double(1e-07),
        positionCalc = cms.PSet(
            algoName = cms.string('Basic2DGenericPFlowPositionCalc'),
            logWeightDenominator = cms.double(0.8),
            minAllowedNormalization = cms.double(1e-09),
            minFractionInCalc = cms.double(1e-09),
            posCalcNCrystals = cms.int32(5)
        ),
        recHitEnergyNorms = cms.VPSet(cms.PSet(
            detector = cms.string('HF_EM'),
            recHitEnergyNorm = cms.double(0.8)
        ), 
            cms.PSet(
                detector = cms.string('HF_HAD'),
                recHitEnergyNorm = cms.double(0.8)
            )),
        showerSigma = cms.double(10.0),
        stoppingTolerance = cms.double(1e-08)
    ),
    positionReCalc = cms.PSet(

    ),
    recHitCleaners = cms.VPSet(cms.PSet(
        algoName = cms.string('SpikeAndDoubleSpikeCleaner'),
        cleaningByDetector = cms.VPSet(cms.PSet(
            detector = cms.string('HF_EM'),
            doubleSpikeS6S2 = cms.double(-1.0),
            doubleSpikeThresh = cms.double(1000000000.0),
            energyThresholdModifier = cms.double(1.0),
            fractionThresholdModifier = cms.double(1.0),
            minS4S1_a = cms.double(0.11),
            minS4S1_b = cms.double(-0.19),
            singleSpikeThresh = cms.double(80.0)
        ), 
            cms.PSet(
                detector = cms.string('HF_HAD'),
                doubleSpikeS6S2 = cms.double(-1.0),
                doubleSpikeThresh = cms.double(1000000000.0),
                energyThresholdModifier = cms.double(1.0),
                fractionThresholdModifier = cms.double(1.0),
                minS4S1_a = cms.double(0.045),
                minS4S1_b = cms.double(-0.08),
                singleSpikeThresh = cms.double(120.0)
            ))
    )),
    recHitsSource = cms.InputTag("hltParticleFlowRecHitHF"),
    seedFinder = cms.PSet(
        algoName = cms.string('LocalMaximumSeedFinder'),
        nNeighbours = cms.int32(0),
        thresholdsByDetector = cms.VPSet(cms.PSet(
            detector = cms.string('HF_EM'),
            seedingThreshold = cms.double(1.4),
            seedingThresholdPt = cms.double(0.0)
        ), 
            cms.PSet(
                detector = cms.string('HF_HAD'),
                seedingThreshold = cms.double(1.4),
                seedingThresholdPt = cms.double(0.0)
            ))
    )
)


process.hltParticleFlowClusterPSUnseeded = cms.EDProducer("PFClusterProducer",
    energyCorrector = cms.PSet(

    ),
    initialClusteringStep = cms.PSet(
        algoName = cms.string('Basic2DGenericTopoClusterizer'),
        thresholdsByDetector = cms.VPSet(cms.PSet(
            detector = cms.string('PS1'),
            gatheringThreshold = cms.double(6e-05),
            gatheringThresholdPt = cms.double(0.0)
        ), 
            cms.PSet(
                detector = cms.string('PS2'),
                gatheringThreshold = cms.double(6e-05),
                gatheringThresholdPt = cms.double(0.0)
            )),
        useCornerCells = cms.bool(False)
    ),
    pfClusterBuilder = cms.PSet(
        algoName = cms.string('Basic2DGenericPFlowClusterizer'),
        excludeOtherSeeds = cms.bool(True),
        maxIterations = cms.uint32(50),
        minFracTot = cms.double(1e-20),
        minFractionToKeep = cms.double(1e-07),
        positionCalc = cms.PSet(
            algoName = cms.string('Basic2DGenericPFlowPositionCalc'),
            logWeightDenominator = cms.double(6e-05),
            minAllowedNormalization = cms.double(1e-09),
            minFractionInCalc = cms.double(1e-09),
            posCalcNCrystals = cms.int32(-1)
        ),
        recHitEnergyNorms = cms.VPSet(cms.PSet(
            detector = cms.string('PS1'),
            recHitEnergyNorm = cms.double(6e-05)
        ), 
            cms.PSet(
                detector = cms.string('PS2'),
                recHitEnergyNorm = cms.double(6e-05)
            )),
        showerSigma = cms.double(0.3),
        stoppingTolerance = cms.double(1e-08)
    ),
    positionReCalc = cms.PSet(

    ),
    recHitCleaners = cms.VPSet(),
    recHitsSource = cms.InputTag("hltParticleFlowRecHitPSUnseeded"),
    seedFinder = cms.PSet(
        algoName = cms.string('LocalMaximumSeedFinder'),
        nNeighbours = cms.int32(4),
        thresholdsByDetector = cms.VPSet(cms.PSet(
            detector = cms.string('PS1'),
            seedingThreshold = cms.double(0.00012),
            seedingThresholdPt = cms.double(0.0)
        ), 
            cms.PSet(
                detector = cms.string('PS2'),
                seedingThreshold = cms.double(0.00012),
                seedingThresholdPt = cms.double(0.0)
            ))
    )
)


process.hltParticleFlowRecHitECALUnseeded = cms.EDProducer("PFRecHitProducer",
    navigator = cms.PSet(
        barrel = cms.PSet(

        ),
        endcap = cms.PSet(

        ),
        name = cms.string('PFRecHitECALNavigator')
    ),
    producers = cms.VPSet(cms.PSet(
        name = cms.string('PFEBRecHitCreator'),
        qualityTests = cms.VPSet(cms.PSet(
            name = cms.string('PFRecHitQTestThreshold'),
            threshold = cms.double(0.08)
        ), 
            cms.PSet(
                cleaningThreshold = cms.double(2.0),
                name = cms.string('PFRecHitQTestECAL'),
                skipTTRecoveredHits = cms.bool(True),
                timingCleaning = cms.bool(True),
                topologicalCleaning = cms.bool(True)
            )),
        src = cms.InputTag("hltEcalRecHit","EcalRecHitsEB")
    ), 
        cms.PSet(
            name = cms.string('PFEERecHitCreator'),
            qualityTests = cms.VPSet(cms.PSet(
                name = cms.string('PFRecHitQTestThreshold'),
                threshold = cms.double(0.3)
            ), 
                cms.PSet(
                    cleaningThreshold = cms.double(2.0),
                    name = cms.string('PFRecHitQTestECAL'),
                    skipTTRecoveredHits = cms.bool(True),
                    timingCleaning = cms.bool(True),
                    topologicalCleaning = cms.bool(True)
                )),
            src = cms.InputTag("hltEcalRecHit","EcalRecHitsEE")
        ))
)


process.hltParticleFlowRecHitHBHE = cms.EDProducer("PFRecHitProducer",
    navigator = cms.PSet(
        name = cms.string('PFRecHitHCALNavigator'),
        sigmaCut = cms.double(4.0),
        timeResolutionCalc = cms.PSet(
            constantTerm = cms.double(1.92),
            constantTermLowE = cms.double(6.0),
            corrTermLowE = cms.double(0.0),
            noiseTerm = cms.double(8.64),
            noiseTermLowE = cms.double(0.0),
            threshHighE = cms.double(8.0),
            threshLowE = cms.double(2.0)
        )
    ),
    producers = cms.VPSet(cms.PSet(
        name = cms.string('PFHBHERecHitCreator'),
        qualityTests = cms.VPSet(cms.PSet(
            name = cms.string('PFRecHitQTestThreshold'),
            threshold = cms.double(0.8)
        ), 
            cms.PSet(
                cleaningThresholds = cms.vdouble(0.0),
                flags = cms.vstring('Standard'),
                maxSeverities = cms.vint32(11),
                name = cms.string('PFRecHitQTestHCALChannel')
            )),
        src = cms.InputTag("hltHbhereco")
    ))
)


process.hltParticleFlowRecHitHCAL = cms.EDProducer("PFCTRecHitProducer",
    ApplyLongShortDPG = cms.bool(True),
    ApplyPulseDPG = cms.bool(False),
    ApplyTimeDPG = cms.bool(False),
    ECAL_Compensate = cms.bool(False),
    ECAL_Compensation = cms.double(0.5),
    ECAL_Dead_Code = cms.uint32(10),
    ECAL_Threshold = cms.double(10.0),
    EM_Depth = cms.double(22.0),
    HAD_Depth = cms.double(47.0),
    HCAL_Calib = cms.bool(True),
    HCAL_Calib_29 = cms.double(1.35),
    HF_Calib = cms.bool(True),
    HF_Calib_29 = cms.double(1.07),
    HcalMaxAllowedChannelStatusSev = cms.int32(9),
    HcalMaxAllowedHFDigiTimeSev = cms.int32(9),
    HcalMaxAllowedHFInTimeWindowSev = cms.int32(9),
    HcalMaxAllowedHFLongShortSev = cms.int32(9),
    LongFibre_Cut = cms.double(120.0),
    LongFibre_Fraction = cms.double(0.1),
    LongShortFibre_Cut = cms.double(1000000000.0),
    MaxLongTiming_Cut = cms.double(5.0),
    MaxShortTiming_Cut = cms.double(5.0),
    MinLongTiming_Cut = cms.double(-5.0),
    MinShortTiming_Cut = cms.double(-5.0),
    ShortFibre_Cut = cms.double(60.0),
    ShortFibre_Fraction = cms.double(0.01),
    caloTowers = cms.InputTag("hltTowerMakerForPF"),
    hcalRecHitsHBHE = cms.InputTag("hltHbhereco"),
    hcalRecHitsHF = cms.InputTag("hltHfreco"),
    navigation_HF = cms.bool(True),
    navigator = cms.PSet(
        name = cms.string('PFRecHitCaloTowerNavigator')
    ),
    thresh_Barrel = cms.double(0.4),
    thresh_Endcap = cms.double(0.4),
    thresh_HF = cms.double(0.4),
    weight_HFem = cms.double(1.0),
    weight_HFhad = cms.double(1.0)
)


process.hltParticleFlowRecHitHF = cms.EDProducer("PFRecHitProducer",
    navigator = cms.PSet(
        barrel = cms.PSet(

        ),
        endcap = cms.PSet(

        ),
        name = cms.string('PFRecHitHCALNavigator')
    ),
    producers = cms.VPSet(cms.PSet(
        EMDepthCorrection = cms.double(22.0),
        HADDepthCorrection = cms.double(25.0),
        HFCalib29 = cms.double(1.07),
        LongFibre_Cut = cms.double(120.0),
        LongFibre_Fraction = cms.double(0.1),
        ShortFibre_Cut = cms.double(60.0),
        ShortFibre_Fraction = cms.double(0.01),
        name = cms.string('PFHFRecHitCreator'),
        qualityTests = cms.VPSet(cms.PSet(
            cleaningThresholds = cms.vdouble(0.0, 120.0, 60.0),
            flags = cms.vstring('Standard', 
                'HFLong', 
                'HFShort'),
            maxSeverities = cms.vint32(11, 9, 9),
            name = cms.string('PFRecHitQTestHCALChannel')
        ), 
            cms.PSet(
                cuts = cms.VPSet(cms.PSet(
                    depth = cms.int32(1),
                    threshold = cms.double(1.2)
                ), 
                    cms.PSet(
                        depth = cms.int32(2),
                        threshold = cms.double(1.8)
                    )),
                name = cms.string('PFRecHitQTestHCALThresholdVsDepth')
            )),
        src = cms.InputTag("hltHfreco"),
        thresh_HF = cms.double(0.4)
    ))
)


process.hltParticleFlowRecHitPSUnseeded = cms.EDProducer("PFRecHitProducer",
    navigator = cms.PSet(
        name = cms.string('PFRecHitPreshowerNavigator')
    ),
    producers = cms.VPSet(cms.PSet(
        name = cms.string('PFPSRecHitCreator'),
        qualityTests = cms.VPSet(cms.PSet(
            name = cms.string('PFRecHitQTestThreshold'),
            threshold = cms.double(7e-06)
        )),
        src = cms.InputTag("hltEcalPreshowerRecHit","EcalRecHitsES")
    ))
)


process.hltPixelLayerPairs = cms.EDProducer("SeedingLayersEDProducer",
    BPix = cms.PSet(
        HitProducer = cms.string('hltSiPixelRecHits'),
        TTRHBuilder = cms.string('hltESPTTRHBuilderPixelOnly'),
        hitErrorRPhi = cms.double(0.0027),
        hitErrorRZ = cms.double(0.006),
        useErrorsFromParam = cms.bool(True)
    ),
    FPix = cms.PSet(
        HitProducer = cms.string('hltSiPixelRecHits'),
        TTRHBuilder = cms.string('hltESPTTRHBuilderPixelOnly'),
        hitErrorRPhi = cms.double(0.0051),
        hitErrorRZ = cms.double(0.0036),
        useErrorsFromParam = cms.bool(True)
    ),
    MTEC = cms.PSet(

    ),
    MTIB = cms.PSet(

    ),
    MTID = cms.PSet(

    ),
    MTOB = cms.PSet(

    ),
    TEC = cms.PSet(

    ),
    TIB = cms.PSet(

    ),
    TID = cms.PSet(

    ),
    TOB = cms.PSet(

    ),
    layerList = cms.vstring('BPix1+BPix2', 
        'BPix1+BPix3', 
        'BPix2+BPix3', 
        'BPix1+FPix1_pos', 
        'BPix1+FPix1_neg', 
        'BPix1+FPix2_pos', 
        'BPix1+FPix2_neg', 
        'BPix2+FPix1_pos', 
        'BPix2+FPix1_neg', 
        'BPix2+FPix2_pos', 
        'BPix2+FPix2_neg', 
        'FPix1_pos+FPix2_pos', 
        'FPix1_neg+FPix2_neg')
)


process.hltPixelLayerTriplets = cms.EDProducer("SeedingLayersEDProducer",
    BPix = cms.PSet(
        HitProducer = cms.string('hltSiPixelRecHits'),
        TTRHBuilder = cms.string('hltESPTTRHBuilderPixelOnly'),
        hitErrorRPhi = cms.double(0.0027),
        hitErrorRZ = cms.double(0.006),
        useErrorsFromParam = cms.bool(True)
    ),
    FPix = cms.PSet(
        HitProducer = cms.string('hltSiPixelRecHits'),
        TTRHBuilder = cms.string('hltESPTTRHBuilderPixelOnly'),
        hitErrorRPhi = cms.double(0.0051),
        hitErrorRZ = cms.double(0.0036),
        useErrorsFromParam = cms.bool(True)
    ),
    MTEC = cms.PSet(

    ),
    MTIB = cms.PSet(

    ),
    MTID = cms.PSet(

    ),
    MTOB = cms.PSet(

    ),
    TEC = cms.PSet(

    ),
    TIB = cms.PSet(

    ),
    TID = cms.PSet(

    ),
    TOB = cms.PSet(

    ),
    layerList = cms.vstring('BPix1+BPix2+BPix3', 
        'BPix1+BPix2+FPix1_pos', 
        'BPix1+BPix2+FPix1_neg', 
        'BPix1+FPix1_pos+FPix2_pos', 
        'BPix1+FPix1_neg+FPix2_neg')
)


process.hltPixelTracks = cms.EDProducer("PixelTrackProducer",
    CleanerPSet = cms.PSet(
        ComponentName = cms.string('PixelTrackCleanerBySharedHits')
    ),
    FilterPSet = cms.PSet(
        ComponentName = cms.string('PixelTrackFilterByKinematics'),
        chi2 = cms.double(1000.0),
        nSigmaInvPtTolerance = cms.double(0.0),
        nSigmaTipMaxTolerance = cms.double(0.0),
        ptMin = cms.double(0.1),
        tipMax = cms.double(1.0)
    ),
    FitterPSet = cms.PSet(
        ComponentName = cms.string('PixelFitterByHelixProjections'),
        TTRHBuilder = cms.string('hltESPTTRHBuilderPixelOnly'),
        fixImpactParameter = cms.double(0.0)
    ),
    OrderedHitsFactoryPSet = cms.PSet(
        ComponentName = cms.string('StandardHitTripletGenerator'),
        GeneratorPSet = cms.PSet(
            ComponentName = cms.string('PixelTripletHLTGenerator'),
            SeedComparitorPSet = cms.PSet(
                ComponentName = cms.string('LowPtClusterShapeSeedComparitor'),
                clusterShapeCacheSrc = cms.InputTag("hltSiPixelClustersCache")
            ),
            extraHitRPhitolerance = cms.double(0.06),
            extraHitRZtolerance = cms.double(0.06),
            maxElement = cms.uint32(100000),
            phiPreFiltering = cms.double(0.3),
            useBending = cms.bool(True),
            useFixedPreFiltering = cms.bool(False),
            useMultScattering = cms.bool(True)
        ),
        SeedingLayers = cms.InputTag("hltPixelLayerTriplets")
    ),
    RegionFactoryPSet = cms.PSet(
        ComponentName = cms.string('GlobalRegionProducerFromBeamSpot'),
        RegionPSet = cms.PSet(
            beamSpot = cms.InputTag("hltOnlineBeamSpot"),
            originHalfLength = cms.double(24.0),
            originRadius = cms.double(0.2),
            precise = cms.bool(True),
            ptMin = cms.double(0.9),
            useFakeVertices = cms.bool(False),
            useMultipleScattering = cms.bool(False)
        )
    ),
    passLabel = cms.string(''),
    useFilterWithES = cms.bool(False)
)


process.hltPixelVertices = cms.EDProducer("PixelVertexProducer",
    Finder = cms.string('DivisiveVertexFinder'),
    Method2 = cms.bool(True),
    NTrkMin = cms.int32(2),
    PVcomparer = cms.PSet(
        refToPSet_ = cms.string('HLTPSetPvClusterComparerForIT')
    ),
    PtMin = cms.double(1.0),
    TrackCollection = cms.InputTag("hltPixelTracks"),
    UseError = cms.bool(True),
    Verbosity = cms.int32(0),
    WtAverage = cms.bool(True),
    ZOffset = cms.double(5.0),
    ZSeparation = cms.double(0.05),
    beamSpot = cms.InputTag("hltOnlineBeamSpot")
)


process.hltRpcRecHits = cms.EDProducer("RPCRecHitProducer",
    deadSource = cms.string('File'),
    deadvecfile = cms.FileInPath('RecoLocalMuon/RPCRecHit/data/RPCDeadVec.dat'),
    maskSource = cms.string('File'),
    maskvecfile = cms.FileInPath('RecoLocalMuon/RPCRecHit/data/RPCMaskVec.dat'),
    recAlgo = cms.string('RPCRecHitStandardAlgo'),
    recAlgoConfig = cms.PSet(

    ),
    rpcDigiLabel = cms.InputTag("hltMuonRPCDigis")
)


process.hltScalersRawToDigi = cms.EDProducer("ScalersRawToDigi",
    scalersInputTag = cms.InputTag("rawDataCollector")
)


process.hltSiPixelClusters = cms.EDProducer("SiPixelClusterProducer",
    ChannelThreshold = cms.int32(1000),
    ClusterThreshold = cms.double(4000.0),
    MissCalibrate = cms.untracked.bool(True),
    SeedThreshold = cms.int32(1000),
    SplitClusters = cms.bool(False),
    VCaltoElectronGain = cms.int32(65),
    VCaltoElectronOffset = cms.int32(-414),
    maxNumberOfClusters = cms.int32(20000),
    payloadType = cms.string('HLT'),
    src = cms.InputTag("hltSiPixelDigis")
)


process.hltSiPixelClustersCache = cms.EDProducer("SiPixelClusterShapeCacheProducer",
    onDemand = cms.bool(False),
    src = cms.InputTag("hltSiPixelClusters")
)


process.hltSiPixelDigis = cms.EDProducer("SiPixelRawToDigi",
    CablingMapLabel = cms.string(''),
    ErrorList = cms.vint32(),
    IncludeErrors = cms.bool(False),
    InputLabel = cms.InputTag("rawDataCollector"),
    Regions = cms.PSet(

    ),
    Timing = cms.untracked.bool(False),
    UsePhase1 = cms.bool(False),
    UsePilotBlade = cms.bool(False),
    UseQualityInfo = cms.bool(False),
    UserErrorList = cms.vint32()
)


process.hltSiPixelRecHits = cms.EDProducer("SiPixelRecHitConverter",
    CPE = cms.string('hltESPPixelCPEGeneric'),
    VerboseLevel = cms.untracked.int32(0),
    src = cms.InputTag("hltSiPixelClusters")
)


process.hltSiStripClusters = cms.EDProducer("MeasurementTrackerEventProducer",
    inactivePixelDetectorLabels = cms.VInputTag(),
    inactiveStripDetectorLabels = cms.VInputTag("hltSiStripExcludedFEDListProducer"),
    measurementTracker = cms.string('hltESPMeasurementTracker'),
    pixelClusterProducer = cms.string('hltSiPixelClusters'),
    skipClusters = cms.InputTag(""),
    stripClusterProducer = cms.string('hltSiStripRawToClustersFacility'),
    switchOffPixelsIfEmpty = cms.bool(True)
)


process.hltSiStripExcludedFEDListProducer = cms.EDProducer("SiStripExcludedFEDListProducer",
    ProductLabel = cms.InputTag("rawDataCollector")
)


process.hltSiStripRawToClustersFacility = cms.EDProducer("SiStripClusterizerFromRaw",
    Algorithms = cms.PSet(
        CommonModeNoiseSubtractionMode = cms.string('Median'),
        PedestalSubtractionFedMode = cms.bool(True),
        SiStripFedZeroSuppressionMode = cms.uint32(4),
        TruncateInSuppressor = cms.bool(True),
        doAPVRestore = cms.bool(False),
        useCMMeanMap = cms.bool(False)
    ),
    Clusterizer = cms.PSet(
        Algorithm = cms.string('ThreeThresholdAlgorithm'),
        ChannelThreshold = cms.double(2.0),
        ClusterThreshold = cms.double(5.0),
        MaxAdjacentBad = cms.uint32(0),
        MaxSequentialBad = cms.uint32(1),
        MaxSequentialHoles = cms.uint32(0),
        QualityLabel = cms.string(''),
        RemoveApvShots = cms.bool(True),
        SeedThreshold = cms.double(3.0),
        clusterChargeCut = cms.PSet(
            refToPSet_ = cms.string('HLTSiStripClusterChargeCutNone')
        ),
        setDetId = cms.bool(True)
    ),
    DoAPVEmulatorCheck = cms.bool(False),
    ProductLabel = cms.InputTag("rawDataCollector"),
    onDemand = cms.bool(True)
)


process.hltTowerMakerForAll = cms.EDProducer("CaloTowersCreator",
    AllowMissingInputs = cms.bool(False),
    EBGrid = cms.vdouble(),
    EBSumThreshold = cms.double(0.2),
    EBThreshold = cms.double(0.07),
    EBWeight = cms.double(1.0),
    EBWeights = cms.vdouble(),
    EEGrid = cms.vdouble(),
    EESumThreshold = cms.double(0.45),
    EEThreshold = cms.double(0.3),
    EEWeight = cms.double(1.0),
    EEWeights = cms.vdouble(),
    EcalRecHitSeveritiesToBeExcluded = cms.vstring('kTime', 
        'kWeird', 
        'kBad'),
    EcalSeveritiesToBeUsedInBadTowers = cms.vstring(),
    EcutTower = cms.double(-1000.0),
    HBGrid = cms.vdouble(),
    HBThreshold = cms.double(0.7),
    HBWeight = cms.double(1.0),
    HBWeights = cms.vdouble(),
    HEDGrid = cms.vdouble(),
    HEDThreshold = cms.double(0.8),
    HEDWeight = cms.double(1.0),
    HEDWeights = cms.vdouble(),
    HESGrid = cms.vdouble(),
    HESThreshold = cms.double(0.8),
    HESWeight = cms.double(1.0),
    HESWeights = cms.vdouble(),
    HF1Grid = cms.vdouble(),
    HF1Threshold = cms.double(0.5),
    HF1Weight = cms.double(1.0),
    HF1Weights = cms.vdouble(),
    HF2Grid = cms.vdouble(),
    HF2Threshold = cms.double(0.85),
    HF2Weight = cms.double(1.0),
    HF2Weights = cms.vdouble(),
    HOGrid = cms.vdouble(),
    HOThreshold0 = cms.double(3.5),
    HOThresholdMinus1 = cms.double(3.5),
    HOThresholdMinus2 = cms.double(3.5),
    HOThresholdPlus1 = cms.double(3.5),
    HOThresholdPlus2 = cms.double(3.5),
    HOWeight = cms.double(1e-99),
    HOWeights = cms.vdouble(),
    HcalAcceptSeverityLevel = cms.uint32(9),
    HcalAcceptSeverityLevelForRejectedHit = cms.uint32(9999),
    HcalPhase = cms.int32(0),
    HcalThreshold = cms.double(-1000.0),
    MomConstrMethod = cms.int32(1),
    MomEBDepth = cms.double(0.3),
    MomEEDepth = cms.double(0.0),
    MomHBDepth = cms.double(0.2),
    MomHEDepth = cms.double(0.4),
    UseEcalRecoveredHits = cms.bool(False),
    UseEtEBTreshold = cms.bool(False),
    UseEtEETreshold = cms.bool(False),
    UseHO = cms.bool(False),
    UseHcalRecoveredHits = cms.bool(False),
    UseRejectedHitsOnly = cms.bool(False),
    UseRejectedRecoveredEcalHits = cms.bool(False),
    UseRejectedRecoveredHcalHits = cms.bool(False),
    UseSymEBTreshold = cms.bool(False),
    UseSymEETreshold = cms.bool(False),
    ecalInputs = cms.VInputTag("hltEcalRecHit:EcalRecHitsEB", "hltEcalRecHit:EcalRecHitsEE"),
    hbheInput = cms.InputTag("hltHbhereco"),
    hfInput = cms.InputTag("hltHfreco"),
    hoInput = cms.InputTag("hltHoreco")
)


process.hltTowerMakerForPF = cms.EDProducer("CaloTowersCreator",
    AllowMissingInputs = cms.bool(False),
    EBGrid = cms.vdouble(),
    EBSumThreshold = cms.double(0.2),
    EBThreshold = cms.double(0.07),
    EBWeight = cms.double(1.0),
    EBWeights = cms.vdouble(),
    EEGrid = cms.vdouble(),
    EESumThreshold = cms.double(0.45),
    EEThreshold = cms.double(0.3),
    EEWeight = cms.double(1.0),
    EEWeights = cms.vdouble(),
    EcalRecHitSeveritiesToBeExcluded = cms.vstring('kTime', 
        'kWeird', 
        'kBad'),
    EcalSeveritiesToBeUsedInBadTowers = cms.vstring(),
    EcutTower = cms.double(-1000.0),
    HBGrid = cms.vdouble(),
    HBThreshold = cms.double(0.4),
    HBWeight = cms.double(1.0),
    HBWeights = cms.vdouble(),
    HEDGrid = cms.vdouble(),
    HEDThreshold = cms.double(0.4),
    HEDWeight = cms.double(1.0),
    HEDWeights = cms.vdouble(),
    HESGrid = cms.vdouble(),
    HESThreshold = cms.double(0.4),
    HESWeight = cms.double(1.0),
    HESWeights = cms.vdouble(),
    HF1Grid = cms.vdouble(),
    HF1Threshold = cms.double(1.2),
    HF1Weight = cms.double(1.0),
    HF1Weights = cms.vdouble(),
    HF2Grid = cms.vdouble(),
    HF2Threshold = cms.double(1.8),
    HF2Weight = cms.double(1.0),
    HF2Weights = cms.vdouble(),
    HOGrid = cms.vdouble(),
    HOThreshold0 = cms.double(1.1),
    HOThresholdMinus1 = cms.double(1.1),
    HOThresholdMinus2 = cms.double(1.1),
    HOThresholdPlus1 = cms.double(1.1),
    HOThresholdPlus2 = cms.double(1.1),
    HOWeight = cms.double(1.0),
    HOWeights = cms.vdouble(),
    HcalAcceptSeverityLevel = cms.uint32(11),
    HcalAcceptSeverityLevelForRejectedHit = cms.uint32(9999),
    HcalPhase = cms.int32(0),
    HcalThreshold = cms.double(-1000.0),
    MomConstrMethod = cms.int32(1),
    MomEBDepth = cms.double(0.3),
    MomEEDepth = cms.double(0.0),
    MomHBDepth = cms.double(0.2),
    MomHEDepth = cms.double(0.4),
    UseEcalRecoveredHits = cms.bool(False),
    UseEtEBTreshold = cms.bool(False),
    UseEtEETreshold = cms.bool(False),
    UseHO = cms.bool(False),
    UseHcalRecoveredHits = cms.bool(True),
    UseRejectedHitsOnly = cms.bool(False),
    UseRejectedRecoveredEcalHits = cms.bool(False),
    UseRejectedRecoveredHcalHits = cms.bool(False),
    UseSymEBTreshold = cms.bool(False),
    UseSymEETreshold = cms.bool(False),
    ecalInputs = cms.VInputTag("hltEcalRecHit:EcalRecHitsEB", "hltEcalRecHit:EcalRecHitsEE"),
    hbheInput = cms.InputTag("hltHbhereco"),
    hfInput = cms.InputTag("hltHfreco"),
    hoInput = cms.InputTag("hltHoreco")
)


process.hltTrackIter0RefsForJets4Iter1 = cms.EDProducer("ChargedRefCandidateProducer",
    particleType = cms.string('pi+'),
    src = cms.InputTag("hltIter0PFlowTrackSelectionHighPurity")
)


process.hltTriggerSummaryAOD = cms.EDProducer("TriggerSummaryProducerAOD",
    processName = cms.string('@')
)


process.hltTriggerSummaryRAW = cms.EDProducer("TriggerSummaryProducerRAW",
    processName = cms.string('@')
)


process.hltTrimmedPixelVertices = cms.EDProducer("PixelVertexCollectionTrimmer",
    PVcomparer = cms.PSet(
        refToPSet_ = cms.string('HLTPSetPvClusterComparerForIT')
    ),
    fractionSumPt2 = cms.double(0.3),
    maxVtx = cms.uint32(100),
    minSumPt2 = cms.double(0.0),
    src = cms.InputTag("hltPixelVertices")
)


process.packCaloStage2 = cms.EDProducer("L1TDigiToRaw",
    FWId = cms.uint32(1),
    FedId = cms.int32(1366),
    InputLabel = cms.InputTag("simCaloStage2Digis"),
    Setup = cms.string('stage2::CaloSetup'),
    TowerInputLabel = cms.InputTag("simCaloStage2Layer1Digis"),
    lenSlinkHeader = cms.untracked.int32(8),
    lenSlinkTrailer = cms.untracked.int32(8)
)


process.packGmtStage2 = cms.EDProducer("L1TDigiToRaw",
    BMTFInputLabel = cms.InputTag("simBmtfDigis","BMTF"),
    EMTFInputLabel = cms.InputTag("simEmtfDigis","EMTF"),
    FWId = cms.uint32(1),
    FedId = cms.int32(1402),
    ImdInputLabelBMTF = cms.InputTag("simGmtStage2Digis","imdMuonsBMTF"),
    ImdInputLabelEMTFNeg = cms.InputTag("simGmtStage2Digis","imdMuonsEMTFNeg"),
    ImdInputLabelEMTFPos = cms.InputTag("simGmtStage2Digis","imdMuonsEMTFPos"),
    ImdInputLabelOMTFNeg = cms.InputTag("simGmtStage2Digis","imdMuonsOMTFNeg"),
    ImdInputLabelOMTFPos = cms.InputTag("simGmtStage2Digis","imdMuonsOMTFPos"),
    InputLabel = cms.InputTag("simGmtStage2Digis"),
    OMTFInputLabel = cms.InputTag("simOmtfDigis","OMTF"),
    Setup = cms.string('stage2::GMTSetup'),
    lenSlinkHeader = cms.untracked.int32(8),
    lenSlinkTrailer = cms.untracked.int32(8)
)


process.packGtStage2 = cms.EDProducer("L1TDigiToRaw",
    EGammaInputTag = cms.InputTag("simCaloStage2Digis"),
    EtSumInputTag = cms.InputTag("simCaloStage2Digis"),
    ExtInputTag = cms.InputTag("simGtExtFakeStage2Digis"),
    FWId = cms.uint32(1),
    FedId = cms.int32(1404),
    GtInputTag = cms.InputTag("simGtStage2Digis"),
    JetInputTag = cms.InputTag("simCaloStage2Digis"),
    MuonInputTag = cms.InputTag("simGmtStage2Digis"),
    Setup = cms.string('stage2::GTSetup'),
    TauInputTag = cms.InputTag("simCaloStage2Digis"),
    lenSlinkHeader = cms.untracked.int32(8),
    lenSlinkTrailer = cms.untracked.int32(8)
)


process.patTriggerCustom = cms.EDProducer("PATTriggerProducer",
    TriggerResults = cms.InputTag("TriggerResults","","TEST"),
    hltTriggerSummaryAOD = cms.InputTag("hltTriggerSummaryAOD","","TEST"),
    l1tAlgBlkInputTag = cms.InputTag("hltgtStage2Digis","","TEST"),
    l1tExtBlkInputTag = cms.InputTag("hltgtStage2Digis","","TEST"),
    onlyStandAlone = cms.bool(True),
    packTriggerPathNames = cms.bool(True),
    processName = cms.string('TEST')
)


process.rawDataCollector = cms.EDProducer("RawDataCollectorByLabel",
    RawCollectionList = cms.VInputTag(cms.InputTag("packCaloStage2"), cms.InputTag("packGmtStage2"), cms.InputTag("packGtStage2"), cms.InputTag("rawDataCollector","","@skipCurrentProcess")),
    verbose = cms.untracked.int32(0)
)


process.simBmtfDigis = cms.EDProducer("L1TMuonBarrelTrackProducer",
    DTDigi_Source = cms.InputTag("simTwinMuxDigis"),
    DTDigi_Theta_Source = cms.InputTag("simDtTriggerPrimitiveDigis"),
    Debug = cms.untracked.int32(0)
)


process.simCaloStage2Digis = cms.EDProducer("L1TStage2Layer2Producer",
    firmware = cms.int32(1),
    towerToken = cms.InputTag("simCaloStage2Layer1Digis")
)


process.simCaloStage2Layer1Digis = cms.EDProducer("L1TCaloLayer1",
    ecalToken = cms.InputTag("unpackEcal","EcalTriggerPrimitives"),
    firmwareVersion = cms.int32(1),
    hcalToken = cms.InputTag("simHcalTriggerPrimitiveDigis"),
    unpackEcalMask = cms.bool(False),
    unpackHcalMask = cms.bool(False),
    useCalib = cms.bool(True),
    useECALLUT = cms.bool(True),
    useHCALLUT = cms.bool(True),
    useHFLUT = cms.bool(True),
    useLSB = cms.bool(True),
    verbose = cms.bool(False)
)


process.simCscTriggerPrimitiveDigis = cms.EDProducer("CSCTriggerPrimitivesProducer",
    CSCComparatorDigiProducer = cms.InputTag("unpackCSC","MuonCSCComparatorDigi"),
    CSCWireDigiProducer = cms.InputTag("unpackCSC","MuonCSCWireDigi"),
    GEMPadDigiProducer = cms.InputTag(""),
    MaxBX = cms.int32(9),
    MinBX = cms.int32(3),
    RPCDigiProducer = cms.InputTag(""),
    alctParam07 = cms.PSet(
        alctAccelMode = cms.uint32(0),
        alctDriftDelay = cms.uint32(2),
        alctEarlyTbins = cms.int32(4),
        alctFifoPretrig = cms.uint32(10),
        alctFifoTbins = cms.uint32(16),
        alctGhostCancellationBxDepth = cms.int32(4),
        alctGhostCancellationSideQuality = cms.bool(False),
        alctHitPersist = cms.uint32(6),
        alctL1aWindowWidth = cms.uint32(7),
        alctNarrowMaskForR1 = cms.bool(False),
        alctNplanesHitAccelPattern = cms.uint32(4),
        alctNplanesHitAccelPretrig = cms.uint32(3),
        alctNplanesHitPattern = cms.uint32(4),
        alctNplanesHitPretrig = cms.uint32(3),
        alctPretrigDeadtime = cms.uint32(4),
        alctTrigMode = cms.uint32(2),
        alctUseCorrectedBx = cms.bool(False),
        verbosity = cms.int32(0)
    ),
    alctParamMTCC = cms.PSet(
        alctAccelMode = cms.uint32(0),
        alctDriftDelay = cms.uint32(3),
        alctFifoPretrig = cms.uint32(10),
        alctFifoTbins = cms.uint32(16),
        alctL1aWindowWidth = cms.uint32(3),
        alctNplanesHitAccelPattern = cms.uint32(4),
        alctNplanesHitAccelPretrig = cms.uint32(2),
        alctNplanesHitPattern = cms.uint32(4),
        alctNplanesHitPretrig = cms.uint32(2),
        alctTrigMode = cms.uint32(2),
        verbosity = cms.int32(0)
    ),
    alctParamOldMC = cms.PSet(
        alctAccelMode = cms.uint32(1),
        alctDriftDelay = cms.uint32(3),
        alctFifoPretrig = cms.uint32(10),
        alctFifoTbins = cms.uint32(16),
        alctL1aWindowWidth = cms.uint32(5),
        alctNplanesHitAccelPattern = cms.uint32(4),
        alctNplanesHitAccelPretrig = cms.uint32(2),
        alctNplanesHitPattern = cms.uint32(4),
        alctNplanesHitPretrig = cms.uint32(2),
        alctTrigMode = cms.uint32(3),
        verbosity = cms.int32(0)
    ),
    alctSLHC = cms.PSet(
        alctAccelMode = cms.uint32(0),
        alctDriftDelay = cms.uint32(2),
        alctEarlyTbins = cms.int32(4),
        alctFifoPretrig = cms.uint32(10),
        alctFifoTbins = cms.uint32(16),
        alctGhostCancellationBxDepth = cms.int32(1),
        alctGhostCancellationSideQuality = cms.bool(True),
        alctHitPersist = cms.uint32(6),
        alctL1aWindowWidth = cms.uint32(7),
        alctNarrowMaskForR1 = cms.bool(True),
        alctNplanesHitAccelPattern = cms.uint32(4),
        alctNplanesHitAccelPretrig = cms.uint32(3),
        alctNplanesHitPattern = cms.uint32(4),
        alctNplanesHitPretrig = cms.uint32(3),
        alctPretrigDeadtime = cms.uint32(0),
        alctTrigMode = cms.uint32(2),
        alctUseCorrectedBx = cms.bool(True),
        verbosity = cms.int32(0)
    ),
    checkBadChambers = cms.bool(False),
    clctParam07 = cms.PSet(
        clctDriftDelay = cms.uint32(2),
        clctFifoPretrig = cms.uint32(7),
        clctFifoTbins = cms.uint32(12),
        clctHitPersist = cms.uint32(4),
        clctMinSeparation = cms.uint32(10),
        clctNplanesHitPattern = cms.uint32(4),
        clctNplanesHitPretrig = cms.uint32(3),
        clctPidThreshPretrig = cms.uint32(2),
        clctStartBxShift = cms.int32(0),
        verbosity = cms.int32(0)
    ),
    clctParamMTCC = cms.PSet(
        clctDriftDelay = cms.uint32(2),
        clctFifoPretrig = cms.uint32(7),
        clctFifoTbins = cms.uint32(12),
        clctHitPersist = cms.uint32(6),
        clctMinSeparation = cms.uint32(10),
        clctNplanesHitPattern = cms.uint32(1),
        clctNplanesHitPretrig = cms.uint32(4),
        clctPidThreshPretrig = cms.uint32(2),
        verbosity = cms.int32(0)
    ),
    clctParamOldMC = cms.PSet(
        clctDriftDelay = cms.uint32(2),
        clctFifoPretrig = cms.uint32(7),
        clctFifoTbins = cms.uint32(12),
        clctHitPersist = cms.uint32(6),
        clctMinSeparation = cms.uint32(10),
        clctNplanesHitPattern = cms.uint32(4),
        clctNplanesHitPretrig = cms.uint32(2),
        clctPidThreshPretrig = cms.uint32(2),
        verbosity = cms.int32(0)
    ),
    clctSLHC = cms.PSet(
        clctDriftDelay = cms.uint32(2),
        clctFifoPretrig = cms.uint32(7),
        clctFifoTbins = cms.uint32(12),
        clctHitPersist = cms.uint32(4),
        clctMinSeparation = cms.uint32(5),
        clctNplanesHitPattern = cms.uint32(4),
        clctNplanesHitPretrig = cms.uint32(3),
        clctPidThreshPretrig = cms.uint32(4),
        clctPretriggerTriggerZone = cms.uint32(5),
        clctStartBxShift = cms.int32(0),
        clctStateMachineZone = cms.uint32(8),
        clctUseCorrectedBx = cms.bool(True),
        useDeadTimeZoning = cms.bool(True),
        useDynamicStateMachineZone = cms.bool(True),
        verbosity = cms.int32(0)
    ),
    commonParam = cms.PSet(
        disableME1a = cms.bool(False),
        disableME42 = cms.bool(False),
        gangedME1a = cms.bool(False),
        isMTCC = cms.bool(False),
        isSLHC = cms.bool(False),
        isTMB07 = cms.bool(True),
        runME11ILT = cms.bool(False),
        runME21ILT = cms.bool(False),
        runME3141ILT = cms.bool(False),
        smartME1aME1b = cms.bool(False)
    ),
    debugParameters = cms.bool(True),
    mpcRun2 = cms.PSet(
        dropInvalidStubs = cms.bool(False),
        dropLowQualityStubs = cms.bool(False),
        sortStubs = cms.bool(False)
    ),
    mpcSLHC = cms.PSet(
        dropInvalidStubs = cms.bool(False),
        dropLowQualityStubs = cms.bool(False),
        mpcMaxStubs = cms.uint32(18),
        sortStubs = cms.bool(False)
    ),
    tmbParam = cms.PSet(
        alctTrigEnable = cms.uint32(0),
        clctTrigEnable = cms.uint32(0),
        matchTrigEnable = cms.uint32(1),
        matchTrigWindowSize = cms.uint32(7),
        mpcBlockMe1a = cms.uint32(0),
        tmbDropUsedAlcts = cms.bool(True),
        tmbEarlyTbins = cms.int32(4),
        tmbL1aWindowSize = cms.uint32(7),
        tmbReadoutEarliest2 = cms.bool(True),
        verbosity = cms.int32(0)
    ),
    tmbSLHC = cms.PSet(
        alctTrigEnable = cms.uint32(0),
        clctToAlct = cms.bool(False),
        clctTrigEnable = cms.uint32(0),
        matchEarliestAlctME11Only = cms.bool(False),
        matchEarliestClctME11Only = cms.bool(False),
        matchTrigEnable = cms.uint32(1),
        matchTrigWindowSize = cms.uint32(3),
        maxME11LCTs = cms.uint32(2),
        mpcBlockMe1a = cms.uint32(0),
        tmbCrossBxAlgorithm = cms.uint32(1),
        tmbDropUsedAlcts = cms.bool(False),
        tmbDropUsedClcts = cms.bool(False),
        tmbEarlyTbins = cms.int32(4),
        tmbL1aWindowSize = cms.uint32(7),
        tmbReadoutEarliest2 = cms.bool(False),
        verbosity = cms.int32(0)
    )
)


process.simDtTriggerPrimitiveDigis = cms.EDProducer("DTTrigProd",
    DTTFSectorNumbering = cms.bool(True),
    debug = cms.untracked.bool(False),
    digiTag = cms.InputTag("unpackDT"),
    lutBtic = cms.untracked.int32(31),
    lutDumpFlag = cms.untracked.bool(False)
)


process.simEmtfDigis = cms.EDProducer("L1TMuonEndCapTrackProducer",
    CSCInput = cms.InputTag("simCscTriggerPrimitiveDigis","MPCSORTED"),
    CSCInputBxShift = cms.untracked.int32(0),
    RPCInput = cms.InputTag("unpackRPC")
)


process.simGmtCaloSumDigis = cms.EDProducer("L1TMuonCaloSumProducer",
    caloStage2Layer2Label = cms.InputTag("simCaloStage2Layer1Digis")
)


process.simGmtStage2Digis = cms.EDProducer("L1TMuonProducer",
    autoBxRange = cms.bool(True),
    barrelTFInput = cms.InputTag("simBmtfDigis","BMTF"),
    bxMax = cms.int32(2),
    bxMin = cms.int32(-2),
    forwardTFInput = cms.InputTag("simEmtfDigis","EMTF"),
    overlapTFInput = cms.InputTag("simOmtfDigis","OMTF"),
    triggerTowerInput = cms.InputTag("simGmtCaloSumDigis","TriggerTowerSums")
)


process.simGtExtFakeStage2Digis = cms.EDProducer("L1TExtCondProducer",
    bxFirst = cms.int32(-2),
    bxLast = cms.int32(2),
    setBptxAND = cms.bool(True),
    setBptxMinus = cms.bool(True),
    setBptxOR = cms.bool(True),
    setBptxPlus = cms.bool(True)
)


process.simGtStage2Digis = cms.EDProducer("L1TGlobalProducer",
    AlgorithmTriggersUnmasked = cms.bool(True),
    AlgorithmTriggersUnprescaled = cms.bool(True),
    EGammaInputTag = cms.InputTag("simCaloStage2Digis"),
    EtSumInputTag = cms.InputTag("simCaloStage2Digis"),
    ExtInputTag = cms.InputTag("simGtExtFakeStage2Digis"),
    JetInputTag = cms.InputTag("simCaloStage2Digis"),
    MuonInputTag = cms.InputTag("simGmtStage2Digis"),
    TauInputTag = cms.InputTag("simCaloStage2Digis")
)


process.simHcalTriggerPrimitiveDigis = cms.EDProducer("HcalTrigPrimDigiProducer",
    FG_HF_threshold = cms.uint32(17),
    FG_threshold = cms.uint32(12),
    FrontEndFormatError = cms.bool(False),
    HFTPScaleShift = cms.PSet(
        NCT = cms.int32(1),
        RCT = cms.int32(3)
    ),
    InputTagFEDRaw = cms.InputTag("rawDataCollector"),
    LSConfig = cms.untracked.PSet(
        HcalFeatureHFEMBit = cms.bool(False),
        Long_Short_Offset = cms.double(10.1),
        Long_vrs_Short_Slope = cms.double(100.2),
        Min_Long_Energy = cms.double(10),
        Min_Short_Energy = cms.double(10)
    ),
    MinSignalThreshold = cms.uint32(0),
    PMTNoiseThreshold = cms.uint32(0),
    PeakFinderAlgorithm = cms.int32(2),
    RunZS = cms.bool(False),
    ZS_threshold = cms.uint32(1),
    inputLabel = cms.VInputTag(cms.InputTag("unpackHcal"), cms.InputTag("unpackHcal")),
    latency = cms.int32(1),
    numberOfPresamples = cms.int32(2),
    numberOfPresamplesHF = cms.int32(2),
    numberOfSamples = cms.int32(4),
    numberOfSamplesHF = cms.int32(4),
    peakFilter = cms.bool(True),
    weights = cms.vdouble(1.0, 1.0)
)


process.simOmtfDigis = cms.EDProducer("L1TMuonOverlapTrackProducer",
    XMLDumpFileName = cms.string('TestEvents.xml'),
    dropCSCPrimitives = cms.bool(False),
    dropDTPrimitives = cms.bool(False),
    dropRPCPrimitives = cms.bool(False),
    dumpDetailedResultToXML = cms.bool(False),
    dumpGPToXML = cms.bool(False),
    dumpResultToXML = cms.bool(False),
    eventsXMLFiles = cms.vstring('TestEvents.xml'),
    readEventsFromXML = cms.bool(False),
    srcCSC = cms.InputTag("simCscTriggerPrimitiveDigis","MPCSORTED"),
    srcDTPh = cms.InputTag("simDtTriggerPrimitiveDigis"),
    srcDTTh = cms.InputTag("simDtTriggerPrimitiveDigis"),
    srcRPC = cms.InputTag("unpackRPC")
)


process.simTwinMuxDigis = cms.EDProducer("L1TTwinMuxProducer",
    DTDigi_Source = cms.InputTag("simDtTriggerPrimitiveDigis"),
    DTThetaDigi_Source = cms.InputTag("simDtTriggerPrimitiveDigis"),
    RPC_Source = cms.InputTag("unpackRPC")
)


process.unpackCSC = cms.EDProducer("CSCDCCUnpacker",
    Debug = cms.untracked.bool(False),
    ErrorMask = cms.uint32(0),
    ExaminerMask = cms.uint32(535557110),
    FormatedEventDump = cms.untracked.bool(False),
    InputObjects = cms.InputTag("rawDataCollector","","@skipCurrentProcess"),
    PrintEventNumber = cms.untracked.bool(False),
    SuppressZeroLCT = cms.untracked.bool(True),
    UnpackStatusDigis = cms.bool(False),
    UseExaminer = cms.bool(True),
    UseFormatStatus = cms.bool(True),
    UseSelectiveUnpacking = cms.bool(True),
    VisualFEDInspect = cms.untracked.bool(False),
    VisualFEDShort = cms.untracked.bool(False),
    runDQM = cms.untracked.bool(False)
)


process.unpackDT = cms.EDProducer("DTUnpackingModule",
    dataType = cms.string('DDU'),
    dqmOnly = cms.bool(False),
    inputLabel = cms.InputTag("rawDataCollector","","@skipCurrentProcess"),
    maxFEDid = cms.untracked.int32(779),
    minFEDid = cms.untracked.int32(770),
    readOutParameters = cms.PSet(
        debug = cms.untracked.bool(False),
        localDAQ = cms.untracked.bool(False),
        performDataIntegrityMonitor = cms.untracked.bool(False),
        rosParameters = cms.PSet(
            debug = cms.untracked.bool(False),
            localDAQ = cms.untracked.bool(False),
            performDataIntegrityMonitor = cms.untracked.bool(False),
            readDDUIDfromDDU = cms.untracked.bool(True),
            readingDDU = cms.untracked.bool(True),
            writeSC = cms.untracked.bool(True)
        )
    ),
    useStandardFEDid = cms.bool(True)
)


process.unpackEcal = cms.EDProducer("EcalRawToDigi",
    DoRegional = cms.bool(False),
    FEDs = cms.vint32(601, 602, 603, 604, 605, 
        606, 607, 608, 609, 610, 
        611, 612, 613, 614, 615, 
        616, 617, 618, 619, 620, 
        621, 622, 623, 624, 625, 
        626, 627, 628, 629, 630, 
        631, 632, 633, 634, 635, 
        636, 637, 638, 639, 640, 
        641, 642, 643, 644, 645, 
        646, 647, 648, 649, 650, 
        651, 652, 653, 654),
    FedLabel = cms.InputTag("listfeds"),
    InputLabel = cms.InputTag("rawDataCollector","","@skipCurrentProcess"),
    eventPut = cms.bool(True),
    feIdCheck = cms.bool(True),
    feUnpacking = cms.bool(True),
    forceToKeepFRData = cms.bool(False),
    headerUnpacking = cms.bool(True),
    memUnpacking = cms.bool(True),
    numbTriggerTSamples = cms.int32(1),
    numbXtalTSamples = cms.int32(10),
    orderedDCCIdList = cms.vint32(1, 2, 3, 4, 5, 
        6, 7, 8, 9, 10, 
        11, 12, 13, 14, 15, 
        16, 17, 18, 19, 20, 
        21, 22, 23, 24, 25, 
        26, 27, 28, 29, 30, 
        31, 32, 33, 34, 35, 
        36, 37, 38, 39, 40, 
        41, 42, 43, 44, 45, 
        46, 47, 48, 49, 50, 
        51, 52, 53, 54),
    orderedFedList = cms.vint32(601, 602, 603, 604, 605, 
        606, 607, 608, 609, 610, 
        611, 612, 613, 614, 615, 
        616, 617, 618, 619, 620, 
        621, 622, 623, 624, 625, 
        626, 627, 628, 629, 630, 
        631, 632, 633, 634, 635, 
        636, 637, 638, 639, 640, 
        641, 642, 643, 644, 645, 
        646, 647, 648, 649, 650, 
        651, 652, 653, 654),
    silentMode = cms.untracked.bool(True),
    srpUnpacking = cms.bool(True),
    syncCheck = cms.bool(True),
    tccUnpacking = cms.bool(True)
)


process.unpackHcal = cms.EDProducer("HcalRawToDigi",
    ComplainEmptyData = cms.untracked.bool(False),
    ElectronicsMap = cms.string(''),
    ExpectedOrbitMessageTime = cms.untracked.int32(-1),
    FEDs = cms.untracked.vint32(),
    FilterDataQuality = cms.bool(True),
    HcalFirstFED = cms.untracked.int32(700),
    InputLabel = cms.InputTag("rawDataCollector","","@skipCurrentProcess"),
    UnpackCalib = cms.untracked.bool(True),
    UnpackTTP = cms.untracked.bool(True),
    UnpackUMNio = cms.untracked.bool(True),
    UnpackZDC = cms.untracked.bool(True),
    UnpackerMode = cms.untracked.int32(0),
    firstSample = cms.int32(0),
    lastSample = cms.int32(9),
    silent = cms.untracked.bool(True)
)


process.unpackRPC = cms.EDProducer("RPCUnpackingModule",
    InputLabel = cms.InputTag("rawDataCollector","","@skipCurrentProcess"),
    doSynchro = cms.bool(True)
)


process.hltAK4CaloJetsPFEt5 = cms.EDFilter("EtMinCaloJetSelector",
    etMin = cms.double(5.0),
    filter = cms.bool(False),
    src = cms.InputTag("hltAK4CaloJetsPF")
)


process.hltBoolEnd = cms.EDFilter("HLTBool",
    result = cms.bool(True)
)


process.hltBoolFalse = cms.EDFilter("HLTBool",
    result = cms.bool(False)
)


process.hltDiPFJetOpenMJJOpen = cms.EDFilter("HLTPFJetVBFFilter",
    etaOpposite = cms.bool(True),
    inputTag = cms.InputTag("hltAK4PFJetsCorrected"),
    leadingJetOnly = cms.bool(False),
    maxEta = cms.double(5.0),
    minDeltaEta = cms.double(3.5),
    minInvMass = cms.double(0.0),
    minPtHigh = cms.double(0.0),
    minPtLow = cms.double(0.0),
    saveTags = cms.bool(True),
    triggerType = cms.int32(85)
)


process.hltDoubleJetOpen = cms.EDFilter("HLT1CaloJet",
    MaxEta = cms.double(5.2),
    MinE = cms.double(-1.0),
    MinMass = cms.double(-1.0),
    MinN = cms.int32(2),
    MinPt = cms.double(0.0),
    inputTag = cms.InputTag("hltCaloJetsCorrectedMatchedToL1"),
    saveTags = cms.bool(True),
    triggerType = cms.int32(86)
)


process.hltL1DiJetVBF = cms.EDFilter("HLTL1TSeed",
    L1EGammaInputTag = cms.InputTag("hltCaloStage2Digis","EGamma"),
    L1EtSumInputTag = cms.InputTag("hltCaloStage2Digis","EtSum"),
    L1GlobalInputTag = cms.InputTag("hltGtStage2Digis"),
    L1JetInputTag = cms.InputTag("hltCaloStage2Digis","Jet"),
    L1MuonInputTag = cms.InputTag("hltGmtStage2Digis","Muon"),
    L1ObjectMapInputTag = cms.InputTag("hltGtStage2ObjectMap"),
    L1SeedsLogicalExpression = cms.string('L1_DoubleJet_90_30_Mj30j30_620 OR L1_DoubleJet_100_30_Mj30j30_620'),
    L1TauInputTag = cms.InputTag("hltCaloStage2Digis","Tau"),
    saveTags = cms.bool(True)
)


process.hltPreVBFDiJetExoTau = cms.EDFilter("HLTPrescaler",
    L1GtReadoutRecordTag = cms.InputTag("hltGtStage2Digis"),
    offset = cms.uint32(0)
)


process.hltTriggerType = cms.EDFilter("HLTTriggerTypeFilter",
    SelectedTriggerType = cms.int32(1)
)


process.selectedPatTriggerCustom = cms.EDFilter("PATTriggerObjectStandAloneSelector",
    cut = cms.string('!filterLabels.empty()'),
    src = cms.InputTag("patTriggerCustom")
)


process.hltGetConditions = cms.EDAnalyzer("EventSetupRecordDataGetter",
    toGet = cms.VPSet(),
    verbose = cms.untracked.bool(False)
)


process.hltGetRaw = cms.EDAnalyzer("HLTGetRaw",
    RawDataCollection = cms.InputTag("rawDataCollector")
)


process.dqmOutput = cms.OutputModule("DQMRootOutputModule",
    fileName = cms.untracked.string('DQMIO.root')
)


process.hltOutputFULL = cms.OutputModule("PoolOutputModule",
    dataset = cms.untracked.PSet(
        dataTier = cms.untracked.string('RECO'),
        filterName = cms.untracked.string('')
    ),
    fastCloning = cms.untracked.bool(False),
    fileName = cms.untracked.string('outputFULLVBFtesting.root'),
    outputCommands = cms.untracked.vstring('drop *', 
        'keep *_TriggerResults_*_*', 
        'keep *_addPileupInfo_*_*', 
        'keep LHEEventProduct_externalLHEProducer__LHE', 
        'keep GenEventInfoProduct_generator__SIM', 
        'keep *TriggerObjectStandAlone_*_*_*', 
        'keep *_*atTrigger*_*_*', 
        'keep *_hltTriggerSummaryAOD_*_*', 
        'keep *_*Stage2*_*_*', 
        'keep *PFTauDiscriminator_*Open*_*_*', 
        'keep double_*ixedGridRho*_*_*', 
        'keep HcalNoiseSummary_hcalnoise__RECO', 
        'keep *BeamSpot_offlineBeamSpot__RECO', 
        'keep *_*limmed*__*', 
        'keep *_lostTracks__*', 
        'keep *_packedPFCandidates__*', 
        'keep *_packedGenParticles__*', 
        'keep *_prunedGenParticles__*', 
        'keep unsignedint_bunchSpacingProducer__*', 
        'keep *_reducedEgamma_reducedGedGsfElectronCores_*', 
        'keep *_reducedEgamma_reducedSuperClusters_*', 
        'keep *TriggerFilterObjectWithRefs_hltDoubleJetOpen__TEST', 
        'keep *TriggerFilterObjectWithRefs_hltPFDoubleJetLooseIDOpen__TEST', 
        'keep *_hltParticleFlowReg_*_*')
)


process.HLTDoLocalPixelSequence = cms.Sequence(process.hltSiPixelDigis+process.hltSiPixelClusters+process.hltSiPixelClustersCache+process.hltSiPixelRecHits)


process.HLTPreshowerSequence = cms.Sequence(process.hltEcalPreshowerDigis+process.hltEcalPreshowerRecHit)


process.HLTParticleFlowSequence = cms.Sequence(process.HLTPreshowerSequence+process.hltParticleFlowRecHitECALUnseeded+process.hltParticleFlowRecHitHBHE+process.hltParticleFlowRecHitHCAL+process.hltParticleFlowRecHitHF+process.hltParticleFlowRecHitPSUnseeded+process.hltParticleFlowClusterECALUncorrectedUnseeded+process.hltParticleFlowClusterPSUnseeded+process.hltParticleFlowClusterECALUnseeded+process.hltParticleFlowClusterHBHE+process.hltParticleFlowClusterHCAL+process.hltParticleFlowClusterHF+process.hltLightPFTracks+process.hltParticleFlowBlock+process.hltParticleFlow)


process.HLTL1UnpackerSequence = cms.Sequence(process.hltGtStage2Digis+process.hltCaloStage2Digis+process.hltGmtStage2Digis+process.hltGtStage2ObjectMap)


process.HLTMuonLocalRecoSequence = cms.Sequence(process.hltMuonDTDigis+process.hltDt1DRecHits+process.hltDt4DSegments+process.hltMuonCSCDigis+process.hltCsc2DRecHits+process.hltCscSegments+process.hltMuonRPCDigis+process.hltRpcRecHits)


process.HLTDoLocalHcalSequence = cms.Sequence(process.hltHcalDigis+process.hltHbhereco+process.hltHfreco+process.hltHoreco)


process.HLTIter0TrackAndTauJet4Iter1Sequence = cms.Sequence(process.hltTrackIter0RefsForJets4Iter1+process.hltAK4Iter0TrackJets4Iter1+process.hltIter0TrackAndTauJets4Iter1)


process.HLTBeamSpot = cms.Sequence(process.hltScalersRawToDigi+process.hltOnlineBeamSpot)


process.HLTL2muonrecoNocandSequence = cms.Sequence(process.HLTMuonLocalRecoSequence+process.hltL2OfflineMuonSeeds+process.hltL2MuonSeeds+process.hltL2Muons)


process.HLTDoFullUnpackingEgammaEcalWithoutPreshowerSequence = cms.Sequence(process.hltEcalDigis+process.hltEcalUncalibRecHit+process.hltEcalDetIdToBeRecovered+process.hltEcalRecHit)


process.HLTAK4CaloCorrectorProducersSequence = cms.Sequence(process.hltAK4CaloFastJetCorrector+process.hltAK4CaloRelativeCorrector+process.hltAK4CaloAbsoluteCorrector+process.hltAK4CaloResidualCorrector+process.hltAK4CaloCorrector)


process.HLTIter1TrackAndTauJets4Iter2Sequence = cms.Sequence(process.hltIter1TrackRefsForJets4Iter2+process.hltAK4Iter1TrackJets4Iter2+process.hltIter1TrackAndTauJets4Iter2)


process.HLTEndSequence = cms.Sequence(process.hltBoolEnd)


process.HLTIterativeTrackingIteration2 = cms.Sequence(process.hltIter2ClustersRefRemoval+process.hltIter2MaskedMeasurementTrackerEvent+process.hltIter2PixelLayerPairs+process.hltIter2PFlowPixelSeeds+process.hltIter2PFlowCkfTrackCandidates+process.hltIter2PFlowCtfWithMaterialTracks+process.hltIter2PFlowTrackCutClassifier+process.hltIter2PFlowTrackSelectionHighPurity)


process.HLTAK4PFCorrectorProducersSequence = cms.Sequence(process.hltAK4PFFastJetCorrector+process.hltAK4PFRelativeCorrector+process.hltAK4PFAbsoluteCorrector+process.hltAK4PFResidualCorrector+process.hltAK4PFCorrector)


process.HLTIterativeTrackingIteration1 = cms.Sequence(process.hltIter1ClustersRefRemoval+process.hltIter1MaskedMeasurementTrackerEvent+process.hltIter1PixelLayerTriplets+process.hltIter1PFlowPixelSeeds+process.hltIter1PFlowCkfTrackCandidates+process.hltIter1PFlowCtfWithMaterialTracks+process.hltIter1PFlowTrackCutClassifierPrompt+process.hltIter1PFlowTrackCutClassifierDetached+process.hltIter1PFlowTrackCutClassifierMerged+process.hltIter1PFlowTrackSelectionHighPurity)


process.HLTIterativeTrackingIteration0 = cms.Sequence(process.hltIter0PFLowPixelSeedsFromPixelTracks+process.hltIter0PFlowCkfTrackCandidates+process.hltIter0PFlowCtfWithMaterialTracks+process.hltIter0PFlowTrackCutClassifier+process.hltIter0PFlowTrackSelectionHighPurity)


process.HLTIterativeTrackingIter02 = cms.Sequence(process.HLTIterativeTrackingIteration0+process.HLTIter0TrackAndTauJet4Iter1Sequence+process.HLTIterativeTrackingIteration1+process.hltIter1Merged+process.HLTIter1TrackAndTauJets4Iter2Sequence+process.HLTIterativeTrackingIteration2+process.hltIter2Merged)


process.HLTRecopixelvertexingSequence = cms.Sequence(process.hltPixelLayerTriplets+process.hltPixelTracks+process.hltPixelVertices+process.hltTrimmedPixelVertices)


process.HLTDoLocalStripSequence = cms.Sequence(process.hltSiStripExcludedFEDListProducer+process.hltSiStripRawToClustersFacility+process.hltSiStripClusters)


process.HLTDoCaloSequencePF = cms.Sequence(process.HLTDoFullUnpackingEgammaEcalWithoutPreshowerSequence+process.HLTDoLocalHcalSequence+process.hltTowerMakerForPF)


process.HLTTrackReconstructionForPF = cms.Sequence(process.HLTDoLocalPixelSequence+process.HLTRecopixelvertexingSequence+process.HLTDoLocalStripSequence+process.HLTIterativeTrackingIter02+process.hltPFMuonMerging+process.hltMuonLinks+process.hltMuons)


process.HLTL3muonTkCandidateSequence = cms.Sequence(process.HLTDoLocalPixelSequence+process.HLTDoLocalStripSequence+process.hltL3TrajSeedOIState+process.hltL3TrackCandidateFromL2OIState+process.hltL3TkTracksFromL2OIState+process.hltL3MuonsOIState+process.hltL3TrajSeedOIHit+process.hltL3TrackCandidateFromL2OIHit+process.hltL3TkTracksFromL2OIHit+process.hltL3MuonsOIHit+process.hltL3TkFromL2OICombination+process.hltPixelLayerTriplets+process.hltPixelLayerPairs+process.hltMixedLayerPairs+process.hltL3TrajSeedIOHit+process.hltL3TrackCandidateFromL2IOHit+process.hltL3TkTracksFromL2IOHit+process.hltL3MuonsIOHit+process.hltL3TrajectorySeed+process.hltL3TrackCandidateFromL2)


process.HLTL2muonrecoSequence = cms.Sequence(process.HLTL2muonrecoNocandSequence+process.hltL2MuonCandidates)


process.HLTBeginSequence = cms.Sequence(process.hltTriggerType+process.HLTL1UnpackerSequence+process.HLTBeamSpot)


process.HLTAK4CaloJetsCorrectionSequence = cms.Sequence(process.hltFixedGridRhoFastjetAllCalo+process.HLTAK4CaloCorrectorProducersSequence+process.hltAK4CaloJetsCorrected+process.hltAK4CaloJetsCorrectedIDPassed)


process.HLTDoCaloSequence = cms.Sequence(process.HLTDoFullUnpackingEgammaEcalWithoutPreshowerSequence+process.HLTDoLocalHcalSequence+process.hltTowerMakerForAll)


process.HLTAK4PFJetsCorrectionSequence = cms.Sequence(process.hltFixedGridRhoFastjetAll+process.HLTAK4PFCorrectorProducersSequence+process.hltAK4PFJetsCorrected+process.hltAK4PFJetsLooseIDCorrected+process.hltAK4PFJetsTightIDCorrected)


process.HLTL3muonrecoNocandSequence = cms.Sequence(process.HLTL3muonTkCandidateSequence+process.hltL3TkTracksMergeStep1+process.hltL3TkTracksFromL2+process.hltL3MuonsLinksCombination+process.hltL3Muons)


process.HLTAK4CaloJetsPrePFRecoSequence = cms.Sequence(process.HLTDoCaloSequencePF+process.hltAK4CaloJetsPF)


process.HLTAK4CaloJetsReconstructionSequence = cms.Sequence(process.HLTDoCaloSequence+process.hltAK4CaloJets+process.hltAK4CaloJetsIDPassed)


process.HLTAK4CaloJetsSequence = cms.Sequence(process.HLTAK4CaloJetsReconstructionSequence+process.HLTAK4CaloJetsCorrectionSequence)


process.HLTL3muonrecoSequence = cms.Sequence(process.HLTL3muonrecoNocandSequence+process.hltL3MuonCandidates)


process.HLTPreAK4PFJetsRecoSequence = cms.Sequence(process.HLTAK4CaloJetsPrePFRecoSequence+process.hltAK4CaloJetsPFEt5)


process.HLTAK4PFJetsReconstructionSequence = cms.Sequence(process.HLTL2muonrecoSequence+process.HLTL3muonrecoSequence+process.HLTTrackReconstructionForPF+process.HLTParticleFlowSequence+process.hltAK4PFJets+process.hltAK4PFJetsLooseID+process.hltAK4PFJetsTightID)


process.HLTAK4PFJetsSequence = cms.Sequence(process.HLTPreAK4PFJetsRecoSequence+process.HLTAK4PFJetsReconstructionSequence+process.HLTAK4PFJetsCorrectionSequence)


process.SimL1Emulator = cms.Sequence(process.unpackRPC+process.unpackDT+process.unpackCSC+process.unpackEcal+process.unpackHcal+process.simHcalTriggerPrimitiveDigis+process.SimL1EmulatorCore+process.packCaloStage2+process.packGmtStage2+process.packGtStage2+process.rawDataCollector)


process.HLTriggerFirstPath = cms.Path(process.SimL1Emulator+process.hltGetConditions+process.hltGetRaw+process.hltBoolFalse)


process.HLT_VBFDiJetExoTau = cms.Path(process.SimL1Emulator+process.HLTBeginSequence+process.hltL1DiJetVBF+process.hltPreVBFDiJetExoTau+process.HLTAK4CaloJetsSequence+process.hltCaloJetsCorrectedMatchedToL1+process.hltDoubleJetOpen+process.HLTAK4PFJetsSequence+process.hltDiPFJetOpenMJJOpen+process.HLTEndSequence)


process.HLTriggerFinalPath = cms.Path(process.SimL1Emulator+process.hltGtStage2Digis+process.hltScalersRawToDigi+process.hltFEDSelector+process.hltTriggerSummaryAOD+process.hltTriggerSummaryRAW+process.hltBoolFalse)


process.HLTAnalyzerEndpath = cms.EndPath(process.patTriggerCustom+process.selectedPatTriggerCustom)


process.FULLOutput = cms.EndPath(process.hltOutputFULL)


process.DQMStore = cms.Service("DQMStore",
    LSbasedMode = cms.untracked.bool(False),
    collateHistograms = cms.untracked.bool(False),
    enableMultiThread = cms.untracked.bool(True),
    forceResetOnBeginLumi = cms.untracked.bool(False),
    referenceFileName = cms.untracked.string(''),
    verbose = cms.untracked.int32(0),
    verboseQT = cms.untracked.int32(0)
)


process.FastTimerService = cms.Service("FastTimerService",
    dqmLumiSectionsRange = cms.untracked.uint32(2500),
    dqmModuleTimeRange = cms.untracked.double(40.0),
    dqmModuleTimeResolution = cms.untracked.double(0.2),
    dqmPath = cms.untracked.string('HLT/TimerService'),
    dqmPathTimeRange = cms.untracked.double(100.0),
    dqmPathTimeResolution = cms.untracked.double(0.5),
    dqmTimeRange = cms.untracked.double(2000.0),
    dqmTimeResolution = cms.untracked.double(5.0),
    enableDQM = cms.untracked.bool(True),
    enableDQMSummary = cms.untracked.bool(True),
    enableDQMbyLumiSection = cms.untracked.bool(True),
    enableDQMbyModule = cms.untracked.bool(False),
    enableDQMbyModuleType = cms.untracked.bool(False),
    enableDQMbyPathActive = cms.untracked.bool(False),
    enableDQMbyPathCounters = cms.untracked.bool(True),
    enableDQMbyPathDetails = cms.untracked.bool(False),
    enableDQMbyPathExclusive = cms.untracked.bool(False),
    enableDQMbyPathOverhead = cms.untracked.bool(False),
    enableDQMbyPathTotal = cms.untracked.bool(True),
    enableDQMbyProcesses = cms.untracked.bool(True),
    enableTimingExclusive = cms.untracked.bool(True),
    enableTimingModules = cms.untracked.bool(True),
    enableTimingPaths = cms.untracked.bool(True),
    enableTimingSummary = cms.untracked.bool(True),
    skipFirstPath = cms.untracked.bool(False),
    useRealTimeClock = cms.untracked.bool(True)
)


process.MessageLogger = cms.Service("MessageLogger",
    FrameworkJobReport = cms.untracked.PSet(
        FwkJob = cms.untracked.PSet(
            limit = cms.untracked.int32(10000000),
            optionalPSet = cms.untracked.bool(True)
        ),
        default = cms.untracked.PSet(
            limit = cms.untracked.int32(0)
        ),
        optionalPSet = cms.untracked.bool(True)
    ),
    categories = cms.untracked.vstring('FwkJob', 
        'FwkReport', 
        'FwkSummary', 
        'Root_NoDictionary'),
    cerr = cms.untracked.PSet(
        FwkJob = cms.untracked.PSet(
            limit = cms.untracked.int32(0),
            optionalPSet = cms.untracked.bool(True)
        ),
        FwkReport = cms.untracked.PSet(
            limit = cms.untracked.int32(10000000),
            optionalPSet = cms.untracked.bool(True),
            reportEvery = cms.untracked.int32(1)
        ),
        FwkSummary = cms.untracked.PSet(
            limit = cms.untracked.int32(10000000),
            optionalPSet = cms.untracked.bool(True),
            reportEvery = cms.untracked.int32(1)
        ),
        INFO = cms.untracked.PSet(
            limit = cms.untracked.int32(0)
        ),
        Root_NoDictionary = cms.untracked.PSet(
            limit = cms.untracked.int32(0),
            optionalPSet = cms.untracked.bool(True)
        ),
        default = cms.untracked.PSet(
            limit = cms.untracked.int32(10000000)
        ),
        noTimeStamps = cms.untracked.bool(False),
        optionalPSet = cms.untracked.bool(True),
        threshold = cms.untracked.string('INFO')
    ),
    cerr_stats = cms.untracked.PSet(
        optionalPSet = cms.untracked.bool(True),
        output = cms.untracked.string('cerr'),
        threshold = cms.untracked.string('WARNING')
    ),
    cout = cms.untracked.PSet(
        placeholder = cms.untracked.bool(True)
    ),
    debugModules = cms.untracked.vstring(),
    debugs = cms.untracked.PSet(
        placeholder = cms.untracked.bool(True)
    ),
    default = cms.untracked.PSet(

    ),
    destinations = cms.untracked.vstring('warnings', 
        'errors', 
        'infos', 
        'debugs', 
        'cout', 
        'cerr'),
    errors = cms.untracked.PSet(
        placeholder = cms.untracked.bool(True)
    ),
    fwkJobReports = cms.untracked.vstring('FrameworkJobReport'),
    infos = cms.untracked.PSet(
        Root_NoDictionary = cms.untracked.PSet(
            limit = cms.untracked.int32(0),
            optionalPSet = cms.untracked.bool(True)
        ),
        optionalPSet = cms.untracked.bool(True),
        placeholder = cms.untracked.bool(True)
    ),
    statistics = cms.untracked.vstring('cerr_stats'),
    suppressDebug = cms.untracked.vstring(),
    suppressInfo = cms.untracked.vstring(),
    suppressWarning = cms.untracked.vstring(),
    warnings = cms.untracked.PSet(
        placeholder = cms.untracked.bool(True)
    )
)


process.ThroughputService = cms.Service("ThroughputService",
    dqmPath = cms.untracked.string('HLT/Throughput'),
    timeRange = cms.untracked.double(60000.0),
    timeResolution = cms.untracked.double(5.828)
)


process.AnyDirectionAnalyticalPropagator = cms.ESProducer("AnalyticalPropagatorESProducer",
    ComponentName = cms.string('AnyDirectionAnalyticalPropagator'),
    MaxDPhi = cms.double(1.6),
    PropagationDirection = cms.string('anyDirection')
)


process.CSCChannelMapperESProducer = cms.ESProducer("CSCChannelMapperESProducer",
    AlgoName = cms.string('CSCChannelMapperPostls1')
)


process.CSCGeometryESModule = cms.ESProducer("CSCGeometryESModule",
    alignmentsLabel = cms.string(''),
    appendToDataLabel = cms.string(''),
    applyAlignment = cms.bool(True),
    debugV = cms.untracked.bool(False),
    useCentreTIOffsets = cms.bool(False),
    useDDD = cms.bool(False),
    useGangedStripsInME1a = cms.bool(False),
    useOnlyWiresInME1a = cms.bool(False),
    useRealWireGeometry = cms.bool(True)
)


process.CSCIndexerESProducer = cms.ESProducer("CSCIndexerESProducer",
    AlgoName = cms.string('CSCIndexerPostls1')
)


process.CSCObjectMapESProducer = cms.ESProducer("CSCObjectMapESProducer",
    appendToDataLabel = cms.string('')
)


process.CaloGeometryBuilder = cms.ESProducer("CaloGeometryBuilder",
    SelectedCalos = cms.vstring('HCAL', 
        'ZDC', 
        'EcalBarrel', 
        'EcalEndcap', 
        'EcalPreshower', 
        'TOWER')
)


process.CaloTPGTranscoder = cms.ESProducer("CaloTPGTranscoderULUTs",
    HFTPScaleShift = cms.PSet(
        NCT = cms.int32(1),
        RCT = cms.int32(3)
    ),
    LUTfactor = cms.vint32(1, 2, 5, 0),
    RCTLSB = cms.double(0.25),
    ZS = cms.vint32(4, 2, 1, 0),
    hcalLUT1 = cms.FileInPath('CalibCalorimetry/CaloTPG/data/outputLUTtranscoder_physics.dat'),
    hcalLUT2 = cms.FileInPath('CalibCalorimetry/CaloTPG/data/TPGcalcDecompress2.txt'),
    ietaLowerBound = cms.vint32(1, 18, 27, 29),
    ietaUpperBound = cms.vint32(17, 26, 28, 32),
    nominal_gain = cms.double(0.177),
    read_Ascii_Compression_LUTs = cms.bool(False),
    read_Ascii_RCT_LUTs = cms.bool(False)
)


process.CaloTopologyBuilder = cms.ESProducer("CaloTopologyBuilder")


process.CaloTowerConstituentsMapBuilder = cms.ESProducer("CaloTowerConstituentsMapBuilder",
    MapFile = cms.untracked.string('Geometry/CaloTopology/data/CaloTowerEEGeometric.map.gz'),
    appendToDataLabel = cms.string('')
)


process.CaloTowerGeometryFromDBEP = cms.ESProducer("CaloTowerGeometryFromDBEP",
    applyAlignment = cms.bool(False),
    hcalTopologyConstants = cms.PSet(
        maxDepthHB = cms.int32(2),
        maxDepthHE = cms.int32(3),
        mode = cms.string('HcalTopologyMode::LHC')
    )
)


process.CaloTowerTopologyEP = cms.ESProducer("CaloTowerTopologyEP",
    appendToDataLabel = cms.string('')
)


process.CastorDbProducer = cms.ESProducer("CastorDbProducer",
    appendToDataLabel = cms.string('')
)


process.ClusterShapeHitFilterESProducer = cms.ESProducer("ClusterShapeHitFilterESProducer",
    ComponentName = cms.string('ClusterShapeHitFilter'),
    PixelShapeFile = cms.string('RecoPixelVertexing/PixelLowPtUtilities/data/pixelShape.par'),
    clusterChargeCut = cms.PSet(
        refToPSet_ = cms.string('HLTSiStripClusterChargeCutNone')
    )
)


process.DTGeometryESModule = cms.ESProducer("DTGeometryESModule",
    alignmentsLabel = cms.string(''),
    appendToDataLabel = cms.string(''),
    applyAlignment = cms.bool(True),
    fromDDD = cms.bool(False)
)


process.DTObjectMapESProducer = cms.ESProducer("DTObjectMapESProducer",
    appendToDataLabel = cms.string('')
)


process.EcalBarrelGeometryFromDBEP = cms.ESProducer("EcalBarrelGeometryFromDBEP",
    applyAlignment = cms.bool(True)
)


process.EcalElectronicsMappingBuilder = cms.ESProducer("EcalElectronicsMappingBuilder")


process.EcalEndcapGeometryFromDBEP = cms.ESProducer("EcalEndcapGeometryFromDBEP",
    applyAlignment = cms.bool(True)
)


process.EcalLaserCorrectionService = cms.ESProducer("EcalLaserCorrectionService")


process.EcalPreshowerGeometryFromDBEP = cms.ESProducer("EcalPreshowerGeometryFromDBEP",
    applyAlignment = cms.bool(True)
)


process.HcalGeometryFromDBEP = cms.ESProducer("HcalGeometryFromDBEP",
    applyAlignment = cms.bool(False),
    hcalTopologyConstants = cms.PSet(
        maxDepthHB = cms.int32(2),
        maxDepthHE = cms.int32(3),
        mode = cms.string('HcalTopologyMode::LHC')
    )
)


process.HcalTPGCoderULUT = cms.ESProducer("HcalTPGCoderULUT",
    FGLUTs = cms.FileInPath('CalibCalorimetry/HcalTPGAlgos/data/HBHE_FG_LUT.dat'),
    LUTGenerationMode = cms.bool(True),
    MaskBit = cms.int32(32768),
    RCalibFile = cms.FileInPath('CalibCalorimetry/HcalTPGAlgos/data/RecHit-TPG-calib.dat'),
    inputLUTs = cms.FileInPath('CalibCalorimetry/HcalTPGAlgos/data/inputLUTcoder_physics.dat'),
    read_Ascii_LUTs = cms.bool(False),
    read_FG_LUTs = cms.bool(False),
    read_XML_LUTs = cms.bool(False)
)


process.HcalTopologyIdealEP = cms.ESProducer("HcalTopologyIdealEP",
    Exclude = cms.untracked.string(''),
    appendToDataLabel = cms.string('')
)


process.HcalTrigTowerGeometryESProducer = cms.ESProducer("HcalTrigTowerGeometryESProducer")


process.L1DTConfigFromDB = cms.ESProducer("DTConfigDBProducer",
    DTTPGMap = cms.untracked.PSet(
    **dict(
        [
            ("wh0st1se1" , cms.untracked.vint32(50, 58, 50, 13) ),
            ("wh0st1se10" , cms.untracked.vint32(50, 58, 50, 13) ),
            ("wh0st1se11" , cms.untracked.vint32(50, 58, 50, 13) ),
            ("wh0st1se12" , cms.untracked.vint32(50, 58, 50, 13) ),
            ("wh0st1se2" , cms.untracked.vint32(50, 58, 50, 13) ),
            ("wh0st1se3" , cms.untracked.vint32(50, 58, 50, 13) ),
            ("wh0st1se4" , cms.untracked.vint32(50, 58, 50, 13) ),
            ("wh0st1se5" , cms.untracked.vint32(50, 58, 50, 13) ),
            ("wh0st1se6" , cms.untracked.vint32(50, 58, 50, 13) ),
            ("wh0st1se7" , cms.untracked.vint32(50, 58, 50, 13) ),
            ("wh0st1se8" , cms.untracked.vint32(50, 58, 50, 13) ),
            ("wh0st1se9" , cms.untracked.vint32(50, 58, 50, 13) ),
            ("wh0st2se1" , cms.untracked.vint32(60, 58, 60, 15) ),
            ("wh0st2se10" , cms.untracked.vint32(60, 58, 60, 15) ),
            ("wh0st2se11" , cms.untracked.vint32(60, 58, 60, 15) ),
            ("wh0st2se12" , cms.untracked.vint32(60, 58, 60, 15) ),
            ("wh0st2se2" , cms.untracked.vint32(60, 58, 60, 15) ),
            ("wh0st2se3" , cms.untracked.vint32(60, 58, 60, 15) ),
            ("wh0st2se4" , cms.untracked.vint32(60, 58, 60, 15) ),
            ("wh0st2se5" , cms.untracked.vint32(60, 58, 60, 15) ),
            ("wh0st2se6" , cms.untracked.vint32(60, 58, 60, 15) ),
            ("wh0st2se7" , cms.untracked.vint32(60, 58, 60, 15) ),
            ("wh0st2se8" , cms.untracked.vint32(60, 58, 60, 15) ),
            ("wh0st2se9" , cms.untracked.vint32(60, 58, 60, 15) ),
            ("wh0st3se1" , cms.untracked.vint32(72, 58, 72, 18) ),
            ("wh0st3se10" , cms.untracked.vint32(72, 58, 72, 18) ),
            ("wh0st3se11" , cms.untracked.vint32(72, 58, 72, 18) ),
            ("wh0st3se12" , cms.untracked.vint32(72, 58, 72, 18) ),
            ("wh0st3se2" , cms.untracked.vint32(72, 58, 72, 18) ),
            ("wh0st3se3" , cms.untracked.vint32(72, 58, 72, 18) ),
            ("wh0st3se4" , cms.untracked.vint32(72, 58, 72, 18) ),
            ("wh0st3se5" , cms.untracked.vint32(72, 58, 72, 18) ),
            ("wh0st3se6" , cms.untracked.vint32(72, 58, 72, 18) ),
            ("wh0st3se7" , cms.untracked.vint32(72, 58, 72, 18) ),
            ("wh0st3se8" , cms.untracked.vint32(72, 58, 72, 18) ),
            ("wh0st3se9" , cms.untracked.vint32(72, 58, 72, 18) ),
            ("wh0st4se1" , cms.untracked.vint32(96, 0, 96, 24) ),
            ("wh0st4se10" , cms.untracked.vint32(60, 0, 60, 15) ),
            ("wh0st4se11" , cms.untracked.vint32(48, 0, 48, 12) ),
            ("wh0st4se12" , cms.untracked.vint32(92, 0, 92, 23) ),
            ("wh0st4se13" , cms.untracked.vint32(72, 0, 72, 18) ),
            ("wh0st4se14" , cms.untracked.vint32(60, 0, 60, 15) ),
            ("wh0st4se2" , cms.untracked.vint32(96, 0, 96, 24) ),
            ("wh0st4se3" , cms.untracked.vint32(96, 0, 96, 24) ),
            ("wh0st4se4" , cms.untracked.vint32(72, 0, 72, 18) ),
            ("wh0st4se5" , cms.untracked.vint32(96, 0, 96, 24) ),
            ("wh0st4se6" , cms.untracked.vint32(96, 0, 96, 24) ),
            ("wh0st4se7" , cms.untracked.vint32(96, 0, 96, 24) ),
            ("wh0st4se8" , cms.untracked.vint32(92, 0, 92, 23) ),
            ("wh0st4se9" , cms.untracked.vint32(48, 0, 48, 12) ),
            ("wh1st1se1" , cms.untracked.vint32(50, 58, 50, 13) ),
            ("wh1st1se10" , cms.untracked.vint32(50, 58, 50, 13) ),
            ("wh1st1se11" , cms.untracked.vint32(50, 58, 50, 13) ),
            ("wh1st1se12" , cms.untracked.vint32(50, 58, 50, 13) ),
            ("wh1st1se2" , cms.untracked.vint32(50, 58, 50, 13) ),
            ("wh1st1se3" , cms.untracked.vint32(50, 58, 50, 13) ),
            ("wh1st1se4" , cms.untracked.vint32(50, 48, 50, 13) ),
            ("wh1st1se5" , cms.untracked.vint32(50, 58, 50, 13) ),
            ("wh1st1se6" , cms.untracked.vint32(50, 58, 50, 13) ),
            ("wh1st1se7" , cms.untracked.vint32(50, 58, 50, 13) ),
            ("wh1st1se8" , cms.untracked.vint32(50, 58, 50, 13) ),
            ("wh1st1se9" , cms.untracked.vint32(50, 58, 50, 13) ),
            ("wh1st2se1" , cms.untracked.vint32(60, 58, 60, 15) ),
            ("wh1st2se10" , cms.untracked.vint32(60, 58, 60, 15) ),
            ("wh1st2se11" , cms.untracked.vint32(60, 58, 60, 15) ),
            ("wh1st2se12" , cms.untracked.vint32(60, 58, 60, 15) ),
            ("wh1st2se2" , cms.untracked.vint32(60, 58, 60, 15) ),
            ("wh1st2se3" , cms.untracked.vint32(60, 58, 60, 15) ),
            ("wh1st2se4" , cms.untracked.vint32(60, 48, 60, 15) ),
            ("wh1st2se5" , cms.untracked.vint32(60, 58, 60, 15) ),
            ("wh1st2se6" , cms.untracked.vint32(60, 58, 60, 15) ),
            ("wh1st2se7" , cms.untracked.vint32(60, 58, 60, 15) ),
            ("wh1st2se8" , cms.untracked.vint32(60, 58, 60, 15) ),
            ("wh1st2se9" , cms.untracked.vint32(60, 58, 60, 15) ),
            ("wh1st3se1" , cms.untracked.vint32(72, 58, 72, 18) ),
            ("wh1st3se10" , cms.untracked.vint32(72, 58, 72, 18) ),
            ("wh1st3se11" , cms.untracked.vint32(72, 58, 72, 18) ),
            ("wh1st3se12" , cms.untracked.vint32(72, 58, 72, 18) ),
            ("wh1st3se2" , cms.untracked.vint32(72, 58, 72, 18) ),
            ("wh1st3se3" , cms.untracked.vint32(72, 58, 72, 18) ),
            ("wh1st3se4" , cms.untracked.vint32(72, 48, 72, 18) ),
            ("wh1st3se5" , cms.untracked.vint32(72, 58, 72, 18) ),
            ("wh1st3se6" , cms.untracked.vint32(72, 58, 72, 18) ),
            ("wh1st3se7" , cms.untracked.vint32(72, 58, 72, 18) ),
            ("wh1st3se8" , cms.untracked.vint32(72, 58, 72, 18) ),
            ("wh1st3se9" , cms.untracked.vint32(72, 58, 72, 18) ),
            ("wh1st4se1" , cms.untracked.vint32(96, 0, 96, 24) ),
            ("wh1st4se10" , cms.untracked.vint32(60, 0, 60, 15) ),
            ("wh1st4se11" , cms.untracked.vint32(48, 0, 48, 12) ),
            ("wh1st4se12" , cms.untracked.vint32(92, 0, 92, 23) ),
            ("wh1st4se13" , cms.untracked.vint32(72, 0, 72, 18) ),
            ("wh1st4se14" , cms.untracked.vint32(60, 0, 60, 15) ),
            ("wh1st4se2" , cms.untracked.vint32(96, 0, 96, 24) ),
            ("wh1st4se3" , cms.untracked.vint32(96, 0, 96, 24) ),
            ("wh1st4se4" , cms.untracked.vint32(72, 0, 72, 18) ),
            ("wh1st4se5" , cms.untracked.vint32(96, 0, 96, 24) ),
            ("wh1st4se6" , cms.untracked.vint32(96, 0, 96, 24) ),
            ("wh1st4se7" , cms.untracked.vint32(96, 0, 96, 24) ),
            ("wh1st4se8" , cms.untracked.vint32(92, 0, 92, 23) ),
            ("wh1st4se9" , cms.untracked.vint32(48, 0, 48, 12) ),
            ("wh2st1se1" , cms.untracked.vint32(50, 58, 50, 13) ),
            ("wh2st1se10" , cms.untracked.vint32(50, 58, 50, 13) ),
            ("wh2st1se11" , cms.untracked.vint32(50, 58, 50, 13) ),
            ("wh2st1se12" , cms.untracked.vint32(50, 58, 50, 13) ),
            ("wh2st1se2" , cms.untracked.vint32(50, 58, 50, 13) ),
            ("wh2st1se3" , cms.untracked.vint32(50, 58, 50, 13) ),
            ("wh2st1se4" , cms.untracked.vint32(50, 58, 50, 13) ),
            ("wh2st1se5" , cms.untracked.vint32(50, 58, 50, 13) ),
            ("wh2st1se6" , cms.untracked.vint32(50, 58, 50, 13) ),
            ("wh2st1se7" , cms.untracked.vint32(50, 58, 50, 13) ),
            ("wh2st1se8" , cms.untracked.vint32(50, 58, 50, 13) ),
            ("wh2st1se9" , cms.untracked.vint32(50, 58, 50, 13) ),
            ("wh2st2se1" , cms.untracked.vint32(60, 58, 60, 15) ),
            ("wh2st2se10" , cms.untracked.vint32(60, 58, 60, 15) ),
            ("wh2st2se11" , cms.untracked.vint32(60, 58, 60, 15) ),
            ("wh2st2se12" , cms.untracked.vint32(60, 58, 60, 15) ),
            ("wh2st2se2" , cms.untracked.vint32(60, 58, 60, 15) ),
            ("wh2st2se3" , cms.untracked.vint32(60, 58, 60, 15) ),
            ("wh2st2se4" , cms.untracked.vint32(60, 58, 60, 15) ),
            ("wh2st2se5" , cms.untracked.vint32(60, 58, 60, 15) ),
            ("wh2st2se6" , cms.untracked.vint32(60, 58, 60, 15) ),
            ("wh2st2se7" , cms.untracked.vint32(60, 58, 60, 15) ),
            ("wh2st2se8" , cms.untracked.vint32(60, 58, 60, 15) ),
            ("wh2st2se9" , cms.untracked.vint32(60, 58, 60, 15) ),
            ("wh2st3se1" , cms.untracked.vint32(72, 58, 72, 18) ),
            ("wh2st3se10" , cms.untracked.vint32(72, 58, 72, 18) ),
            ("wh2st3se11" , cms.untracked.vint32(72, 58, 72, 18) ),
            ("wh2st3se12" , cms.untracked.vint32(72, 58, 72, 18) ),
            ("wh2st3se2" , cms.untracked.vint32(72, 58, 72, 18) ),
            ("wh2st3se3" , cms.untracked.vint32(72, 58, 72, 18) ),
            ("wh2st3se4" , cms.untracked.vint32(72, 58, 72, 18) ),
            ("wh2st3se5" , cms.untracked.vint32(72, 58, 72, 18) ),
            ("wh2st3se6" , cms.untracked.vint32(72, 58, 72, 18) ),
            ("wh2st3se7" , cms.untracked.vint32(72, 58, 72, 18) ),
            ("wh2st3se8" , cms.untracked.vint32(72, 58, 72, 18) ),
            ("wh2st3se9" , cms.untracked.vint32(72, 58, 72, 18) ),
            ("wh2st4se1" , cms.untracked.vint32(96, 0, 96, 24) ),
            ("wh2st4se10" , cms.untracked.vint32(60, 0, 60, 15) ),
            ("wh2st4se11" , cms.untracked.vint32(48, 0, 48, 12) ),
            ("wh2st4se12" , cms.untracked.vint32(92, 0, 92, 23) ),
            ("wh2st4se13" , cms.untracked.vint32(72, 0, 72, 18) ),
            ("wh2st4se14" , cms.untracked.vint32(60, 0, 60, 15) ),
            ("wh2st4se2" , cms.untracked.vint32(96, 0, 96, 24) ),
            ("wh2st4se3" , cms.untracked.vint32(96, 0, 96, 24) ),
            ("wh2st4se4" , cms.untracked.vint32(72, 0, 72, 18) ),
            ("wh2st4se5" , cms.untracked.vint32(96, 0, 96, 24) ),
            ("wh2st4se6" , cms.untracked.vint32(96, 0, 96, 24) ),
            ("wh2st4se7" , cms.untracked.vint32(96, 0, 96, 24) ),
            ("wh2st4se8" , cms.untracked.vint32(92, 0, 92, 23) ),
            ("wh2st4se9" , cms.untracked.vint32(48, 0, 48, 12) ),
            ("whm1st1se1" , cms.untracked.vint32(50, 58, 50, 13) ),
            ("whm1st1se10" , cms.untracked.vint32(50, 58, 50, 13) ),
            ("whm1st1se11" , cms.untracked.vint32(50, 58, 50, 13) ),
            ("whm1st1se12" , cms.untracked.vint32(50, 58, 50, 13) ),
            ("whm1st1se2" , cms.untracked.vint32(50, 58, 50, 13) ),
            ("whm1st1se3" , cms.untracked.vint32(50, 48, 50, 13) ),
            ("whm1st1se4" , cms.untracked.vint32(50, 58, 50, 13) ),
            ("whm1st1se5" , cms.untracked.vint32(50, 58, 50, 13) ),
            ("whm1st1se6" , cms.untracked.vint32(50, 58, 50, 13) ),
            ("whm1st1se7" , cms.untracked.vint32(50, 58, 50, 13) ),
            ("whm1st1se8" , cms.untracked.vint32(50, 58, 50, 13) ),
            ("whm1st1se9" , cms.untracked.vint32(50, 58, 50, 13) ),
            ("whm1st2se1" , cms.untracked.vint32(60, 58, 60, 15) ),
            ("whm1st2se10" , cms.untracked.vint32(60, 58, 60, 15) ),
            ("whm1st2se11" , cms.untracked.vint32(60, 58, 60, 15) ),
            ("whm1st2se12" , cms.untracked.vint32(60, 58, 60, 15) ),
            ("whm1st2se2" , cms.untracked.vint32(60, 58, 60, 15) ),
            ("whm1st2se3" , cms.untracked.vint32(60, 48, 60, 15) ),
            ("whm1st2se4" , cms.untracked.vint32(60, 58, 60, 15) ),
            ("whm1st2se5" , cms.untracked.vint32(60, 58, 60, 15) ),
            ("whm1st2se6" , cms.untracked.vint32(60, 58, 60, 15) ),
            ("whm1st2se7" , cms.untracked.vint32(60, 58, 60, 15) ),
            ("whm1st2se8" , cms.untracked.vint32(60, 58, 60, 15) ),
            ("whm1st2se9" , cms.untracked.vint32(60, 58, 60, 15) ),
            ("whm1st3se1" , cms.untracked.vint32(72, 58, 72, 18) ),
            ("whm1st3se10" , cms.untracked.vint32(72, 58, 72, 18) ),
            ("whm1st3se11" , cms.untracked.vint32(72, 58, 72, 18) ),
            ("whm1st3se12" , cms.untracked.vint32(72, 58, 72, 18) ),
            ("whm1st3se2" , cms.untracked.vint32(72, 58, 72, 18) ),
            ("whm1st3se3" , cms.untracked.vint32(72, 48, 72, 18) ),
            ("whm1st3se4" , cms.untracked.vint32(72, 58, 72, 18) ),
            ("whm1st3se5" , cms.untracked.vint32(72, 58, 72, 18) ),
            ("whm1st3se6" , cms.untracked.vint32(72, 58, 72, 18) ),
            ("whm1st3se7" , cms.untracked.vint32(72, 58, 72, 18) ),
            ("whm1st3se8" , cms.untracked.vint32(72, 58, 72, 18) ),
            ("whm1st3se9" , cms.untracked.vint32(72, 58, 72, 18) ),
            ("whm1st4se1" , cms.untracked.vint32(96, 0, 96, 24) ),
            ("whm1st4se10" , cms.untracked.vint32(60, 0, 60, 15) ),
            ("whm1st4se11" , cms.untracked.vint32(48, 0, 48, 12) ),
            ("whm1st4se12" , cms.untracked.vint32(92, 0, 92, 23) ),
            ("whm1st4se13" , cms.untracked.vint32(72, 0, 72, 18) ),
            ("whm1st4se14" , cms.untracked.vint32(60, 0, 60, 15) ),
            ("whm1st4se2" , cms.untracked.vint32(96, 0, 96, 24) ),
            ("whm1st4se3" , cms.untracked.vint32(96, 0, 96, 24) ),
            ("whm1st4se4" , cms.untracked.vint32(72, 0, 72, 18) ),
            ("whm1st4se5" , cms.untracked.vint32(96, 0, 96, 24) ),
            ("whm1st4se6" , cms.untracked.vint32(96, 0, 96, 24) ),
            ("whm1st4se7" , cms.untracked.vint32(96, 0, 96, 24) ),
            ("whm1st4se8" , cms.untracked.vint32(92, 0, 92, 23) ),
            ("whm1st4se9" , cms.untracked.vint32(48, 0, 48, 12) ),
        ] +
        [
            ("whm2st1se1" , cms.untracked.vint32(50, 58, 50, 13) ),
            ("whm2st1se10" , cms.untracked.vint32(50, 58, 50, 13) ),
            ("whm2st1se11" , cms.untracked.vint32(50, 58, 50, 13) ),
            ("whm2st1se12" , cms.untracked.vint32(50, 58, 50, 13) ),
            ("whm2st1se2" , cms.untracked.vint32(50, 58, 50, 13) ),
            ("whm2st1se3" , cms.untracked.vint32(50, 58, 50, 13) ),
            ("whm2st1se4" , cms.untracked.vint32(50, 58, 50, 13) ),
            ("whm2st1se5" , cms.untracked.vint32(50, 58, 50, 13) ),
            ("whm2st1se6" , cms.untracked.vint32(50, 58, 50, 13) ),
            ("whm2st1se7" , cms.untracked.vint32(50, 58, 50, 13) ),
            ("whm2st1se8" , cms.untracked.vint32(50, 58, 50, 13) ),
            ("whm2st1se9" , cms.untracked.vint32(50, 58, 50, 13) ),
            ("whm2st2se1" , cms.untracked.vint32(60, 58, 60, 15) ),
            ("whm2st2se10" , cms.untracked.vint32(60, 58, 60, 15) ),
            ("whm2st2se11" , cms.untracked.vint32(60, 58, 60, 15) ),
            ("whm2st2se12" , cms.untracked.vint32(60, 58, 60, 15) ),
            ("whm2st2se2" , cms.untracked.vint32(60, 58, 60, 15) ),
            ("whm2st2se3" , cms.untracked.vint32(60, 58, 60, 15) ),
            ("whm2st2se4" , cms.untracked.vint32(60, 58, 60, 15) ),
            ("whm2st2se5" , cms.untracked.vint32(60, 58, 60, 15) ),
            ("whm2st2se6" , cms.untracked.vint32(60, 58, 60, 15) ),
            ("whm2st2se7" , cms.untracked.vint32(60, 58, 60, 15) ),
            ("whm2st2se8" , cms.untracked.vint32(60, 58, 60, 15) ),
            ("whm2st2se9" , cms.untracked.vint32(60, 58, 60, 15) ),
            ("whm2st3se1" , cms.untracked.vint32(72, 58, 72, 18) ),
            ("whm2st3se10" , cms.untracked.vint32(72, 58, 72, 18) ),
            ("whm2st3se11" , cms.untracked.vint32(72, 58, 72, 18) ),
            ("whm2st3se12" , cms.untracked.vint32(72, 58, 72, 18) ),
            ("whm2st3se2" , cms.untracked.vint32(72, 58, 72, 18) ),
            ("whm2st3se3" , cms.untracked.vint32(72, 58, 72, 18) ),
            ("whm2st3se4" , cms.untracked.vint32(72, 58, 72, 18) ),
            ("whm2st3se5" , cms.untracked.vint32(72, 58, 72, 18) ),
            ("whm2st3se6" , cms.untracked.vint32(72, 58, 72, 18) ),
            ("whm2st3se7" , cms.untracked.vint32(72, 58, 72, 18) ),
            ("whm2st3se8" , cms.untracked.vint32(72, 58, 72, 18) ),
            ("whm2st3se9" , cms.untracked.vint32(72, 58, 72, 18) ),
            ("whm2st4se1" , cms.untracked.vint32(96, 0, 96, 24) ),
            ("whm2st4se10" , cms.untracked.vint32(60, 0, 60, 15) ),
            ("whm2st4se11" , cms.untracked.vint32(48, 0, 48, 12) ),
            ("whm2st4se12" , cms.untracked.vint32(92, 0, 92, 23) ),
            ("whm2st4se13" , cms.untracked.vint32(72, 0, 72, 18) ),
            ("whm2st4se14" , cms.untracked.vint32(60, 0, 60, 15) ),
            ("whm2st4se2" , cms.untracked.vint32(96, 0, 96, 24) ),
            ("whm2st4se3" , cms.untracked.vint32(96, 0, 96, 24) ),
            ("whm2st4se4" , cms.untracked.vint32(72, 0, 72, 18) ),
            ("whm2st4se5" , cms.untracked.vint32(96, 0, 96, 24) ),
            ("whm2st4se6" , cms.untracked.vint32(96, 0, 96, 24) ),
            ("whm2st4se7" , cms.untracked.vint32(96, 0, 96, 24) ),
            ("whm2st4se8" , cms.untracked.vint32(92, 0, 92, 23) ),
            ("whm2st4se9" , cms.untracked.vint32(48, 0, 48, 12) ),
            ]
        )
    ),
    DTTPGParameters = cms.PSet(
        Debug = cms.untracked.bool(False),
        SectCollParameters = cms.PSet(
            Debug = cms.untracked.bool(False),
            SCCSP1 = cms.int32(0),
            SCCSP2 = cms.int32(0),
            SCCSP3 = cms.int32(0),
            SCCSP4 = cms.int32(0),
            SCCSP5 = cms.int32(0),
            SCECF1 = cms.bool(False),
            SCECF2 = cms.bool(False),
            SCECF3 = cms.bool(False),
            SCECF4 = cms.bool(False)
        ),
        TUParameters = cms.PSet(
            BtiParameters = cms.PSet(
                AC1 = cms.int32(0),
                AC2 = cms.int32(3),
                ACH = cms.int32(1),
                ACL = cms.int32(2),
                CH = cms.int32(41),
                CL = cms.int32(22),
                DEAD = cms.int32(31),
                Debug = cms.untracked.int32(0),
                KACCTHETA = cms.int32(1),
                KMAX = cms.int32(64),
                LH = cms.int32(21),
                LL = cms.int32(2),
                LTS = cms.int32(3),
                PTMS0 = cms.int32(0),
                PTMS1 = cms.int32(0),
                PTMS10 = cms.int32(1),
                PTMS11 = cms.int32(1),
                PTMS12 = cms.int32(1),
                PTMS13 = cms.int32(1),
                PTMS14 = cms.int32(1),
                PTMS15 = cms.int32(1),
                PTMS16 = cms.int32(1),
                PTMS17 = cms.int32(1),
                PTMS18 = cms.int32(1),
                PTMS19 = cms.int32(1),
                PTMS2 = cms.int32(0),
                PTMS20 = cms.int32(1),
                PTMS21 = cms.int32(1),
                PTMS22 = cms.int32(1),
                PTMS23 = cms.int32(1),
                PTMS24 = cms.int32(1),
                PTMS25 = cms.int32(1),
                PTMS26 = cms.int32(1),
                PTMS27 = cms.int32(1),
                PTMS28 = cms.int32(1),
                PTMS29 = cms.int32(1),
                PTMS3 = cms.int32(0),
                PTMS30 = cms.int32(0),
                PTMS31 = cms.int32(0),
                PTMS4 = cms.int32(1),
                PTMS5 = cms.int32(1),
                PTMS6 = cms.int32(1),
                PTMS7 = cms.int32(1),
                PTMS8 = cms.int32(1),
                PTMS9 = cms.int32(1),
                RE43 = cms.int32(2),
                RH = cms.int32(61),
                RL = cms.int32(42),
                RON = cms.bool(True),
                SET = cms.int32(7),
                ST43 = cms.int32(42),
                WEN0 = cms.int32(1),
                WEN1 = cms.int32(1),
                WEN2 = cms.int32(1),
                WEN3 = cms.int32(1),
                WEN4 = cms.int32(1),
                WEN5 = cms.int32(1),
                WEN6 = cms.int32(1),
                WEN7 = cms.int32(1),
                WEN8 = cms.int32(1),
                XON = cms.bool(False)
            ),
            Debug = cms.untracked.bool(False),
            LutParameters = cms.PSet(
                BTIC = cms.untracked.int32(0),
                D = cms.untracked.double(0),
                Debug = cms.untracked.bool(False),
                WHEEL = cms.untracked.int32(-1),
                XCN = cms.untracked.double(0)
            ),
            TSPhiParameters = cms.PSet(
                Debug = cms.untracked.bool(False),
                TSMCCE1 = cms.bool(True),
                TSMCCE2 = cms.bool(False),
                TSMCCEC = cms.bool(False),
                TSMCGS1 = cms.bool(True),
                TSMCGS2 = cms.bool(True),
                TSMGS1 = cms.int32(1),
                TSMGS2 = cms.int32(1),
                TSMHSP = cms.int32(1),
                TSMHTE1 = cms.bool(True),
                TSMHTE2 = cms.bool(False),
                TSMHTEC = cms.bool(False),
                TSMMSK1 = cms.int32(312),
                TSMMSK2 = cms.int32(312),
                TSMNOE1 = cms.bool(True),
                TSMNOE2 = cms.bool(False),
                TSMNOEC = cms.bool(False),
                TSMWORD = cms.int32(255),
                TSSCCE1 = cms.bool(True),
                TSSCCE2 = cms.bool(False),
                TSSCCEC = cms.bool(False),
                TSSCGS1 = cms.bool(True),
                TSSCGS2 = cms.bool(True),
                TSSGS1 = cms.int32(1),
                TSSGS2 = cms.int32(1),
                TSSHTE1 = cms.bool(True),
                TSSHTE2 = cms.bool(False),
                TSSHTEC = cms.bool(False),
                TSSMSK1 = cms.int32(312),
                TSSMSK2 = cms.int32(312),
                TSSNOE1 = cms.bool(True),
                TSSNOE2 = cms.bool(False),
                TSSNOEC = cms.bool(False),
                TSTREN0 = cms.bool(True),
                TSTREN1 = cms.bool(True),
                TSTREN10 = cms.bool(True),
                TSTREN11 = cms.bool(True),
                TSTREN12 = cms.bool(True),
                TSTREN13 = cms.bool(True),
                TSTREN14 = cms.bool(True),
                TSTREN15 = cms.bool(True),
                TSTREN16 = cms.bool(True),
                TSTREN17 = cms.bool(True),
                TSTREN18 = cms.bool(True),
                TSTREN19 = cms.bool(True),
                TSTREN2 = cms.bool(True),
                TSTREN20 = cms.bool(True),
                TSTREN21 = cms.bool(True),
                TSTREN22 = cms.bool(True),
                TSTREN23 = cms.bool(True),
                TSTREN3 = cms.bool(True),
                TSTREN4 = cms.bool(True),
                TSTREN5 = cms.bool(True),
                TSTREN6 = cms.bool(True),
                TSTREN7 = cms.bool(True),
                TSTREN8 = cms.bool(True),
                TSTREN9 = cms.bool(True)
            ),
            TSThetaParameters = cms.PSet(
                Debug = cms.untracked.bool(False)
            ),
            TracoParameters = cms.PSet(
                BTIC = cms.int32(32),
                DD = cms.int32(18),
                Debug = cms.untracked.int32(0),
                FHISM = cms.int32(0),
                FHTMSK = cms.int32(0),
                FHTPRF = cms.int32(1),
                FLTMSK = cms.int32(1),
                FPRGCOMP = cms.int32(2),
                FSLMSK = cms.int32(0),
                IBTIOFF = cms.int32(0),
                KPRGCOM = cms.int32(255),
                KRAD = cms.int32(0),
                LTF = cms.int32(0),
                LTS = cms.int32(0),
                LVALIDIFH = cms.int32(0),
                REUSEI = cms.int32(1),
                REUSEO = cms.int32(1),
                SHISM = cms.int32(0),
                SHTMSK = cms.int32(0),
                SHTPRF = cms.int32(1),
                SLTMSK = cms.int32(1),
                SPRGCOMP = cms.int32(2),
                SSLMSK = cms.int32(0),
                TRGENB0 = cms.int32(1),
                TRGENB1 = cms.int32(1),
                TRGENB10 = cms.int32(1),
                TRGENB11 = cms.int32(1),
                TRGENB12 = cms.int32(1),
                TRGENB13 = cms.int32(1),
                TRGENB14 = cms.int32(1),
                TRGENB15 = cms.int32(1),
                TRGENB2 = cms.int32(1),
                TRGENB3 = cms.int32(1),
                TRGENB4 = cms.int32(1),
                TRGENB5 = cms.int32(1),
                TRGENB6 = cms.int32(1),
                TRGENB7 = cms.int32(1),
                TRGENB8 = cms.int32(1),
                TRGENB9 = cms.int32(1)
            )
        )
    ),
    TracoLutsFromDB = cms.bool(True),
    UseBtiAcceptParam = cms.bool(True),
    UseT0 = cms.bool(False),
    bxOffset = cms.int32(19),
    cfgConfig = cms.bool(False),
    debug = cms.bool(False),
    debugBti = cms.int32(0),
    debugDB = cms.bool(False),
    debugLUTs = cms.bool(False),
    debugPed = cms.bool(False),
    debugSC = cms.bool(False),
    debugTSP = cms.bool(False),
    debugTST = cms.bool(False),
    debugTU = cms.bool(False),
    debugTraco = cms.int32(0),
    finePhase = cms.double(25.0)
)


process.L1TGlobalPrescalesVetos = cms.ESProducer("L1TGlobalPrescalesVetosESProducer",
    AlgoBxMaskDefault = cms.int32(1),
    AlgoBxMaskXMLFile = cms.string('UGT_BASE_RS_ALGOBX_MASK_V1.xml'),
    FinOrMaskXMLFile = cms.string('UGT_BASE_RS_FINOR_MASK_v17.xml'),
    PrescaleXMLFile = cms.string('UGT_BASE_RS_PRESCALES_v11.xml'),
    TriggerMenuLuminosity = cms.string('startup'),
    Verbosity = cms.int32(0),
    VetoMaskXMLFile = cms.string('UGT_BASE_RS_VETO_MASK_v1.xml')
)


process.MaterialPropagator = cms.ESProducer("PropagatorWithMaterialESProducer",
    ComponentName = cms.string('PropagatorWithMaterial'),
    Mass = cms.double(0.105),
    MaxDPhi = cms.double(1.6),
    PropagationDirection = cms.string('alongMomentum'),
    SimpleMagneticField = cms.string(''),
    ptMin = cms.double(-1.0),
    useRungeKutta = cms.bool(False)
)


process.MaterialPropagatorForHI = cms.ESProducer("PropagatorWithMaterialESProducer",
    ComponentName = cms.string('PropagatorWithMaterialForHI'),
    Mass = cms.double(0.139),
    MaxDPhi = cms.double(1.6),
    PropagationDirection = cms.string('alongMomentum'),
    SimpleMagneticField = cms.string('ParabolicMf'),
    ptMin = cms.double(-1.0),
    useRungeKutta = cms.bool(False)
)


process.MaterialPropagatorParabolicMF = cms.ESProducer("PropagatorWithMaterialESProducer",
    ComponentName = cms.string('PropagatorWithMaterialParabolicMf'),
    Mass = cms.double(0.105),
    MaxDPhi = cms.double(1.6),
    PropagationDirection = cms.string('alongMomentum'),
    SimpleMagneticField = cms.string('ParabolicMf'),
    ptMin = cms.double(-1.0),
    useRungeKutta = cms.bool(False)
)


process.OppositeMaterialPropagator = cms.ESProducer("PropagatorWithMaterialESProducer",
    ComponentName = cms.string('PropagatorWithMaterialOpposite'),
    Mass = cms.double(0.105),
    MaxDPhi = cms.double(1.6),
    PropagationDirection = cms.string('oppositeToMomentum'),
    SimpleMagneticField = cms.string(''),
    ptMin = cms.double(-1.0),
    useRungeKutta = cms.bool(False)
)


process.OppositeMaterialPropagatorForHI = cms.ESProducer("PropagatorWithMaterialESProducer",
    ComponentName = cms.string('PropagatorWithMaterialOppositeForHI'),
    Mass = cms.double(0.139),
    MaxDPhi = cms.double(1.6),
    PropagationDirection = cms.string('oppositeToMomentum'),
    SimpleMagneticField = cms.string('ParabolicMf'),
    ptMin = cms.double(-1.0),
    useRungeKutta = cms.bool(False)
)


process.OppositeMaterialPropagatorParabolicMF = cms.ESProducer("PropagatorWithMaterialESProducer",
    ComponentName = cms.string('PropagatorWithMaterialParabolicMfOpposite'),
    Mass = cms.double(0.105),
    MaxDPhi = cms.double(1.6),
    PropagationDirection = cms.string('oppositeToMomentum'),
    SimpleMagneticField = cms.string('ParabolicMf'),
    ptMin = cms.double(-1.0),
    useRungeKutta = cms.bool(False)
)


process.OppositePropagatorWithMaterialForMixedStep = cms.ESProducer("PropagatorWithMaterialESProducer",
    ComponentName = cms.string('PropagatorWithMaterialForMixedStepOpposite'),
    Mass = cms.double(0.105),
    MaxDPhi = cms.double(1.6),
    PropagationDirection = cms.string('oppositeToMomentum'),
    SimpleMagneticField = cms.string('ParabolicMf'),
    ptMin = cms.double(0.1),
    useRungeKutta = cms.bool(False)
)


process.ParametrizedMagneticFieldProducer = cms.ESProducer("AutoParametrizedMagneticFieldProducer",
    label = cms.untracked.string('ParabolicMf'),
    valueOverride = cms.int32(-1),
    version = cms.string('Parabolic')
)


process.PropagatorWithMaterialForLoopers = cms.ESProducer("PropagatorWithMaterialESProducer",
    ComponentName = cms.string('PropagatorWithMaterialForLoopers'),
    Mass = cms.double(0.1396),
    MaxDPhi = cms.double(4.0),
    PropagationDirection = cms.string('alongMomentum'),
    SimpleMagneticField = cms.string('ParabolicMf'),
    ptMin = cms.double(-1.0),
    useRungeKutta = cms.bool(False)
)


process.PropagatorWithMaterialForMixedStep = cms.ESProducer("PropagatorWithMaterialESProducer",
    ComponentName = cms.string('PropagatorWithMaterialForMixedStep'),
    Mass = cms.double(0.105),
    MaxDPhi = cms.double(1.6),
    PropagationDirection = cms.string('alongMomentum'),
    SimpleMagneticField = cms.string('ParabolicMf'),
    ptMin = cms.double(0.1),
    useRungeKutta = cms.bool(False)
)


process.RPCGeometryESModule = cms.ESProducer("RPCGeometryESModule",
    compatibiltyWith11 = cms.untracked.bool(True),
    useDDD = cms.untracked.bool(False)
)


process.SiStripGainESProducer = cms.ESProducer("SiStripGainESProducer",
    APVGain = cms.VPSet(cms.PSet(
        Label = cms.untracked.string(''),
        NormalizationFactor = cms.untracked.double(1.0),
        Record = cms.string('SiStripApvGainRcd')
    ), 
        cms.PSet(
            Label = cms.untracked.string(''),
            NormalizationFactor = cms.untracked.double(1.0),
            Record = cms.string('SiStripApvGain2Rcd')
        )),
    AutomaticNormalization = cms.bool(False),
    appendToDataLabel = cms.string(''),
    printDebug = cms.untracked.bool(False)
)


process.SiStripQualityESProducer = cms.ESProducer("SiStripQualityESProducer",
    ListOfRecordToMerge = cms.VPSet(cms.PSet(
        record = cms.string('SiStripDetVOffRcd'),
        tag = cms.string('')
    ), 
        cms.PSet(
            record = cms.string('SiStripDetCablingRcd'),
            tag = cms.string('')
        ), 
        cms.PSet(
            record = cms.string('SiStripBadChannelRcd'),
            tag = cms.string('')
        ), 
        cms.PSet(
            record = cms.string('SiStripBadFiberRcd'),
            tag = cms.string('')
        ), 
        cms.PSet(
            record = cms.string('SiStripBadModuleRcd'),
            tag = cms.string('')
        )),
    PrintDebugOutput = cms.bool(False),
    ReduceGranularity = cms.bool(False),
    ThresholdForReducedGranularity = cms.double(0.3),
    UseEmptyRunInfo = cms.bool(False),
    appendToDataLabel = cms.string('')
)


process.SiStripRecHitMatcherESProducer = cms.ESProducer("SiStripRecHitMatcherESProducer",
    ComponentName = cms.string('StandardMatcher'),
    NSigmaInside = cms.double(3.0),
    PreFilter = cms.bool(False)
)


process.SiStripRegionConnectivity = cms.ESProducer("SiStripRegionConnectivity",
    EtaDivisions = cms.untracked.uint32(20),
    EtaMax = cms.untracked.double(2.5),
    PhiDivisions = cms.untracked.uint32(20)
)


process.SimpleSecondaryVertex3TrkComputer = cms.ESProducer("SimpleSecondaryVertexESProducer",
    minTracks = cms.uint32(3),
    minVertices = cms.uint32(1),
    unBoost = cms.bool(False),
    use3d = cms.bool(True),
    useSignificance = cms.bool(True)
)


process.StableParameters = cms.ESProducer("StableParametersTrivialProducer",
    IfCaloEtaNumberBits = cms.uint32(4),
    IfMuEtaNumberBits = cms.uint32(6),
    NumberChips = cms.uint32(5),
    NumberConditionChips = cms.uint32(1),
    NumberL1CenJet = cms.uint32(4),
    NumberL1EGamma = cms.uint32(12),
    NumberL1ForJet = cms.uint32(4),
    NumberL1IsoEG = cms.uint32(4),
    NumberL1Jet = cms.uint32(12),
    NumberL1JetCounts = cms.uint32(12),
    NumberL1Mu = cms.uint32(4),
    NumberL1Muon = cms.uint32(12),
    NumberL1NoIsoEG = cms.uint32(4),
    NumberL1Tau = cms.uint32(8),
    NumberL1TauJet = cms.uint32(4),
    NumberPhysTriggers = cms.uint32(512),
    NumberPhysTriggersExtended = cms.uint32(64),
    NumberPsbBoards = cms.int32(7),
    NumberTechnicalTriggers = cms.uint32(64),
    OrderConditionChip = cms.vint32(1),
    OrderOfChip = cms.vint32(1),
    PinsOnChip = cms.uint32(512),
    PinsOnConditionChip = cms.uint32(512),
    TotalBxInEvent = cms.int32(5),
    UnitLength = cms.int32(8),
    WordLength = cms.int32(64),
    appendToDataLabel = cms.string('')
)


process.SteppingHelixPropagatorAny = cms.ESProducer("SteppingHelixPropagatorESProducer",
    ApplyRadX0Correction = cms.bool(True),
    AssumeNoMaterial = cms.bool(False),
    ComponentName = cms.string('SteppingHelixPropagatorAny'),
    NoErrorPropagation = cms.bool(False),
    PropagationDirection = cms.string('anyDirection'),
    SetVBFPointer = cms.bool(False),
    VBFName = cms.string('VolumeBasedMagneticField'),
    debug = cms.bool(False),
    endcapShiftInZNeg = cms.double(0.0),
    endcapShiftInZPos = cms.double(0.0),
    returnTangentPlane = cms.bool(True),
    sendLogWarning = cms.bool(False),
    useEndcapShiftsInZ = cms.bool(False),
    useInTeslaFromMagField = cms.bool(False),
    useIsYokeFlag = cms.bool(True),
    useMagVolumes = cms.bool(True),
    useMatVolumes = cms.bool(True),
    useTuningForL2Speed = cms.bool(False)
)


process.TrackerDigiGeometryESModule = cms.ESProducer("TrackerDigiGeometryESModule",
    alignmentsLabel = cms.string(''),
    appendToDataLabel = cms.string(''),
    applyAlignment = cms.bool(True),
    fromDDD = cms.bool(False)
)


process.TrackerGeometricDetESModule = cms.ESProducer("TrackerGeometricDetESModule",
    appendToDataLabel = cms.string(''),
    fromDDD = cms.bool(False)
)


process.TransientTrackBuilderESProducer = cms.ESProducer("TransientTrackBuilderESProducer",
    ComponentName = cms.string('TransientTrackBuilder')
)


process.VolumeBasedMagneticFieldESProducer = cms.ESProducer("VolumeBasedMagneticFieldESProducerFromDB",
    debugBuilder = cms.untracked.bool(False),
    label = cms.untracked.string(''),
    valueOverride = cms.int32(-1)
)


process.ZdcGeometryFromDBEP = cms.ESProducer("ZdcGeometryFromDBEP",
    applyAlignment = cms.bool(False)
)


process.caloDetIdAssociator = cms.ESProducer("DetIdAssociatorESProducer",
    ComponentName = cms.string('CaloDetIdAssociator'),
    etaBinSize = cms.double(0.087),
    includeBadChambers = cms.bool(False),
    nEta = cms.int32(70),
    nPhi = cms.int32(72)
)


process.caloStage2Params = cms.ESProducer("L1TCaloStage2ParamsESProducer",
    centralityLUTFile = cms.FileInPath('L1Trigger/L1TCalorimeter/data/centralityLUT_stage1.txt'),
    centralityNodeVersion = cms.int32(1),
    centralityRegionMask = cms.int32(0),
    egBypassEGVetos = cms.uint32(0),
    egCalibrationLUTFile = cms.FileInPath('L1Trigger/L1TCalorimeter/data/corrections_Trimming10_compressedieta_compressedE_compressedshape_v16.03.14.txt'),
    egCalibrationType = cms.string('compressed'),
    egCalibrationVersion = cms.uint32(0),
    egCompressShapesLUTFile = cms.FileInPath('L1Trigger/L1TCalorimeter/data/egCompressLUT_v4.txt'),
    egEtaCut = cms.int32(0),
    egHOverEcutBarrel = cms.int32(5),
    egHOverEcutEndcap = cms.int32(4),
    egHcalThreshold = cms.double(0.0),
    egIsoAreaNrTowersEta = cms.uint32(2),
    egIsoAreaNrTowersPhi = cms.uint32(4),
    egIsoLUTFile = cms.FileInPath('L1Trigger/L1TCalorimeter/data/IsoIdentification_adapt_extrap_v16.08.08.txt'),
    egIsoMaxEtaAbsForIsoSum = cms.uint32(27),
    egIsoMaxEtaAbsForTowerSum = cms.uint32(4),
    egIsoPUEstTowerGranularity = cms.uint32(1),
    egIsoVetoNrTowersPhi = cms.uint32(2),
    egIsolationType = cms.string('compressed'),
    egLsb = cms.double(0.5),
    egMaxHOverE = cms.double(0.15),
    egMaxHOverELUTFile = cms.FileInPath('L1Trigger/L1TCalorimeter/data/HoverEIdentification_0.995_v15.12.23.txt'),
    egMaxHcalEt = cms.double(0.0),
    egMaxPtHOverE = cms.double(128.0),
    egMaxPtHOverEIsolation = cms.int32(40),
    egMaxPtJetIsolation = cms.int32(63),
    egMinPtHOverEIsolation = cms.int32(1),
    egMinPtJetIsolation = cms.int32(25),
    egNeighbourThreshold = cms.double(1.0),
    egPUSParams = cms.vdouble(1, 4, 32),
    egPUSType = cms.string('None'),
    egSeedThreshold = cms.double(2.0),
    egShapeIdLUTFile = cms.FileInPath('L1Trigger/L1TCalorimeter/data/shapeIdentification_adapt0.99_compressedieta_compressedE_compressedshape_v15.12.08.txt'),
    egShapeIdType = cms.string('compressed'),
    egShapeIdVersion = cms.uint32(0),
    egTrimmingLUTFile = cms.FileInPath('L1Trigger/L1TCalorimeter/data/egTrimmingLUT_10_v16.01.19.txt'),
    etSumEcalSumPUSLUTFile = cms.FileInPath('L1Trigger/L1TCalorimeter/data/lut_etSumPUS_dummy.txt'),
    etSumEtThreshold = cms.vdouble(0.0, 30.0, 0.0, 30.0, 0.0),
    etSumEtaMax = cms.vint32(28, 28, 28, 28, 27),
    etSumEtaMin = cms.vint32(1, 1, 1, 1, 1),
    etSumEttPUSLUTFile = cms.FileInPath('L1Trigger/L1TCalorimeter/data/lut_etSumPUS_dummy.txt'),
    etSumLsb = cms.double(0.5),
    etSumXPUSLUTFile = cms.FileInPath('L1Trigger/L1TCalorimeter/data/lut_etSumPUS_dummy.txt'),
    etSumYPUSLUTFile = cms.FileInPath('L1Trigger/L1TCalorimeter/data/lut_etSumPUS_dummy.txt'),
    isoTauEtaMax = cms.int32(17),
    isoTauEtaMin = cms.int32(0),
    jetBypassPUS = cms.uint32(0),
    jetCalibrationLUTFile = cms.FileInPath('L1Trigger/L1TCalorimeter/data/lut_30to40_hfHighPt_experiment2_changeLimits_add_mult.txt'),
    jetCalibrationParams = cms.vdouble(1, 0, 1, 0, 1, 
        1, 1.36123039014, 1024, 1, 0, 
        1, 0, 1, 1, 1.37830172245, 
        1024, 1, 0, 1, 0, 
        1, 1, 1.37157036457, 1024, 1, 
        0, 1, 0, 1, 1, 
        1.42460009989, 1024, 10.1179757811, -697.422255848, 55.9767511168, 
        599.040770412, 0.00930772659892, -21.9921521313, 1.77585386314, 24.1202894336, 
        12.2578170485, -736.96846599, 45.3225355911, 848.976802835, 0.00946235693865, 
        -21.7970133915, 2.04623980351, 19.6049149791, 14.0198255047, -769.175319944, 
        38.687351315, 1072.9785137, 0.00951954709279, -21.6277409602, 2.08021511285, 
        22.265051562, 14.119589176, -766.199501821, 38.7767169666, 1059.63374337, 
        0.00952979125289, -21.6477483043, 2.05901166216, 23.8125466978, 13.7594864391, 
        -761.860391454, 39.9060363401, 1019.30588542, 0.00952105483129, -21.6814176696, 
        2.03808638982, 22.2127275989, 10.2635352836, -466.890522023, 32.5408463829, 
        2429.03382746, 0.0111274121697, -22.0890253377, 2.04880080215, 22.5083699943, 
        5.46086027683, -150.888778124, 18.3292242153, 16968.6469599, 0.0147496053457, 
        -22.4089831889, 2.08107691501, 22.4129703515, 5.46086027683, -150.888778124, 
        18.3292242153, 16968.6469599, 0.0147496053457, -22.4089831889, 2.08107691501, 
        22.4129703515, 10.2635352836, -466.890522023, 32.5408463829, 2429.03382746, 
        0.0111274121697, -22.0890253377, 2.04880080215, 22.5083699943, 13.7594864391, 
        -761.860391454, 39.9060363401, 1019.30588542, 0.00952105483129, -21.6814176696, 
        2.03808638982, 22.2127275989, 14.119589176, -766.199501821, 38.7767169666, 
        1059.63374337, 0.00952979125289, -21.6477483043, 2.05901166216, 23.8125466978, 
        14.0198255047, -769.175319944, 38.687351315, 1072.9785137, 0.00951954709279, 
        -21.6277409602, 2.08021511285, 22.265051562, 12.2578170485, -736.96846599, 
        45.3225355911, 848.976802835, 0.00946235693865, -21.7970133915, 2.04623980351, 
        19.6049149791, 10.1179757811, -697.422255848, 55.9767511168, 599.040770412, 
        0.00930772659892, -21.9921521313, 1.77585386314, 24.1202894336, 1, 
        0, 1, 0, 1, 1, 
        1.42460009989, 1024, 1, 0, 1, 
        0, 1, 1, 1.37157036457, 1024, 
        1, 0, 1, 0, 1, 
        1, 1.37830172245, 1024, 1, 0, 
        1, 0, 1, 1, 1.36123039014, 
        1024),
    jetCalibrationType = cms.string('LUT'),
    jetCompressEtaLUTFile = cms.FileInPath('L1Trigger/L1TCalorimeter/data/lut_30to40_hfHighPt_experiment2_changeLimits_eta.txt'),
    jetCompressPtLUTFile = cms.FileInPath('L1Trigger/L1TCalorimeter/data/lut_pt_compress.txt'),
    jetLsb = cms.double(0.5),
    jetNeighbourThreshold = cms.double(0.0),
    jetPUSType = cms.string('ChunkyDonut'),
    jetRegionMask = cms.int32(0),
    jetSeedThreshold = cms.double(4.0),
    layer1ECalScaleETBins = cms.vint32(10, 15, 20, 25, 30, 
        35, 40, 45, 256),
    layer1ECalScaleFactors = cms.vdouble(1.1847, 1.16759, 1.17779, 1.19955, 1.21125, 
        1.214, 1.21503, 1.22515, 1.24151, 1.27836, 
        1.30292, 1.33526, 1.42338, 1.4931, 1.49597, 
        1.50405, 1.52785, 1.81552, 1.59856, 1.75692, 
        1.76496, 1.77562, 1.69527, 1.66827, 1.61861, 
        1.56645, 1.56645, 1.56645, 1.1351, 1.12589, 
        1.12834, 1.13725, 1.14408, 1.1494, 1.14296, 
        1.14852, 1.1578, 1.17634, 1.18038, 1.19386, 
        1.23758, 1.27605, 1.27818, 1.28195, 1.34881, 
        1.71053, 1.37338, 1.52571, 1.54801, 1.53316, 
        1.4397, 1.40497, 1.37743, 1.33914, 1.33914, 
        1.33914, 1.18043, 1.17823, 1.1751, 1.17608, 
        1.19152, 1.196, 1.20125, 1.2068, 1.22584, 
        1.22476, 1.22395, 1.22302, 1.25137, 1.28097, 
        1.29871, 1.2862, 1.33489, 1.60937, 1.28365, 
        1.41367, 1.42521, 1.42041, 1.36784, 1.34922, 
        1.32754, 1.29825, 1.29825, 1.29825, 1.11664, 
        1.11852, 1.11861, 1.12367, 1.12405, 1.14814, 
        1.14304, 1.15337, 1.16607, 1.18698, 1.17048, 
        1.17463, 1.2185, 1.23842, 1.23214, 1.24744, 
        1.30047, 1.47152, 1.22868, 1.33121, 1.34841, 
        1.35178, 1.30048, 1.28537, 1.27012, 1.24159, 
        1.24159, 1.24159, 1.08422, 1.08146, 1.08706, 
        1.08906, 1.08636, 1.10092, 1.10363, 1.11102, 
        1.1186, 1.13301, 1.12369, 1.14377, 1.16477, 
        1.17801, 1.18782, 1.17168, 1.24593, 1.36835, 
        1.20252, 1.28349, 1.29828, 1.30328, 1.26848, 
        1.25817, 1.2464, 1.22259, 1.22259, 1.22259, 
        1.07444, 1.06774, 1.06883, 1.0707, 1.07881, 
        1.08859, 1.08285, 1.08747, 1.09736, 1.10678, 
        1.10008, 1.10717, 1.12858, 1.15383, 1.15826, 
        1.14855, 1.19911, 1.32567, 1.17553, 1.25976, 
        1.27926, 1.28459, 1.24524, 1.23706, 1.22597, 
        1.20006, 1.20006, 1.20006, 1.06224, 1.05968, 
        1.05767, 1.06254, 1.06729, 1.0691, 1.07125, 
        1.07312, 1.08124, 1.08966, 1.08695, 1.08826, 
        1.10611, 1.13115, 1.12641, 1.13093, 1.17074, 
        1.28958, 1.16217, 1.22844, 1.24812, 1.25352, 
        1.22065, 1.21287, 1.20544, 1.18344, 1.18344, 
        1.18344, 1.03589, 1.03224, 1.03229, 1.03623, 
        1.03979, 1.04403, 1.04574, 1.049, 1.04821, 
        1.06183, 1.0588, 1.06655, 1.08582, 1.10289, 
        1.10052, 1.10506, 1.143, 1.27373, 1.1459, 
        1.2156, 1.23455, 1.23968, 1.20753, 1.20127, 
        1.19629, 1.16809, 1.16809, 1.16809, 1.03456, 
        1.02955, 1.03079, 1.03509, 1.03949, 1.0437, 
        1.04236, 1.04486, 1.0517, 1.05864, 1.05516, 
        1.06167, 1.07738, 1.0985, 1.09317, 1.09559, 
        1.13557, 1.26076, 1.14118, 1.20545, 1.22137, 
        1.22802, 1.19936, 1.19676, 1.19088, 1.16709, 
        1.16709, 1.16709),
    layer1HCalScaleETBins = cms.vint32(10, 15, 20, 25, 30, 
        35, 40, 45, 256),
    layer1HCalScaleFactors = cms.vdouble(1.511112, 1.5199, 1.499483, 1.48856, 1.528111, 
        1.475114, 1.476616, 1.514163, 1.515306, 1.542464, 
        1.511663, 1.593745, 1.493667, 1.485315, 1.419925, 
        1.349169, 1.312518, 1.423302, 1.478461, 1.525868, 
        1.525868, 1.525868, 1.525868, 1.525868, 1.525868, 
        1.525868, 1.525868, 1.525868, 1.38335, 1.3657, 
        1.36847, 1.35461, 1.34848, 1.32972, 1.27225, 
        1.30171, 1.32221, 1.36086, 1.33385, 1.3922, 
        1.40306, 1.39487, 1.32205, 1.24457, 1.20691, 
        1.32187, 1.34416, 1.40327, 1.40327, 1.40327, 
        1.40327, 1.40327, 1.40327, 1.40327, 1.40327, 
        1.40327, 1.24569, 1.23832, 1.24542, 1.23483, 
        1.24373, 1.24979, 1.17945, 1.21362, 1.21903, 
        1.25213, 1.20956, 1.25071, 1.28049, 1.2628, 
        1.25406, 1.18681, 1.12783, 1.26, 1.27514, 
        1.30585, 1.30585, 1.30585, 1.30585, 1.30585, 
        1.30585, 1.30585, 1.30585, 1.30585, 1.18994, 
        1.18912, 1.17712, 1.17969, 1.18551, 1.15059, 
        1.15183, 1.16786, 1.15431, 1.16319, 1.1617, 
        1.1361, 1.16187, 1.19505, 1.15391, 1.1179, 
        1.10675, 1.20812, 1.16002, 1.2328, 1.2328, 
        1.2328, 1.2328, 1.2328, 1.2328, 1.2328, 
        1.2328, 1.2328, 1.12254, 1.12952, 1.12508, 
        1.11515, 1.11825, 1.09619, 1.10817, 1.08749, 
        1.10975, 1.09978, 1.081, 1.05061, 1.07827, 
        1.07946, 1.04774, 1.0414, 1.04175, 1.11688, 
        1.09773, 1.12578, 1.12578, 1.12578, 1.12578, 
        1.12578, 1.12578, 1.12578, 1.12578, 1.12578, 
        1.11047, 1.11734, 1.11598, 1.08849, 1.08826, 
        1.07823, 1.06272, 1.05469, 1.05327, 1.08664, 
        1.05062, 1.03847, 1.04644, 1.05913, 1.01224, 
        1.03903, 1.03604, 1.08846, 1.07888, 1.0906, 
        1.0906, 1.0906, 1.0906, 1.0906, 1.0906, 
        1.0906, 1.0906, 1.0906, 1.11597, 1.11101, 
        1.11317, 1.07939, 1.07685, 1.06373, 1.0393, 
        1.04991, 1.0401, 1.02582, 1.01583, 1.01585, 
        1.01081, 1.01421, 0.980321, 1.02358, 1.04599, 
        1.07322, 1.05775, 1.05985, 1.05985, 1.05985, 
        1.05985, 1.05985, 1.05985, 1.05985, 1.05985, 
        1.05985, 1.06118, 1.05977, 1.07121, 1.06442, 
        1.06534, 1.04307, 1.0414, 1.02268, 1.01741, 
        1.01769, 1.00561, 1.00636, 0.99942, 0.990866, 
        0.986723, 0.989036, 0.995116, 1.04562, 1.02433, 
        1.04066, 1.04066, 1.04066, 1.04066, 1.04066, 
        1.04066, 1.04066, 1.04066, 1.04066, 1.08315, 
        1.06709, 1.08318, 1.06101, 1.07564, 1.05164, 
        1.03876, 1.04267, 1.01091, 1.01158, 1.00656, 
        0.984468, 0.986642, 0.985799, 0.968133, 1.00029, 
        1.01121, 1.04669, 1.01667, 1.02047, 1.02047, 
        1.02047, 1.02047, 1.02047, 1.02047, 1.02047, 
        1.02047, 1.02047),
    layer1HFScaleETBins = cms.vint32(5, 20, 30, 50, 256),
    layer1HFScaleFactors = cms.vdouble(1.0, 1.0, 1.0, 1.0, 1.0, 
        1.0, 1.0, 1.0, 1.0, 1.0, 
        1.0, 1.0, 1.76708, 1.76708, 1.755186, 
        1.769951, 1.763527, 1.791043, 1.898787, 1.982235, 
        2.071074, 2.193011, 2.356886, 2.403384, 2.170477, 
        2.170477, 2.12354, 2.019866, 1.907698, 1.963179, 
        1.989122, 2.035251, 2.184642, 2.436399, 2.810884, 
        2.92375, 1.943941, 1.943941, 1.899826, 1.81395, 
        1.714978, 1.736184, 1.785928, 1.834211, 1.94423, 
        2.153565, 2.720887, 2.749795, 1.679984, 1.679984, 
        1.669753, 1.601871, 1.547276, 1.577805, 1.611497, 
        1.670184, 1.775022, 1.937061, 2.488311, 2.618629),
    minimumBiasThresholds = cms.vint32(0, 0, 0, 0),
    q2LUTFile = cms.FileInPath('L1Trigger/L1TCalorimeter/data/q2LUT_stage1.txt'),
    regionLsb = cms.double(0.5),
    regionPUSParams = cms.vdouble(),
    regionPUSType = cms.string('None'),
    regionPUSVersion = cms.int32(0),
    tauCalibrationLUTFile = cms.FileInPath('L1Trigger/L1TCalorimeter/data/Tau_Calibration_LUT_NewLayer1Calibration_SK1616_EmuOldFormat_v11.0.0.txt'),
    tauCalibrationLUTFileEta = cms.FileInPath('L1Trigger/L1TCalorimeter/data/tauCalibrationLUTEta.txt'),
    tauCompressLUTFile = cms.FileInPath('L1Trigger/L1TCalorimeter/data/tauCompressAllLUT_12bit_v3.txt'),
    tauEtToHFRingEtLUTFile = cms.FileInPath('L1Trigger/L1TCalorimeter/data/tauHwEtToHFRingScale_LUT.txt'),
    tauIsoAreaNrTowersEta = cms.uint32(2),
    tauIsoAreaNrTowersPhi = cms.uint32(4),
    tauIsoLUTFile = cms.FileInPath('L1Trigger/L1TCalorimeter/data/Tau_Iso_LUT_Option_22_NewLayer1Calibration_noCompressionBlock_SK1616_EmuOldFormat_v6.2.0.txt'),
    tauIsoLUTFile2 = cms.FileInPath('L1Trigger/L1TCalorimeter/data/Tau_Iso_LUT_Option_22_NewLayer1Calibration_noCompressionBlock_SK1616_EmuOldFormat_v6.2.0.txt'),
    tauIsoVetoNrTowersPhi = cms.uint32(2),
    tauLsb = cms.double(0.5),
    tauMaxJetIsolationA = cms.double(0.1),
    tauMaxJetIsolationB = cms.double(100.0),
    tauMaxPtTauVeto = cms.double(64.0),
    tauMinPtJetIsolationB = cms.double(192.0),
    tauNeighbourThreshold = cms.double(0.0),
    tauPUSParams = cms.vdouble(1, 4, 32),
    tauPUSType = cms.string('None'),
    tauRegionMask = cms.int32(0),
    tauSeedThreshold = cms.double(0.0),
    towerEncoding = cms.bool(True),
    towerLsbE = cms.double(0.5),
    towerLsbH = cms.double(0.5),
    towerLsbSum = cms.double(0.5),
    towerNBitsE = cms.int32(8),
    towerNBitsH = cms.int32(8),
    towerNBitsRatio = cms.int32(3),
    towerNBitsSum = cms.int32(9)
)


process.cosmicsNavigationSchoolESProducer = cms.ESProducer("NavigationSchoolESProducer",
    ComponentName = cms.string('CosmicNavigationSchool'),
    SimpleMagneticField = cms.string('')
)


process.ecalDetIdAssociator = cms.ESProducer("DetIdAssociatorESProducer",
    ComponentName = cms.string('EcalDetIdAssociator'),
    etaBinSize = cms.double(0.02),
    includeBadChambers = cms.bool(False),
    nEta = cms.int32(300),
    nPhi = cms.int32(360)
)


process.ecalSeverityLevel = cms.ESProducer("EcalSeverityLevelESProducer",
    dbstatusMask = cms.PSet(
        kBad = cms.vstring('kNonRespondingIsolated', 
            'kDeadVFE', 
            'kDeadFE', 
            'kNoDataNoTP'),
        kGood = cms.vstring('kOk'),
        kProblematic = cms.vstring('kDAC', 
            'kNoLaser', 
            'kNoisy', 
            'kNNoisy', 
            'kNNNoisy', 
            'kNNNNoisy', 
            'kNNNNNoisy', 
            'kFixedG6', 
            'kFixedG1', 
            'kFixedG0'),
        kRecovered = cms.vstring(),
        kTime = cms.vstring(),
        kWeird = cms.vstring()
    ),
    flagMask = cms.PSet(
        kBad = cms.vstring('kFaultyHardware', 
            'kDead', 
            'kKilled'),
        kGood = cms.vstring('kGood'),
        kProblematic = cms.vstring('kPoorReco', 
            'kPoorCalib', 
            'kNoisy', 
            'kSaturated'),
        kRecovered = cms.vstring('kLeadingEdgeRecovered', 
            'kTowerRecovered'),
        kTime = cms.vstring('kOutOfTime'),
        kWeird = cms.vstring('kWeird', 
            'kDiWeird')
    ),
    timeThresh = cms.double(2.0)
)


process.emtfParams = cms.ESProducer("L1TMuonEndCapForestESProducer")


process.fakeBmtfParams = cms.ESProducer("L1TMuonBarrelParamsESProducer",
    AssLUTPath = cms.string('L1Trigger/L1TMuon/data/bmtf_luts/LUTs_Ass/'),
    BX_max = cms.int32(2),
    BX_min = cms.int32(-2),
    DisableNewAlgo = cms.bool(False),
    EtaTrackFinder = cms.bool(True),
    Extrapolation_21 = cms.bool(False),
    Extrapolation_Filter = cms.int32(1),
    Extrapolation_nbits_Phi = cms.int32(8),
    Extrapolation_nbits_PhiB = cms.int32(8),
    Open_LUTs = cms.bool(False),
    OutOfTime_Filter = cms.bool(False),
    OutOfTime_Filter_Window = cms.int32(1),
    PHI_Assignment_nbits_Phi = cms.int32(12),
    PHI_Assignment_nbits_PhiB = cms.int32(10),
    PT_Assignment_nbits_Phi = cms.int32(12),
    PT_Assignment_nbits_PhiB = cms.int32(10),
    configFromXML = cms.bool(False),
    fwVersion = cms.uint32(2),
    hwXmlFile = cms.string('L1Trigger/L1TMuonBarell/test/BMTF_HW.xml'),
    mask_ettf_st1 = cms.vstring('111111111111', 
        '000000000000', 
        '000000000000', 
        '000000000000', 
        '000000000000', 
        '000000000000', 
        '111111111111'),
    mask_ettf_st2 = cms.vstring('000000000000', 
        '000000000000', 
        '000000000000', 
        '000000000000', 
        '000000000000', 
        '000000000000', 
        '000000000000'),
    mask_ettf_st3 = cms.vstring('000000000000', 
        '000000000000', 
        '000000000000', 
        '000000000000', 
        '000000000000', 
        '000000000000', 
        '000000000000'),
    mask_phtf_st1 = cms.vstring('111111111111', 
        '000000000000', 
        '000000000000', 
        '000000000000', 
        '000000000000', 
        '000000000000', 
        '111111111111'),
    mask_phtf_st2 = cms.vstring('000000000000', 
        '000000000000', 
        '000000000000', 
        '000000000000', 
        '000000000000', 
        '000000000000', 
        '000000000000'),
    mask_phtf_st3 = cms.vstring('000000000000', 
        '000000000000', 
        '000000000000', 
        '000000000000', 
        '000000000000', 
        '000000000000', 
        '000000000000'),
    mask_phtf_st4 = cms.vstring('000000000000', 
        '000000000000', 
        '000000000000', 
        '000000000000', 
        '000000000000', 
        '000000000000', 
        '000000000000'),
    topCfgXmlFile = cms.string('L1Trigger/L1TMuonBarell/test/bmtf_top_config_p5.xml'),
    xmlCfgKey = cms.string('RunKey_1')
)


process.gmtParams = cms.ESProducer("L1TMuonGlobalParamsESProducer",
    AbsIsoCheckMemLUTPath = cms.string('L1Trigger/L1TMuon/data/microgmt_luts/AbsIsoCheckMem.txt'),
    BEtaExtrapolationLUTPath = cms.string('L1Trigger/L1TMuon/data/microgmt_luts/BEtaExtrapolation.txt'),
    BONegMatchQualLUTMaxDR = cms.double(0.1),
    BONegMatchQualLUTPath = cms.string(''),
    BONegMatchQualLUTfEta = cms.double(1),
    BONegMatchQualLUTfEtaCoarse = cms.double(1),
    BONegMatchQualLUTfPhi = cms.double(1),
    BOPosMatchQualLUTMaxDR = cms.double(0.1),
    BOPosMatchQualLUTPath = cms.string(''),
    BOPosMatchQualLUTfEta = cms.double(1),
    BOPosMatchQualLUTfEtaCoarse = cms.double(1),
    BOPosMatchQualLUTfPhi = cms.double(1),
    BPhiExtrapolationLUTPath = cms.string('L1Trigger/L1TMuon/data/microgmt_luts/BPhiExtrapolation.txt'),
    FEtaExtrapolationLUTPath = cms.string('L1Trigger/L1TMuon/data/microgmt_luts/EEtaExtrapolation.txt'),
    FONegMatchQualLUTMaxDR = cms.double(0.1),
    FONegMatchQualLUTPath = cms.string(''),
    FONegMatchQualLUTfEta = cms.double(1),
    FONegMatchQualLUTfEtaCoarse = cms.double(1),
    FONegMatchQualLUTfPhi = cms.double(1),
    FOPosMatchQualLUTMaxDR = cms.double(0.1),
    FOPosMatchQualLUTPath = cms.string(''),
    FOPosMatchQualLUTfEta = cms.double(1),
    FOPosMatchQualLUTfEtaCoarse = cms.double(1),
    FOPosMatchQualLUTfPhi = cms.double(1),
    FPhiExtrapolationLUTPath = cms.string('L1Trigger/L1TMuon/data/microgmt_luts/EPhiExtrapolation.txt'),
    FwdNegSingleMatchQualLUTMaxDR = cms.double(0.1),
    FwdNegSingleMatchQualLUTPath = cms.string(''),
    FwdNegSingleMatchQualLUTfEta = cms.double(1),
    FwdNegSingleMatchQualLUTfPhi = cms.double(1),
    FwdPosSingleMatchQualLUTMaxDR = cms.double(0.1),
    FwdPosSingleMatchQualLUTPath = cms.string(''),
    FwdPosSingleMatchQualLUTfEta = cms.double(1),
    FwdPosSingleMatchQualLUTfPhi = cms.double(1),
    IdxSelMemEtaLUTPath = cms.string('L1Trigger/L1TMuon/data/microgmt_luts/IdxSelMemEta.txt'),
    IdxSelMemPhiLUTPath = cms.string('L1Trigger/L1TMuon/data/microgmt_luts/IdxSelMemPhi.txt'),
    OEtaExtrapolationLUTPath = cms.string('L1Trigger/L1TMuon/data/microgmt_luts/OEtaExtrapolation.txt'),
    OPhiExtrapolationLUTPath = cms.string('L1Trigger/L1TMuon/data/microgmt_luts/OPhiExtrapolation.txt'),
    OvlNegSingleMatchQualLUTMaxDR = cms.double(0.1),
    OvlNegSingleMatchQualLUTPath = cms.string(''),
    OvlNegSingleMatchQualLUTfEta = cms.double(1),
    OvlNegSingleMatchQualLUTfEtaCoarse = cms.double(1),
    OvlNegSingleMatchQualLUTfPhi = cms.double(1),
    OvlPosSingleMatchQualLUTMaxDR = cms.double(0.1),
    OvlPosSingleMatchQualLUTPath = cms.string(''),
    OvlPosSingleMatchQualLUTfEta = cms.double(1),
    OvlPosSingleMatchQualLUTfEtaCoarse = cms.double(1),
    OvlPosSingleMatchQualLUTfPhi = cms.double(1),
    RelIsoCheckMemLUTPath = cms.string('L1Trigger/L1TMuon/data/microgmt_luts/RelIsoCheckMem.txt'),
    SortRankLUTPath = cms.string('L1Trigger/L1TMuon/data/microgmt_luts/SortRank.txt'),
    SortRankLUTPtFactor = cms.uint32(1),
    SortRankLUTQualFactor = cms.uint32(4),
    bmtfInputsToDisable = cms.vuint32(0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 
        0, 0),
    caloInputsDisable = cms.bool(False),
    caloInputsMasked = cms.bool(False),
    configFromXml = cms.bool(False),
    emtfInputsToDisable = cms.vuint32(0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 
        0, 0),
    fwVersion = cms.uint32(33685504),
    hwXmlFile = cms.string('L1Trigger/L1TMuon/data/o2o/ugmt/UGMT_HW.xml'),
    maskedBmtfInputs = cms.vuint32(0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 
        0, 0),
    maskedEmtfInputs = cms.vuint32(0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 
        0, 0),
    maskedOmtfInputs = cms.vuint32(0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 
        0, 0),
    omtfInputsToDisable = cms.vuint32(0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 
        0, 0),
    topCfgXmlFile = cms.string('L1Trigger/L1TMuon/data/o2o/ugmt/ugmt_top_config_p5.xml'),
    uGmtProcessorId = cms.string('ugmt_processor'),
    xmlCfgKey = cms.string('TestKey1')
)


process.hcalDDDRecConstants = cms.ESProducer("HcalDDDRecConstantsESModule",
    appendToDataLabel = cms.string('')
)


process.hcalDDDSimConstants = cms.ESProducer("HcalDDDSimConstantsESModule",
    appendToDataLabel = cms.string('')
)


process.hcalDetIdAssociator = cms.ESProducer("DetIdAssociatorESProducer",
    ComponentName = cms.string('HcalDetIdAssociator'),
    etaBinSize = cms.double(0.087),
    includeBadChambers = cms.bool(False),
    nEta = cms.int32(70),
    nPhi = cms.int32(72)
)


process.hcalRecAlgos = cms.ESProducer("HcalRecAlgoESProducer",
    DropChannelStatusBits = cms.vstring('HcalCellMask', 
        'HcalCellOff', 
        'HcalCellDead'),
    RecoveredRecHitBits = cms.vstring('TimingAddedBit', 
        'TimingSubtractedBit'),
    SeverityLevels = cms.VPSet(cms.PSet(
        ChannelStatus = cms.vstring(),
        Level = cms.int32(0),
        RecHitFlags = cms.vstring()
    ), 
        cms.PSet(
            ChannelStatus = cms.vstring('HcalCellCaloTowerProb'),
            Level = cms.int32(1),
            RecHitFlags = cms.vstring()
        ), 
        cms.PSet(
            ChannelStatus = cms.vstring('HcalCellExcludeFromHBHENoiseSummary'),
            Level = cms.int32(5),
            RecHitFlags = cms.vstring('HSCP_R1R2', 
                'HSCP_FracLeader', 
                'HSCP_OuterEnergy', 
                'HSCP_ExpFit', 
                'ADCSaturationBit', 
                'HBHEIsolatedNoise', 
                'AddedSimHcalNoise')
        ), 
        cms.PSet(
            ChannelStatus = cms.vstring(),
            Level = cms.int32(8),
            RecHitFlags = cms.vstring('HBHEHpdHitMultiplicity', 
                'HBHEPulseShape', 
                'HOBit', 
                'HFInTimeWindow', 
                'ZDCBit', 
                'CalibrationBit', 
                'TimingErrorBit', 
                'HBHETriangleNoise', 
                'HBHETS4TS5Noise')
        ), 
        cms.PSet(
            ChannelStatus = cms.vstring(),
            Level = cms.int32(11),
            RecHitFlags = cms.vstring('HFLongShort', 
                'HFPET', 
                'HFS8S1Ratio', 
                'HFDigiTime')
        ), 
        cms.PSet(
            ChannelStatus = cms.vstring('HcalCellCaloTowerMask'),
            Level = cms.int32(12),
            RecHitFlags = cms.vstring('HBHEFlatNoise', 
                'HBHESpikeNoise')
        ), 
        cms.PSet(
            ChannelStatus = cms.vstring('HcalCellHot'),
            Level = cms.int32(15),
            RecHitFlags = cms.vstring()
        ), 
        cms.PSet(
            ChannelStatus = cms.vstring('HcalCellOff', 
                'HcalCellDead'),
            Level = cms.int32(20),
            RecHitFlags = cms.vstring()
        ))
)


process.hcal_db_producer = cms.ESProducer("HcalDbProducer")


process.hltCombinedSecondaryVertex = cms.ESProducer("CombinedSecondaryVertexESProducer",
    SoftLeptonFlip = cms.bool(False),
    calibrationRecords = cms.vstring('CombinedSVRecoVertex', 
        'CombinedSVPseudoVertex', 
        'CombinedSVNoVertex'),
    categoryVariableName = cms.string('vertexCategory'),
    charmCut = cms.double(1.5),
    correctVertexMass = cms.bool(True),
    minimumTrackWeight = cms.double(0.5),
    pseudoMultiplicityMin = cms.uint32(2),
    pseudoVertexV0Filter = cms.PSet(
        k0sMassWindow = cms.double(0.05)
    ),
    recordLabel = cms.string('HLT'),
    trackFlip = cms.bool(False),
    trackMultiplicityMin = cms.uint32(3),
    trackPairV0Filter = cms.PSet(
        k0sMassWindow = cms.double(0.03)
    ),
    trackPseudoSelection = cms.PSet(
        jetDeltaRMax = cms.double(0.3),
        maxDecayLen = cms.double(5.0),
        maxDistToAxis = cms.double(0.07),
        normChi2Max = cms.double(99999.9),
        pixelHitsMin = cms.uint32(0),
        ptMin = cms.double(0.0),
        qualityClass = cms.string('any'),
        sip2dSigMax = cms.double(99999.9),
        sip2dSigMin = cms.double(2.0),
        sip2dValMax = cms.double(99999.9),
        sip2dValMin = cms.double(-99999.9),
        sip3dSigMax = cms.double(99999.9),
        sip3dSigMin = cms.double(-99999.9),
        sip3dValMax = cms.double(99999.9),
        sip3dValMin = cms.double(-99999.9),
        totalHitsMin = cms.uint32(0)
    ),
    trackSelection = cms.PSet(
        jetDeltaRMax = cms.double(0.3),
        maxDecayLen = cms.double(5.0),
        maxDistToAxis = cms.double(0.07),
        normChi2Max = cms.double(99999.9),
        pixelHitsMin = cms.uint32(0),
        ptMin = cms.double(0.0),
        qualityClass = cms.string('any'),
        sip2dSigMax = cms.double(99999.9),
        sip2dSigMin = cms.double(-99999.9),
        sip2dValMax = cms.double(99999.9),
        sip2dValMin = cms.double(-99999.9),
        sip3dSigMax = cms.double(99999.9),
        sip3dSigMin = cms.double(-99999.9),
        sip3dValMax = cms.double(99999.9),
        sip3dValMin = cms.double(-99999.9),
        totalHitsMin = cms.uint32(0)
    ),
    trackSort = cms.string('sip2dSig'),
    useCategories = cms.bool(True),
    useTrackWeights = cms.bool(True),
    vertexFlip = cms.bool(False)
)


process.hltCombinedSecondaryVertexV2 = cms.ESProducer("CombinedSecondaryVertexESProducer",
    SoftLeptonFlip = cms.bool(False),
    calibrationRecords = cms.vstring('CombinedSVIVFV2RecoVertex', 
        'CombinedSVIVFV2PseudoVertex', 
        'CombinedSVIVFV2NoVertex'),
    categoryVariableName = cms.string('vertexCategory'),
    charmCut = cms.double(1.5),
    correctVertexMass = cms.bool(True),
    minimumTrackWeight = cms.double(0.5),
    pseudoMultiplicityMin = cms.uint32(2),
    pseudoVertexV0Filter = cms.PSet(
        k0sMassWindow = cms.double(0.05)
    ),
    recordLabel = cms.string('HLT'),
    trackFlip = cms.bool(False),
    trackMultiplicityMin = cms.uint32(3),
    trackPairV0Filter = cms.PSet(
        k0sMassWindow = cms.double(0.03)
    ),
    trackPseudoSelection = cms.PSet(
        a_dR = cms.double(-0.001053),
        a_pT = cms.double(0.005263),
        b_dR = cms.double(0.6263),
        b_pT = cms.double(0.3684),
        jetDeltaRMax = cms.double(0.3),
        maxDecayLen = cms.double(5.0),
        maxDistToAxis = cms.double(0.07),
        max_pT = cms.double(500.0),
        max_pT_dRcut = cms.double(0.1),
        max_pT_trackPTcut = cms.double(3.0),
        min_pT = cms.double(120.0),
        min_pT_dRcut = cms.double(0.5),
        normChi2Max = cms.double(99999.9),
        pixelHitsMin = cms.uint32(0),
        ptMin = cms.double(0.0),
        qualityClass = cms.string('any'),
        sip2dSigMax = cms.double(99999.9),
        sip2dSigMin = cms.double(2.0),
        sip2dValMax = cms.double(99999.9),
        sip2dValMin = cms.double(-99999.9),
        sip3dSigMax = cms.double(99999.9),
        sip3dSigMin = cms.double(-99999.9),
        sip3dValMax = cms.double(99999.9),
        sip3dValMin = cms.double(-99999.9),
        totalHitsMin = cms.uint32(0),
        useVariableJTA = cms.bool(False)
    ),
    trackSelection = cms.PSet(
        a_dR = cms.double(-0.001053),
        a_pT = cms.double(0.005263),
        b_dR = cms.double(0.6263),
        b_pT = cms.double(0.3684),
        jetDeltaRMax = cms.double(0.3),
        maxDecayLen = cms.double(5.0),
        maxDistToAxis = cms.double(0.07),
        max_pT = cms.double(500.0),
        max_pT_dRcut = cms.double(0.1),
        max_pT_trackPTcut = cms.double(3.0),
        min_pT = cms.double(120.0),
        min_pT_dRcut = cms.double(0.5),
        normChi2Max = cms.double(99999.9),
        pixelHitsMin = cms.uint32(0),
        ptMin = cms.double(0.0),
        qualityClass = cms.string('any'),
        sip2dSigMax = cms.double(99999.9),
        sip2dSigMin = cms.double(-99999.9),
        sip2dValMax = cms.double(99999.9),
        sip2dValMin = cms.double(-99999.9),
        sip3dSigMax = cms.double(99999.9),
        sip3dSigMin = cms.double(-99999.9),
        sip3dValMax = cms.double(99999.9),
        sip3dValMin = cms.double(-99999.9),
        totalHitsMin = cms.uint32(0),
        useVariableJTA = cms.bool(False)
    ),
    trackSort = cms.string('sip2dSig'),
    useCategories = cms.bool(True),
    useTrackWeights = cms.bool(True),
    vertexFlip = cms.bool(False)
)


process.hltDisplacedDijethltESPPromptTrackCountingESProducer = cms.ESProducer("PromptTrackCountingESProducer",
    deltaR = cms.double(-1.0),
    deltaRmin = cms.double(0.0),
    impactParameterType = cms.int32(1),
    maxImpactParameter = cms.double(0.1),
    maxImpactParameterSig = cms.double(999999.0),
    maximumDecayLength = cms.double(999999.0),
    maximumDistanceToJetAxis = cms.double(999999.0),
    minimumImpactParameter = cms.double(-1.0),
    nthTrack = cms.int32(-1),
    trackQualityClass = cms.string('any'),
    useSignedImpactParameterSig = cms.bool(True)
)


process.hltDisplacedDijethltESPTrackCounting2D1st = cms.ESProducer("TrackCountingESProducer",
    a_dR = cms.double(-0.001053),
    a_pT = cms.double(0.005263),
    b_dR = cms.double(0.6263),
    b_pT = cms.double(0.3684),
    deltaR = cms.double(-1.0),
    impactParameterType = cms.int32(1),
    max_pT = cms.double(500.0),
    max_pT_dRcut = cms.double(0.1),
    max_pT_trackPTcut = cms.double(3.0),
    maximumDecayLength = cms.double(999999.0),
    maximumDistanceToJetAxis = cms.double(9999999.0),
    min_pT = cms.double(120.0),
    min_pT_dRcut = cms.double(0.5),
    minimumImpactParameter = cms.double(0.05),
    nthTrack = cms.int32(1),
    trackQualityClass = cms.string('any'),
    useSignedImpactParameterSig = cms.bool(False),
    useVariableJTA = cms.bool(False)
)


process.hltESPAnalyticalPropagator = cms.ESProducer("AnalyticalPropagatorESProducer",
    ComponentName = cms.string('hltESPAnalyticalPropagator'),
    MaxDPhi = cms.double(1.6),
    PropagationDirection = cms.string('alongMomentum')
)


process.hltESPBwdAnalyticalPropagator = cms.ESProducer("AnalyticalPropagatorESProducer",
    ComponentName = cms.string('hltESPBwdAnalyticalPropagator'),
    MaxDPhi = cms.double(1.6),
    PropagationDirection = cms.string('oppositeToMomentum')
)


process.hltESPBwdElectronPropagator = cms.ESProducer("PropagatorWithMaterialESProducer",
    ComponentName = cms.string('hltESPBwdElectronPropagator'),
    Mass = cms.double(0.000511),
    MaxDPhi = cms.double(1.6),
    PropagationDirection = cms.string('oppositeToMomentum'),
    SimpleMagneticField = cms.string(''),
    ptMin = cms.double(-1.0),
    useRungeKutta = cms.bool(False)
)


process.hltESPChi2ChargeLooseMeasurementEstimator16 = cms.ESProducer("Chi2ChargeMeasurementEstimatorESProducer",
    ComponentName = cms.string('hltESPChi2ChargeLooseMeasurementEstimator16'),
    MaxChi2 = cms.double(16.0),
    MaxDisplacement = cms.double(0.5),
    MaxSagitta = cms.double(2.0),
    MinPtForHitRecoveryInGluedDet = cms.double(1000000.0),
    MinimalTolerance = cms.double(0.5),
    appendToDataLabel = cms.string(''),
    clusterChargeCut = cms.PSet(
        refToPSet_ = cms.string('HLTSiStripClusterChargeCutLoose')
    ),
    nSigma = cms.double(3.0),
    pTChargeCutThreshold = cms.double(-1.0)
)


process.hltESPChi2ChargeMeasurementEstimator16 = cms.ESProducer("Chi2ChargeMeasurementEstimatorESProducer",
    ComponentName = cms.string('hltESPChi2ChargeMeasurementEstimator16'),
    MaxChi2 = cms.double(16.0),
    MaxDisplacement = cms.double(0.5),
    MaxSagitta = cms.double(2.0),
    MinPtForHitRecoveryInGluedDet = cms.double(1000000.0),
    MinimalTolerance = cms.double(0.5),
    appendToDataLabel = cms.string(''),
    clusterChargeCut = cms.PSet(
        refToPSet_ = cms.string('HLTSiStripClusterChargeCutLoose')
    ),
    nSigma = cms.double(3.0),
    pTChargeCutThreshold = cms.double(-1.0)
)


process.hltESPChi2ChargeMeasurementEstimator2000 = cms.ESProducer("Chi2ChargeMeasurementEstimatorESProducer",
    ComponentName = cms.string('hltESPChi2ChargeMeasurementEstimator2000'),
    MaxChi2 = cms.double(2000.0),
    MaxDisplacement = cms.double(100.0),
    MaxSagitta = cms.double(-1.0),
    MinPtForHitRecoveryInGluedDet = cms.double(1000000.0),
    MinimalTolerance = cms.double(10.0),
    appendToDataLabel = cms.string(''),
    clusterChargeCut = cms.PSet(
        refToPSet_ = cms.string('HLTSiStripClusterChargeCutNone')
    ),
    nSigma = cms.double(3.0),
    pTChargeCutThreshold = cms.double(-1.0)
)


process.hltESPChi2ChargeMeasurementEstimator30 = cms.ESProducer("Chi2ChargeMeasurementEstimatorESProducer",
    ComponentName = cms.string('hltESPChi2ChargeMeasurementEstimator30'),
    MaxChi2 = cms.double(30.0),
    MaxDisplacement = cms.double(100.0),
    MaxSagitta = cms.double(-1.0),
    MinPtForHitRecoveryInGluedDet = cms.double(1000000.0),
    MinimalTolerance = cms.double(10.0),
    appendToDataLabel = cms.string(''),
    clusterChargeCut = cms.PSet(
        refToPSet_ = cms.string('HLTSiStripClusterChargeCutNone')
    ),
    nSigma = cms.double(3.0),
    pTChargeCutThreshold = cms.double(-1.0)
)


process.hltESPChi2ChargeMeasurementEstimator9 = cms.ESProducer("Chi2ChargeMeasurementEstimatorESProducer",
    ComponentName = cms.string('hltESPChi2ChargeMeasurementEstimator9'),
    MaxChi2 = cms.double(9.0),
    MaxDisplacement = cms.double(0.5),
    MaxSagitta = cms.double(2.0),
    MinPtForHitRecoveryInGluedDet = cms.double(1000000.0),
    MinimalTolerance = cms.double(0.5),
    appendToDataLabel = cms.string(''),
    clusterChargeCut = cms.PSet(
        refToPSet_ = cms.string('HLTSiStripClusterChargeCutLoose')
    ),
    nSigma = cms.double(3.0),
    pTChargeCutThreshold = cms.double(15.0)
)


process.hltESPChi2ChargeMeasurementEstimator9ForHI = cms.ESProducer("Chi2ChargeMeasurementEstimatorESProducer",
    ComponentName = cms.string('hltESPChi2ChargeMeasurementEstimator9ForHI'),
    MaxChi2 = cms.double(9.0),
    MaxDisplacement = cms.double(100.0),
    MaxSagitta = cms.double(-1.0),
    MinPtForHitRecoveryInGluedDet = cms.double(1000000.0),
    MinimalTolerance = cms.double(10.0),
    appendToDataLabel = cms.string(''),
    clusterChargeCut = cms.PSet(
        refToPSet_ = cms.string('HLTSiStripClusterChargeCutForHI')
    ),
    nSigma = cms.double(3.0),
    pTChargeCutThreshold = cms.double(15.0)
)


process.hltESPChi2ChargeTightMeasurementEstimator16 = cms.ESProducer("Chi2ChargeMeasurementEstimatorESProducer",
    ComponentName = cms.string('hltESPChi2ChargeTightMeasurementEstimator16'),
    MaxChi2 = cms.double(16.0),
    MaxDisplacement = cms.double(0.5),
    MaxSagitta = cms.double(2.0),
    MinPtForHitRecoveryInGluedDet = cms.double(1000000.0),
    MinimalTolerance = cms.double(0.5),
    appendToDataLabel = cms.string(''),
    clusterChargeCut = cms.PSet(
        refToPSet_ = cms.string('HLTSiStripClusterChargeCutTight')
    ),
    nSigma = cms.double(3.0),
    pTChargeCutThreshold = cms.double(-1.0)
)


process.hltESPChi2MeasurementEstimator16 = cms.ESProducer("Chi2MeasurementEstimatorESProducer",
    ComponentName = cms.string('hltESPChi2MeasurementEstimator16'),
    MaxChi2 = cms.double(16.0),
    MaxDisplacement = cms.double(100.0),
    MaxSagitta = cms.double(-1.0),
    MinPtForHitRecoveryInGluedDet = cms.double(1000000.0),
    MinimalTolerance = cms.double(10.0),
    appendToDataLabel = cms.string(''),
    nSigma = cms.double(3.0)
)


process.hltESPChi2MeasurementEstimator30 = cms.ESProducer("Chi2MeasurementEstimatorESProducer",
    ComponentName = cms.string('hltESPChi2MeasurementEstimator30'),
    MaxChi2 = cms.double(30.0),
    MaxDisplacement = cms.double(100.0),
    MaxSagitta = cms.double(-1.0),
    MinPtForHitRecoveryInGluedDet = cms.double(1000000.0),
    MinimalTolerance = cms.double(10.0),
    appendToDataLabel = cms.string(''),
    nSigma = cms.double(3.0)
)


process.hltESPChi2MeasurementEstimator9 = cms.ESProducer("Chi2MeasurementEstimatorESProducer",
    ComponentName = cms.string('hltESPChi2MeasurementEstimator9'),
    MaxChi2 = cms.double(9.0),
    MaxDisplacement = cms.double(100.0),
    MaxSagitta = cms.double(-1.0),
    MinPtForHitRecoveryInGluedDet = cms.double(1000000.0),
    MinimalTolerance = cms.double(10.0),
    appendToDataLabel = cms.string(''),
    nSigma = cms.double(3.0)
)


process.hltESPCloseComponentsMerger5D = cms.ESProducer("CloseComponentsMergerESProducer5D",
    ComponentName = cms.string('hltESPCloseComponentsMerger5D'),
    DistanceMeasure = cms.string('hltESPKullbackLeiblerDistance5D'),
    MaxComponents = cms.int32(12)
)


process.hltESPDetachedStepTrajectoryCleanerBySharedHits = cms.ESProducer("TrajectoryCleanerESProducer",
    ComponentName = cms.string('hltESPDetachedStepTrajectoryCleanerBySharedHits'),
    ComponentType = cms.string('TrajectoryCleanerBySharedHits'),
    MissingHitPenalty = cms.double(20.0),
    ValidHitBonus = cms.double(5.0),
    allowSharedFirstHit = cms.bool(True),
    fractionShared = cms.double(0.13)
)


process.hltESPDisplacedDijethltPromptTrackCountingESProducer = cms.ESProducer("PromptTrackCountingESProducer",
    deltaR = cms.double(-1.0),
    deltaRmin = cms.double(0.0),
    impactParameterType = cms.int32(1),
    maxImpactParameter = cms.double(0.1),
    maxImpactParameterSig = cms.double(999999.0),
    maximumDecayLength = cms.double(999999.0),
    maximumDistanceToJetAxis = cms.double(999999.0),
    minimumImpactParameter = cms.double(-1.0),
    nthTrack = cms.int32(-1),
    trackQualityClass = cms.string('any'),
    useSignedImpactParameterSig = cms.bool(True)
)


process.hltESPDisplacedDijethltPromptTrackCountingESProducerLong = cms.ESProducer("PromptTrackCountingESProducer",
    deltaR = cms.double(-1.0),
    deltaRmin = cms.double(0.0),
    impactParameterType = cms.int32(1),
    maxImpactParameter = cms.double(0.2),
    maxImpactParameterSig = cms.double(999999.0),
    maximumDecayLength = cms.double(999999.0),
    maximumDistanceToJetAxis = cms.double(999999.0),
    minimumImpactParameter = cms.double(-1.0),
    nthTrack = cms.int32(-1),
    trackQualityClass = cms.string('any'),
    useSignedImpactParameterSig = cms.bool(True)
)


process.hltESPDisplacedDijethltTrackCounting2D1st = cms.ESProducer("TrackCountingESProducer",
    a_dR = cms.double(-0.001053),
    a_pT = cms.double(0.005263),
    b_dR = cms.double(0.6263),
    b_pT = cms.double(0.3684),
    deltaR = cms.double(-1.0),
    impactParameterType = cms.int32(1),
    max_pT = cms.double(500.0),
    max_pT_dRcut = cms.double(0.1),
    max_pT_trackPTcut = cms.double(3.0),
    maximumDecayLength = cms.double(999999.0),
    maximumDistanceToJetAxis = cms.double(9999999.0),
    min_pT = cms.double(120.0),
    min_pT_dRcut = cms.double(0.5),
    minimumImpactParameter = cms.double(0.05),
    nthTrack = cms.int32(1),
    trackQualityClass = cms.string('any'),
    useSignedImpactParameterSig = cms.bool(False),
    useVariableJTA = cms.bool(False)
)


process.hltESPDisplacedDijethltTrackCounting2D2ndLong = cms.ESProducer("TrackCountingESProducer",
    a_dR = cms.double(-0.001053),
    a_pT = cms.double(0.005263),
    b_dR = cms.double(0.6263),
    b_pT = cms.double(0.3684),
    deltaR = cms.double(-1.0),
    impactParameterType = cms.int32(1),
    max_pT = cms.double(500.0),
    max_pT_dRcut = cms.double(0.1),
    max_pT_trackPTcut = cms.double(3.0),
    maximumDecayLength = cms.double(999999.0),
    maximumDistanceToJetAxis = cms.double(9999999.0),
    min_pT = cms.double(120.0),
    min_pT_dRcut = cms.double(0.5),
    minimumImpactParameter = cms.double(0.2),
    nthTrack = cms.int32(2),
    trackQualityClass = cms.string('any'),
    useSignedImpactParameterSig = cms.bool(True),
    useVariableJTA = cms.bool(False)
)


process.hltESPDummyDetLayerGeometry = cms.ESProducer("DetLayerGeometryESProducer",
    ComponentName = cms.string('hltESPDummyDetLayerGeometry')
)


process.hltESPEcalTrigTowerConstituentsMapBuilder = cms.ESProducer("EcalTrigTowerConstituentsMapBuilder",
    MapFile = cms.untracked.string('Geometry/EcalMapping/data/EndCap_TTMap.txt')
)


process.hltESPElectronMaterialEffects = cms.ESProducer("GsfMaterialEffectsESProducer",
    BetheHeitlerCorrection = cms.int32(2),
    BetheHeitlerParametrization = cms.string('BetheHeitler_cdfmom_nC6_O5.par'),
    ComponentName = cms.string('hltESPElectronMaterialEffects'),
    EnergyLossUpdator = cms.string('GsfBetheHeitlerUpdator'),
    Mass = cms.double(0.000511),
    MultipleScatteringUpdator = cms.string('MultipleScatteringUpdator')
)


process.hltESPFastSteppingHelixPropagatorAny = cms.ESProducer("SteppingHelixPropagatorESProducer",
    ApplyRadX0Correction = cms.bool(True),
    AssumeNoMaterial = cms.bool(False),
    ComponentName = cms.string('hltESPFastSteppingHelixPropagatorAny'),
    NoErrorPropagation = cms.bool(False),
    PropagationDirection = cms.string('anyDirection'),
    SetVBFPointer = cms.bool(False),
    VBFName = cms.string('VolumeBasedMagneticField'),
    debug = cms.bool(False),
    endcapShiftInZNeg = cms.double(0.0),
    endcapShiftInZPos = cms.double(0.0),
    returnTangentPlane = cms.bool(True),
    sendLogWarning = cms.bool(False),
    useEndcapShiftsInZ = cms.bool(False),
    useInTeslaFromMagField = cms.bool(False),
    useIsYokeFlag = cms.bool(True),
    useMagVolumes = cms.bool(True),
    useMatVolumes = cms.bool(True),
    useTuningForL2Speed = cms.bool(True)
)


process.hltESPFastSteppingHelixPropagatorOpposite = cms.ESProducer("SteppingHelixPropagatorESProducer",
    ApplyRadX0Correction = cms.bool(True),
    AssumeNoMaterial = cms.bool(False),
    ComponentName = cms.string('hltESPFastSteppingHelixPropagatorOpposite'),
    NoErrorPropagation = cms.bool(False),
    PropagationDirection = cms.string('oppositeToMomentum'),
    SetVBFPointer = cms.bool(False),
    VBFName = cms.string('VolumeBasedMagneticField'),
    debug = cms.bool(False),
    endcapShiftInZNeg = cms.double(0.0),
    endcapShiftInZPos = cms.double(0.0),
    returnTangentPlane = cms.bool(True),
    sendLogWarning = cms.bool(False),
    useEndcapShiftsInZ = cms.bool(False),
    useInTeslaFromMagField = cms.bool(False),
    useIsYokeFlag = cms.bool(True),
    useMagVolumes = cms.bool(True),
    useMatVolumes = cms.bool(True),
    useTuningForL2Speed = cms.bool(True)
)


process.hltESPFittingSmootherIT = cms.ESProducer("KFFittingSmootherESProducer",
    BreakTrajWith2ConsecutiveMissing = cms.bool(True),
    ComponentName = cms.string('hltESPFittingSmootherIT'),
    EstimateCut = cms.double(-1.0),
    Fitter = cms.string('hltESPTrajectoryFitterRK'),
    LogPixelProbabilityCut = cms.double(-16.0),
    MaxFractionOutliers = cms.double(0.3),
    MaxNumberOfOutliers = cms.int32(3),
    MinDof = cms.int32(2),
    MinNumberOfHits = cms.int32(3),
    NoInvalidHitsBeginEnd = cms.bool(True),
    NoOutliersBeginEnd = cms.bool(False),
    RejectTracks = cms.bool(True),
    Smoother = cms.string('hltESPTrajectorySmootherRK'),
    appendToDataLabel = cms.string('')
)


process.hltESPFittingSmootherRK = cms.ESProducer("KFFittingSmootherESProducer",
    BreakTrajWith2ConsecutiveMissing = cms.bool(False),
    ComponentName = cms.string('hltESPFittingSmootherRK'),
    EstimateCut = cms.double(-1.0),
    Fitter = cms.string('hltESPTrajectoryFitterRK'),
    LogPixelProbabilityCut = cms.double(-16.0),
    MaxFractionOutliers = cms.double(0.3),
    MaxNumberOfOutliers = cms.int32(3),
    MinDof = cms.int32(2),
    MinNumberOfHits = cms.int32(5),
    NoInvalidHitsBeginEnd = cms.bool(False),
    NoOutliersBeginEnd = cms.bool(False),
    RejectTracks = cms.bool(True),
    Smoother = cms.string('hltESPTrajectorySmootherRK'),
    appendToDataLabel = cms.string('')
)


process.hltESPFlexibleKFFittingSmoother = cms.ESProducer("FlexibleKFFittingSmootherESProducer",
    ComponentName = cms.string('hltESPFlexibleKFFittingSmoother'),
    appendToDataLabel = cms.string(''),
    looperFitter = cms.string('hltESPKFFittingSmootherForLoopers'),
    standardFitter = cms.string('hltESPKFFittingSmootherWithOutliersRejectionAndRK')
)


process.hltESPFwdElectronPropagator = cms.ESProducer("PropagatorWithMaterialESProducer",
    ComponentName = cms.string('hltESPFwdElectronPropagator'),
    Mass = cms.double(0.000511),
    MaxDPhi = cms.double(1.6),
    PropagationDirection = cms.string('alongMomentum'),
    SimpleMagneticField = cms.string(''),
    ptMin = cms.double(-1.0),
    useRungeKutta = cms.bool(False)
)


process.hltESPGlobalDetLayerGeometry = cms.ESProducer("GlobalDetLayerGeometryESProducer",
    ComponentName = cms.string('hltESPGlobalDetLayerGeometry')
)


process.hltESPGlobalTrackingGeometryESProducer = cms.ESProducer("GlobalTrackingGeometryESProducer")


process.hltESPGsfElectronFittingSmoother = cms.ESProducer("KFFittingSmootherESProducer",
    BreakTrajWith2ConsecutiveMissing = cms.bool(True),
    ComponentName = cms.string('hltESPGsfElectronFittingSmoother'),
    EstimateCut = cms.double(-1.0),
    Fitter = cms.string('hltESPGsfTrajectoryFitter'),
    LogPixelProbabilityCut = cms.double(-16.0),
    MaxFractionOutliers = cms.double(0.3),
    MaxNumberOfOutliers = cms.int32(3),
    MinDof = cms.int32(2),
    MinNumberOfHits = cms.int32(5),
    NoInvalidHitsBeginEnd = cms.bool(True),
    NoOutliersBeginEnd = cms.bool(False),
    RejectTracks = cms.bool(True),
    Smoother = cms.string('hltESPGsfTrajectorySmoother'),
    appendToDataLabel = cms.string('')
)


process.hltESPGsfTrajectoryFitter = cms.ESProducer("GsfTrajectoryFitterESProducer",
    ComponentName = cms.string('hltESPGsfTrajectoryFitter'),
    GeometricalPropagator = cms.string('hltESPAnalyticalPropagator'),
    MaterialEffectsUpdator = cms.string('hltESPElectronMaterialEffects'),
    Merger = cms.string('hltESPCloseComponentsMerger5D'),
    RecoGeometry = cms.string('hltESPGlobalDetLayerGeometry')
)


process.hltESPGsfTrajectorySmoother = cms.ESProducer("GsfTrajectorySmootherESProducer",
    ComponentName = cms.string('hltESPGsfTrajectorySmoother'),
    ErrorRescaling = cms.double(100.0),
    GeometricalPropagator = cms.string('hltESPBwdAnalyticalPropagator'),
    MaterialEffectsUpdator = cms.string('hltESPElectronMaterialEffects'),
    Merger = cms.string('hltESPCloseComponentsMerger5D'),
    RecoGeometry = cms.string('hltESPGlobalDetLayerGeometry')
)


process.hltESPInitialStepChi2ChargeMeasurementEstimator30 = cms.ESProducer("Chi2ChargeMeasurementEstimatorESProducer",
    ComponentName = cms.string('hltESPInitialStepChi2ChargeMeasurementEstimator30'),
    MaxChi2 = cms.double(30.0),
    MaxDisplacement = cms.double(0.5),
    MaxSagitta = cms.double(2.0),
    MinPtForHitRecoveryInGluedDet = cms.double(1000000.0),
    MinimalTolerance = cms.double(0.5),
    appendToDataLabel = cms.string(''),
    clusterChargeCut = cms.PSet(
        value = cms.double(800.0)
    ),
    nSigma = cms.double(3.0),
    pTChargeCutThreshold = cms.double(15.0)
)


process.hltESPInitialStepChi2MeasurementEstimator36 = cms.ESProducer("Chi2MeasurementEstimatorESProducer",
    ComponentName = cms.string('hltESPInitialStepChi2MeasurementEstimator36'),
    MaxChi2 = cms.double(36.0),
    MaxDisplacement = cms.double(100.0),
    MaxSagitta = cms.double(-1.0),
    MinPtForHitRecoveryInGluedDet = cms.double(1000000.0),
    MinimalTolerance = cms.double(10.0),
    appendToDataLabel = cms.string(''),
    nSigma = cms.double(3.0)
)


process.hltESPKFFittingSmoother = cms.ESProducer("KFFittingSmootherESProducer",
    BreakTrajWith2ConsecutiveMissing = cms.bool(False),
    ComponentName = cms.string('hltESPKFFittingSmoother'),
    EstimateCut = cms.double(-1.0),
    Fitter = cms.string('hltESPKFTrajectoryFitter'),
    LogPixelProbabilityCut = cms.double(-16.0),
    MaxFractionOutliers = cms.double(0.3),
    MaxNumberOfOutliers = cms.int32(3),
    MinDof = cms.int32(2),
    MinNumberOfHits = cms.int32(5),
    NoInvalidHitsBeginEnd = cms.bool(False),
    NoOutliersBeginEnd = cms.bool(False),
    RejectTracks = cms.bool(True),
    Smoother = cms.string('hltESPKFTrajectorySmoother'),
    appendToDataLabel = cms.string('')
)


process.hltESPKFFittingSmootherForL2Muon = cms.ESProducer("KFFittingSmootherESProducer",
    BreakTrajWith2ConsecutiveMissing = cms.bool(False),
    ComponentName = cms.string('hltESPKFFittingSmootherForL2Muon'),
    EstimateCut = cms.double(-1.0),
    Fitter = cms.string('hltESPKFTrajectoryFitterForL2Muon'),
    LogPixelProbabilityCut = cms.double(-16.0),
    MaxFractionOutliers = cms.double(0.3),
    MaxNumberOfOutliers = cms.int32(3),
    MinDof = cms.int32(2),
    MinNumberOfHits = cms.int32(5),
    NoInvalidHitsBeginEnd = cms.bool(False),
    NoOutliersBeginEnd = cms.bool(False),
    RejectTracks = cms.bool(True),
    Smoother = cms.string('hltESPKFTrajectorySmootherForL2Muon'),
    appendToDataLabel = cms.string('')
)


process.hltESPKFFittingSmootherForLoopers = cms.ESProducer("KFFittingSmootherESProducer",
    BreakTrajWith2ConsecutiveMissing = cms.bool(True),
    ComponentName = cms.string('hltESPKFFittingSmootherForLoopers'),
    EstimateCut = cms.double(20.0),
    Fitter = cms.string('hltESPKFTrajectoryFitterForLoopers'),
    LogPixelProbabilityCut = cms.double(-14.0),
    MaxFractionOutliers = cms.double(0.3),
    MaxNumberOfOutliers = cms.int32(3),
    MinDof = cms.int32(2),
    MinNumberOfHits = cms.int32(3),
    NoInvalidHitsBeginEnd = cms.bool(True),
    NoOutliersBeginEnd = cms.bool(False),
    RejectTracks = cms.bool(True),
    Smoother = cms.string('hltESPKFTrajectorySmootherForLoopers'),
    appendToDataLabel = cms.string('')
)


process.hltESPKFFittingSmootherWithOutliersRejectionAndRK = cms.ESProducer("KFFittingSmootherESProducer",
    BreakTrajWith2ConsecutiveMissing = cms.bool(True),
    ComponentName = cms.string('hltESPKFFittingSmootherWithOutliersRejectionAndRK'),
    EstimateCut = cms.double(20.0),
    Fitter = cms.string('hltESPRKTrajectoryFitter'),
    LogPixelProbabilityCut = cms.double(-14.0),
    MaxFractionOutliers = cms.double(0.3),
    MaxNumberOfOutliers = cms.int32(3),
    MinDof = cms.int32(2),
    MinNumberOfHits = cms.int32(3),
    NoInvalidHitsBeginEnd = cms.bool(True),
    NoOutliersBeginEnd = cms.bool(False),
    RejectTracks = cms.bool(True),
    Smoother = cms.string('hltESPRKTrajectorySmoother'),
    appendToDataLabel = cms.string('')
)


process.hltESPKFTrajectoryFitter = cms.ESProducer("KFTrajectoryFitterESProducer",
    ComponentName = cms.string('hltESPKFTrajectoryFitter'),
    Estimator = cms.string('hltESPChi2MeasurementEstimator30'),
    Propagator = cms.string('PropagatorWithMaterialParabolicMf'),
    RecoGeometry = cms.string('hltESPDummyDetLayerGeometry'),
    Updator = cms.string('hltESPKFUpdator'),
    appendToDataLabel = cms.string(''),
    minHits = cms.int32(3)
)


process.hltESPKFTrajectoryFitterForL2Muon = cms.ESProducer("KFTrajectoryFitterESProducer",
    ComponentName = cms.string('hltESPKFTrajectoryFitterForL2Muon'),
    Estimator = cms.string('hltESPChi2MeasurementEstimator30'),
    Propagator = cms.string('hltESPFastSteppingHelixPropagatorAny'),
    RecoGeometry = cms.string('hltESPDummyDetLayerGeometry'),
    Updator = cms.string('hltESPKFUpdator'),
    appendToDataLabel = cms.string(''),
    minHits = cms.int32(3)
)


process.hltESPKFTrajectoryFitterForLoopers = cms.ESProducer("KFTrajectoryFitterESProducer",
    ComponentName = cms.string('hltESPKFTrajectoryFitterForLoopers'),
    Estimator = cms.string('hltESPChi2MeasurementEstimator30'),
    Propagator = cms.string('PropagatorWithMaterialForLoopers'),
    RecoGeometry = cms.string('hltESPGlobalDetLayerGeometry'),
    Updator = cms.string('hltESPKFUpdator'),
    appendToDataLabel = cms.string(''),
    minHits = cms.int32(3)
)


process.hltESPKFTrajectorySmoother = cms.ESProducer("KFTrajectorySmootherESProducer",
    ComponentName = cms.string('hltESPKFTrajectorySmoother'),
    Estimator = cms.string('hltESPChi2MeasurementEstimator30'),
    Propagator = cms.string('PropagatorWithMaterialParabolicMf'),
    RecoGeometry = cms.string('hltESPDummyDetLayerGeometry'),
    Updator = cms.string('hltESPKFUpdator'),
    appendToDataLabel = cms.string(''),
    errorRescaling = cms.double(100.0),
    minHits = cms.int32(3)
)


process.hltESPKFTrajectorySmootherForL2Muon = cms.ESProducer("KFTrajectorySmootherESProducer",
    ComponentName = cms.string('hltESPKFTrajectorySmootherForL2Muon'),
    Estimator = cms.string('hltESPChi2MeasurementEstimator30'),
    Propagator = cms.string('hltESPFastSteppingHelixPropagatorOpposite'),
    RecoGeometry = cms.string('hltESPDummyDetLayerGeometry'),
    Updator = cms.string('hltESPKFUpdator'),
    appendToDataLabel = cms.string(''),
    errorRescaling = cms.double(100.0),
    minHits = cms.int32(3)
)


process.hltESPKFTrajectorySmootherForLoopers = cms.ESProducer("KFTrajectorySmootherESProducer",
    ComponentName = cms.string('hltESPKFTrajectorySmootherForLoopers'),
    Estimator = cms.string('hltESPChi2MeasurementEstimator30'),
    Propagator = cms.string('PropagatorWithMaterialForLoopers'),
    RecoGeometry = cms.string('hltESPGlobalDetLayerGeometry'),
    Updator = cms.string('hltESPKFUpdator'),
    appendToDataLabel = cms.string(''),
    errorRescaling = cms.double(10.0),
    minHits = cms.int32(3)
)


process.hltESPKFTrajectorySmootherForMuonTrackLoader = cms.ESProducer("KFTrajectorySmootherESProducer",
    ComponentName = cms.string('hltESPKFTrajectorySmootherForMuonTrackLoader'),
    Estimator = cms.string('hltESPChi2MeasurementEstimator30'),
    Propagator = cms.string('hltESPSmartPropagatorAnyOpposite'),
    RecoGeometry = cms.string('hltESPDummyDetLayerGeometry'),
    Updator = cms.string('hltESPKFUpdator'),
    appendToDataLabel = cms.string(''),
    errorRescaling = cms.double(10.0),
    minHits = cms.int32(3)
)


process.hltESPKFUpdator = cms.ESProducer("KFUpdatorESProducer",
    ComponentName = cms.string('hltESPKFUpdator')
)


process.hltESPKullbackLeiblerDistance5D = cms.ESProducer("DistanceBetweenComponentsESProducer5D",
    ComponentName = cms.string('hltESPKullbackLeiblerDistance5D'),
    DistanceMeasure = cms.string('KullbackLeibler')
)


process.hltESPL3MuKFTrajectoryFitter = cms.ESProducer("KFTrajectoryFitterESProducer",
    ComponentName = cms.string('hltESPL3MuKFTrajectoryFitter'),
    Estimator = cms.string('hltESPChi2MeasurementEstimator30'),
    Propagator = cms.string('hltESPSmartPropagatorAny'),
    RecoGeometry = cms.string('hltESPDummyDetLayerGeometry'),
    Updator = cms.string('hltESPKFUpdator'),
    appendToDataLabel = cms.string(''),
    minHits = cms.int32(3)
)


process.hltESPLowPtStepTrajectoryCleanerBySharedHits = cms.ESProducer("TrajectoryCleanerESProducer",
    ComponentName = cms.string('hltESPLowPtStepTrajectoryCleanerBySharedHits'),
    ComponentType = cms.string('TrajectoryCleanerBySharedHits'),
    MissingHitPenalty = cms.double(20.0),
    ValidHitBonus = cms.double(5.0),
    allowSharedFirstHit = cms.bool(True),
    fractionShared = cms.double(0.16)
)


process.hltESPMeasurementTracker = cms.ESProducer("MeasurementTrackerESProducer",
    ComponentName = cms.string('hltESPMeasurementTracker'),
    DebugPixelModuleQualityDB = cms.untracked.bool(False),
    DebugPixelROCQualityDB = cms.untracked.bool(False),
    DebugStripAPVFiberQualityDB = cms.untracked.bool(False),
    DebugStripModuleQualityDB = cms.untracked.bool(False),
    DebugStripStripQualityDB = cms.untracked.bool(False),
    HitMatcher = cms.string('StandardMatcher'),
    MaskBadAPVFibers = cms.bool(True),
    PixelCPE = cms.string('hltESPPixelCPEGeneric'),
    SiStripQualityLabel = cms.string(''),
    StripCPE = cms.string('hltESPStripCPEfromTrackAngle'),
    UsePixelModuleQualityDB = cms.bool(True),
    UsePixelROCQualityDB = cms.bool(True),
    UseStripAPVFiberQualityDB = cms.bool(True),
    UseStripModuleQualityDB = cms.bool(True),
    UseStripStripQualityDB = cms.bool(True),
    badStripCuts = cms.PSet(
        TEC = cms.PSet(
            maxBad = cms.uint32(4),
            maxConsecutiveBad = cms.uint32(2)
        ),
        TIB = cms.PSet(
            maxBad = cms.uint32(4),
            maxConsecutiveBad = cms.uint32(2)
        ),
        TID = cms.PSet(
            maxBad = cms.uint32(4),
            maxConsecutiveBad = cms.uint32(2)
        ),
        TOB = cms.PSet(
            maxBad = cms.uint32(4),
            maxConsecutiveBad = cms.uint32(2)
        )
    )
)


process.hltESPMixedStepClusterShapeHitFilter = cms.ESProducer("ClusterShapeHitFilterESProducer",
    ComponentName = cms.string('hltESPMixedStepClusterShapeHitFilter'),
    PixelShapeFile = cms.string('RecoPixelVertexing/PixelLowPtUtilities/data/pixelShape.par'),
    clusterChargeCut = cms.PSet(
        refToPSet_ = cms.string('HLTSiStripClusterChargeCutTight')
    )
)


process.hltESPMixedStepTrajectoryCleanerBySharedHits = cms.ESProducer("TrajectoryCleanerESProducer",
    ComponentName = cms.string('hltESPMixedStepTrajectoryCleanerBySharedHits'),
    ComponentType = cms.string('TrajectoryCleanerBySharedHits'),
    MissingHitPenalty = cms.double(20.0),
    ValidHitBonus = cms.double(5.0),
    allowSharedFirstHit = cms.bool(True),
    fractionShared = cms.double(0.11)
)


process.hltESPMuonDetLayerGeometryESProducer = cms.ESProducer("MuonDetLayerGeometryESProducer")


process.hltESPMuonTransientTrackingRecHitBuilder = cms.ESProducer("MuonTransientTrackingRecHitBuilderESProducer",
    ComponentName = cms.string('hltESPMuonTransientTrackingRecHitBuilder')
)


process.hltESPPixelCPEGeneric = cms.ESProducer("PixelCPEGenericESProducer",
    Alpha2Order = cms.bool(True),
    ClusterProbComputationFlag = cms.int32(0),
    ComponentName = cms.string('hltESPPixelCPEGeneric'),
    DoCosmics = cms.bool(False),
    EdgeClusterErrorX = cms.double(50.0),
    EdgeClusterErrorY = cms.double(85.0),
    IrradiationBiasCorrection = cms.bool(False),
    LoadTemplatesFromDB = cms.bool(True),
    MagneticFieldRecord = cms.ESInputTag(""),
    PixelErrorParametrization = cms.string('NOTcmsim'),
    TruncatePixelCharge = cms.bool(True),
    UseErrorsFromTemplates = cms.bool(True),
    eff_charge_cut_highX = cms.double(1.0),
    eff_charge_cut_highY = cms.double(1.0),
    eff_charge_cut_lowX = cms.double(0.0),
    eff_charge_cut_lowY = cms.double(0.0),
    inflate_all_errors_no_trk_angle = cms.bool(False),
    inflate_errors = cms.bool(False),
    size_cutX = cms.double(3.0),
    size_cutY = cms.double(3.0),
    useLAAlignmentOffsets = cms.bool(False),
    useLAWidthFromDB = cms.bool(False)
)


process.hltESPPixelCPETemplateReco = cms.ESProducer("PixelCPETemplateRecoESProducer",
    Alpha2Order = cms.bool(True),
    ClusterProbComputationFlag = cms.int32(0),
    ComponentName = cms.string('hltESPPixelCPETemplateReco'),
    DoCosmics = cms.bool(False),
    DoLorentz = cms.bool(True),
    LoadTemplatesFromDB = cms.bool(True),
    UseClusterSplitter = cms.bool(False),
    speed = cms.int32(-2)
)


process.hltESPPixelLessStepClusterShapeHitFilter = cms.ESProducer("ClusterShapeHitFilterESProducer",
    ComponentName = cms.string('hltESPPixelLessStepClusterShapeHitFilter'),
    PixelShapeFile = cms.string('RecoPixelVertexing/PixelLowPtUtilities/data/pixelShape.par'),
    clusterChargeCut = cms.PSet(
        refToPSet_ = cms.string('HLTSiStripClusterChargeCutTight')
    )
)


process.hltESPPixelLessStepTrajectoryCleanerBySharedHits = cms.ESProducer("TrajectoryCleanerESProducer",
    ComponentName = cms.string('hltESPPixelLessStepTrajectoryCleanerBySharedHits'),
    ComponentType = cms.string('TrajectoryCleanerBySharedHits'),
    MissingHitPenalty = cms.double(20.0),
    ValidHitBonus = cms.double(5.0),
    allowSharedFirstHit = cms.bool(True),
    fractionShared = cms.double(0.11)
)


process.hltESPPixelPairStepChi2MeasurementEstimator25 = cms.ESProducer("Chi2MeasurementEstimatorESProducer",
    ComponentName = cms.string('hltESPPixelPairStepChi2MeasurementEstimator25'),
    MaxChi2 = cms.double(25.0),
    MaxDisplacement = cms.double(100.0),
    MaxSagitta = cms.double(-1.0),
    MinPtForHitRecoveryInGluedDet = cms.double(1000000.0),
    MinimalTolerance = cms.double(10.0),
    appendToDataLabel = cms.string(''),
    nSigma = cms.double(3.0)
)


process.hltESPPixelPairTrajectoryCleanerBySharedHits = cms.ESProducer("TrajectoryCleanerESProducer",
    ComponentName = cms.string('hltESPPixelPairTrajectoryCleanerBySharedHits'),
    ComponentType = cms.string('TrajectoryCleanerBySharedHits'),
    MissingHitPenalty = cms.double(20.0),
    ValidHitBonus = cms.double(5.0),
    allowSharedFirstHit = cms.bool(True),
    fractionShared = cms.double(0.19)
)


process.hltESPRKTrajectoryFitter = cms.ESProducer("KFTrajectoryFitterESProducer",
    ComponentName = cms.string('hltESPRKTrajectoryFitter'),
    Estimator = cms.string('hltESPChi2MeasurementEstimator30'),
    Propagator = cms.string('hltESPRungeKuttaTrackerPropagator'),
    RecoGeometry = cms.string('hltESPGlobalDetLayerGeometry'),
    Updator = cms.string('hltESPKFUpdator'),
    appendToDataLabel = cms.string(''),
    minHits = cms.int32(3)
)


process.hltESPRKTrajectorySmoother = cms.ESProducer("KFTrajectorySmootherESProducer",
    ComponentName = cms.string('hltESPRKTrajectorySmoother'),
    Estimator = cms.string('hltESPChi2MeasurementEstimator30'),
    Propagator = cms.string('hltESPRungeKuttaTrackerPropagator'),
    RecoGeometry = cms.string('hltESPGlobalDetLayerGeometry'),
    Updator = cms.string('hltESPKFUpdator'),
    appendToDataLabel = cms.string(''),
    errorRescaling = cms.double(100.0),
    minHits = cms.int32(3)
)


process.hltESPRungeKuttaTrackerPropagator = cms.ESProducer("PropagatorWithMaterialESProducer",
    ComponentName = cms.string('hltESPRungeKuttaTrackerPropagator'),
    Mass = cms.double(0.105),
    MaxDPhi = cms.double(1.6),
    PropagationDirection = cms.string('alongMomentum'),
    SimpleMagneticField = cms.string(''),
    ptMin = cms.double(-1.0),
    useRungeKutta = cms.bool(True)
)


process.hltESPSmartPropagator = cms.ESProducer("SmartPropagatorESProducer",
    ComponentName = cms.string('hltESPSmartPropagator'),
    Epsilon = cms.double(5.0),
    MuonPropagator = cms.string('hltESPSteppingHelixPropagatorAlong'),
    PropagationDirection = cms.string('alongMomentum'),
    TrackerPropagator = cms.string('PropagatorWithMaterial')
)


process.hltESPSmartPropagatorAny = cms.ESProducer("SmartPropagatorESProducer",
    ComponentName = cms.string('hltESPSmartPropagatorAny'),
    Epsilon = cms.double(5.0),
    MuonPropagator = cms.string('SteppingHelixPropagatorAny'),
    PropagationDirection = cms.string('alongMomentum'),
    TrackerPropagator = cms.string('PropagatorWithMaterial')
)


process.hltESPSmartPropagatorAnyOpposite = cms.ESProducer("SmartPropagatorESProducer",
    ComponentName = cms.string('hltESPSmartPropagatorAnyOpposite'),
    Epsilon = cms.double(5.0),
    MuonPropagator = cms.string('SteppingHelixPropagatorAny'),
    PropagationDirection = cms.string('oppositeToMomentum'),
    TrackerPropagator = cms.string('PropagatorWithMaterialOpposite')
)


process.hltESPSoftLeptonByDistance = cms.ESProducer("LeptonTaggerByDistanceESProducer",
    distance = cms.double(0.5)
)


process.hltESPSteppingHelixPropagatorAlong = cms.ESProducer("SteppingHelixPropagatorESProducer",
    ApplyRadX0Correction = cms.bool(True),
    AssumeNoMaterial = cms.bool(False),
    ComponentName = cms.string('hltESPSteppingHelixPropagatorAlong'),
    NoErrorPropagation = cms.bool(False),
    PropagationDirection = cms.string('alongMomentum'),
    SetVBFPointer = cms.bool(False),
    VBFName = cms.string('VolumeBasedMagneticField'),
    debug = cms.bool(False),
    endcapShiftInZNeg = cms.double(0.0),
    endcapShiftInZPos = cms.double(0.0),
    returnTangentPlane = cms.bool(True),
    sendLogWarning = cms.bool(False),
    useEndcapShiftsInZ = cms.bool(False),
    useInTeslaFromMagField = cms.bool(False),
    useIsYokeFlag = cms.bool(True),
    useMagVolumes = cms.bool(True),
    useMatVolumes = cms.bool(True),
    useTuningForL2Speed = cms.bool(False)
)


process.hltESPSteppingHelixPropagatorOpposite = cms.ESProducer("SteppingHelixPropagatorESProducer",
    ApplyRadX0Correction = cms.bool(True),
    AssumeNoMaterial = cms.bool(False),
    ComponentName = cms.string('hltESPSteppingHelixPropagatorOpposite'),
    NoErrorPropagation = cms.bool(False),
    PropagationDirection = cms.string('oppositeToMomentum'),
    SetVBFPointer = cms.bool(False),
    VBFName = cms.string('VolumeBasedMagneticField'),
    debug = cms.bool(False),
    endcapShiftInZNeg = cms.double(0.0),
    endcapShiftInZPos = cms.double(0.0),
    returnTangentPlane = cms.bool(True),
    sendLogWarning = cms.bool(False),
    useEndcapShiftsInZ = cms.bool(False),
    useInTeslaFromMagField = cms.bool(False),
    useIsYokeFlag = cms.bool(True),
    useMagVolumes = cms.bool(True),
    useMatVolumes = cms.bool(True),
    useTuningForL2Speed = cms.bool(False)
)


process.hltESPStripCPEfromTrackAngle = cms.ESProducer("StripCPEESProducer",
    ComponentName = cms.string('hltESPStripCPEfromTrackAngle'),
    ComponentType = cms.string('StripCPEfromTrackAngle'),
    parameters = cms.PSet(
        mLC_P0 = cms.double(-0.326),
        mLC_P1 = cms.double(0.618),
        mLC_P2 = cms.double(0.3),
        mTEC_P0 = cms.double(-1.885),
        mTEC_P1 = cms.double(0.471),
        mTIB_P0 = cms.double(-0.742),
        mTIB_P1 = cms.double(0.202),
        mTID_P0 = cms.double(-1.427),
        mTID_P1 = cms.double(0.433),
        mTOB_P0 = cms.double(-1.026),
        mTOB_P1 = cms.double(0.253),
        maxChgOneMIP = cms.double(6000.0),
        useLegacyError = cms.bool(False)
    )
)


process.hltESPTTRHBWithTrackAngle = cms.ESProducer("TkTransientTrackingRecHitBuilderESProducer",
    ComponentName = cms.string('hltESPTTRHBWithTrackAngle'),
    ComputeCoarseLocalPositionFromDisk = cms.bool(False),
    Matcher = cms.string('StandardMatcher'),
    PixelCPE = cms.string('hltESPPixelCPEGeneric'),
    StripCPE = cms.string('hltESPStripCPEfromTrackAngle')
)


process.hltESPTTRHBuilderAngleAndTemplate = cms.ESProducer("TkTransientTrackingRecHitBuilderESProducer",
    ComponentName = cms.string('hltESPTTRHBuilderAngleAndTemplate'),
    ComputeCoarseLocalPositionFromDisk = cms.bool(False),
    Matcher = cms.string('StandardMatcher'),
    PixelCPE = cms.string('hltESPPixelCPETemplateReco'),
    StripCPE = cms.string('hltESPStripCPEfromTrackAngle')
)


process.hltESPTTRHBuilderPixelOnly = cms.ESProducer("TkTransientTrackingRecHitBuilderESProducer",
    ComponentName = cms.string('hltESPTTRHBuilderPixelOnly'),
    ComputeCoarseLocalPositionFromDisk = cms.bool(False),
    Matcher = cms.string('StandardMatcher'),
    PixelCPE = cms.string('hltESPPixelCPEGeneric'),
    StripCPE = cms.string('Fake')
)


process.hltESPTTRHBuilderWithoutAngle4PixelTriplets = cms.ESProducer("TkTransientTrackingRecHitBuilderESProducer",
    ComponentName = cms.string('hltESPTTRHBuilderWithoutAngle4PixelTriplets'),
    ComputeCoarseLocalPositionFromDisk = cms.bool(False),
    Matcher = cms.string('StandardMatcher'),
    PixelCPE = cms.string('hltESPPixelCPEGeneric'),
    StripCPE = cms.string('Fake')
)


process.hltESPTobTecStepClusterShapeHitFilter = cms.ESProducer("ClusterShapeHitFilterESProducer",
    ComponentName = cms.string('hltESPTobTecStepClusterShapeHitFilter'),
    PixelShapeFile = cms.string('RecoPixelVertexing/PixelLowPtUtilities/data/pixelShape.par'),
    clusterChargeCut = cms.PSet(
        refToPSet_ = cms.string('HLTSiStripClusterChargeCutTight')
    )
)


process.hltESPTobTecStepFittingSmoother = cms.ESProducer("KFFittingSmootherESProducer",
    BreakTrajWith2ConsecutiveMissing = cms.bool(False),
    ComponentName = cms.string('hltESPTobTecStepFitterSmoother'),
    EstimateCut = cms.double(30.0),
    Fitter = cms.string('hltESPTobTecStepRKFitter'),
    LogPixelProbabilityCut = cms.double(-16.0),
    MaxFractionOutliers = cms.double(0.3),
    MaxNumberOfOutliers = cms.int32(3),
    MinDof = cms.int32(2),
    MinNumberOfHits = cms.int32(7),
    NoInvalidHitsBeginEnd = cms.bool(False),
    NoOutliersBeginEnd = cms.bool(False),
    RejectTracks = cms.bool(True),
    Smoother = cms.string('hltESPTobTecStepRKSmoother'),
    appendToDataLabel = cms.string('')
)


process.hltESPTobTecStepFittingSmootherForLoopers = cms.ESProducer("KFFittingSmootherESProducer",
    BreakTrajWith2ConsecutiveMissing = cms.bool(False),
    ComponentName = cms.string('hltESPTobTecStepFitterSmootherForLoopers'),
    EstimateCut = cms.double(30.0),
    Fitter = cms.string('hltESPTobTecStepRKFitterForLoopers'),
    LogPixelProbabilityCut = cms.double(-16.0),
    MaxFractionOutliers = cms.double(0.3),
    MaxNumberOfOutliers = cms.int32(3),
    MinDof = cms.int32(2),
    MinNumberOfHits = cms.int32(7),
    NoInvalidHitsBeginEnd = cms.bool(False),
    NoOutliersBeginEnd = cms.bool(False),
    RejectTracks = cms.bool(True),
    Smoother = cms.string('hltESPTobTecStepRKSmootherForLoopers'),
    appendToDataLabel = cms.string('')
)


process.hltESPTobTecStepFlexibleKFFittingSmoother = cms.ESProducer("FlexibleKFFittingSmootherESProducer",
    ComponentName = cms.string('hltESPTobTecStepFlexibleKFFittingSmoother'),
    appendToDataLabel = cms.string(''),
    looperFitter = cms.string('hltESPTobTecStepFitterSmootherForLoopers'),
    standardFitter = cms.string('hltESPTobTecStepFitterSmoother')
)


process.hltESPTobTecStepRKTrajectoryFitter = cms.ESProducer("KFTrajectoryFitterESProducer",
    ComponentName = cms.string('hltESPTobTecStepRKFitter'),
    Estimator = cms.string('hltESPChi2MeasurementEstimator30'),
    Propagator = cms.string('PropagatorWithMaterialParabolicMf'),
    RecoGeometry = cms.string('hltESPDummyDetLayerGeometry'),
    Updator = cms.string('hltESPKFUpdator'),
    appendToDataLabel = cms.string(''),
    minHits = cms.int32(7)
)


process.hltESPTobTecStepRKTrajectoryFitterForLoopers = cms.ESProducer("KFTrajectoryFitterESProducer",
    ComponentName = cms.string('hltESPTobTecStepRKFitterForLoopers'),
    Estimator = cms.string('hltESPChi2MeasurementEstimator30'),
    Propagator = cms.string('PropagatorWithMaterialForLoopers'),
    RecoGeometry = cms.string('hltESPDummyDetLayerGeometry'),
    Updator = cms.string('hltESPKFUpdator'),
    appendToDataLabel = cms.string(''),
    minHits = cms.int32(7)
)


process.hltESPTobTecStepRKTrajectorySmoother = cms.ESProducer("KFTrajectorySmootherESProducer",
    ComponentName = cms.string('hltESPTobTecStepRKSmoother'),
    Estimator = cms.string('hltESPChi2MeasurementEstimator30'),
    Propagator = cms.string('PropagatorWithMaterialParabolicMf'),
    RecoGeometry = cms.string('hltESPDummyDetLayerGeometry'),
    Updator = cms.string('hltESPKFUpdator'),
    appendToDataLabel = cms.string(''),
    errorRescaling = cms.double(10.0),
    minHits = cms.int32(7)
)


process.hltESPTobTecStepRKTrajectorySmootherForLoopers = cms.ESProducer("KFTrajectorySmootherESProducer",
    ComponentName = cms.string('hltESPTobTecStepRKSmootherForLoopers'),
    Estimator = cms.string('hltESPChi2MeasurementEstimator30'),
    Propagator = cms.string('PropagatorWithMaterialForLoopers'),
    RecoGeometry = cms.string('hltESPDummyDetLayerGeometry'),
    Updator = cms.string('hltESPKFUpdator'),
    appendToDataLabel = cms.string(''),
    errorRescaling = cms.double(10.0),
    minHits = cms.int32(7)
)


process.hltESPTobTecStepTrajectoryCleanerBySharedHits = cms.ESProducer("TrajectoryCleanerESProducer",
    ComponentName = cms.string('hltESPTobTecStepTrajectoryCleanerBySharedHits'),
    ComponentType = cms.string('TrajectoryCleanerBySharedHits'),
    MissingHitPenalty = cms.double(20.0),
    ValidHitBonus = cms.double(5.0),
    allowSharedFirstHit = cms.bool(True),
    fractionShared = cms.double(0.09)
)


process.hltESPTrackerRecoGeometryESProducer = cms.ESProducer("TrackerRecoGeometryESProducer",
    appendToDataLabel = cms.string(''),
    trackerGeometryLabel = cms.untracked.string('')
)


process.hltESPTrajectoryCleanerBySharedHits = cms.ESProducer("TrajectoryCleanerESProducer",
    ComponentName = cms.string('hltESPTrajectoryCleanerBySharedHits'),
    ComponentType = cms.string('TrajectoryCleanerBySharedHits'),
    MissingHitPenalty = cms.double(0.0),
    ValidHitBonus = cms.double(100.0),
    allowSharedFirstHit = cms.bool(False),
    fractionShared = cms.double(0.5)
)


process.hltESPTrajectoryCleanerBySharedSeeds = cms.ESProducer("TrajectoryCleanerESProducer",
    ComponentName = cms.string('hltESPTrajectoryCleanerBySharedSeeds'),
    ComponentType = cms.string('TrajectoryCleanerBySharedSeeds'),
    MissingHitPenalty = cms.double(0.0),
    ValidHitBonus = cms.double(100.0),
    allowSharedFirstHit = cms.bool(True),
    fractionShared = cms.double(0.5)
)


process.hltESPTrajectoryFitterRK = cms.ESProducer("KFTrajectoryFitterESProducer",
    ComponentName = cms.string('hltESPTrajectoryFitterRK'),
    Estimator = cms.string('hltESPChi2MeasurementEstimator30'),
    Propagator = cms.string('hltESPRungeKuttaTrackerPropagator'),
    RecoGeometry = cms.string('hltESPDummyDetLayerGeometry'),
    Updator = cms.string('hltESPKFUpdator'),
    appendToDataLabel = cms.string(''),
    minHits = cms.int32(3)
)


process.hltESPTrajectorySmootherRK = cms.ESProducer("KFTrajectorySmootherESProducer",
    ComponentName = cms.string('hltESPTrajectorySmootherRK'),
    Estimator = cms.string('hltESPChi2MeasurementEstimator30'),
    Propagator = cms.string('hltESPRungeKuttaTrackerPropagator'),
    RecoGeometry = cms.string('hltESPDummyDetLayerGeometry'),
    Updator = cms.string('hltESPKFUpdator'),
    appendToDataLabel = cms.string(''),
    errorRescaling = cms.double(100.0),
    minHits = cms.int32(3)
)


process.hoDetIdAssociator = cms.ESProducer("DetIdAssociatorESProducer",
    ComponentName = cms.string('HODetIdAssociator'),
    etaBinSize = cms.double(0.087),
    includeBadChambers = cms.bool(False),
    nEta = cms.int32(30),
    nPhi = cms.int32(72)
)


process.muonDetIdAssociator = cms.ESProducer("DetIdAssociatorESProducer",
    ComponentName = cms.string('MuonDetIdAssociator'),
    etaBinSize = cms.double(0.125),
    includeBadChambers = cms.bool(False),
    nEta = cms.int32(48),
    nPhi = cms.int32(48)
)


process.navigationSchoolESProducer = cms.ESProducer("NavigationSchoolESProducer",
    ComponentName = cms.string('SimpleNavigationSchool'),
    SimpleMagneticField = cms.string('ParabolicMf')
)


process.omtfParams = cms.ESProducer("L1TMuonOverlapParamsESProducer",
    configXMLFile = cms.FileInPath('L1Trigger/L1TMuon/data/omtf_config/hwToLogicLayer_0x0004.xml'),
    patternsXMLFiles = cms.VPSet(cms.PSet(
        patternsXMLFile = cms.FileInPath('L1Trigger/L1TMuon/data/omtf_config/Patterns_0x0003.xml')
    ))
)


process.preshowerDetIdAssociator = cms.ESProducer("DetIdAssociatorESProducer",
    ComponentName = cms.string('PreshowerDetIdAssociator'),
    etaBinSize = cms.double(0.1),
    includeBadChambers = cms.bool(False),
    nEta = cms.int32(60),
    nPhi = cms.int32(30)
)


process.siPixelQualityESProducer = cms.ESProducer("SiPixelQualityESProducer",
    ListOfRecordToMerge = cms.VPSet(cms.PSet(
        record = cms.string('SiPixelQualityFromDbRcd'),
        tag = cms.string('')
    ), 
        cms.PSet(
            record = cms.string('SiPixelDetVOffRcd'),
            tag = cms.string('')
        ))
)


process.siPixelTemplateDBObjectESProducer = cms.ESProducer("SiPixelTemplateDBObjectESProducer")


process.siStripBackPlaneCorrectionDepESProducer = cms.ESProducer("SiStripBackPlaneCorrectionDepESProducer",
    BackPlaneCorrectionDeconvMode = cms.PSet(
        label = cms.untracked.string('deconvolution'),
        record = cms.string('SiStripBackPlaneCorrectionRcd')
    ),
    BackPlaneCorrectionPeakMode = cms.PSet(
        label = cms.untracked.string('peak'),
        record = cms.string('SiStripBackPlaneCorrectionRcd')
    ),
    LatencyRecord = cms.PSet(
        label = cms.untracked.string(''),
        record = cms.string('SiStripLatencyRcd')
    )
)


process.siStripLorentzAngleDepESProducer = cms.ESProducer("SiStripLorentzAngleDepESProducer",
    LatencyRecord = cms.PSet(
        label = cms.untracked.string(''),
        record = cms.string('SiStripLatencyRcd')
    ),
    LorentzAngleDeconvMode = cms.PSet(
        label = cms.untracked.string('deconvolution'),
        record = cms.string('SiStripLorentzAngleRcd')
    ),
    LorentzAnglePeakMode = cms.PSet(
        label = cms.untracked.string('peak'),
        record = cms.string('SiStripLorentzAngleRcd')
    )
)


process.sistripconn = cms.ESProducer("SiStripConnectivity")


process.trackerTopology = cms.ESProducer("TrackerTopologyEP",
    appendToDataLabel = cms.string('')
)


process.CSCChannelMapperESSource = cms.ESSource("EmptyESSource",
    firstValid = cms.vuint32(1),
    iovIsRunNotTime = cms.bool(True),
    recordName = cms.string('CSCChannelMapperRecord')
)


process.CSCINdexerESSource = cms.ESSource("EmptyESSource",
    firstValid = cms.vuint32(1),
    iovIsRunNotTime = cms.bool(True),
    recordName = cms.string('CSCIndexerRecord')
)


process.GlobalParametersRcdSource = cms.ESSource("EmptyESSource",
    firstValid = cms.vuint32(1),
    iovIsRunNotTime = cms.bool(True),
    recordName = cms.string('L1TGlobalParametersRcd')
)


process.GlobalTag = cms.ESSource("PoolDBESSource",
    DBParameters = cms.PSet(
        authenticationPath = cms.untracked.string('.'),
        connectionRetrialPeriod = cms.untracked.int32(10),
        connectionRetrialTimeOut = cms.untracked.int32(60),
        connectionTimeOut = cms.untracked.int32(0),
        enableConnectionSharing = cms.untracked.bool(True),
        enablePoolAutomaticCleanUp = cms.untracked.bool(False),
        enableReadOnlySessionOnUpdateConnection = cms.untracked.bool(False),
        idleConnectionCleanupPeriod = cms.untracked.int32(10),
        messageLevel = cms.untracked.int32(0)
    ),
    DumpStat = cms.untracked.bool(False),
    ReconnectEachRun = cms.untracked.bool(False),
    RefreshAlways = cms.untracked.bool(False),
    RefreshEachRun = cms.untracked.bool(False),
    RefreshOpenIOVs = cms.untracked.bool(False),
    connect = cms.string('frontier://FrontierProd/CMS_CONDITIONS'),
    globaltag = cms.string('80X_mcRun2_asymptotic_v20'),
    snapshotTime = cms.string(''),
    toGet = cms.VPSet(cms.PSet(
        connect = cms.string('frontier://FrontierProd/CMS_CONDITIONS'),
        record = cms.string('L1TUtmTriggerMenuRcd'),
        snapshotTime = cms.string('2016-10-06 19:36:53.000'),
        tag = cms.string('L1Menu_Collisions2016_v9_m2_xml')
    ), 
        cms.PSet(
            connect = cms.string('frontier://FrontierProd/CMS_CONDITIONS'),
            record = cms.string('L1TUtmTriggerMenuRcd'),
            snapshotTime = cms.string('2016-10-06 19:36:53.000'),
            tag = cms.string('L1Menu_Collisions2016_v9_m2_xml')
        ))
)


process.HepPDTESSource = cms.ESSource("HepPDTESSource",
    pdtFileName = cms.FileInPath('SimGeneral/HepPDTESSource/data/pythiaparticle.tbl')
)


process.L1TGlobalPrescalesVetosRcdSource = cms.ESSource("EmptyESSource",
    firstValid = cms.vuint32(1),
    iovIsRunNotTime = cms.bool(True),
    recordName = cms.string('L1TGlobalPrescalesVetosRcd')
)


process.StableParametersRcdSource = cms.ESSource("EmptyESSource",
    firstValid = cms.vuint32(1),
    iovIsRunNotTime = cms.bool(True),
    recordName = cms.string('L1TGlobalStableParametersRcd')
)


process.bmbtfParamsSource = cms.ESSource("EmptyESSource",
    firstValid = cms.vuint32(1),
    iovIsRunNotTime = cms.bool(True),
    recordName = cms.string('L1TMuonBarrelParamsRcd')
)


process.caloParamsSource = cms.ESSource("EmptyESSource",
    firstValid = cms.vuint32(1),
    iovIsRunNotTime = cms.bool(True),
    recordName = cms.string('L1TCaloStage2ParamsRcd')
)


process.eegeom = cms.ESSource("EmptyESSource",
    firstValid = cms.vuint32(1),
    iovIsRunNotTime = cms.bool(True),
    recordName = cms.string('EcalMappingRcd')
)


process.emtfParamsSource = cms.ESSource("EmptyESSource",
    firstValid = cms.vuint32(1),
    iovIsRunNotTime = cms.bool(True),
    recordName = cms.string('L1TMuonEndCapForestRcd')
)


process.es_hardcode = cms.ESSource("HcalHardcodeCalibrations",
    fromDDD = cms.untracked.bool(False),
    toGet = cms.untracked.vstring('GainWidths')
)


process.gmtParamsSource = cms.ESSource("EmptyESSource",
    firstValid = cms.vuint32(1),
    iovIsRunNotTime = cms.bool(True),
    recordName = cms.string('L1TMuonGlobalParamsRcd')
)


process.hltESSBTagRecord = cms.ESSource("EmptyESSource",
    firstValid = cms.vuint32(1),
    iovIsRunNotTime = cms.bool(True),
    recordName = cms.string('JetTagComputerRecord')
)


process.hltESSEcalSeverityLevel = cms.ESSource("EmptyESSource",
    firstValid = cms.vuint32(1),
    iovIsRunNotTime = cms.bool(True),
    recordName = cms.string('EcalSeverityLevelAlgoRcd')
)


process.hltESSHcalSeverityLevel = cms.ESSource("EmptyESSource",
    firstValid = cms.vuint32(1),
    iovIsRunNotTime = cms.bool(True),
    recordName = cms.string('HcalSeverityLevelComputerRcd')
)


process.omtfParamsSource = cms.ESSource("EmptyESSource",
    firstValid = cms.vuint32(1),
    iovIsRunNotTime = cms.bool(True),
    recordName = cms.string('L1TMuonOverlapParamsRcd')
)


