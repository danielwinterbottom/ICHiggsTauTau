# Brilcalc set-up instructions:

    ssh <user>@lxplus.cern.ch
    export PATH=$HOME/.local/bin:/afs/cern.ch/cms/lumi/brilconda-1.0.3/bin:$PATH
    pip uninstall -y brilws
    pip install --install-option="--prefix=$HOME/.local" brilws
    pip show brilws

Total lumi using latest PromptReco I could find:

    brilcalc lumi -i /afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions18/13TeV/PromptReco/Cert_314472-325175_13TeV_PromptReco_Collisions18_JSON.txt

    Summary:
    +-------+------+--------+--------+-----------------------+-----------------------+
    | nfill | nrun | nls    | ncms   | totdelivered(/ub)     | totrecorded(/ub)      |
    +-------+------+--------+--------+-----------------------+-----------------------+
    | 195   | 477  | 233928 | 233536 | 61281229980.854873657 | 58826846900.918640137 |
    +-------+------+--------+--------+-----------------------+-----------------------+

** 58.8 1/fb **

## Trigger summary
Get the trigger summary from this:

    wget http://fwyzard.web.cern.ch/fwyzard/hlt/2018/summary


## mu tau triggers

    cat hlt2018_summary.txt | grep HLT_IsoMu | sed -e 's/^\w*\ *//' | sed 's/.$//' > IsoMu2018.txt

## e tau triggers

    cat hlt2018_summary.txt | grep HLT_Ele | sed -e 's/^\w*\ *//' | sed 's/.$//' > Ele2018.txt

## tau tau triggers

    cat hlt2018_summary.txt | grep -E "(DoubleLoose|DoubleMedium|DoubleTight)" | sed -e 's/^\w*\ *//' | sed 's/.$//' > Tau2018.txt

## e mu triggers
    
    cat hlt2018_summary.txt | grep "HLT_Mu" | grep "TrkIsoVVL" | sed -e 's/^\w*\ *//' | sed 's/.$//' > EMu2018.txt
