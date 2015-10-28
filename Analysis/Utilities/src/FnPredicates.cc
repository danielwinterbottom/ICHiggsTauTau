#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPredicates.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPairs.h"
#include "UserCode/ICHiggsTauTau//interface/city.h"

#include <boost/lexical_cast.hpp>
#include "TRandom2.h"
#include "TMath.h"
#include <utility>
#include <set>

namespace ic {

  bool GoodVertex(Vertex const* vertex) {
    if (     vertex->ndof() > 4 
          && fabs(vertex->vz()) < 24.0 
          && vertex->point().rho() < 2.0) {
      return true;
    } else {
      return false; 
    }
  }

  bool GenParticleInMassBand(GenParticle const* part, int pdgid, double m_low, double m_high) {
    return (abs(part->pdgid()) == pdgid && part->vector().M() > m_low && part->vector().M() < m_high);
  }

  bool IsFilterMatched(Candidate const* cand, std::vector<TriggerObject *> const& objs, std::string const& filter, double const& max_dr) {
    std::size_t hash = CityHash64(filter);
    for (unsigned i = 0; i < objs.size(); ++i) {
      std::vector<std::size_t> const& labels = objs[i]->filters();
      if (std::find(labels.begin(),labels.end(), hash) == labels.end()) continue;
      if (DR(cand, objs[i]) < max_dr) return true;
    }
    return false;
  }

 std::pair<bool,unsigned> IsFilterMatchedWithIndex(Candidate const* cand, std::vector<TriggerObject*> const& objs, std::string const& filter, double const& max_dr){
    std::size_t hash = CityHash64(filter);
    for (unsigned i = 0; i < objs.size(); ++i) {
      std::vector<std::size_t> const& labels = objs[i]->filters();
      if (std::find(labels.begin(),labels.end(), hash) == labels.end()) continue;
      if (DR(cand, objs[i]) < max_dr){
         const unsigned index_val = i;
         return std::make_pair(true,index_val);
      }
    }
    return std::make_pair(false,0);
  }

  bool VertexDz(Tau const* cand, double const& vertexZ) {
    return ( fabs(cand->vz() - vertexZ)==0) ; 
  }

  bool MinPtMaxEta(Candidate const* cand, double const& minPt, double const& maxEta) {
    return ( (cand->pt() > minPt) && (std::fabs(cand->eta()) < maxEta) ); 
  }


  bool CSVMediumWP(Jet const* cand, double const& minDiscri) {
    return (cand->GetBDiscriminator("combinedSecondaryVertexBJetTags") > minDiscri);
  }



  bool InEcalGap(Electron const* electron) {
    double scEta = fabs(electron->sc_eta());
    return (scEta >= 1.4442 && scEta <= 1.566);
  }

  bool MinPtMaxEtaSC(SuperCluster const* cand, double const& minPt, double const& maxEta) {
    return ( (cand->pt() > minPt) && (std::fabs(cand->eta()) < maxEta) ); 
  }

  bool InEcalGapSC(SuperCluster const* electron) {
    double scEta = fabs(electron->eta());
    return (scEta >= 1.4442 && scEta <= 1.566);
  }

  bool InEcalGapCand(Candidate const* cand) {
    double scEta = fabs(cand->eta());
    return (scEta >= 1.4442 && scEta <= 1.566);
  }

  bool MaxEtaSC(Electron const* electron, double const& maxEta) {
    double scEta = fabs(electron->sc_eta());
    return (scEta < maxEta);
  }

  bool PFJetID(PFJet const* jet) {
    int n_pu = jet->charged_multiplicity() + jet->neutral_multiplicity() + jet->HF_had_multiplicity() + jet->HF_em_multiplicity();
    double eta = fabs(jet->eta());
    bool result = false;

    double neutralFrac = ( jet->neutral_had_energy() + jet->HF_had_energy() ) / jet->uncorrected_energy();

    if (eta < 2.4) {
      result = neutralFrac   < 0.99
      && jet->neutral_em_energy_frac()    < 0.99
            && n_pu                             > 0
            && jet->charged_had_energy_frac()   > 0.0
            && jet->charged_multiplicity()      > 0
            && jet->charged_em_energy_frac()    < 0.99;
    } else {
      result = neutralFrac   < 0.99
            && jet->neutral_em_energy_frac()    < 0.99
            && n_pu                             > 0;
    }
    return result;
  }

  bool PFJetIDNoHFCut(PFJet const* jet) {
    int n_pu = jet->charged_multiplicity() + jet->neutral_multiplicity() + jet->HF_had_multiplicity() + jet->HF_em_multiplicity();
    double eta = fabs(jet->eta());
    bool result = false;

    double neutralFrac = jet->neutral_had_energy() / jet->uncorrected_energy();

    if (eta < 2.4) {
      result = neutralFrac   < 0.99
      && jet->neutral_em_energy_frac()    < 0.99
            && n_pu                             > 0
            && jet->charged_had_energy_frac()   > 0.0
            && jet->charged_multiplicity()      > 0
            && jet->charged_em_energy_frac()    < 0.99;
    } else {
      result = neutralFrac   < 0.99
            && jet->neutral_em_energy_frac()    < 0.99
            && n_pu                             > 0;
    }
    return result;
  }

  bool PFJetID2015(PFJet const* jet) {
    double eta = fabs(jet->eta());
    bool result = false;

    double neutralFrac = jet->neutral_had_energy() / jet->uncorrected_energy();
//    int n_pf = jet->charged_multiplicity() + jet->neutral_multiplicity() + jet->HF_had_multiplicity() + jet->HF_em_multiplicity();

    if (eta <= 2.4) {
      result = neutralFrac   < 0.99
      && jet->neutral_em_energy_frac()    < 0.99
            && jet->charged_multiplicity()+jet->neutral_multiplicity() > 1
            && jet->charged_had_energy_frac()   > 0.0
            && jet->charged_multiplicity()      > 0
            && jet->charged_em_energy_frac()    < 0.99;
    } else if(eta<=3.0){
      result = neutralFrac   < 0.99
            && jet->neutral_em_energy_frac()    < 0.99
            && jet->charged_multiplicity()+jet->neutral_multiplicity() > 1;
    }
    else{
      result = jet->neutral_em_energy_frac()    < 0.90
	    && jet->neutral_multiplicity()>10;
    }
    return result;
  }


  bool PUJetID(PFJet const* jet, bool is_2012) {
    // Pt2030_Loose   = cms.vdouble(-0.80,-0.85,-0.84,-0.85),
    // Pt3050_Loose   = cms.vdouble(-0.80,-0.74,-0.68,-0.77)
    // #4 Eta Categories  0-2.5 2.5-2.75 2.75-3.0 3.0-5.0
    double abs_eta = fabs(jet->eta());
    double pt = jet->pt();
    double pu_id_mva_value = jet->pu_id_mva_value();
    if (is_2012) {
      if (pt > 20. && pt <= 30) {
            if (abs_eta < 2.5) {
                  return (pu_id_mva_value > -0.80);
            } else if (abs_eta < 2.75) {
                  return (pu_id_mva_value > -0.85);
            } else if (abs_eta < 3.0) {
                  return (pu_id_mva_value > -0.84);
            } else if (abs_eta < 5.0) {
                  return (pu_id_mva_value > -0.85);
            } else return true;
      } else if (pt > 30.) {
            if (abs_eta < 2.5) {
                  return (pu_id_mva_value > -0.80);
            } else if (abs_eta < 2.75) {
                  return (pu_id_mva_value > -0.74);
            } else if (abs_eta < 3.0) {
                  return (pu_id_mva_value > -0.68);
            } else if (abs_eta < 5.0) {
                  return (pu_id_mva_value > -0.77);
            } else return true;
      } else return true;
    } else {
      if (pt > 20. && pt <= 30) {
            if (abs_eta < 2.5) {
                  return (pu_id_mva_value > -0.40);
            } else if (abs_eta < 2.75) {
                  return (pu_id_mva_value > -0.85);
            } else if (abs_eta < 3.0) {
                  return (pu_id_mva_value > -0.7);
            } else if (abs_eta < 5.0) {
                  return (pu_id_mva_value > -0.6);
            } else return true;
      } else if (pt > 30.) {
            if (abs_eta < 2.5) {
                  return (pu_id_mva_value > -0.40);
            } else if (abs_eta < 2.75) {
                  return (pu_id_mva_value > -0.85);
            } else if (abs_eta < 3.0) {
                  return (pu_id_mva_value > -0.7);
            } else if (abs_eta < 5.0) {
                  return (pu_id_mva_value > -0.6);
            } else return true;
      } else return true;
    }
  }


