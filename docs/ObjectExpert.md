Introduction
============

Knowning what's going on in CMS and what the latest POG recommendations are is most easily achieved by keeping an eye on a large number of twiki's and HN fora. Most changes in recommendations are communicated via HN, but it's usually good to check the relevant twiki's before a big production, to make sure nothing has been missed.

Useful HN fora to subscribe to
------------------------------
Names correspond to those on the HN subscription page   

**Objects**:   
B-Tagging    
Egamma ID (main Egamma HN - the following three fora receive significantly less traffic but can be useful)    
   [Egamma ID: efficiency measurement]         
   [Egamma ID: electron ID]        
JetMET   
JetMET: Algorithms and Reconstruction    
JetMET: Jet Energy Scale   
JetMET: Missing ET    
MuonID   
Tau ID

**General**:   
CMS Commissioning    
Physics Validation    
Luminosity Discussions    
[Luminosity Operations]    
Dataset Definition    
CERN Computing Announcements    
Grid Announcements    

Twiki's to keep an eye on
-------

**General**:   
https://twiki.cern.ch/twiki/bin/view/CMSPublic/WorkBookMiniAOD2016 (2016 miniAOD)   
https://twiki.cern.ch/twiki/bin/view/CMS/PdmV2016Analysis (Runs, certification, etc for 2016)   
https://twiki.cern.ch/twiki/bin/view/CMS/PdmV (main pdmv page)


**PU reweighting**:   
https://twiki.cern.ch/twiki/bin/view/CMS/PileupJSONFileforData#Pileup_JSON_Files_For_Run_II (calculating PU distribution)   
https://twiki.cern.ch/twiki/bin/view/CMS/PdmVPileUpDescription (General info on PU profiles)   

**Taus**:   
https://twiki.cern.ch/twiki/bin/view/CMS/TauIDRecommendation13TeV   

**Muons**:   
https://twiki.cern.ch/twiki/bin/view/CMS/SWGuideMuonIdRun2  (ID/iso recommendations)   
[https://twiki.cern.ch/twiki/bin/view/CMS/MuonPOG (main page)]

**Electrons**:    
https://twiki.cern.ch/twiki/bin/view/CMS/MultivariateElectronIdentificationRun2 (MVA ID)   
https://twiki.cern.ch/twiki/bin/viewauth/CMS/CutBasedElectronIdentificationRun2 (Cut-based ID)   
https://twiki.cern.ch/twiki/bin/view/CMS/EgammaPFBasedIsolationRun2 (Isolation)  
https://twiki.cern.ch/twiki/bin/view/CMS/EgammaIDRecipesRun2 (main ID recipe page)   

**Jets**:    
Lots of pages...    
https://twiki.cern.ch/twiki/bin/view/CMS/JetMET#Quick_links_to_current_recommend (main jetMET pog twiki, with links to pages that have relevant information. Some of those links below...)    
https://twiki.cern.ch/twiki/bin/view/CMS/JECDataMC (Recommended JECs)    
https://twiki.cern.ch/twiki/bin/view/CMS/JECUncertaintySources (JES uncertainty sources)    
https://twiki.cern.ch/twiki/bin/viewauth/CMS/JetID (Jet identification)   

**MET**:     
https://twiki.cern.ch/twiki/bin/viewauth/CMS/MissingETOptionalFiltersRun2 (MET filters)     
https://twiki.cern.ch/twiki/bin/viewauth/CMS/PdmVPileUpDescription#Pileup_Information

**B-tagging**:    
https://twiki.cern.ch/twiki/bin/viewauth/CMS/BtagRecommendation#Recommendation_for_13_TeV_Data (main recommendation page)    
https://twiki.cern.ch/twiki/bin/view/CMS/BTagSFMethods (b-tag scale factor application methods)

Navigating McM:
---------------

Sometimes you'll notice something odd about your MC samples and you'll want to check what settings they were produced with. Unfortunately this means having to battle with McM - some hints here...   

* Access McM: https://cms-pdmv.cern.ch/mcm/    
* Click `Request`
* Click `Select View`, then hit `Select All` and then `Save selection`
* Click `Output Dataset` and dump the full name of the dataset you're interested in, as shown on DAS, in the search box; hit search
* Some of the information you might be interested may already be shown in one of the boxes (scroll to the right to see them all).The `actions` section is your best friend when it comes to finding the initial settings, though.

To find information on the chain via which the dataset was produced, hit the icon underneath `Actions` that looks like a sideways bar chart. This will open information on the chains used to produce the sample, clicking the first clickable link under `Chain` will give you the first file in the chain used to create the dataset (could be an LHE file for example). Now click the `get test command` button, which is the little tick in a circle (again under `actions`). Somewhere in the command that gives there will be a line `curl -s --insecure <some linke to cms-pdmv.cern.ch/mcm/...` Clicking that link will display a link to the card used for production.

In some cases you might be able to find the information you need by clicking the `get dictionary` button, the first icon from the left under `actions`. 
