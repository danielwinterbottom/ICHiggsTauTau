# Brilcalc set-up instructions:

    ssh <user>@lxplus.cern.ch
    export PATH=$HOME/.local/bin:/afs/cern.ch/cms/lumi/brilconda-1.0.3/bin:$PATH
    pip uninstall -y brilws
    pip install --install-option="--prefix=$HOME/.local" brilws
    pip show brilws

Total lumi using latest PromptReco I could find:
`brilcalc lumi -i /afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions18/13TeV/PromptReco/Cert_314472-325175_13TeV_PromptReco_Collisions18_JSON.txt`

Summary:
+-------+------+--------+--------+-----------------------+-----------------------+
| nfill | nrun | nls    | ncms   | totdelivered(/ub)     | totrecorded(/ub)      |
+-------+------+--------+--------+-----------------------+-----------------------+
| 195   | 477  | 233928 | 233536 | 61281229980.854873657 | 58826846900.918640137 |
+-------+------+--------+--------+-----------------------+-----------------------+

### 58.8 1/fb