  bool PileupJetID(PFJet const* jet, unsigned training) {
    double abs_eta = fabs(jet->eta());
    double pt = jet->pt();
    double pu_id_mva_value = jet->pu_id_mva_value();
    if (training == 0) {
      if (pt > 20. && pt <= 30.) {
            if (abs_eta < 2.5) {
                  return (pu_id_mva_value > -0.80);
            } else if (abs_eta < 2.75) {
                  return (pu_id_mva_value > -0.85);
            } else if (abs_eta < 3.0) {
                  return (pu_id_mva_value > -0.84);
            } else if (abs_eta < 5.0) {
                  return (pu_id_mva_value > -0.85);
            } else return true;
      } else if (pt > 30.) {
            if (abs_eta < 2.5) {
                  return (pu_id_mva_value > -0.80);
            } else if (abs_eta < 2.75) {
                  return (pu_id_mva_value > -0.74);
            } else if (abs_eta < 3.0) {
                  return (pu_id_mva_value > -0.68);
            } else if (abs_eta < 5.0) {
                  return (pu_id_mva_value > -0.77);
            } else return true;
      } else return true;
    } else if (training == 1) {
      if (pt > 20. && pt <= 30.) {
            if (abs_eta < 2.5) {
                  return (pu_id_mva_value > -0.40);
            } else if (abs_eta < 2.75) {
                  return (pu_id_mva_value > -0.85);
            } else if (abs_eta < 3.0) {
                  return (pu_id_mva_value > -0.7);
            } else if (abs_eta < 5.0) {
                  return (pu_id_mva_value > -0.6);
            } else return true;
      } else if (pt > 30.) {
            if (abs_eta < 2.5) {
                  return (pu_id_mva_value > -0.40);
            } else if (abs_eta < 2.75) {
                  return (pu_id_mva_value > -0.85);
            } else if (abs_eta < 3.0) {
                  return (pu_id_mva_value > -0.7);
            } else if (abs_eta < 5.0) {
                  return (pu_id_mva_value > -0.6);
            } else return true;
      } else return true;
    } else if (training == 2) {
      if (pt > 20. && pt <= 30.) {
            if (abs_eta < 2.5) {
                  return (pu_id_mva_value > -0.63);
            } else if (abs_eta < 2.75) {
                  return (pu_id_mva_value > -0.60);
            } else if (abs_eta < 3.0) {
                  return (pu_id_mva_value > -0.55);
            } else if (abs_eta < 5.0) {
                  return (pu_id_mva_value > -0.45);
            } else return true;
      } else if (pt > 30.) {
            if (abs_eta < 2.5) {
                  return (pu_id_mva_value > -0.63);
            } else if (abs_eta < 2.75) {
                  return (pu_id_mva_value > -0.60);
            } else if (abs_eta < 3.0) {
                  return (pu_id_mva_value > -0.55);
            } else if (abs_eta < 5.0) {
                  return (pu_id_mva_value > -0.45);
            } else return true;
      } else return true;
    } else {
      return true;
    }
  }

  bool IsReBTagged(Jet const* jet, std::map<std::size_t, bool> const& tag_map) {
    std::map<std::size_t,bool>::const_iterator it = tag_map.find(jet->id());
    if (it != tag_map.end()) {
      return it->second;
    } else {
      std::cout << "Warning in IsReBTagged: Jet with id " << jet->id() << " not found in result map" << std::endl;
      return true;
    }
  }


  double PZeta(CompositeCandidate const* cand, Candidate const* met, double const& alpha) {
    if (cand->AsVector().size() < 2) return 0.0;
    Candidate const* leg1 = cand->At(0);
    Candidate const* leg2 = cand->At(1);
    double leg1x = cos(leg1->phi());
    double leg1y = sin(leg1->phi());
    double leg2x = cos(leg2->phi());
    double leg2y = sin(leg2->phi());
    double zetaX = leg1x + leg2x;
    double zetaY = leg1y + leg2y;
    double zetaR = TMath::Sqrt(zetaX*zetaX + zetaY*zetaY);
    if ( zetaR > 0. ) {
     zetaX /= zetaR;
     zetaY /= zetaR;
   }

   double visPx = leg1->vector().px() + leg2->vector().px();
   double visPy = leg1->vector().py() + leg2->vector().py();
   double pZetaVis = visPx*zetaX + visPy*zetaY;

   double px = /*visPx + */met->vector().px();
   double py = /*visPy + */met->vector().py();
   double pZetaMiss = px*zetaX + py*zetaY;

   return (pZetaMiss - alpha * pZetaVis);
  }

  double PZetaVis(CompositeCandidate const* cand) {
     if (cand->AsVector().size() < 2) return 0.0;
     Candidate const* leg1 = cand->At(0);
     Candidate const* leg2 = cand->At(1); //MUG
     double leg1x = cos(leg1->phi());
     double leg1y = sin(leg1->phi());
     double leg2x = cos(leg2->phi());
     double leg2y = sin(leg2->phi());
     double zetaX = leg1x + leg2x;
     double zetaY = leg1y + leg2y;
     double zetaR = TMath::Sqrt(zetaX*zetaX + zetaY*zetaY);
     if ( zetaR > 0. ) {
      zetaX /= zetaR;
      zetaY /= zetaR;
    }

    double visPx = leg1->vector().px() + leg2->vector().px();
    double visPy = leg1->vector().py() + leg2->vector().py();
    double pZetaVis = visPx*zetaX + visPy*zetaY;
    return pZetaVis;
  }



  double MT(Candidate const* cand1, Candidate const* cand2) {
    double mt = 2. * cand1->pt() * cand2->pt() * (1. - cos(ROOT::Math::VectorUtil::DeltaPhi(cand1->vector(), cand2->vector())));
    if (mt > 0) {
      return std::sqrt(mt);
    } else {
      std::cerr << "Transverse mass would be negative! Returning 0.0" << std::endl;
    }
    return 0.0;
  }

  bool passAntiEMVA(Tau const * tau, int WP){
    
    double z_2 = tau->vz() + (130. / tan(tau->vector().theta()));
    if (z_2 > -1.5 && z_2 < 0.5) return false;
   
    int iCat=(int)(tau->GetTauID("againstElectronMVA3category")+0.5);
    float raw=tau->GetTauID("againstElectronMVA3raw");

    //Code from Ivo
    if(iCat<0) return false;
    if(iCat>15) return true;

    static float cutsMedium[16]={0.933,0.921,0.944,0.945,0.918,0.941,0.981,0.943,0.956,0.947,0.951,0.95,0.897,0.958,0.955,0.942};
    static float cutsTight[16]={ 0.96,0.968,0.971,0.972,0.969,0.959,0.981,0.965,0.975,0.972,0.974,0.971,0.897,0.971,0.961,0.97};
    static float cutsVeryTight[16]={0.978,0.98,0.982,0.985,0.977,0.974,0.989,0.977,0.986,0.983,0.984,0.983,0.971,0.987,0.977,0.981};
    float cut=0;
    if(WP==0) cut = cutsMedium[iCat];
    if(WP==1) cut = cutsTight[iCat];
    if(WP==2) cut = cutsVeryTight[iCat];
    return (raw>cut);
  }

  bool TauEoverP(Tau const* tau, double cut) {
    if (tau->decay_mode() != 0) {
      return true;
    } else {
      double e_over_p = (tau->lead_ecal_energy() + tau->lead_hcal_energy()) / tau->lead_p();
      if (e_over_p != e_over_p) return true;
      return e_over_p > cut;
    }
  }

  bool ElectronZbbID(Electron const * elec) {
    if (InEcalGap(elec)) return false;
    if (elec->has_matched_conversion()) return false;
    if (elec->gsf_tk_nhits() > 1) return false;
    bool in_barrel = true;
    if (fabs(elec->sc_eta()) > 1.4442) in_barrel = false;
    double OneOverEOneOverP = fabs( (1./elec->ecal_energy()) - (1./(elec->ecal_energy()/elec->sc_e_over_p())) );
    return ( (in_barrel 
              && elec->sigma_IetaIeta()           < 0.01
              && fabs(elec->dphi_sc_tk_at_vtx())  < 0.06
              && fabs(elec->deta_sc_tk_at_vtx())  < 0.004
              && OneOverEOneOverP                 < 0.05
              && elec->hadronic_over_em()         < 0.12)
          || (!in_barrel 
              && elec->sigma_IetaIeta()           < 0.03
              && fabs(elec->dphi_sc_tk_at_vtx())  < 0.03
              && fabs(elec->deta_sc_tk_at_vtx())  < 0.007
              && OneOverEOneOverP                 < 0.05) );
  }
  
  bool MVAMETElectron(Electron const * elec) {
    if (fabs(elec->eta()) > 1.4442 && fabs(elec->eta()) < 1.566) return false;
    bool pass_kin = elec->pt() > 9.5 && fabs(elec->eta()) < 2.5;
    bool in_barrel = true;
    if (fabs(elec->eta()) > 1.4442) in_barrel = false;
    double OneOverEOneOverP = fabs( (1./elec->ecal_energy()) - (1./(elec->ecal_energy()/elec->sc_e_over_p())) );
    double pass_iso = (elec->dr03_tk_sum_pt()/elec->pt()) < 0.2;
    return pass_kin && pass_iso && 
          ( (in_barrel 
              && elec->gsf_tk_nhits()            == 0
              && elec->sigma_IetaIeta()           < 0.01
              && fabs(elec->dphi_sc_tk_at_vtx())  < 0.8
              && fabs(elec->deta_sc_tk_at_vtx())  < 0.007
              && OneOverEOneOverP                 < 0.05
              && elec->hadronic_over_em()         < 0.15)
          || (!in_barrel 
              && elec->gsf_tk_nhits()            == 0
              && elec->sigma_IetaIeta()           < 0.03
              && fabs(elec->dphi_sc_tk_at_vtx())  < 0.10
              && fabs(elec->deta_sc_tk_at_vtx())  < 0.009
              && OneOverEOneOverP                 < 0.05
              && elec->hadronic_over_em()         < 0.10) );
  }

