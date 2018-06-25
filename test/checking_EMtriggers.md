# Brilcalc set-up instructions:

ssh -Y <user>@lxplus.cern.ch
export PATH=$HOME/.local/bin:/afs/cern.ch/cms/lumi/brilconda-1.0.3/bin:$PATH
pip uninstall -y brilws
pip install --install-option="--prefix=$HOME/.local" brilws
pip show brilws


# Commands and output of brilcalc for EMu channel 2017
brilcalc lumi -i /afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions17/13TeV/ReReco/Cert_294927-306462_13TeV_EOY2017ReReco_Collisions17_JSON_v1.txt

#Summary:
+-------+------+--------+--------+-------------------+------------------+
| nfill | nrun | nls    | ncms   | totdelivered(/ub) | totrecorded(/ub) |
+-------+------+--------+--------+-------------------+------------------+ 
| 175   | 474  | 206559 | 205440 | 44526513978.821   | 41859516607.442  |
+-------+------+--------+--------+-------------------+------------------+

brilcalc lumi -i /afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions17/13TeV/ReReco/Cert_294927-306462_13TeV_EOY2017ReReco_Collisions17_JSON_v1.txt --hltpath HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_v*

#Summary:
+----------------------------------------------------+-------+------+--------+-------------------+------------------+
| hltpath| nfill | nrun | ncms   | totdelivered(/ub) | totrecorded(/ub) |
+----------------------------------------------------+-------+------+--------+-------------------+------------------+
| HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_v1 | 9     | 21   | 5908   | 1418674990.143    | 1286319491.868   |
| HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_v2 | 20    | 84   | 23011  | 5520914975.267    | 4981636131.956   |
| HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_v3 | 27    | 56   | 23304  | 3604338656.863    | 3465593598.567   |
| HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_v4 | 91    | 218  | 124446 | 27759673613.826   | 26466617033.276  |
| HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_v5 | 2     | 13   | 4468   | 903398671.207     | 864441964.983    |
+----------------------------------------------------+-------+------+--------+-------------------+------------------+
#Sum delivered : 39207000907.306
#Sum recorded : 37064608220.650


brilcalc lumi -i /afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions17/13TeV/ReReco/Cert_294927-306462_13TeV_EOY2017ReReco_Collisions17_JSON_v1.txt --hltpath HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v*
#Summary:
+--------------------------------------------------------+-------+------+--------+-------------------+------------------+
| hltpath    | nfill | nrun | ncms   | totdelivered(/ub) | totrecorded(/ub) |
+--------------------------------------------------------+-------+------+--------+-------------------+------------------+
| HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v10 | 20    | 84   | 23011  | 5520914975.267    | 4981636131.956   |
| HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v11 | 27    | 56   | 23304  | 3604338656.863    | 3465593598.567   |
| HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v12 | 91    | 218  | 124446 | 27759673613.826   | 26466617033.276  |
| HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v13 | 2     | 13   | 4468   | 903398671.207     | 864441964.983    |
| HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v5  | 2     | 3    | 1775   | 242362947.488     | 234161067.685    |
| HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v6  | 11    | 43   | 14172  | 2778805306.786    | 2670477908.103   |
| HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v8  | 13    | 36   | 8349   | 2027454168.581    | 1888010198.202   |
| HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v9  | 9     | 21   | 5908   | 1418674990.143    | 1286319491.868   |
+--------------------------------------------------------+-------+------+--------+-------------------+------------------+
#Sum delivered : 44255623330.162
#Sum recorded : 41857257394.641

brilcalc lumi -i /afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions17/13TeV/ReReco/Cert_294927-306462_13TeV_EOY2017ReReco_Collisions17_JSON_v1.txt --hltpath HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_v*


#Summary:               
+---------------------------------------------------+-------+------+--------+-------------------+------------------+
| hltpath                     | nfill | nrun | ncms   | totdelivered(/ub) | totrecorded(/ub) |
+---------------------------------------------------+-------+------+--------+-------------------+------------------+  
| HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_v5 | 9     | 21   | 5908   | 183383704.676     | 168310063.251    |
| HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_v6 | 20    | 84   | 23011  | 645221795.949     | 586885900.271    | 
| HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_v7 | 27    | 56   | 23304  | 315681651.217     | 304200935.725    |
| HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_v8 | 91    | 218  | 124446 | 2367271053.920    | 2263168931.665   |                  
| HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_v9 | 2     | 13   | 4468   | 86691262.555      | 83185757.460     |
+---------------------------------------------------+-------+------+--------+-------------------+------------------+                   
#Sum delivered : 3598249468.317
#Sum recorded : 3405751588.372

brilcalc lumi -i /afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions17/13TeV/ReReco/Cert_294927-306462_13TeV_EOY2017ReReco_Collisions17_JSON_v1.txt --hltpath HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ_v*

#Summary:
+-------------------------------------------------------+-------+------+--------+-------------------+------------------+
| hltpath   | nfill | nrun | ncms   | totdelivered(/ub) | totrecorded(/ub) |
+-------------------------------------------------------+-------+------+--------+-------------------+------------------+
| HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ_v10 | 91    | 218  | 124446 | 27759673613.826   | 26466617033.276  |
| HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ_v11 | 2     | 13   | 4468   | 903398671.207     | 864441964.983    |
| HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ_v4  | 13    | 46   | 15947  | 3021168254.274    | 2904638975.789   |
| HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ_v6  | 13    | 36   | 8349   | 2027454168.581    | 1888010198.202   |
| HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ_v7  | 9     | 21   | 5908   | 1418674990.143    | 1286319491.868   |
| HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ_v8  | 20    | 84   | 23011  | 5520914975.267    | 4981636131.956   |
| HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ_v9  | 27    | 56   | 23304  | 3604338656.863    | 3465593598.567   |
+-------------------------------------------------------+-------+------+--------+-------------------+------------------+
#Sum delivered : 44255623330.162
#Sum recorded : 41857257394.641


brilcalc lumi -i /afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions17/13TeV/ReReco/Cert_294927-306462_13TeV_EOY2017ReReco_Collisions17_JSON_v1.txt --hltpath HLT_Mu12_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_v*
#Summary:               
+----------------------------------------------------+-------+------+--------+-------------------+------------------+
| hltpath| nfill | nrun | ncms   | totdelivered(/ub) | totrecorded(/ub) |
+----------------------------------------------------+-------+------+--------+-------------------+------------------+
| HLT_Mu12_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_v1 | 9     | 21   | 5908   | 210919681.684     | 193237733.842    |  
| HLT_Mu12_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_v2 | 20    | 84   | 23011  | 770451735.257     | 700414361.122    |          
| HLT_Mu12_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_v3 | 27    | 56   | 23304  | 348007567.342     | 335426278.812    |                
| HLT_Mu12_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_v4 | 91    | 218  | 124446 | 2702183188.011    | 2585260937.824   |
| HLT_Mu12_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_v5 | 2     | 13   | 4468   | 97305559.965      | 93447710.049     |
+----------------------------------------------------+-------+------+--------+-------------------+------------------+ 
#Sum delivered : 4128867732.259
#Sum recorded : 3907787021.648


brilcalc lumi -i /afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions17/13TeV/ReReco/Cert_294927-306462_13TeV_EOY2017ReReco_Collisions17_JSON_v1.txt --hltpath HLT_Mu12_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ_v*