  bool ElectronZbbIso(Electron const* elec, bool is_data, double const& rho, double const& cut) {
    double eta = fabs(elec->sc_eta());
    double photon_area, neutral_area;
    if (!is_data) {
      if (eta < 1.0)                  { neutral_area = 0.022; photon_area = 0.084; }    
      if (eta >= 1.0 && eta < 1.479)  { neutral_area = 0.039; photon_area = 0.090; }    
      if (eta >= 1.479 && eta < 2.0)  { neutral_area = 0.040; photon_area = 0.049; }    
      if (eta >= 2.0 && eta < 2.2)    { neutral_area = 0.028; photon_area = 0.099; }    
      if (eta >= 2.2 && eta < 2.3)    { neutral_area = 0.027; photon_area = 0.122; }    
      if (eta >= 2.3 && eta < 2.4)    { neutral_area = 0.024; photon_area = 0.132; }
      if (eta >= 2.4)                 { neutral_area = 0.030; photon_area = 0.155; }    
    } else {
      if (eta < 1.0)                  { neutral_area = 0.024; photon_area = 0.081; }    
      if (eta >= 1.0 && eta < 1.479)  { neutral_area = 0.037; photon_area = 0.084; }    
      if (eta >= 1.479 && eta < 2.0)  { neutral_area = 0.037; photon_area = 0.048; }    
      if (eta >= 2.0 && eta < 2.2)    { neutral_area = 0.023; photon_area = 0.089; }    
      if (eta >= 2.2 && eta < 2.3)    { neutral_area = 0.023; photon_area = 0.092; }    
      if (eta >= 2.3 && eta < 2.4)    { neutral_area = 0.021; photon_area = 0.097; }
      if (eta >= 2.4)                 { neutral_area = 0.021; photon_area = 0.110; }    
    }

    double iso = elec->dr03_pfiso_charged()
      + std::max(elec->dr03_pfiso_gamma() - rho * photon_area, 0.)
      + std::max(elec->dr03_pfiso_neutral() - rho * neutral_area, 0.);
      iso = iso / elec->pt();

    return (iso < cut);
  }



  bool Electron2011WP85Iso(Electron const* elec, double const& rho) {
    bool in_barrel = true;
    if (fabs(elec->sc_eta()) > 1.4442) in_barrel = false;
    // Lazy way
    //double hcal_sum = elec->dr03_hcal_tower_sum_et() + elec->hadronic_over_em() * elec->sc_energy() * fabs(sin(elec->sc_theta()));
    // Correct way
    double hcal_sum = elec->hcal_sum();
    double iso = 0;
    if (in_barrel) {
      iso = elec->dr03_tk_sum_pt() 
            + std::max(0., elec->dr03_ecal_rechit_sum_et() - 1.)
            + hcal_sum
            - rho * TMath::Pi() * 0.3 * 0.3;
    } else {
      iso = elec->dr03_tk_sum_pt()
            + elec->dr03_ecal_rechit_sum_et()
            + hcal_sum
            - rho * TMath::Pi() * 0.3 * 0.3;
    }

    iso = iso / elec->pt();
    return ( (in_barrel && iso < 0.053) || (!in_barrel && iso < 0.042) );
  }

  bool Electron2011WP85IsoNoRhoSub(Electron const* elec) {
    bool in_barrel = true;
    if (fabs(elec->sc_eta()) > 1.4442) in_barrel = false;
    // Lazy way
    //double hcal_sum = elec->dr03_hcal_tower_sum_et() + elec->hadronic_over_em() * elec->sc_energy() * fabs(sin(elec->sc_theta()));
    // Correct way
    double hcal_sum = elec->hcal_sum();
    double iso = 0;
    if (in_barrel) {
      iso = elec->dr03_tk_sum_pt() 
            + std::max(0., elec->dr03_ecal_rechit_sum_et() - 1.)
            + hcal_sum;
    } else {
      iso = elec->dr03_tk_sum_pt()
            + elec->dr03_ecal_rechit_sum_et()
            + hcal_sum;
    }

    iso = iso / elec->pt();
    return ( (in_barrel && iso < 0.053) || (!in_barrel && iso < 0.042) );
  }

  bool ElectronSimpleWP85Iso(Electron const* elec) {
    bool in_barrel = true;
    if (fabs(elec->sc_eta()) > 1.4442) in_barrel = false;
    double trk_iso = elec->dr03_tk_sum_pt() / elec->pt();
    double ecal_iso = elec->dr03_ecal_rechit_sum_et() / elec->pt();
    double hcal_iso = elec->dr03_hcal_tower_sum_et() / elec->pt();
    return ( (in_barrel 
              && trk_iso  < 0.09 
              && ecal_iso < 0.08 
              && hcal_iso < 0.10) 
          || (!in_barrel 
              && trk_iso  < 0.05 
              && ecal_iso < 0.05 
              && hcal_iso < 0.025) );
  }

  bool Electron2011WP85ID(Electron const* elec) {
    bool in_barrel = true;
    if (fabs(elec->sc_eta()) > 1.4442) in_barrel = false;

    return ( (in_barrel 
              && elec->sigma_IetaIeta()           < 0.01
              && fabs(elec->dphi_sc_tk_at_vtx())  < 0.039
              && fabs(elec->deta_sc_tk_at_vtx())  < 0.005
              && elec->gsf_tk_nhits()            == 0
              && (fabs(elec->conv_dist()) >= 0.02 || fabs(elec->conv_dcot()) >= 0.02))
          || (!in_barrel 
              && elec->sigma_IetaIeta()           < 0.031
              && fabs(elec->dphi_sc_tk_at_vtx())  < 0.028
              && fabs(elec->deta_sc_tk_at_vtx())  < 0.007
              && elec->gsf_tk_nhits()            == 0
              && (fabs(elec->conv_dist()) >= 0.02 || fabs(elec->conv_dcot()) >= 0.02)));
  }

  bool Electron2011WP70ID(Electron const* elec) {
    bool in_barrel = true;
    if (fabs(elec->sc_eta()) > 1.4442) in_barrel = false;

    double ooemoop = fabs((1.0/elec->ecal_energy() - elec->sc_e_over_p()/elec->ecal_energy()));

    return ( elec->gsf_tk_nhits()            <= 0
       && !elec->has_matched_conversion()
       && ( (in_barrel 
       && elec->sigma_IetaIeta()           < 0.01
       && fabs(elec->dphi_sc_tk_at_vtx())  < 0.03
       && fabs(elec->deta_sc_tk_at_vtx())  < 0.004
       && elec->hadronic_over_em() < 0.120
       && ooemoop < 0.050
       ) || 
      (!in_barrel 
       && elec->sigma_IetaIeta()           < 0.03
       && fabs(elec->dphi_sc_tk_at_vtx())  < 0.02
       && fabs(elec->deta_sc_tk_at_vtx())  < 0.005
       && elec->hadronic_over_em() < 0.100
       && ooemoop < 0.050
       )
      )
       );
  }

  bool TightElectronIDSpring15(Electron const* elec) {//function for spring15 ID 25ns !! needs updating
    bool in_barrel = true;
    if (fabs(elec->sc_eta()) > 1.479) in_barrel = false;
    
    double ooemoop = fabs((1.0/elec->ecal_energy() - elec->sc_e_over_p()/elec->ecal_energy()));
//    double dbiso = elec->dr03_pfiso_charged() + std::max(0., elec->dr03_pfiso_neutral()+elec->dr03_pfiso_gamma() - 0.5*elec->dr03_pfiso_pu());
    
    return(
       !elec->has_matched_conversion()
        && ( (in_barrel       
	&& elec->full5x5_sigma_IetaIeta()   <0.0101
        && fabs(elec->deta_sc_tk_at_vtx())  <0.00926
        && fabs(elec->dphi_sc_tk_at_vtx())  <0.0336
        && elec->hadronic_over_em()         <0.0597
        && ooemoop                          <0.012
        && elec->gsf_tk_nhits()             <=2
     //   && fabs(elec->dxy_vertex())         <0.060279
	      //  && fabs(elec->dz_vertex())          <0.800538
	      //  && dbiso/elec->pt()                 <0.164369
        ) ||
        (!in_barrel       
        && elec->full5x5_sigma_IetaIeta()   <0.0279
        && fabs(elec->deta_sc_tk_at_vtx())  <0.00724
        && fabs(elec->dphi_sc_tk_at_vtx())  <0.0918
        && elec->hadronic_over_em()         <0.0615
        && ooemoop                          <0.00999
        && elec->gsf_tk_nhits()             <=1
    //    && fabs(elec->dxy_vertex())         <0.273097
     //   && fabs(elec->dz_vertex())          <0.885860
   //     && dbiso/elec->pt()                 <0.212604
        ) )
        );

  }

  bool VetoElectronIDPhys14(Electron const* elec){
    bool in_barrel = true;
    if (fabs(elec->sc_eta()) > 1.479) in_barrel = false;
    
    double ooemoop = fabs((1.0/elec->ecal_energy() - elec->sc_e_over_p()/elec->ecal_energy()));
    //double dbiso = elec->dr03_pfiso_charged() + std::max(0., elec->dr03_pfiso_neutral()+elec->dr03_pfiso_gamma() - 0.5*elec->dr03_pfiso_pu());
    
    return(
       !elec->has_matched_conversion()
        && ( (in_barrel       
        && elec->full5x5_sigma_IetaIeta()   <0.011586
        && fabs(elec->deta_sc_tk_at_vtx())  <0.013625
        && fabs(elec->dphi_sc_tk_at_vtx())  <0.230374
        && elec->hadronic_over_em()         <0.181130
        && ooemoop                          <0.295751
        && elec->gsf_tk_nhits()             <=2
     //   && fabs(elec->dxy_vertex())         <0.060279
      //  && fabs(elec->dz_vertex())          <0.800538
  //      && dbiso/elec->pt()                 <0.164369
        ) ||
        (!in_barrel       
        && elec->full5x5_sigma_IetaIeta()   <0.031849
        && fabs(elec->deta_sc_tk_at_vtx())  <0.011932
        && fabs(elec->dphi_sc_tk_at_vtx())  <0.255450
        && elec->hadronic_over_em()         <0.223870
        && ooemoop                          <0.155501
        && elec->gsf_tk_nhits()             <=3
    //    && fabs(elec->dxy_vertex())         <0.273097
     //   && fabs(elec->dz_vertex())          <0.885860
   //     && dbiso/elec->pt()                 <0.212604
        ) )
        );

   } 

  
  bool VetoElectronIDSpring15(Electron const* elec){//function for spring15 veto 25ns
    bool in_barrel = true;
    if (fabs(elec->sc_eta()) > 1.479) in_barrel = false;
    
    double ooemoop = fabs((1.0/elec->ecal_energy() - elec->sc_e_over_p()/elec->ecal_energy()));
//    double dbiso = elec->dr03_pfiso_charged() + std::max(0., elec->dr03_pfiso_neutral()+elec->dr03_pfiso_gamma() - 0.5*elec->dr03_pfiso_pu());
    
    return(
       !elec->has_matched_conversion()
        && ( (in_barrel       
	&& elec->full5x5_sigma_IetaIeta()   <0.0114
        && fabs(elec->deta_sc_tk_at_vtx())  <0.0152
        && fabs(elec->dphi_sc_tk_at_vtx())  <0.216
        && elec->hadronic_over_em()         <0.181
        && ooemoop                          <0.207
        && elec->gsf_tk_nhits()             <=2
     //   && fabs(elec->dxy_vertex())         <0.060279
	      //  && fabs(elec->dz_vertex())          <0.800538
	      //  && dbiso/elec->pt()                 <0.164369
        ) ||
        (!in_barrel       
        && elec->full5x5_sigma_IetaIeta()   <0.0352
        && fabs(elec->deta_sc_tk_at_vtx())  <0.0113
        && fabs(elec->dphi_sc_tk_at_vtx())  <0.237
        && elec->hadronic_over_em()         <0.116
        && ooemoop                          <0.174
        && elec->gsf_tk_nhits()             <=3
    //    && fabs(elec->dxy_vertex())         <0.273097
     //   && fabs(elec->dz_vertex())          <0.885860
   //     && dbiso/elec->pt()                 <0.212604
        ) )
        );

   } 


  bool VetoElectronID(Electron const* elec) {
    bool in_barrel = true;
    if (fabs(elec->sc_eta()) > 1.4442) in_barrel = false;

    double ooemoop = fabs((1.0/elec->ecal_energy() - elec->sc_e_over_p()/elec->ecal_energy()));

    return ( elec->gsf_tk_nhits()            <= 999
       //&& !elec->has_matched_conversion()
       && ( (in_barrel 
       && elec->sigma_IetaIeta()           < 0.01
       && fabs(elec->dphi_sc_tk_at_vtx())  < 0.800
       && fabs(elec->deta_sc_tk_at_vtx())  < 0.007
       && elec->hadronic_over_em() < 0.150
       && ooemoop < 999.9
       ) || 
      (!in_barrel 
       && elec->sigma_IetaIeta()           < 0.03
       && fabs(elec->dphi_sc_tk_at_vtx())  < 0.700
       && fabs(elec->deta_sc_tk_at_vtx())  < 0.010
       && elec->hadronic_over_em() < 999.9
       && ooemoop < 999.9
       )
      )
       );
  }

  bool Electron2011WP95ID(Electron const* elec) {
    //if (elec->has_matched_conversion()) return false;
    bool in_barrel = true;
    if (fabs(elec->sc_eta()) > 1.479) in_barrel = false;
//    if (fabs(elec->sc_eta()) > 1.4442) in_barrel = false;

    return ( (in_barrel 
              && elec->sigma_IetaIeta()           < 0.01
              && elec->hadronic_over_em()         < 0.15
              && fabs(elec->dphi_sc_tk_at_vtx())  < 0.8
              && fabs(elec->deta_sc_tk_at_vtx())  < 0.007
              /*&& elec->gsf_tk_nhits()            == 0*/)
          || (!in_barrel 
              && elec->sigma_IetaIeta()           < 0.03
              && fabs(elec->dphi_sc_tk_at_vtx())  < 0.7
              && fabs(elec->deta_sc_tk_at_vtx())  < 0.01
              /*&& elec->gsf_tk_nhits()            == 0*/));
  }


  bool ElectronSimpleWP85ID(Electron const* elec) {

    bool in_barrel = true;
    if (fabs(elec->sc_eta()) > 1.4442) in_barrel = false;

    return ( (in_barrel 
              && elec->sigma_IetaIeta()           < 0.01
              && fabs(elec->dphi_sc_tk_at_vtx())  < 0.06
              && fabs(elec->deta_sc_tk_at_vtx())  < 0.006
              && elec->hadronic_over_em()         < 0.04
              && elec->gsf_tk_nhits()            <= 1
              && (fabs(elec->conv_dist()) >= 0.02 || fabs(elec->conv_dcot()) >= 0.02))
          || (!in_barrel 
              && elec->sigma_IetaIeta()           < 0.03
              && fabs(elec->dphi_sc_tk_at_vtx())  < 0.04
              && fabs(elec->deta_sc_tk_at_vtx())  < 0.007
              && elec->hadronic_over_em()         < 0.025
              && elec->gsf_tk_nhits()            <= 1
              && (fabs(elec->conv_dist()) >= 0.02 || fabs(elec->conv_dcot()) >= 0.02)));
  }


  bool ElectronHTTIdIso(Electron const* elec, unsigned const& mode) {
    //Do some cut-based pre-selection
    if (elec->has_matched_conversion()) return false;
    if (elec->gsf_tk_nhits() > 0) return false;
    bool pass_mva = false;
    double pt = elec->pt();
    double eta = fabs(elec->sc_eta());
    double idmva = elec->GetIdIso("mvaNonTrigV0");
    if (pt <= 20.0 && eta <= 0.8                  && idmva > 0.925) pass_mva = true;
    if (pt <= 20.0 && eta >  0.8 && eta <= 1.479  && idmva > 0.915) pass_mva = true;
    if (pt <= 20.0 && eta >  1.479                && idmva > 0.965) pass_mva = true;
    if (mode != 2) {
      if (pt >  20.0 && eta <= 0.8                  && idmva > 0.925) pass_mva = true;
      if (pt >  20.0 && eta >  0.8 && eta <= 1.479  && idmva > 0.975) pass_mva = true;
      if (pt >  20.0 && eta >  1.479                && idmva > 0.985) pass_mva = true;
    } else {
      if (pt >  20.0 && eta <= 0.8                  && idmva > 0.905) pass_mva = true;
      if (pt >  20.0 && eta >  0.8 && eta <= 1.479  && idmva > 0.955) pass_mva = true;
      if (pt >  20.0 && eta >  1.479                && idmva > 0.975) pass_mva = true;
    }
    return pass_mva;
  }

  bool ElectronHTTVHID(Electron const* elec) {
    //Do some cut-based pre-selection
    if (elec->has_matched_conversion()) return false;
    if (elec->gsf_tk_nhits() > 0) return false;
    bool pass_mva = false;
    double pt = elec->pt();
    double eta = fabs(elec->sc_eta());
    double idmva = elec->GetIdIso("mvaNonTrigV0");
    if (pt >= 10.0 && eta <= 0.8                  && idmva > 0.5) pass_mva = true;
    if (pt >= 10.0 && eta >  0.8 && eta <= 1.479  && idmva > 0.12) pass_mva = true;
    if (pt >= 10.0 && eta >  1.479                && idmva > 0.6) pass_mva = true;
    return pass_mva;
  }

  bool ElectronHTTId(Electron const* elec, bool loose_wp) {
    //Do some cut-based pre-selection
    if (elec->has_matched_conversion()) return false;
    if (elec->gsf_tk_nhits() > 0) return false;
    bool pass_mva = false;
    double pt = elec->pt();
    double eta = fabs(elec->sc_eta());
    double idmva = elec->GetIdIso("mvaNonTrigV0");
    if (pt <= 20.0 && eta <= 0.8                  && idmva > 0.925) pass_mva = true;
    if (pt <= 20.0 && eta >  0.8 && eta <= 1.479  && idmva > 0.915) pass_mva = true;
    if (pt <= 20.0 && eta >  1.479                && idmva > 0.965) pass_mva = true;
    if (!loose_wp) {
      if (pt >  20.0 && eta <= 0.8                  && idmva > 0.925) pass_mva = true;
      if (pt >  20.0 && eta >  0.8 && eta <= 1.479  && idmva > 0.975) pass_mva = true;
      if (pt >  20.0 && eta >  1.479                && idmva > 0.985) pass_mva = true;
    } else {
      if (pt >  20.0 && eta <= 0.8                  && idmva > 0.905) pass_mva = true;
      if (pt >  20.0 && eta >  0.8 && eta <= 1.479  && idmva > 0.955) pass_mva = true;
      if (pt >  20.0 && eta >  1.479                && idmva > 0.975) pass_mva = true;
    }
    return pass_mva;
  }
  
  bool ElectronHTTIdPhys14(Electron const* elec, bool loose_wp) {
    //Do some cut-based pre-selection
    if (elec->has_matched_conversion()) return false;
    if (elec->gsf_tk_nhits() > 0) return false;
    bool pass_mva = false;
    double eta = fabs(elec->sc_eta());
    double idmva = elec->GetIdIso("mvaNonTrigV025nsPHYS14");
    if (!loose_wp) {
      if (eta <= 0.8                  && idmva > 0.73) pass_mva = true;
      if (eta >  0.8 && eta <= 1.479  && idmva > 0.57) pass_mva = true;
      if (eta >  1.479                && idmva > 0.05) pass_mva = true;
    } else {
      if (eta <= 0.8                  && idmva > 0.35) pass_mva = true;
      if (eta >  0.8 && eta <= 1.479  && idmva > 0.20) pass_mva = true;
      if (eta >  1.479                && idmva > -0.52) pass_mva = true;
    }
    return pass_mva;
  }

  bool ElectronHTTIdSpring15(Electron const* elec, bool loose_wp) {
    //Do some cut-based pre-selection
    if (elec->has_matched_conversion()) return false;
    if (elec->gsf_tk_nhits() > 1) return false;
    bool pass_mva = false;
    double eta = fabs(elec->sc_eta());
    double pt = fabs(elec->pt());
    double idmva = elec->GetIdIso("mvaNonTrigSpring15");
    if (!loose_wp) {
      if (eta <= 0.8 && pt <= 10                  && idmva > 0.287435) pass_mva = true;
      if (eta >  0.8 && eta <= 1.479 && pt <=10   && idmva > 0.221846) pass_mva = true;
      if (eta >  1.479 && pt <= 10                && idmva > -0.303263) pass_mva = true;
      if (eta <= 0.8 && pt > 10                   && idmva > 0.967083) pass_mva = true;
      if (eta >  0.8 && eta <= 1.479 && pt > 10   && idmva > 0.929117) pass_mva = true;
      if (eta >  1.479 && pt > 10                 && idmva > 0.726311) pass_mva = true;

    } else {
      if (eta <= 0.8 && pt <= 10                  && idmva > -0.083313) pass_mva = true;
      if (eta >  0.8 && eta <= 1.479 && pt <=10   && idmva > -0.235222) pass_mva = true;
      if (eta >  1.479 && pt <= 10                && idmva > -0.67099) pass_mva = true;
      if (eta <= 0.8 && pt > 10                   && idmva > 0.913286) pass_mva = true;
      if (eta >  0.8 && eta <= 1.479 && pt > 10   && idmva > 0.805013) pass_mva = true;
      if (eta >  1.479 && pt > 10                 && idmva > 0.358969) pass_mva = true;
    }
    return pass_mva;
  }


  bool ElectronHTTTrigNoIPId(Electron const* elec, bool loose_wp) {
    //Do some cut-based pre-selection
    if (elec->has_matched_conversion()) return false;
    if (elec->gsf_tk_nhits() > 0) return false;
    bool pass_mva = false;
    double pt = elec->pt();
    double eta = fabs(elec->sc_eta());
    double idmva = elec->GetIdIso("mvaTrigNoIPV0");
    if (pt <= 20.0 && eta <= 0.8                  && idmva > -0.3500) pass_mva = true;
    if (pt <= 20.0 && eta >  0.8 && eta <= 1.479  && idmva >  0.0000) pass_mva = true;
    if (pt <= 20.0 && eta >  1.479                && idmva >  0.0250) pass_mva = true;
    if (!loose_wp) {
      if (pt >  20.0 && eta <= 0.8                  && idmva > 0.7875) pass_mva = true;
      if (pt >  20.0 && eta >  0.8 && eta <= 1.479  && idmva > 0.9500) pass_mva = true;
      if (pt >  20.0 && eta >  1.479                && idmva > 0.9250) pass_mva = true;
    } else {
      if (pt >  20.0 && eta <= 0.8                  && idmva > 0.7000) pass_mva = true;
      if (pt >  20.0 && eta >  0.8 && eta <= 1.479  && idmva > 0.9000) pass_mva = true;
      if (pt >  20.0 && eta >  1.479                && idmva > 0.8375) pass_mva = true;
    }
    return pass_mva;
  }

  bool HttEMuFakeElectron(Electron const* elec) {
    if (elec->has_matched_conversion()) return false; // ConversionTools::hasMatchedConversion(...)
    bool in_barrel = fabs(elec->sc_eta()) < 1.479;
    if (elec->gsf_tk_nhits() > 0) return false;
    bool pass_iso = false;
    if (in_barrel) {
      pass_iso =        ((elec->dr03_tk_sum_pt() / elec->pt())                                < 0.2) // dr03TkSumPt()
                    &&  ((TMath::Max(elec->dr03_ecal_rechit_sum_et()-1.0, 0.0) / elec->pt())  < 0.2) // dr03EcalRecHitSumEt()
                    &&  ((elec->dr03_hcal_tower_sum_et() / elec->pt())                        < 0.2); // dr03HcalTowerSumEt()
    } else {
      pass_iso =        ((elec->dr03_tk_sum_pt() / elec->pt())          < 0.2) 
                    &&  ((elec->dr03_ecal_rechit_sum_et() / elec->pt()) < 0.2)
                    &&  ((elec->dr03_hcal_tower_sum_et() / elec->pt())  < 0.2);
    }
   if (!pass_iso) return false;     
   bool pass_id = ( 
            fabs(elec->dxy_vertex()) < 0.02 &&
            fabs(elec->dz_vertex()) < 0.1 &&
            ((in_barrel 
             && elec->sigma_IetaIeta()           < 0.01   // sigmaIetaIeta()
             && fabs(elec->dphi_sc_tk_at_vtx())  < 0.15   // deltaPhiSuperClusterTrackAtVtx()
             && fabs(elec->deta_sc_tk_at_vtx())  < 0.007
             && elec->hadronic_over_em()         < 0.12) // deltaEtaSuperClusterTrackAtVtx()
         || (!in_barrel 
             && elec->sigma_IetaIeta()           < 0.03
             && fabs(elec->dphi_sc_tk_at_vtx())  < 0.1
             && fabs(elec->deta_sc_tk_at_vtx())  < 0.009
             && elec->hadronic_over_em()         < 0.10)));
   return pass_id && !(ElectronHTTId(elec, true) && PF04IsolationEB(elec, 0.5, 0.15, 0.1) && fabs(elec->dxy_vertex()) < 0.02 && fabs(elec->dz_vertex()) < 0.1 ); // !(usual MVA ID && pf isolation < 0.15(0.1) barrel(endcap))
  }

  

  bool PF04IsolationEBElec(Electron const* cand, double const& dbeta, double const& cut_barrel, double const& cut_endcap) {
    double iso =  PF04IsolationVal(cand, dbeta);
    if (fabs(cand->sc_eta()) < 1.479) {
      return (iso < cut_barrel);
    } else {
      return (iso < cut_endcap);
    }
  }


  bool ElectronHTT2011IdIso(Electron const* elec) {
    //Do some cut-based pre-selection
    if (elec->has_matched_conversion()) return false;
    if (elec->gsf_tk_nhits() > 0) return false;
    bool pass_preid = false;
    if(fabs(elec->sc_eta()) < 1.479) {
      if(elec->sigma_IetaIeta()                       < 0.01 
        && elec->hadronic_over_em()                   < 0.12 
        && fabs(elec->deta_sc_tk_at_vtx())            < 0.007 
        && fabs(elec->dphi_sc_tk_at_vtx())            < 0.15 
        && elec->gsf_tk_nhits() == 0)
        pass_preid = true;
    }
    else {
      if(elec->sigma_IetaIeta()                       < 0.03 
        && elec->hadronic_over_em()                   < 0.10 
        && fabs(elec->deta_sc_tk_at_vtx())            < 0.009 
        && fabs(elec->dphi_sc_tk_at_vtx())            < 0.10
        && elec->gsf_tk_nhits() == 0)
        pass_preid = true;
    }
    if (!pass_preid) return false;
    bool pass_mva = false;
    double pt = elec->pt();
    double eta = fabs(elec->sc_eta());
    double idmva = elec->GetIdIso("id2011mva");
    if (pt <= 20.0 && eta <= 1.0                  && idmva > 0.133) pass_mva = true;
    if (pt <= 20.0 && eta >  1.0 && eta <= 1.5    && idmva > 0.465) pass_mva = true;
    if (pt <= 20.0 && eta >  1.5                  && idmva > 0.518) pass_mva = true;
    if (pt >  20.0 && eta <= 1.0                  && idmva > 0.942) pass_mva = true;
    if (pt >  20.0 && eta >  1.0 && eta <= 1.5    && idmva > 0.947) pass_mva = true;
    if (pt >  20.0 && eta >  1.5                  && idmva > 0.878) pass_mva = true;
    return pass_mva;
  }

  //Photon IDs taken from https://twiki.cern.ch/twiki/bin/view/CMS/CutBasedPhotonIdentificationRun2
  bool TightPhotonIDSpring15(Photon const* photon,double const& rho) {//function for spring15 ID
    double eta = fabs(photon->eta());

    bool in_barrel = true;
    if (eta > 1.479) in_barrel = false;
    bool in_endcap = true;
    if (eta < 1.479) in_endcap = false;

    double photon_area, neutral_area, charged_area;
    if (eta < 1.0)                  { charged_area=0.0158; neutral_area=0.0143; photon_area=0.0725; }
    if (eta >= 1.0 && eta < 1.479)  { charged_area=0.0143; neutral_area=0.0210; photon_area=0.0604; }
    if (eta >= 1.479 && eta < 2.0)  { charged_area=0.0115; neutral_area=0.0147; photon_area=0.0320; }
    if (eta >= 2.0 && eta < 2.2)    { charged_area=0.0094; neutral_area=0.0082; photon_area=0.0512; }
    if (eta >= 2.2 && eta < 2.3)    { charged_area=0.0095; neutral_area=0.0124; photon_area=0.0766; }
    if (eta >= 2.3 && eta < 2.4)    { charged_area=0.0068; neutral_area=0.0186; photon_area=0.0949; }
    if (eta >= 2.4)                 { charged_area=0.0053; neutral_area=0.0320; photon_area=0.1160; }

    return(
	( (in_barrel       
	   && photon->had_tower_over_em()<0.05
	   && photon->sigma_IetaIeta()<0.01
	   && std::max(photon->dr03_pfiso_charged()-rho*charged_area,0.)<0.91
	   && std::max(photon->dr03_pfiso_neutral()-rho*neutral_area,0.)<(0.33+exp(0.0044*photon->pt()+0.5809))
           && std::max(photon->dr03_pfiso_gamma()-rho*photon_area,0.)<(0.61+0.0043*photon->pt())
	   ) ||
	  (in_endcap       
	   && photon->had_tower_over_em()<0.05
	   && photon->sigma_IetaIeta()<0.0267
	   && std::max(photon->dr03_pfiso_charged()-rho*charged_area,0.)<0.65
	   && std::max(photon->dr03_pfiso_neutral()-rho*neutral_area,0.)<(0.93+exp(0.004*photon->pt()+0.9402))
	   && std::max(photon->dr03_pfiso_gamma()-rho*photon_area,0.)<(0.54+0.0041*photon->pt())
	   ) )
        );

  }

  bool MediumPhotonIDSpring15(Photon const* photon,double const& rho) {//function for spring15 ID
    double eta = fabs(photon->eta());
    bool in_barrel = true;
    if (eta > 1.479) in_barrel = false;
    bool in_endcap = true;
    if (eta < 1.479) in_endcap = false;

    double photon_area, neutral_area, charged_area;
    if (eta < 1.0)                  { charged_area=0.0158; neutral_area=0.0143; photon_area=0.0725; }
    if (eta >= 1.0 && eta < 1.479)  { charged_area=0.0143; neutral_area=0.0210; photon_area=0.0604; }
    if (eta >= 1.479 && eta < 2.0)  { charged_area=0.0115; neutral_area=0.0147; photon_area=0.0320; }
    if (eta >= 2.0 && eta < 2.2)    { charged_area=0.0094; neutral_area=0.0082; photon_area=0.0512; }
    if (eta >= 2.2 && eta < 2.3)    { charged_area=0.0095; neutral_area=0.0124; photon_area=0.0766; }
    if (eta >= 2.3 && eta < 2.4)    { charged_area=0.0068; neutral_area=0.0186; photon_area=0.0949; }
    if (eta >= 2.4)                 { charged_area=0.0053; neutral_area=0.0320; photon_area=0.1160; }
    
    return(
	( (in_barrel       
	   && photon->had_tower_over_em()<0.05
	   && photon->sigma_IetaIeta()<0.01
	   && std::max(photon->dr03_pfiso_charged()-rho*charged_area,0.)<1.31
	   && std::max(photon->dr03_pfiso_neutral()-rho*neutral_area,0.)<(0.6+exp(0.0044*photon->pt()+0.5809))
           && std::max(photon->dr03_pfiso_gamma()-rho*photon_area,0.)<(1.33+0.0043*photon->pt())
	   ) ||
	  (in_endcap       
	   && photon->had_tower_over_em()<0.05
	   && photon->sigma_IetaIeta()<0.0267
	   && std::max(photon->dr03_pfiso_charged()-rho*charged_area,0.)<1.25
	   && std::max(photon->dr03_pfiso_neutral()-rho*neutral_area,0.)<(1.65+exp(0.004*photon->pt()+0.9402))
	   && std::max(photon->dr03_pfiso_gamma()-rho*photon_area,0.)<(1.02+0.0041*photon->pt())
	   ) )
	   );
    
  }
  

  bool LoosePhotonIDSpring15(Photon const* photon,double const& rho) {//function for spring15 ID
    double eta = fabs(photon->eta());
    bool in_barrel = true;
    if (eta > 1.479) in_barrel = false;
    bool in_endcap = true;
    if (eta < 1.479) in_endcap = false;
    
    double photon_area, neutral_area, charged_area;
    if (eta < 1.0)                  { charged_area=0.0158; neutral_area=0.0143; photon_area=0.0725; }
    if (eta >= 1.0 && eta < 1.479)  { charged_area=0.0143; neutral_area=0.0210; photon_area=0.0604; }
    if (eta >= 1.479 && eta < 2.0)  { charged_area=0.0115; neutral_area=0.0147; photon_area=0.0320; }
    if (eta >= 2.0 && eta < 2.2)    { charged_area=0.0094; neutral_area=0.0082; photon_area=0.0512; }
    if (eta >= 2.2 && eta < 2.3)    { charged_area=0.0095; neutral_area=0.0124; photon_area=0.0766; }
    if (eta >= 2.3 && eta < 2.4)    { charged_area=0.0068; neutral_area=0.0186; photon_area=0.0949; }
    if (eta >= 2.4)                 { charged_area=0.0053; neutral_area=0.0320; photon_area=0.1160; }

    return(
	( (in_barrel       
	   && photon->had_tower_over_em()<0.05
	   && photon->sigma_IetaIeta()<0.0103
	   && std::max(photon->dr03_pfiso_charged()-rho*charged_area,0.)<2.44
	   && std::max(photon->dr03_pfiso_neutral()-rho*neutral_area,0.)<(2.57+exp(0.0044*photon->pt()+0.5809))
           && std::max(photon->dr03_pfiso_gamma()-rho*photon_area,0.)<(1.92+0.0043*photon->pt())
	      ) ||
        (in_endcap       
	 && photon->had_tower_over_em()<0.05
	 && photon->sigma_IetaIeta()<0.0277
	 && std::max(photon->dr03_pfiso_charged()-rho*charged_area,0.)<1.84
	 && std::max(photon->dr03_pfiso_neutral()-rho*neutral_area,0.)<(4+exp(0.004*photon->pt()+0.9402))
	 && std::max(photon->dr03_pfiso_gamma()-rho*photon_area,0.)<(2.15+0.0041*photon->pt())
        ) )
        );

  }


  bool PairOneWithPt(CompositeCandidate const* cand, double const& ptMin) {
    std::vector<Candidate*> const& vec = cand->AsVector();
    for (unsigned i = 0; i < vec.size(); ++i) {
      if (vec[i]->pt() > ptMin) return true;
    }
    return false;
  }
  
  bool PairPtSelection(CompositeCandidate const* cand, double const& jetpt1, double const& jetpt2){
    if (cand->At(0)->pt()>jetpt1 && cand->At(1)->pt()>jetpt2) return true;
    return false;
  }

  bool OrderedPairPtSelection(CompositeCandidate const* cand, double const& jetpt1, double const& jetpt2, bool const& aboveorbelow){
    double at0pt=cand->At(0)->pt();
    double at1pt=cand->At(1)->pt();
    if(at0pt>at1pt){
      if(aboveorbelow){
  if (at0pt>jetpt1 && at1pt>jetpt2) return true;
      }
      else{
  if (at0pt<jetpt1 && at1pt<jetpt2) return true;
      }
      return false;
    }
    else{
      if(aboveorbelow){
  if (at1pt>jetpt1 && at0pt>jetpt2) return true;
      }
      else{
  if (at1pt<jetpt1 && at0pt<jetpt2) return true;
      }
      return false;
    }
  }

  bool PairMassInRange(CompositeCandidate const* cand, double const& mLow, double const& mHigh) {
    double mass = (cand->At(0)->vector() + cand->At(1)->vector()).M();
    return (mass > mLow && mass < mHigh);
  }
  
  double PairMass(CompositeCandidate const* cand) {
    double mass = (cand->At(0)->vector() + cand->At(1)->vector()).M();
    return (mass);
  }
  //Return abolute difference between mass of candidate pair and given mass (e.g. Higgs), for sorting by closest candidate pair
  double PairMassDiff(CompositeCandidate const* cand, double const& mass) {
    double pairmass = (cand->At(0)->vector() + cand->At(1)->vector()).M();
    return abs(pairmass-mass);
  }

  bool PairEtaProdLessThan(CompositeCandidate const* cand, double const& max){
    if ((cand->At(0)->eta() * cand->At(1)->eta()) < max) return true;
    return false;
  }

  bool PairDEtaLessThan(CompositeCandidate const* cand, double const& max){
    if (fabs(cand->At(0)->eta() - cand->At(1)->eta()) < max) return true;
    return false;
  }

  bool PairDPhiLessThan(CompositeCandidate const* cand, double const& max){
    if (ROOT::Math::VectorUtil::DeltaPhi(cand->At(0)->vector(),cand->At(1)->vector()) < max) return true;
    return false;
  }

  bool PairAbsDPhiLessThan(CompositeCandidate const* cand, double const& max){
    if (fabs(ROOT::Math::VectorUtil::DeltaPhi(cand->At(0)->vector(),cand->At(1)->vector())) < max) return true;
    return false;
  }

  bool PairOppCharge(CompositeCandidate const& cand) {
    int charge = (cand.At(0)->charge() * cand.At(1)->charge());
    return (charge == -1);
  }

  bool PairOppSign(CompositeCandidate const* cand) {
    int charge = (cand->At(0)->charge() * cand->At(1)->charge());
    return (charge == -1 && abs(cand->At(0)->charge()) == 1 && abs(cand->At(1)->charge()) == 1);
  }

  bool PairSameSign(CompositeCandidate const* cand) {
    int charge = (cand->At(0)->charge() * cand->At(1)->charge());
    return (charge == 1 && abs(cand->At(0)->charge()) == 1 && abs(cand->At(1)->charge()) == 1);
  }

  bool MuonTight(Muon const* muon) {
    bool tightCut = ( 
        muon->is_global() && 
        //muon->is_tracker() &&
        //muon->it_tracker_hits() > 10 &&
        muon->it_pixel_hits() > 0 &&
        muon->gt_valid_muon_hits() > 0 &&
        muon->it_layers_with_measurement() > 5 &&
        muon->gt_normalized_chi2() < 10 &&
        muon->matched_stations() > 1
        );
    return tightCut;
  }

  //Defined in https://twiki.cern.ch/twiki/bin/view/CMS/SWGuideMuonId2015
  bool MuonMedium(Muon const* muon) {
    bool goodGlob = muon->is_global() && 
      muon->gt_normalized_chi2() < 3 &&
      muon->cq_chi2_localposition()<12 &&
      muon->cq_trk_kink()<20;

    bool isMedium =
      muon->is_pf() &&
      (muon->is_global()||muon->is_tracker()) && //Require loose muon except pf isolation should be done in individual analyses
      muon->it_valid_fraction() > 0.8 && 
      muon->segment_compatibility() > (goodGlob ? 0.303 : 0.451); 
    return isMedium;
  }

  bool MuonLoose(Muon const* muon) {
    bool isLoose = 
      muon->is_pf() &&
      (muon->is_global()||muon->is_tracker());
    return isLoose;
  }

  bool HttEMuFakeMuon(Muon const* muon) {
    bool pass_iso = false;
    if ( muon->pt() > 20. &&
         (muon->dr03_tk_sum_pt()/muon->pt())          < 0.4 &&
         (muon->dr03_ecal_rechit_sum_et()/muon->pt()) < 0.4 &&
         (muon->dr03_hcal_tower_sum_et()/muon->pt())  < 0.4 ) pass_iso = true;
    if (  muon->pt() <= 20. &&
          muon->dr03_tk_sum_pt()          < 8. &&
          muon->dr03_ecal_rechit_sum_et() < 8. &&
          muon->dr03_hcal_tower_sum_et()  < 8.) pass_iso = true;
    bool pass = ( 
        muon->is_global() &&  // isGlobalMuon()
        // muon->gt_valid_muon_hits() > 0 &&  // globalTrack()->hitPattern().numberOfValidMuonHits()
        // muon->gt_normalized_chi2() < 10 && // globalTrack()->normalizedChi2()
        // muon->matched_stations() > 1 && // numberOfMatchedStations()
        fabs(muon->dxy_vertex()) < 0.2 &&
        pass_iso
        );
    return (pass && !( MuonTight(muon) && PF04IsolationEB(muon, 0.5, 0.15, 0.1) && fabs(muon->dxy_vertex()) < 0.02 && fabs(muon->dz_vertex()) < 0.1 )); // !(usual tight muon && pf iso < 0.15(0.1) barrel(endcap))
  }

  bool HttEMuFakeMuon2011(Muon const* muon) {
    bool pass_iso = false;
    if ( (muon->dr03_tk_sum_pt()/muon->pt())          < 0.2 &&
         (muon->dr03_ecal_rechit_sum_et()/muon->pt()) < 0.2 &&
         (muon->dr03_hcal_tower_sum_et()/muon->pt())  < 0.2 ) pass_iso = true;
    
    bool pass = ( 
        muon->is_global() &&  // isGlobalMuon()
        muon->gt_valid_muon_hits() > 0 &&  // globalTrack()->hitPattern().numberOfValidMuonHits()
        muon->gt_normalized_chi2() < 10 && // globalTrack()->normalizedChi2()
        muon->matched_stations() > 1 && // numberOfMatchedStations()
        fabs(muon->dxy_vertex()) < 0.2 &&
        fabs(muon->dz_vertex()) < 0.1 &&
        pass_iso
        );
    return (pass && !( MuonTight(muon) && PF04IsolationEB(muon, 0.5, 0.15, 0.1) && fabs(muon->dxy_vertex()) < 0.02 && fabs(muon->dz_vertex()) < 0.1 )); // !(usual tight muon && pf iso < 0.15(0.1) barrel(endcap))
  }


  bool MuonIso(Muon const* muon) {
    bool isoCut = (
        ((muon->dr03_tk_sum_pt() + 
          muon->dr03_ecal_rechit_sum_et() + 
          muon->dr03_hcal_tower_sum_et()) 
	 / muon->pt()) < 0.15);
    return isoCut;
  }

  bool MuonTkIso(Muon const* muon) {
    bool isoCut = (((muon->dr03_tk_sum_pt())/muon->pt())<0.1); 
    return isoCut;
  }
  
  std::vector<Track *> GetTracksAtVertex(std::vector<Track *> const& trks, std::vector<Vertex *> const& vtxs, unsigned idx, double const& dz) {
    std::vector<Track *> result;
    std::vector<std::size_t> vtx_tracks = ExtractFirst(vtxs.at(idx)->tracks());
    for (unsigned i = 0; i < trks.size(); ++i) {
      //Is track in vtx fit -> keep
      if (std::find(vtx_tracks.begin(),vtx_tracks.end(),trks[i]->id()) != vtx_tracks.end()) {
        result.push_back(trks[i]);
        continue;
      }
      //If not, is track within dz and is it the shortest distance?
      double dist = fabs(trks[i]->vz() - vtxs.at(idx)->vz());
      if (dist < dz) {
        std::vector<double> dists(vtxs.size(),0.0);
        for (unsigned j = 0; j < vtxs.size(); ++j) {
          dists[j] = fabs(trks[i]->vz() - vtxs.at(j)->vz());
        }
        unsigned min_ele = unsigned(std::min_element(dists.begin(),dists.end()) - dists.begin());
        if (min_ele == idx) result.push_back(trks[i]);
      }
    }
    return result;
  }

  bool MassDiffCompare(Candidate const* p1, Candidate const* p2, 
      double const& mass) {
      double m1 = p1->vector().M();
      double m2 = p2->vector().M();
      double md1 = fabs(m1 - mass);
      double md2 = fabs (m2 - mass);
      return (md1 < md2);
  }

  std::vector<GenParticle *> ExtractStableDaughters(GenParticle * part, std::vector<GenParticle *> const& input) {
    std::vector<GenParticle *> tmp = ExtractDaughters(part, input);
    std::set<GenParticle *> result_set;
    bool all_found = false;
    while (!all_found) {
      std::vector<GenParticle *> tmp_other;
      unsigned n_st1 = 0;
      for (unsigned i = 0; i < tmp.size(); ++i) {
        if (tmp[i]->status() == 1) {
          ++n_st1;
          result_set.insert(tmp[i]);
        } else {
          std::vector<GenParticle *> tmp_daughters = ExtractDaughters(tmp[i], input);
          for(unsigned j = 0; j < tmp_daughters.size(); ++j) tmp_other.push_back(tmp_daughters[j]);
        }
      }
      tmp = tmp_other;
      if (tmp.size() == 0) all_found = true;
    }
    std::vector<GenParticle *> result;
    for (auto it : result_set) result.push_back(it);
    return result;
  }

  std::vector<GenParticle*> ExtractDaughters(
      GenParticle* part, std::vector<GenParticle*> const& input) {
    std::vector<GenParticle*> result;
    std::vector<int> daughters = part->daughters();
    for (unsigned i = 0; i < input.size(); ++i) {
      if (std::find(daughters.begin(), daughters.end(), input[i]->index()) !=
          daughters.end()) {
        result.push_back(input[i]);
      }
    }
    return result;
  }

  std::vector<GenParticle*> ExtractMothers(
      GenParticle* part, std::vector<GenParticle*> const& input) {
    std::vector<GenParticle*> result;
    std::vector<int> mothers = part->mothers();
    for (unsigned i = 0; i < input.size(); ++i) {
      if (std::find(mothers.begin(), mothers.end(), input[i]->index()) !=
          mothers.end()) {
        result.push_back(input[i]);
      }
    }
    return result;
  }


  std::vector<GenParticle*> ExtractDaughtersRecursive(
      GenParticle* part, std::vector<GenParticle*> const& input) {
    std::vector<GenParticle*> tmp = ExtractDaughters(part, input);
    std::set<GenParticle*> result_set;
    bool all_found = false;
    while (!all_found) {
      std::vector<GenParticle*> tmp_other;
      for (unsigned i = 0; i < tmp.size(); ++i) {
        result_set.insert(tmp[i]);
        std::vector<GenParticle*> tmp_daughters =
            ExtractDaughters(tmp[i], input);
        for (unsigned j = 0; j < tmp_daughters.size(); ++j)
          tmp_other.push_back(tmp_daughters[j]);
      }
      tmp = tmp_other;
      if (tmp.size() == 0) all_found = true;
    }
    std::vector<GenParticle*> result;
    for (auto it : result_set) result.push_back(it);
    std::sort(result.begin(), result.end(),
              [](GenParticle const* p1,
                 GenParticle const* p2) { return p1->index() < p2->index();
              });
    return result;
  }

  std::vector<GenJet> BuildTauJets(std::vector<GenParticle *> const& parts, bool include_leptonic) {
    std::vector<GenJet> taus;
    for (unsigned i = 0; i < parts.size(); ++i) {
      if (abs(parts[i]->pdgid()) == 15) {
        std::vector<GenParticle *> daughters = ExtractDaughters(parts[i], parts);
        bool has_tau_daughter = false;
        bool has_lepton_daughter = false;
        for (unsigned j = 0; j < daughters.size(); ++j) {
          if (abs(daughters[j]->pdgid()) == 15) has_tau_daughter = true;
          if (abs(daughters[j]->pdgid()) == 11 || abs(daughters[j]->pdgid()) == 13) has_lepton_daughter = true;
        }
        if (has_tau_daughter) continue;
        if (has_lepton_daughter && !include_leptonic) continue;
        std::vector<GenParticle *> jet_parts = ExtractStableDaughters(parts[i], parts);
        taus.push_back(GenJet());
        ROOT::Math::PtEtaPhiEVector vec;
        std::vector<std::size_t> id_vec;
        for (unsigned k = 0; k < jet_parts.size(); ++k) {
          if (  abs(jet_parts[k]->pdgid()) == 12 || 
                abs(jet_parts[k]->pdgid()) == 14 ||
                abs(jet_parts[k]->pdgid()) == 16 ) continue;
          vec += jet_parts[k]->vector();
          taus.back().set_charge(taus.back().charge() + jet_parts[k]->charge());
          id_vec.push_back(jet_parts[k]->id());
        }
        taus.back().set_vector(vec);
        taus.back().set_constituents(id_vec);
      }
    }
    return taus;
  }

  ROOT::Math::PtEtaPhiEVector reconstructWboson(Candidate const*  lepton, Candidate const* met){


    const double metX = met->vector().px();
    const double metY = met->vector().py();
    ROOT::Math::PxPyPzEVector nuP4;
    nuP4.SetPx(metX);
    nuP4.SetPy(metY);
    double massW = 80.4;
    double bestW = -1000;
    double lowestMassDifference = -1;
    double step = 1;
    for (int i = -500; i<500;++i){

      double currentW = step*i;
      nuP4.SetPz(currentW);
      nuP4.SetE(std::sqrt(metX*metX+metY*metY+currentW*currentW ) );
  
      double M = (nuP4+lepton->vector()).M();
      double diff = std::abs(massW-M);
      //      std::cout << i << " " << diff << std::endl;
      if ( diff < lowestMassDifference  || lowestMassDifference <0 ) {
          lowestMassDifference = diff;
          bestW = currentW;
      }

    }

    if (bestW > -700) {
      nuP4.SetPz(bestW);
      nuP4.SetE(std::sqrt(metX*metX+metY*metY+bestW*bestW ) );
      //std::cout << " NUz=" << bestW << " M_W=" << (nuP4+lepton->vector()).M() <<std::endl;
    } else {
      std::cout << " Cannot reconstruct nu!\n";
      nuP4.SetPz(0);
      nuP4.SetE( std::sqrt(metX*metX+metY*metY) );
      //return;
    }

    ROOT::Math::PtEtaPhiEVector lWreco;
    lWreco = lepton->vector() + nuP4;
    return lWreco;
    
  }

  /*
  

  //---------------------------------------------------------------------------
  //Collection Generators
  //---------------------------------------------------------------------------
  std::vector<GenParticle*> MakeFinalBQuarksCollection(
      std::vector<GenParticle*> const& partVec){
    std::vector<GenParticle*> bquarks;
    std::vector<GenParticle*> bquarksFinal;
    std::vector<int> bquarkIdx;
    BOOST_FOREACH (GenParticle *particle, partVec) {
      if (unsigned(abs(particle->genVars().pdgId)) == 5) {
        bquarks.push_back(particle);
        bquarkIdx.push_back(particle->genVars().index);
      }
    }
    BOOST_FOREACH (GenParticle* bquark, bquarks) {
      bool has_bquark_daughter = false;
      std::vector<unsigned> daughterIdx = bquark->getChildren();
      BOOST_FOREACH (int Idx, daughterIdx) {
        if (std::count(bquarkIdx.begin(),bquarkIdx.end(), Idx) > 0){
          has_bquark_daughter = true;
          break;//No need to keep looping
        }//If this daughter index is in the list of bquark indices
      }//Loop through daughter indices, see if exists in bquarkIdx
      if (!has_bquark_daughter && (bquark->genVars().status != 3)) bquarksFinal.push_back(bquark);
    }//Loop through bquarks
    return bquarksFinal;
  }
  
  std::vector<GenParticle*> MakeInitialBQuarksCollection(
      std::vector<GenParticle*> const& partVec){
    std::vector<GenParticle*> bquarks;
    bool foundP5 = false;
    bool foundM5 = false;
    BOOST_FOREACH (GenParticle *particle, partVec) {
      if (foundP5 && foundM5) break;
      if ((particle->genVars().pdgId) == 5 && !foundP5
          && particle->genVars().status == 2) {
        bquarks.push_back(particle);
        foundP5 = true;
      }
      if ((particle->genVars().pdgId) == -5 && !foundM5
          && particle->genVars().status == 2) {
        bquarks.push_back(particle);
        foundM5 = true;
      }
    }
    return bquarks;
  }
  
  std::vector<GenParticle*> MakeFinalBHadronsCollection(
    std::vector<GenParticle*> const& partVec) {
    std::vector<GenParticle*> bhadrons;
    std::vector<GenParticle*> bhadronsFinal;
    std::vector<int> bhadronIdx;
    BOOST_FOREACH (GenParticle *particle, partVec) {
      if (IsBflavPdgId(particle->genVars().pdgId)) {
        bhadrons.push_back(particle);
        bhadronIdx.push_back(particle->genVars().index);
      }
    }
    BOOST_FOREACH (GenParticle* bhadron, bhadrons) {
      bool has_bhadron_daughter = false;
      std::vector<unsigned> daughterIdx = bhadron->getChildren();
      BOOST_FOREACH (int Idx, daughterIdx) {
        if (std::count(bhadronIdx.begin(),bhadronIdx.end(), Idx) > 0){
          has_bhadron_daughter = true;
          break;//No need to keep looping
        }//If this daughter index is in the list of bhadron indices
      }//Loop through daughter indices, see if exists in bhadronIdx
      if (!has_bhadron_daughter && bhadron->Pt() > 3) bhadronsFinal.push_back(bhadron);
    }//Loop through bhadrons
    return bhadronsFinal;
  }
  
  std::vector<GenParticle*> MakeDaughterCollection(
      std::vector<GenParticle*> const& partVec, unsigned const& pdgid) {
    std::vector<unsigned> dIdx;
    std::vector<GenParticle*> daughters;
    int maxDaughterIdx = 0;
    bool foundMother = false;
    BOOST_FOREACH (GenParticle *particle, partVec) {
      if (foundMother && particle->genVars().index > maxDaughterIdx) break;
      if (foundMother && std::count(dIdx.begin(), dIdx.end(), particle->genVars().index)) {
        daughters.push_back(particle);
      }
      if (!foundMother && unsigned(abs(particle->genVars().pdgId)) == pdgid) {
        dIdx = particle->getChildren();
        if (dIdx.size() == 0) {
          break;
        } else {
          foundMother = true;
          maxDaughterIdx = *std::max_element(dIdx.begin(),dIdx.end());
        }
      }
    }//Iterate through particles
    return daughters;
  }

  std::vector<GenParticle*> MakeDaughterCollectionMCATNLO(
      std::vector<GenParticle*> const& partVec, unsigned const& pdgid) {
    std::vector<GenParticle*> daughters;
    unsigned nFound = 0;
    BOOST_FOREACH (GenParticle *particle, partVec) {
      if (nFound == 2) break;
      if (unsigned(abs(particle->genVars().pdgId)) == pdgid &&
          particle->genVars().status == 3) {
        daughters.push_back(particle);
        ++nFound;
      }
    }//Iterate through particles
    return daughters;
  }

  std::vector<GenParticle*> GetStatus1Muons(
      std::vector<GenParticle*> const& partVec) {
    std::vector<GenParticle*> muons;
    BOOST_FOREACH (GenParticle *particle, partVec) {
      if (particle->genVars().status == 1 && abs(particle->genVars().pdgId) == 13) {
        muons.push_back(particle);
      }
    }//Iterate through particles
    std::vector<GenPGenP> muonPairs = MakePairs(muons);
    std::sort(muonPairs.begin(),muonPairs.end(), 
        boost::bind(MassDiffCompare<GenParticle*,GenParticle*>,_1, _2, 91.19));
    if (muonPairs.size() == 0) muons.clear();
    if (muonPairs.size() > 0) {
      muons.clear();
      muons.push_back(muonPairs.at(0).first);
      muons.push_back(muonPairs.at(0).second);
    }
    return muons;
  }

  bool IsBflavPdgId(int pdgid) {
    unsigned pdgidNoSign = unsigned(abs(pdgid));
    std::string asString = boost::lexical_cast<std::string>(pdgidNoSign);
    if (asString.size() > 1 && asString.at(0) == char('5')) return true;
    return false;
  }

  double ScaleReso(double pT, double eta, double jerUnc){
    unsigned int etaBin = 0;
    if (fabs(eta)> 1.1 && fabs(eta)<= 1.7) etaBin = 1;
    else if (fabs(eta)> 1.7 && fabs(eta)<= 2.3) etaBin = 2;
    else etaBin = 3;

    double N[4] = {-1.18,-1.50,-1.52,-0.34};
    //double Nerr[4] = {0.02,0.10,0.15,0.45};

    double S[4] = {0.41,0.63,0.77,0.57};
    //double Serr[4] = {0.00,0.01,0.03,0.03};
    double M[4] = {0.3521,0.2184,0.0229,0.1447};
    //double Merr[4] = {0.0021,0.0080,0.0181,0.0216};
    double C = 0;
    int signN = static_cast<int>(fabs(N[etaBin])/N[etaBin]);

    double sigPtoverPt_sq = (signN*pow(N[etaBin]/pT,2)+
        pow(S[etaBin],2)*pow(pT,M[etaBin]-1)+
        pow(C,2));
    double sigPtoverPt = sqrt(fabs(sigPtoverPt_sq));

    //std::cout << " -- pT = " << pT << ", sigPtoverPt = " << sigPtoverPt
    //  << " (" <<  sigPtoverPt*pT << ")"
    //  << std::endl;

    double fData = 0.10;

    if (fabs(eta) > 1.5) fData = 0.20;

    return sigPtoverPt*(sqrt(pow(fData,2)+pow(jerUnc,2)))*pT;
  }
  */
} //namespace
