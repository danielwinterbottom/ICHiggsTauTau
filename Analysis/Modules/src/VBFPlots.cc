#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/VBFPlots.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPredicates.h"
#include "TVector3.h"

struct greater_Candidate{
  bool operator() (const ic::Candidate *a,const ic::Candidate *b) {
    return (a->pt() > b->pt());
  }
};

namespace ic {

  VBFPlots::VBFPlots(std::string const& name, fwlite::TFileService *fs, std::string output_name, double JetsMinPt, std::string channel) : ModuleBase(name) {
      
    
    channel_ = channel;
      
    JetsOfflineMinPt = JetsMinPt;
    
    TFileDirectory subDir = fs->mkdir(output_name.c_str());
      
    h_qq_DeltaEta = subDir.make<TH1D>("h_qq_DeltaEta","h_qq_DeltaEta",100, 0,10);
    h_qq_DeltaEta->GetXaxis()->SetTitle("|#Delta#eta_{qq}|");
    h_qq_DeltaEta->GetYaxis()->SetTitle("Entries");
    
    h_qq_Mqq = subDir.make<TH1D>("h_qq_Mqq","h_qq_Mqq",100, 0,2000); 
    h_qq_Mqq->GetXaxis()->SetTitle("m_{qq} [GeV]");
    h_qq_Mqq->GetYaxis()->SetTitle("Entries");

    h_q1_Pt = subDir.make<TH1D>("h_q1_Pt","h_q1_Pt",150, 0,300); 
    h_q1_Pt->GetXaxis()->SetTitle("Leading quark p_{T} [GeV]");
    h_q1_Pt->GetYaxis()->SetTitle("Entries");
    
    h_q2_Pt = subDir.make<TH1D>("h_q2_Pt","h_q2_Pt",150, 0,300); 
    h_q2_Pt->GetXaxis()->SetTitle("Sub-leading quark p_{T} [GeV]");
    h_q2_Pt->GetYaxis()->SetTitle("Entries");
    
    h_qq_AvePt = subDir.make<TH1D>("h_qq_AvePt","h_qq_AvePt",150, 0,300); 
    h_qq_AvePt->GetXaxis()->SetTitle("Quarks <p_{T}>_{qq} [GeV]");
    h_qq_AvePt->GetYaxis()->SetTitle("Entries");
    
    h_genjj_DeltaEta = subDir.make<TH1D>("h_genjj_DeltaEta","h_genjj_DeltaEta",100, 0,10);
    h_genjj_DeltaEta->GetXaxis()->SetTitle("|#Delta#eta_{jj}|");
    h_genjj_DeltaEta->GetYaxis()->SetTitle("Entries");
    
    h_genjj_Mqq = subDir.make<TH1D>("h_genjj_Mqq","h_genjj_Mqq",100, 0,2000); 
    h_genjj_Mqq->GetXaxis()->SetTitle("m_{jj} [GeV]");
    h_genjj_Mqq->GetYaxis()->SetTitle("Entries");

    h_genj1_Pt = subDir.make<TH1D>("h_genj1_Pt","h_genj1_Pt",150, 0,300); 
    h_genj1_Pt->GetXaxis()->SetTitle("Leading jet p_{T} [GeV]");
    h_genj1_Pt->GetYaxis()->SetTitle("Entries");
    
    h_genj2_Pt = subDir.make<TH1D>("h_genj2_Pt","h_genj2_Pt",150, 0,300); 
    h_genj2_Pt->GetXaxis()->SetTitle("Sub-leading jet p_{T} [GeV]");
    h_genj2_Pt->GetYaxis()->SetTitle("Entries");
    
    h_genjj_AvePt = subDir.make<TH1D>("h_genjj_AvePt","h_genjj_AvePt",150, 0,300); 
    h_genjj_AvePt->GetXaxis()->SetTitle("Jets <p_{T}>_{jj} [GeV]");
    h_genjj_AvePt->GetYaxis()->SetTitle("Entries");
    
    h_jj_Mqq = subDir.make<TH1D>("h_jj_Mjj","h_jj_Mjj",100, 0,2000); 
    h_jj_Mqq->GetXaxis()->SetTitle("m_{jj} [GeV]");
    h_jj_Mqq->GetYaxis()->SetTitle("Entries");
    
    h_jj_DeltaEta = subDir.make<TH1D>("h_jj_DeltaEta","h_jj_DeltaEta",100, 0,10);
    h_jj_DeltaEta->GetXaxis()->SetTitle("|#Delta#eta_{jj}|");
    h_jj_DeltaEta->GetYaxis()->SetTitle("Entries");

    h_j1_Pt = subDir.make<TH1D>("h_j1_Pt","h_j1_Pt",150, 0,300); 
    h_j1_Pt->GetXaxis()->SetTitle("Leading jet p_{T} [GeV]");
    h_j1_Pt->GetYaxis()->SetTitle("Entries");
    
    h_j2_Pt = subDir.make<TH1D>("h_j2_Pt","h_j2_Pt",150, 0,300); 
    h_j2_Pt->GetXaxis()->SetTitle("Sub-leading jet p_{T} [GeV]");
    h_j2_Pt->GetYaxis()->SetTitle("Entries");
    
    h_jj_AvePt = subDir.make<TH1D>("h_jj_AvePt","h_jj_AvePt",150, 0,300); 
    h_jj_AvePt->GetXaxis()->SetTitle("Jets <p_{T}>_{jj} [GeV]");
    h_jj_AvePt->GetYaxis()->SetTitle("Entries");
    
    h_l1jj_Mqq = subDir.make<TH1D>("h_l1jj_l1Mjj","h_l1M_Mjj",100, 0,2000); 
    h_l1jj_Mqq->GetXaxis()->SetTitle("m_{jj} [GeV]");
    h_l1jj_Mqq->GetYaxis()->SetTitle("Entries");
    
    h_l1jj_DeltaEta = subDir.make<TH1D>("h_l1jj_DeltaEta","h_l1jj_DeltaEta",100, 0,10);
    h_l1jj_DeltaEta->GetXaxis()->SetTitle("|#Delta#eta_{jj}|");
    h_l1jj_DeltaEta->GetYaxis()->SetTitle("Entries");

    h_l1j1_Pt = subDir.make<TH1D>("h_l1j1_Pt","h_l1j1_Pt",150, 0,300); 
    h_l1j1_Pt->GetXaxis()->SetTitle("Leading jet p_{T} [GeV]");
    h_l1j1_Pt->GetYaxis()->SetTitle("Entries");
    
    h_l1j2_Pt = subDir.make<TH1D>("h_l1j2_Pt","h_l1j2_Pt",150, 0,300); 
    h_l1j2_Pt->GetXaxis()->SetTitle("Sub-leading jet p_{T} [GeV]");
    h_l1j2_Pt->GetYaxis()->SetTitle("Entries");
    
    h_l1jj_AvePt = subDir.make<TH1D>("h_l1jj_AvePt","h_l1jj_AvePt",150, 0,300); 
    h_l1jj_AvePt->GetXaxis()->SetTitle("Jets <p_{T}>_{jj} [GeV]");
    h_l1jj_AvePt->GetYaxis()->SetTitle("Entries");
    
    h_l1jj_VecPt = subDir.make<TH1D>("h_l1jj_VecPt","h_l1jj_VecPt",150, 0,300); 
    h_l1jj_VecPt->GetXaxis()->SetTitle("Di-jet p_{T}_{jj} [GeV]");
    h_l1jj_VecPt->GetYaxis()->SetTitle("Entries");
    
    h_l1jj_VecPt_NonVBF = subDir.make<TH1D>("h_l1jj_VecPt_NonVBF","h_l1jj_VecPt_NonVBF",150, 0,300); 
    h_l1jj_VecPt_NonVBF->GetXaxis()->SetTitle("Di-jet p_{T}_{jj} [GeV]");
    h_l1jj_VecPt_NonVBF->GetYaxis()->SetTitle("Entries");
    
    h_genjj_DeltaEta_NonVBF = subDir.make<TH1D>("h_genjj_DeltaEta_NonVBF","h_genjj_DeltaEta_NonVBF",100, 0,10);
    h_genjj_DeltaEta_NonVBF->GetXaxis()->SetTitle("|#Delta#eta_{jj}|");
    h_genjj_DeltaEta_NonVBF->GetYaxis()->SetTitle("Entries");
    
    h_genjj_Mqq_NonVBF = subDir.make<TH1D>("h_genjj_Mqq_NonVBF","h_genjj_Mqq_NonVBF",100, 0,2000); 
    h_genjj_Mqq_NonVBF->GetXaxis()->SetTitle("m_{jj} [GeV]");
    h_genjj_Mqq_NonVBF->GetYaxis()->SetTitle("Entries");

    h_genj1_Pt_NonVBF = subDir.make<TH1D>("h_genj1_Pt_NonVBF","h_genj1_Pt_NonVBF",150, 0,300); 
    h_genj1_Pt_NonVBF->GetXaxis()->SetTitle("Leading jet p_{T} [GeV]");
    h_genj1_Pt_NonVBF->GetYaxis()->SetTitle("Entries");
    
    h_genj2_Pt_NonVBF = subDir.make<TH1D>("h_genj2_Pt_NonVBF","h_genj2_Pt_NonVBF",150, 0,300); 
    h_genj2_Pt_NonVBF->GetXaxis()->SetTitle("Sub-leading jet p_{T} [GeV]");
    h_genj2_Pt_NonVBF->GetYaxis()->SetTitle("Entries");
    
    h_jj_Mqq_NonVBF = subDir.make<TH1D>("h_jj_Mjj_NonVBF","h_jj_Mjj_NonVBF",100, 0,2000); 
    h_jj_Mqq_NonVBF->GetXaxis()->SetTitle("m_{jj} [GeV]");
    h_jj_Mqq_NonVBF->GetYaxis()->SetTitle("Entries");
    
    h_jj_DeltaEta_NonVBF = subDir.make<TH1D>("h_jj_DeltaEta_NonVBF","h_jj_DeltaEta_NonVBF",100, 0,10);
    h_jj_DeltaEta_NonVBF->GetXaxis()->SetTitle("|#Delta#eta_{jj}|");
    h_jj_DeltaEta_NonVBF->GetYaxis()->SetTitle("Entries");

    h_j1_Pt_NonVBF = subDir.make<TH1D>("h_j1_Pt_NonVBF","h_j1_Pt_NonVBF",150, 0,300); 
    h_j1_Pt_NonVBF->GetXaxis()->SetTitle("Leading jet p_{T} [GeV]");
    h_j1_Pt_NonVBF->GetYaxis()->SetTitle("Entries");
    
    h_j2_Pt_NonVBF = subDir.make<TH1D>("h_j2_Pt_NonVBF","h_j2_Pt_NonVBF",150, 0,300); 
    h_j2_Pt_NonVBF->GetXaxis()->SetTitle("Sub-leading jet p_{T} [GeV]");
    h_j2_Pt_NonVBF->GetYaxis()->SetTitle("Entries");
    
    h_l1jj_Mqq_NonVBF = subDir.make<TH1D>("h_l1jj_l1Mjj_NonVBF","h_l1M_Mjj_NonVBF",100, 0,2000); 
    h_l1jj_Mqq_NonVBF->GetXaxis()->SetTitle("m_{jj} [GeV]");
    h_l1jj_Mqq_NonVBF->GetYaxis()->SetTitle("Entries");
    
    h_l1jj_DeltaEta_NonVBF = subDir.make<TH1D>("h_l1jj_DeltaEta_NonVBF","h_l1jj_DeltaEta_NonVBF",100, 0,10);
    h_l1jj_DeltaEta_NonVBF->GetXaxis()->SetTitle("|#Delta#eta_{jj}|");
    h_l1jj_DeltaEta_NonVBF->GetYaxis()->SetTitle("Entries");

    h_l1j1_Pt_NonVBF = subDir.make<TH1D>("h_l1j1_Pt_NonVBF","h_l1j1_Pt_NonVBF",150, 0,300); 
    h_l1j1_Pt_NonVBF->GetXaxis()->SetTitle("Leading jet p_{T} [GeV]");
    h_l1j1_Pt_NonVBF->GetYaxis()->SetTitle("Entries");
    
    h_l1j2_Pt_NonVBF = subDir.make<TH1D>("h_l1j2_Pt_NonVBF","h_l1j2_Pt_NonVBF",150, 0,300); 
    h_l1j2_Pt_NonVBF->GetXaxis()->SetTitle("Sub-leading jet p_{T} [GeV]");
    h_l1j2_Pt_NonVBF->GetYaxis()->SetTitle("Entries");
    
    h_MjjVsDeltaEta_SOverB = subDir.make<TH2D>("h_MjjVsDeltaEta_SOverB","h_MjjVsDeltaEta_SOverB",100, 0, 10, 100, 0,2000); 
    h_MjjVsDeltaEta_SOverB->GetXaxis()->SetTitle("|#DeltaEta_{jj}|");
    h_MjjVsDeltaEta_SOverB->GetYaxis()->SetTitle("m_{jj} [GeV]");
    
    h_MjjVsDeltaEta_SOverB_S = subDir.make<TH2D>("h_MjjVsDeltaEta_SOverB_S","h_MjjVsDeltaEta_SOverB_S",100, 0, 10, 100, 0,2000); 
    h_MjjVsDeltaEta_SOverB_S->GetXaxis()->SetTitle("|#DeltaEta_{jj}|");
    h_MjjVsDeltaEta_SOverB_S->GetYaxis()->SetTitle("m_{jj} [GeV]");
    
    h_MjjVsDeltaEta_SOverB_B = subDir.make<TH2D>("h_MjjVsDeltaEta_SOverB_B","h_MjjVsDeltaEta_SOverB_B",100, 0, 10, 100, 0,2000); 
    h_MjjVsDeltaEta_SOverB_B->GetXaxis()->SetTitle("|#DeltaEta_{jj}|");
    h_MjjVsDeltaEta_SOverB_B->GetYaxis()->SetTitle("m_{jj} [GeV]");
    
    h_MjjVsDeltaEta_S = subDir.make<TH2D>("h_MjjVsDeltaEta_S","h_MjjVsDeltaEta_S",100, 0, 10, 100, 0,2000); 
    h_MjjVsDeltaEta_S->GetXaxis()->SetTitle("|#DeltaEta_{jj}|");
    h_MjjVsDeltaEta_S->GetYaxis()->SetTitle("m_{jj} [GeV]");
    
    h_MjjVsDeltaEta_B = subDir.make<TH2D>("h_MjjVsDeltaEta_B","h_MjjVsDeltaEta_B",100, 0, 10, 100, 0,2000); 
    h_MjjVsDeltaEta_B->GetXaxis()->SetTitle("|#DeltaEta_{jj}|");
    h_MjjVsDeltaEta_B->GetYaxis()->SetTitle("m_{jj} [GeV]");
    
    h_PercentOppSides = subDir.make<TH1D>("h_PercentOppSides","h_PercentOppSides",2, 0,2); 
    h_PercentOppSides->GetXaxis()->SetTitle("");
    h_PercentOppSides->GetYaxis()->SetTitle("%");
    h_PercentOppSides->GetXaxis()->SetBinLabel(1,"Opposite sides");
    h_PercentOppSides->GetXaxis()->SetBinLabel(2,"Same sides");
    
    h_PercentOppSidesVsSubPt = subDir.make<TH1D>("h_PercentOppSidesVsSubPt","h_PercentOppSidesVsSubPt",100, 0, 200); 
    h_PercentOppSidesVsSubPt->GetXaxis()->SetTitle("Sub-Leading Jet Pt [GeV]");
    h_PercentOppSidesVsSubPt->GetYaxis()->SetTitle("% Opposites side of detector");
    
    h_PercentSameSidesVsSubPt = subDir.make<TH1D>("h_PercentSameSidesVsSubPt","h_PercentSameSidesVsSubPt",100, 0, 200); 
    h_PercentSameSidesVsSubPt->GetXaxis()->SetTitle("Sub-Leading Jet Pt [GeV]");
    h_PercentSameSidesVsSubPt->GetYaxis()->SetTitle("% Same side of detector");
    
    h_TotalDiJetsVsSubPt = subDir.make<TH1D>("h_TotalDiJetsVsSubPt","h_TotalDiJetsVsSubPt",100, 0, 200); 
    h_TotalDiJetsVsSubPt->GetXaxis()->SetTitle("Sub-Leading Jet Pt [GeV]");
    h_TotalDiJetsVsSubPt->GetYaxis()->SetTitle("% Same side of detector");

    h_PtDiff = subDir.make<TH1D>("h_PtDiff","h_PtDiff",100, 0,200); 
    h_PtDiff->GetXaxis()->SetTitle("|Leading - Sub-leading jet p_{T}| [GeV]");
    h_PtDiff->GetYaxis()->SetTitle("Entries");
    
    h_PtDiffVsSubPt = subDir.make<TH2D>("h_PtDiffVsSubPt","h_PtDiffVsSubPt",150,0,300,100, 0,200); 
    h_PtDiffVsSubPt->GetYaxis()->SetTitle("|Leading - Sub-leading jet p_{T}| [GeV]");
    h_PtDiffVsSubPt->GetXaxis()->SetTitle("Sub-Leading jet p_{T} [GeV]");

    h_LeadJetEta = subDir.make<TH1D>("h_LeadJetEta","h_LeadJetEta",100, -10,10); 
    h_LeadJetEta->GetXaxis()->SetTitle("Gen leading jet #eta");
    h_LeadJetEta->GetYaxis()->SetTitle("Entries");
    
    h_SubLeadJetEta = subDir.make<TH1D>("h_SubLeadJetEta","h_SubLeadJetEta",100, -10,10); 
    h_SubLeadJetEta->GetXaxis()->SetTitle("Gen sub-leading jet #eta");
    h_SubLeadJetEta->GetYaxis()->SetTitle("Entries");
    
    h_SubLeadJetPtRes = subDir.make<TH1D>("h_SubLeadJetPtRes","h_SubLeadJetPtRes",100, -2,2); 
    h_SubLeadJetPtRes->GetXaxis()->SetTitle("Sub-Leading jet (gen p_{T}- parton p_{T})/(parton p_{T})");
    h_SubLeadJetPtRes->GetYaxis()->SetTitle("Entries");
    
    h_LeadJetPtRes = subDir.make<TH1D>("h_LeadJetPtRes","h_LeadJetPtRes",100, -2,2); 
    h_LeadJetPtRes->GetXaxis()->SetTitle("Leading jet (gen p_{T}- parton p_{T})/(parton p_{T})");
    h_LeadJetPtRes->GetYaxis()->SetTitle("Entries");
    
    h_LeadJetPtResVsPartonPt = subDir.make<TH2D>("h_LeadJetPtResVsPartonPt","h_LeadJetPtResVsPartonPt",100,0,200,100,-1,1); 
    h_LeadJetPtResVsPartonPt->GetYaxis()->SetTitle("Leading jet (gen p_{T}- parton p_{T})/(parton p_{T})");
    h_LeadJetPtResVsPartonPt->GetXaxis()->SetTitle("Parton p_{T}");
    
    h_SubLeadJetPtResVsPartonPt = subDir.make<TH2D>("h_SubLeadJetPtResVsPartonPt","h_SubLeadJetPtResVsPartonPt",100,0,200,100,-1,1); 
    h_SubLeadJetPtResVsPartonPt->GetYaxis()->SetTitle("Leading jet (gen p_{T}- parton p_{T})/(parton p_{T})");
    h_SubLeadJetPtResVsPartonPt->GetXaxis()->SetTitle("Parton p_{T}");
    
    h_MatchedToGenStats = subDir.make<TH1D>("h_MatchedToGenStats","h_MatchedToGenStats",3, 0,3); 
    h_MatchedToGenStats->GetXaxis()->SetTitle("Number of gen-jets matched to partons");
    h_MatchedToGenStats->GetYaxis()->SetTitle("%");
    h_MatchedToGenStats->GetXaxis()->SetBinLabel(1,"0 matched");
    h_MatchedToGenStats->GetXaxis()->SetBinLabel(2,"1 matched");
    h_MatchedToGenStats->GetXaxis()->SetBinLabel(3,"2 matched");
    
    h_genjets_Index = subDir.make<TH2D>("h_genjets_Index","h_genjets_Index_1",11, -1, 10, 11, -1,10); 
    h_genjets_Index->GetXaxis()->SetTitle("Lead-Jet Index");
    h_genjets_Index->GetYaxis()->SetTitle("Sub-Lead Jet Index");
    
    h_jets_Index = subDir.make<TH2D>("h_jets_Index","h_jets_Index_1",11, -1, 10, 11, -1,10); 
    h_jets_Index->GetXaxis()->SetTitle("Lead-Jet Index");
    h_jets_Index->GetYaxis()->SetTitle("Sub-Lead Jet Index");
    
    h_l1jets_Index = subDir.make<TH2D>("h_l1jets_Index","h_l1jets_Index_1",11, -1, 10, 11, -1,10); 
    h_l1jets_Index->GetXaxis()->SetTitle("Lead-Jet Index");
    h_l1jets_Index->GetYaxis()->SetTitle("Sub-Lead Jet Index");
    
    h_SubLeadJetPtResGenJet = subDir.make<TH1D>("h_SubLeadJetPtResGenJet","h_SubLeadJetPtResGenJet",100, -2,2); 
    h_SubLeadJetPtResGenJet->GetXaxis()->SetTitle("Sub-Leading jet (l1 p_{T}- gen p_{T})/(gen p_{T})");
    h_SubLeadJetPtResGenJet->GetYaxis()->SetTitle("Entries");
    
    h_LeadJetPtResGenJet = subDir.make<TH1D>("h_LeadJetPtResGenJet","h_LeadJetPtResGenJet",100, -2,2); 
    h_LeadJetPtResGenJet->GetXaxis()->SetTitle("Leading jet (l1 p_{T}- gen p_{T})/(gen p_{T})");
    h_LeadJetPtResGenJet->GetYaxis()->SetTitle("Entries");
    
    h_JetsMjjResGenJet = subDir.make<TH1D>("h_JetsMjjResGenJet","h_JetsMjjResGenJet",100, -2,2); 
    h_JetsMjjResGenJet->GetXaxis()->SetTitle("(l1 m_{jj}- gen m_{jj})/(gen m_{jj})");
    h_JetsMjjResGenJet->GetYaxis()->SetTitle("Entries");
    
    h_JetsDeltaEtaResGenJet = subDir.make<TH1D>("h_JetsDeltaEtaResGenJet","h_JetsDeltaEtaResGenJet",100, -0.5,0.5); 
    h_JetsDeltaEtaResGenJet->GetXaxis()->SetTitle("(l1 |#Delta#eta_{jj}| - gen |#Delta#eta_{jj}|)/(gen |#Delta#eta_{jj}|)");
    h_JetsDeltaEtaResGenJet->GetYaxis()->SetTitle("Entries");
    
    h_AveJetPtResGenJet = subDir.make<TH1D>("h_AveJetPtResGenJet","h_AveJetPtResGenJet",100, -2,2); 
    h_AveJetPtResGenJet->GetXaxis()->SetTitle("Jets (l1 <p_{T}>- gen <p_{T}>)/(gen <p_{T}>)");
    h_AveJetPtResGenJet->GetYaxis()->SetTitle("Entries");    
    
    h_EtaJetRes = subDir.make<TH1D>("h_EtaJetRes","h_EtaJetRes",100, -0.5,0.5); 
    h_EtaJetRes->GetXaxis()->SetTitle("Jets (l1 #eta- gen #eta)/(gen #eta)");
    h_EtaJetRes->GetYaxis()->SetTitle("Entries");  
    
    h_PhiJetRes = subDir.make<TH1D>("h_PhiJetRes","h_PhiJetRes",100, -0.5,0.5); 
    h_PhiJetRes->GetXaxis()->SetTitle("Jets (l1 #phi- gen #phi)/(gen #phi)");
    h_PhiJetRes->GetYaxis()->SetTitle("Entries");  
    
    h_JetEtaVsPhi = subDir.make<TH2D>("h_JetEtaVsPhi","h_JetEtaVsPhi",100, -3.2,3.2, 100, -5,5); 
    h_JetEtaVsPhi->GetYaxis()->SetTitle("#eta");
    h_JetEtaVsPhi->GetXaxis()->SetTitle("#phi"); 
    
    h_l1j1_Eta = subDir.make<TH1D>("h_l1j1_Eta","h_l1j1_Eta",100, -6,6); 
    h_l1j1_Eta->GetXaxis()->SetTitle("Leading jet #eta");
    h_l1j1_Eta->GetYaxis()->SetTitle("Entries");
    
    h_l1j2_Eta = subDir.make<TH1D>("h_l1j2_Eta","h_l1j2_Eta",100, -6,6); 
    h_l1j2_Eta->GetXaxis()->SetTitle("Sub-Leading jet #eta");
    h_l1j2_Eta->GetYaxis()->SetTitle("Entries");
    
    h_l1j1_Phi = subDir.make<TH1D>("h_l1j1_Phi","h_l1j1_Phi",100, -3.2,3.2); 
    h_l1j1_Phi->GetXaxis()->SetTitle("Leading jet #phi");
    h_l1j1_Phi->GetYaxis()->SetTitle("Entries");
    
    h_l1j2_Phi = subDir.make<TH1D>("h_l1j2_Phi","h_l1j2_Phi",100, -3.2,3.2); 
    h_l1j2_Phi->GetXaxis()->SetTitle("Sub-Leading jet #phi");
    h_l1j2_Phi->GetYaxis()->SetTitle("Entries");
    
    h_VBFJetsWithMaxVariables = subDir.make<TH1D>("h_VBFJetsWithMaxVariables","h_VBFJetsWithMaxVariables",6, 0,6); 
    h_VBFJetsWithMaxVariables->GetYaxis()->SetTitle("Entries");
    h_VBFJetsWithMaxVariables->GetXaxis()->SetBinLabel(2,"Largest <p_{T}>");
    h_VBFJetsWithMaxVariables->GetXaxis()->SetBinLabel(3,"Largest m_{jj}");
    h_VBFJetsWithMaxVariables->GetXaxis()->SetBinLabel(4,"Largest |#Delat#eta|");
    h_VBFJetsWithMaxVariables->GetXaxis()->SetBinLabel(5,"Largest Max p_{T}");
    h_VBFJetsWithMaxVariables->GetXaxis()->SetBinLabel(1,"Total");
    
    h_LeadJetEta_Cut = subDir.make<TH1D>("h_LeadJetEta_Cut","h_LeadJetEta_Cut",100, -10,10); 
    h_LeadJetEta_Cut->GetXaxis()->SetTitle("Gen leading jet #eta");
    h_LeadJetEta_Cut->GetYaxis()->SetTitle("Entries");
    
    h_SubLeadJetEta_Cut = subDir.make<TH1D>("h_SubLeadJetEta_Cut","h_SubLeadJetEta_Cut",100, -10,10); 
    h_SubLeadJetEta_Cut->GetXaxis()->SetTitle("Gen sub-leading jet #eta");
    h_SubLeadJetEta_Cut->GetYaxis()->SetTitle("Entries");
    
    h_genjjEta2D = subDir.make<TH2D>("h_genjjEta2D","h_genjjEta2D",100, -10,10,100, -10,10); 
    h_genjjEta2D->GetXaxis()->SetTitle("Gen leading jet #eta");
    h_genjjEta2D->GetYaxis()->SetTitle("Gen sub-leading jet #eta");
    
    h_genjjEta2D_Cut = subDir.make<TH2D>("h_genjjEta2D_Cut","h_genjjEta2D_Cut",100, -10,10,100, -10,10); 
    h_genjjEta2D_Cut->GetXaxis()->SetTitle("Gen leading jet #eta");
    h_genjjEta2D_Cut->GetYaxis()->SetTitle("Gen sub-leading jet #eta");
    
    h_tauEta = subDir.make<TH1D>("h_tauEta","h_tauEta",100, -10,10); 
    h_tauEta->GetXaxis()->SetTitle("Gen tau #eta");
    h_tauEta->GetYaxis()->SetTitle("Entries");
    
    h_muonEta = subDir.make<TH1D>("h_muonEta","h_muonEta",100, -10,10); 
    h_muonEta->GetXaxis()->SetTitle("Gen muon #eta");
    h_muonEta->GetYaxis()->SetTitle("Entries");
    
    h_electronEta = subDir.make<TH1D>("h_electronEta","h_electronEta",100, -10,10); 
    h_electronEta->GetXaxis()->SetTitle("Gen electron #eta");
    h_electronEta->GetYaxis()->SetTitle("Entries");
    
    h_etauEta2D = subDir.make<TH2D>("h_etauEta2D","h_etauEta2D",100, -10,10,100, -10,10); 
    h_etauEta2D->GetXaxis()->SetTitle("Gen tau #eta");
    h_etauEta2D->GetYaxis()->SetTitle("Gen electron #eta");
    
    h_tautauEta2D = subDir.make<TH2D>("h_tautauEta2D","h_tautauEta2D",100, -10,10,100, -10,10); 
    h_tautauEta2D->GetXaxis()->SetTitle("Gen leading tau_{vis} #eta");
    h_tautauEta2D->GetYaxis()->SetTitle("Gen sub-leading tau_{vis} #eta");
    
    h_l1jjEta2D = subDir.make<TH2D>("h_l1jjEta2D","h_l1jjEta2D",100, -6,6,100, -6,6); 
    h_l1jjEta2D->GetXaxis()->SetTitle("L1 leading VBF jet #eta");
    h_l1jjEta2D->GetYaxis()->SetTitle("L1 sub-leading VBF jet #eta");
    
    h_l1jjEta2D_Cut = subDir.make<TH2D>("h_l1jjEta2D_Cut","h_l1jjEta2D_Cut",100, -6,6,100, -6,6); 
    h_l1jjEta2D_Cut->GetXaxis()->SetTitle("L1 leading VBF jet #eta");
    h_l1jjEta2D_Cut->GetYaxis()->SetTitle("L1 sub-leading VBF jet #eta");
    
    h_l1jj_DiffOverSum = subDir.make<TH1D>("h_l1jj_DiffOverSum","h_l1jj_DiffOverSum",100, 0,1); 
    h_l1jj_DiffOverSum->GetXaxis()->SetTitle("Di-jet #Delta p_{T}/#Sigma p_{T} [GeV]");
    h_l1jj_DiffOverSum->GetYaxis()->SetTitle("Entries");
    
    h_l1jj_Diff = subDir.make<TH1D>("h_l1jj_Diff","h_l1jj_Diff",100, 0,150); 
    h_l1jj_Diff->GetXaxis()->SetTitle("Di-jet #Delta p_{T} [GeV]");
    h_l1jj_Diff->GetYaxis()->SetTitle("Entries");
    
    h_JetsMathedTo = subDir.make<TH1D>("h_JetsMathedTo","h_JetsMathedTo",6, 0,6);
    h_JetsMathedTo_LeadMjj = subDir.make<TH1D>("h_JetsMathedTo_LeadMjj","h_JetsMathedTo_LeadMjj",6, 0,6);
    h_JetsMathedTo_LeadMjjLeadAvePt = subDir.make<TH1D>("h_JetsMathedTo_LeadMjjLeadAvePt","h_JetsMathedTo_LeadMjjLeadAvePt",6, 0,6);
    h_L1VBFMultiplicity = subDir.make<TH1D>("h_L1VBFMultiplicity","h_L1VBFMultiplicity",10, 0,10); 
    
    EventsTotal=0;

  }

  VBFPlots::~VBFPlots() {
    ;
  }

  int VBFPlots::PreAnalysis() {

    return 0;
  }

  int VBFPlots::Execute(TreeEvent *event) {
      EventsTotal++;
      
  std::vector<PFJet*> jets = event->GetPtrVec<PFJet>("ak4PFJetsCHS");
  std::vector<ic::GenJet*> genjets = event->GetPtrVec<ic::GenJet>("genJets");
  std::vector<ic::GenParticle*> GenParticles = event->GetPtrVec<ic::GenParticle>("genParticles");
  std::vector<ic::L1TJet*>  l1jets = event->GetPtrVec<ic::L1TJet>("L1Jets");
  
  std::sort(l1jets.begin(),l1jets.end(),greater_Candidate());
   
  n_jets_ = jets.size();
  n_genjets_ = genjets.size();
  n_genParticles_ = GenParticles.size();
  n_l1jets_ = l1jets.size();
  
  double electronEta=0;
  double tauEta=0;
  double tau1Eta=0;
  double tau2Eta=0;
  double tau1Pt=0;
  double tau2Pt=0;
  int tauCount =0;
  int electronCount=0;
  
  for(unsigned j=0; j< n_genParticles_; j++){
      int ID = std::fabs(GenParticles[j]->pdgid());
      bool isPrompt =  false;
      if(GenParticles[j]->statusFlags().at(0) || GenParticles[j]->statusFlags().at(5)) isPrompt = true; 
      
      if(ID == 15){
          
          ic::Candidate tausVis;
          
          for(unsigned k=0; k < GenParticles[j]->daughters().size(); k++){
              if(std::fabs(GenParticles[GenParticles[j]->daughters().at(k)]->pdgid()) == 11 || std::fabs(GenParticles[GenParticles[j]->daughters().at(k)]->pdgid()) == 13) isPrompt = false;  
              if(std::fabs(GenParticles[GenParticles[j]->daughters().at(k)]->pdgid()) == 16) tausVis.set_vector(GenParticles[j]->vector() - GenParticles[GenParticles[j]->daughters().at(k)]->vector());
          }
          
          if(isPrompt){
              h_tauEta->Fill(tausVis.vector().Rapidity());
              if(tauCount == 0){
                  tauEta = tausVis.vector().Rapidity();
                  tau1Eta = tausVis.vector().Rapidity(); 
                  tau1Pt = tausVis.vector().Pt();
              }
              if(tauCount == 1){
                  tau2Eta = tausVis.vector().Rapidity();
                  tau2Pt = tausVis.vector().Pt();
              }
              tauCount++;
          }
      }
      
      if(ID == 11 && isPrompt){
          h_electronEta->Fill(GenParticles[j]->vector().Rapidity());
          if(electronCount == 0) electronEta = GenParticles[j]->vector().Rapidity();
          electronCount++;
      }
      if(ID == 13 && isPrompt) h_muonEta->Fill(GenParticles[j]->vector().Rapidity());
  }
  
  double temp;
  if(tau2Pt > tau1Pt){
      temp = tau1Eta;
      tau1Eta = tau2Eta;
      tau2Eta = temp;
  }
  
  if(channel_ == "et" && tauCount > 0 && electronCount > 0) h_etauEta2D->Fill(tauEta, electronEta);
  if(channel_ == "tt" && tauCount > 1) h_tautauEta2D->Fill(tau1Eta, tau2Eta);
    
  if(n_jets_ > 1) if(jets[0]->vector().Pt() < JetsOfflineMinPt || jets[1]->vector().Pt() < JetsOfflineMinPt){
      return 0;
  }

  if(GenParticles.size() > 6){
      
      double Mqq = (GenParticles[5]->vector() + GenParticles[6]->vector()).M();
      double DeltaEta = std::fabs(GenParticles[5]->vector().Rapidity() - GenParticles[6]->vector().Rapidity());
      double Pt1 = GenParticles[5]->vector().Pt();
      double Pt2 = GenParticles[6]->vector().Pt();
      if(Pt1 < Pt2){
          double PtTemp = Pt1;
          Pt1 = Pt2;
          Pt2 = PtTemp;
      }
      
      h_q1_Pt->Fill(Pt1);
      h_q2_Pt->Fill(Pt2);
      h_qq_DeltaEta->Fill(DeltaEta);
      h_qq_Mqq->Fill(Mqq);
      double AvePt = (Pt1 + Pt2)/2;
      h_qq_AvePt->Fill(AvePt);
  }
  
  
  double DeltaRMin1 = 100000;
  double DeltaRMin2 = 100000;
  int genjetIndex1 = -1;
  int genjetIndex2 = -1;
  
  
  for(unsigned i=0; i< n_genjets_; i++){
      
      double DeltaR1 = sqrt(pow(genjets[i]->vector().Phi()-GenParticles[5]->vector().Phi(),2) + pow(genjets[i]->vector().Rapidity()-GenParticles[5]->vector().Rapidity(),2));
      if(DeltaR1 < DeltaRMin1 && DeltaR1 < 0.5){
          DeltaRMin1 = DeltaR1;
          genjetIndex1 = i;
      }
      
      double DeltaR2 = sqrt(pow(genjets[i]->vector().Phi()-GenParticles[6]->vector().Phi(),2) + pow(genjets[i]->vector().Rapidity()-GenParticles[6]->vector().Rapidity(),2));
      if(DeltaR2 < DeltaRMin2 && DeltaR2 < 0.5){
          DeltaRMin2 = DeltaR2;
          genjetIndex2 = i;
      }
      
  }
  
  /////////////////////////////////
  std::vector<ic::Candidate> PromptTausVis;
  
  for(unsigned i=0; i< n_genParticles_; i++){
      
      int genID = std::fabs(GenParticles[i]->pdgid());
              
      if(genID == 15){
          
          ic::Candidate tausVis;
          
          if(std::fabs(GenParticles[GenParticles[i]->mothers().at(0)]->pdgid()) == 25){
              bool isMuon = false;
              bool isElectron = false;
        
              for(unsigned l=0; l < GenParticles[i]->daughters().size(); l++) {
                      if(std::fabs(GenParticles[GenParticles[i]->daughters().at(l)]->pdgid()) == 11) isElectron = true;
                      if(std::fabs(GenParticles[GenParticles[i]->daughters().at(l)]->pdgid()) == 13) isMuon = true;
                      if(std::fabs(GenParticles[GenParticles[i]->daughters().at(l)]->pdgid()) == 16) tausVis.set_vector(GenParticles[i]->vector() - GenParticles[GenParticles[i]->daughters().at(l)]->vector());
              }
              
              if(!isMuon && isElectron) PromptTausVis.push_back(tausVis);
              
          }
      }
  }
  
  unsigned n_prompttaus_ = PromptTausVis.size();
      

  double LargestMjj = -1;
  double LargestAvePt =-1;
  unsigned VBFPassCount=0;
  
  for(unsigned i=0; i< n_l1jets_; i++){
  
      for(unsigned j=i+1; j< n_l1jets_; j++){
  
          double Mjj = (l1jets[i]->vector() + l1jets[j]->vector()).M();
          double AvePt = (l1jets[i]->vector().Pt()+l1jets[j]->vector().Pt())/2;
          
          if(Mjj >= 855 && AvePt >= 60){
              
              VBFPassCount++;
              
              bool L1Jet1isVBF = false;
              bool L1Jet2isVBF = false;
              bool L1Jet1isPromptTau = false;
              bool L1Jet2isPromptTau = false;
              
              bool LargestMjjL1Jet1isVBF = false;
              bool LargestMjjL1Jet2isVBF = false;
              bool LargestMjjL1Jet1isPromptTau = false;
              bool LargestMjjL1Jet2isPromptTau = false;
              
              bool LargestAvePtL1Jet1isVBF = false;
              bool LargestAvePtL1Jet2isVBF = false;
              bool LargestAvePtL1Jet1isPromptTau = false;
              bool LargestAvePtL1Jet2isPromptTau = false;
              
              if(Mjj > LargestMjj) LargestMjj = Mjj;
              if(Mjj > LargestAvePt) LargestAvePt = AvePt;
              
              if(genjetIndex1 != -1){
                  double DeltaR = sqrt(pow(genjets[genjetIndex1]->vector().Phi()-l1jets[i]->vector().Phi(),2) + pow(genjets[genjetIndex1]->vector().Rapidity()-l1jets[i]->vector().Rapidity(),2));
                  if(DeltaR < 0.5){
                      L1Jet1isVBF = true;
                      if(Mjj > LargestMjj) LargestMjjL1Jet1isVBF = true;
                      if(Mjj > LargestAvePt) LargestAvePtL1Jet1isVBF = true;
                  }
              }
              if(genjetIndex2 != -1){
                  double DeltaR = sqrt(pow(genjets[genjetIndex2]->vector().Phi()-l1jets[i]->vector().Phi(),2) + pow(genjets[genjetIndex2]->vector().Rapidity()-l1jets[i]->vector().Rapidity(),2));
                  if(DeltaR < 0.5){
                      L1Jet1isVBF = true;
                      if(Mjj > LargestMjj) LargestMjjL1Jet1isVBF = true;
                      if(Mjj > LargestAvePt) LargestAvePtL1Jet1isVBF = true;
                  }
              }
              if(genjetIndex1 != -1){
                  double DeltaR = sqrt(pow(genjets[genjetIndex1]->vector().Phi()-l1jets[j]->vector().Phi(),2) + pow(genjets[genjetIndex1]->vector().Rapidity()-l1jets[j]->vector().Rapidity(),2));
                  if(DeltaR < 0.5){
                      L1Jet2isVBF = true;
                      if(Mjj > LargestMjj) LargestMjjL1Jet2isVBF = true;
                      if(Mjj > LargestAvePt) LargestAvePtL1Jet2isVBF = true;
                  }
              }
              if(genjetIndex2 != -1){
                  double DeltaR = sqrt(pow(genjets[genjetIndex2]->vector().Phi()-l1jets[j]->vector().Phi(),2) + pow(genjets[genjetIndex2]->vector().Rapidity()-l1jets[j]->vector().Rapidity(),2));
                  if(DeltaR < 0.5){
                      L1Jet2isVBF = true;
                      if(Mjj > LargestMjj) LargestMjjL1Jet2isVBF = true;
                      if(Mjj > LargestAvePt) LargestAvePtL1Jet2isVBF = true;
                  }
              }
              
              for(unsigned k=0; k< n_prompttaus_; k++){
                  
                  double DeltaR1 = sqrt(pow(PromptTausVis[k].vector().Phi()-l1jets[i]->vector().Phi(),2) + pow(PromptTausVis[k].vector().Rapidity()-l1jets[i]->vector().Rapidity(),2));
                  if(DeltaR1 < 0.5){
                      L1Jet1isPromptTau = true;
                      if(Mjj > LargestMjj) LargestMjjL1Jet1isPromptTau = true;
                      if(Mjj > LargestAvePt) LargestAvePtL1Jet1isPromptTau = true;
                  }
                  
                  double DeltaR2 = sqrt(pow(PromptTausVis[k].vector().Phi()-l1jets[j]->vector().Phi(),2) + pow(PromptTausVis[k].vector().Rapidity()-l1jets[j]->vector().Rapidity(),2));
                  if(DeltaR2 < 0.5){
                      L1Jet2isPromptTau = true;
                      if(Mjj > LargestMjj) LargestMjjL1Jet2isPromptTau = true;
                      if(Mjj > LargestAvePt) LargestAvePtL1Jet2isPromptTau = true;
                  }
    
              }
              
              if(L1Jet1isVBF && L1Jet1isVBF) h_JetsMathedTo->Fill(0);
              else if((L1Jet1isVBF && L1Jet1isPromptTau) || (L1Jet2isVBF && L1Jet2isPromptTau)) h_JetsMathedTo->Fill(1);
              else if(L1Jet1isPromptTau && L1Jet2isPromptTau) h_JetsMathedTo->Fill(2);
              else if(L1Jet1isVBF || L1Jet2isVBF) h_JetsMathedTo->Fill(3);
              else if(L1Jet1isPromptTau || L1Jet2isPromptTau) h_JetsMathedTo->Fill(4);
              h_JetsMathedTo->Fill(5);
              
              if(LargestMjjL1Jet1isVBF && LargestMjjL1Jet1isVBF) h_JetsMathedTo_LeadMjj->Fill(0);
              else if((LargestMjjL1Jet1isVBF && LargestMjjL1Jet1isPromptTau) || (LargestMjjL1Jet2isVBF && LargestMjjL1Jet2isPromptTau)) h_JetsMathedTo_LeadMjj->Fill(1);
              else if(LargestMjjL1Jet1isPromptTau && LargestMjjL1Jet2isPromptTau) h_JetsMathedTo_LeadMjj->Fill(2);
              else if(LargestMjjL1Jet1isVBF || LargestMjjL1Jet2isVBF) h_JetsMathedTo_LeadMjj->Fill(3);
              else if(LargestMjjL1Jet1isPromptTau || LargestMjjL1Jet2isPromptTau) h_JetsMathedTo_LeadMjj->Fill(4);
              h_JetsMathedTo_LeadMjj->Fill(5);
              
              if(LargestAvePtL1Jet1isVBF && LargestAvePtL1Jet1isVBF) h_JetsMathedTo_LeadMjjLeadAvePt->Fill(0);
              else if((LargestAvePtL1Jet1isVBF && LargestAvePtL1Jet1isPromptTau) || (LargestAvePtL1Jet2isVBF && LargestAvePtL1Jet2isPromptTau)) h_JetsMathedTo_LeadMjjLeadAvePt->Fill(1);
              else if(LargestAvePtL1Jet1isPromptTau && LargestAvePtL1Jet2isPromptTau) h_JetsMathedTo_LeadMjjLeadAvePt->Fill(2);
              else if(LargestAvePtL1Jet1isVBF || LargestAvePtL1Jet2isVBF) h_JetsMathedTo_LeadMjjLeadAvePt->Fill(3);
              else if(LargestAvePtL1Jet1isPromptTau || LargestAvePtL1Jet2isPromptTau) h_JetsMathedTo_LeadMjjLeadAvePt->Fill(4);
              h_JetsMathedTo_LeadMjjLeadAvePt->Fill(5);

          }
      }
  } 
  
  h_L1VBFMultiplicity->Fill(VBFPassCount);
  /////////////////////////////////
  
  if(std::fabs(GenParticles[5]->vector().Rapidity())<5 && std::fabs(GenParticles[6]->vector().Rapidity())<5){
      if(genjetIndex1 != -1 && genjetIndex2 != -1) h_MatchedToGenStats->Fill(2);
      else if (genjetIndex1 != -1 || genjetIndex2 != -1) h_MatchedToGenStats->Fill(1);
      else h_MatchedToGenStats->Fill(0);
  }
  
  if(genjetIndex1 == -1 && genjetIndex2 == -1) h_genjets_Index->Fill(genjetIndex1,genjetIndex2);
  else if(genjetIndex1 == -1) h_genjets_Index->Fill(genjetIndex2,genjetIndex1);
  else if(genjetIndex2 == -1) h_genjets_Index->Fill(genjetIndex1,genjetIndex2);
  
  if(n_genjets_-2 > 1){
      int NGenJets = n_genjets_;
      for(int i=0; i<NGenJets; i++){
          for(int j=i+1; j<NGenJets; j++){
              
              if(!(i == genjetIndex1 || i == genjetIndex2 || j == genjetIndex1 || j == genjetIndex2)){
            
                  if(std::fabs(genjets[0]->vector().Rapidity()) < 5 && std::fabs(genjets[1]->vector().Rapidity()) < 5){
                      double Mjj = (genjets[i]->vector() + genjets[j]->vector()).M();
                      double DeltaEta = std::fabs(genjets[i]->vector().Rapidity() - genjets[j]->vector().Rapidity());
                      double Pt1 = genjets[i]->vector().Pt();
                      double Pt2 = genjets[j]->vector().Pt();
                      h_genj1_Pt_NonVBF->Fill(Pt1);
                      h_genj2_Pt_NonVBF->Fill(Pt2);
                      h_genjj_Mqq_NonVBF->Fill(Mjj);
                      h_genjj_DeltaEta_NonVBF->Fill(DeltaEta);
                  }

              }
          }
      }
  }
  
  if(n_genjets_ > 1 && genjetIndex1 != -1 && genjetIndex2 != -1 && genjetIndex1 != genjetIndex2){
      
      double Pt1 = genjets[genjetIndex1]->vector().Pt();
      double Pt2 = genjets[genjetIndex2]->vector().Pt();
      int PartonIndex1 = 5;
      int PartonIndex2 = 6;
      if(Pt1 < Pt2){
          double PtTemp = Pt1;
          Pt1 = Pt2;
          Pt2 = PtTemp;
          int tempIndex = genjetIndex1;
          genjetIndex1 = genjetIndex2;
          genjetIndex2 = tempIndex;
          PartonIndex1 = 6;
          PartonIndex2 = 5;
      }
      
      h_LeadJetEta->Fill(genjets[genjetIndex1]->vector().Rapidity());
      h_SubLeadJetEta->Fill(genjets[genjetIndex2]->vector().Rapidity());
      h_genjets_Index->Fill(genjetIndex1,genjetIndex2);
      
      h_genjjEta2D->Fill(genjets[genjetIndex1]->vector().Rapidity(), genjets[genjetIndex2]->vector().Rapidity());
      
      double genMjj = (genjets[genjetIndex1]->vector() + genjets[genjetIndex2]->vector()).M();
      double genDeltaEta = std::fabs(genjets[genjetIndex1]->vector().Rapidity() - genjets[genjetIndex2]->vector().Rapidity());
      
      if(genMjj > 400. && genDeltaEta > 3.){
          
          h_LeadJetEta_Cut->Fill(genjets[genjetIndex1]->vector().Rapidity());
          h_SubLeadJetEta_Cut->Fill(genjets[genjetIndex2]->vector().Rapidity());
          h_genjjEta2D_Cut->Fill(genjets[genjetIndex1]->vector().Rapidity(), genjets[genjetIndex2]->vector().Rapidity());
          
      }
      
      if(std::fabs(genjets[genjetIndex1]->vector().Rapidity()) < 5 && std::fabs(genjets[genjetIndex2]->vector().Rapidity()) < 5){
          
          double Mjj = (genjets[genjetIndex1]->vector() + genjets[genjetIndex2]->vector()).M();
          double DeltaEta = std::fabs(genjets[genjetIndex1]->vector().Rapidity() - genjets[genjetIndex2]->vector().Rapidity());
          
          h_genj1_Pt->Fill(Pt1);
          h_genj2_Pt->Fill(Pt2);
          h_genjj_DeltaEta->Fill(DeltaEta);
          h_genjj_Mqq->Fill(Mjj);
          
          double AvePt = (Pt1 + Pt2)/2;
          h_genjj_AvePt->Fill(AvePt);
          
          double SubJetPtRes = (genjets[genjetIndex1]->vector().Pt() - GenParticles[PartonIndex1]->vector().Pt()) / (GenParticles[PartonIndex1]->vector().Pt());
          double JetPtRes = (genjets[genjetIndex2]->vector().Pt() - GenParticles[PartonIndex2]->vector().Pt()) / (GenParticles[PartonIndex2]->vector().Pt());
          
          h_LeadJetPtRes->Fill(JetPtRes);
          h_SubLeadJetPtRes->Fill(SubJetPtRes);
          
          h_LeadJetPtResVsPartonPt->Fill(GenParticles[PartonIndex1]->vector().Pt(), JetPtRes);
          h_SubLeadJetPtResVsPartonPt->Fill(GenParticles[PartonIndex2]->vector().Pt(), SubJetPtRes);
          
          double PtDiff = std::fabs(genjets[genjetIndex1]->vector().Pt() - genjets[genjetIndex2]->vector().Pt());
          
          h_PtDiff->Fill(PtDiff);
          h_PtDiffVsSubPt->Fill(genjets[genjetIndex2]->vector().Pt(), PtDiff);
          
          double SignJet1 = genjets[genjetIndex1]->vector().Rapidity() / std::fabs(genjets[genjetIndex1]->vector().Rapidity());
          double SignJet2 = genjets[genjetIndex2]->vector().Rapidity() / std::fabs(genjets[genjetIndex2]->vector().Rapidity());
          
          if(SignJet1 == SignJet2){
              h_PercentOppSides->Fill(1);
              h_PercentSameSidesVsSubPt->Fill(genjets[genjetIndex2]->vector().Pt());
          }
          else{
              h_PercentOppSides->Fill(0);
              h_PercentOppSidesVsSubPt->Fill(genjets[genjetIndex2]->vector().Pt());
          }
        
          h_TotalDiJetsVsSubPt->Fill(genjets[genjetIndex2]->vector().Pt());
      }
      
  }
  
  DeltaRMin1 = 100000;
  DeltaRMin2 = 100000;
  int jetIndex1 = -1;
  int jetIndex2 = -1;
  
  if(n_jets_ > 1 && genjetIndex1 != -1 && genjetIndex2 != -1){
      
      for(unsigned i=0; i< n_jets_; i++){
          
          double DeltaR1 = sqrt(pow(jets[i]->vector().Phi()-genjets[genjetIndex1]->vector().Phi(),2) + pow(jets[i]->vector().Rapidity()-genjets[genjetIndex1]->vector().Rapidity(),2));
          if(DeltaR1 < DeltaRMin1 && DeltaR1 < 0.5){
              DeltaRMin1 = DeltaR1;
              jetIndex1 = i;
          }
          
          double DeltaR2 = sqrt(pow(jets[i]->vector().Phi()-genjets[genjetIndex2]->vector().Phi(),2) + pow(jets[i]->vector().Rapidity()-genjets[genjetIndex2]->vector().Rapidity(),2));
          if(DeltaR2 < DeltaRMin2 && DeltaR2 < 0.5){
              DeltaRMin2 = DeltaR2;
              jetIndex2 = i;
          }
          
      }
      
      if(n_jets_-2 > 1){
          
          int NJets = n_jets_;
          for(int i=0; i<NJets; i++){
              for(int j=i+1; j<NJets; j++){
                  
                  if(!(i == jetIndex1 || i == jetIndex2 || j == jetIndex1 || j == jetIndex2)){
                      //double EtaGap = std::fabs(jets[i]->vector().Rapidity() - jets[j]->vector().Rapidity());
                      if(std::fabs(jets[i]->vector().Rapidity()) < 5 && std::fabs(jets[j]->vector().Rapidity()) < 5 ){
                          double Mjj = (jets[i]->vector() + jets[j]->vector()).M();
                          double DeltaEta = std::fabs(jets[i]->vector().Rapidity() - jets[j]->vector().Rapidity());
                          double Pt1 = jets[i]->vector().Pt();
                          double Pt2 = jets[j]->vector().Pt();
                          h_j1_Pt_NonVBF->Fill(Pt1);
                          h_j2_Pt_NonVBF->Fill(Pt2);
                          h_jj_Mqq_NonVBF->Fill(Mjj);
                          h_jj_DeltaEta_NonVBF->Fill(DeltaEta);
                          h_MjjVsDeltaEta_B->Fill(DeltaEta,Mjj);
                      }
     
                  }
              }
          }
      }
      
      if(jetIndex1 == -1 && jetIndex2 == -1) h_jets_Index->Fill(jetIndex1,jetIndex2);
      else if(jetIndex1 == -1) h_jets_Index->Fill(jetIndex2,jetIndex1);
      else if(jetIndex2 == -1) h_jets_Index->Fill(jetIndex1,jetIndex2);
      
      if(jetIndex1 != -1 && jetIndex2 != -1 && jetIndex1 != jetIndex2){
          
          double Pt1 = jets[jetIndex1]->vector().Pt();
          double Pt2 = jets[jetIndex2]->vector().Pt();
          if(Pt1 < Pt2){
              double PtTemp = Pt1;
              Pt1 = Pt2;
              Pt2 = PtTemp;
              int tempIndex = jetIndex1;
              jetIndex1 = jetIndex2;
              jetIndex2 = tempIndex;
          }
          
          h_jets_Index->Fill(jetIndex1,jetIndex2);
          //double EtaGap = std::fabs(jets[jetIndex1]->vector().Rapidity() - jets[jetIndex2]->vector().Rapidity());
          if(std::fabs(jets[jetIndex1]->vector().Rapidity()) < 5 && std::fabs(jets[jetIndex2]->vector().Rapidity()) < 5){
              
              double Mjj = (jets[jetIndex1]->vector() + jets[jetIndex2]->vector()).M();
              double DeltaEta = std::fabs(jets[jetIndex1]->vector().Rapidity() - jets[jetIndex2]->vector().Rapidity());
          
              h_j1_Pt->Fill(Pt1);
              h_j2_Pt->Fill(Pt2);
              h_jj_DeltaEta->Fill(DeltaEta);
              h_jj_Mqq->Fill(Mjj);
              h_MjjVsDeltaEta_S->Fill(DeltaEta,Mjj);
              double AvePt = (Pt1 + Pt2)/2;
              h_jj_AvePt->Fill(AvePt);
          }
          
      }
  }
  
  DeltaRMin1 = 100000;
  DeltaRMin2 = 100000;
  int l1jetIndex1 = -1;
  int l1jetIndex2 = -1;
  
  if(n_l1jets_ > 1 && genjetIndex1 != -1 && genjetIndex2 != -1){
      
      for(unsigned i=0; i< n_l1jets_; i++){
          
          double DeltaR1 = sqrt(pow(l1jets[i]->vector().Phi()-genjets[genjetIndex1]->vector().Phi(),2) + pow(l1jets[i]->vector().Rapidity()-genjets[genjetIndex1]->vector().Rapidity(),2));
          if(DeltaR1 < DeltaRMin1 && DeltaR1 < 0.5){
              DeltaRMin1 = DeltaR1;
              l1jetIndex1 = i;
          }
          
          double DeltaR2 = sqrt(pow(l1jets[i]->vector().Phi()-genjets[genjetIndex2]->vector().Phi(),2) + pow(l1jets[i]->vector().Rapidity()-genjets[genjetIndex2]->vector().Rapidity(),2));
          if(DeltaR2 < DeltaRMin2 && DeltaR2 < 0.5){
              DeltaRMin2 = DeltaR2;
              l1jetIndex2 = i;
          }
          
      }
      
      if(n_l1jets_-2 > 1){
          
          int NL1Jets = n_l1jets_;
          for(int i=0; i<NL1Jets; i++){
              for(int j=i+1; j<NL1Jets; j++){
                  
                  if(!(i == l1jetIndex1 || i == l1jetIndex2 || j == l1jetIndex1 || j == l1jetIndex2)){
                      
                      if(std::fabs(l1jets[0]->vector().Rapidity()) < 5 && std::fabs(l1jets[1]->vector().Rapidity()) < 5){
                          double Mjj = (l1jets[i]->vector() + l1jets[j]->vector()).M();
                          double DeltaEta = std::fabs(l1jets[i]->vector().Rapidity() - l1jets[j]->vector().Rapidity());
                          double Pt1 = l1jets[i]->vector().Pt();
                          double Pt2 = l1jets[j]->vector().Pt();
                          h_l1j1_Pt_NonVBF->Fill(Pt1);
                          h_l1j2_Pt_NonVBF->Fill(Pt2);
                          h_l1jj_Mqq_NonVBF->Fill(Mjj);
                          h_l1jj_DeltaEta_NonVBF->Fill(DeltaEta);
                          h_l1jj_VecPt_NonVBF->Fill((l1jets[i]->vector() + l1jets[j]->vector()).Pt());
                          h_l1jj_DiffOverSum->Fill((l1jets[i]->vector().Pt() - l1jets[j]->vector().Pt())/(l1jets[i]->vector().Pt() + l1jets[j]->vector().Pt()));
                          h_l1jj_Diff->Fill(std::fabs(l1jets[i]->vector().Pt() - l1jets[j]->vector().Pt()));
                      }
                  }
              }
          }
      }      
      
      if(l1jetIndex1 == -1 && l1jetIndex2 == -1) h_l1jets_Index->Fill(l1jetIndex1,l1jetIndex2);
      else if(l1jetIndex1 == -1) h_l1jets_Index->Fill(l1jetIndex2,l1jetIndex1);
      else if(l1jetIndex2 == -1) h_l1jets_Index->Fill(l1jetIndex1,l1jetIndex2);
      
      if(l1jetIndex1 != -1 && l1jetIndex2 != -1 && l1jetIndex1 != l1jetIndex2){
          
          double Pt1 = l1jets[l1jetIndex1]->vector().Pt();
          double Pt2 = l1jets[l1jetIndex2]->vector().Pt();
          double genPt1 = genjets[genjetIndex1]->vector().Pt();
          double genPt2 = genjets[genjetIndex2]->vector().Pt();
          if(Pt1 < Pt2){
              double PtTemp = Pt1;
              Pt1 = Pt2;
              Pt2 = PtTemp;
              int tempL1Index = l1jetIndex1;
              l1jetIndex1 = l1jetIndex2;
              l1jetIndex2 = tempL1Index;
              double genPtTemp = genPt1;
              genPt1 = genPt2;
              genPt2 = genPtTemp;
              int tempgenjetIndex = genjetIndex1;
              genjetIndex1 = genjetIndex2;
              genjetIndex2 = tempgenjetIndex;
          }
          
          h_l1jets_Index->Fill(l1jetIndex1,l1jetIndex2);
          
          if(std::fabs(l1jets[l1jetIndex1]->vector().Rapidity()) < 5 && std::fabs(l1jets[l1jetIndex2]->vector().Rapidity()) < 5){
              
              double Mjj = (l1jets[l1jetIndex1]->vector() + l1jets[l1jetIndex2]->vector()).M();
              double DeltaEta = std::fabs(l1jets[l1jetIndex1]->vector().Rapidity() - l1jets[l1jetIndex2]->vector().Rapidity());
              
              h_l1j1_Pt->Fill(Pt1);
              h_l1j2_Pt->Fill(Pt2);
              h_l1jj_DeltaEta->Fill(DeltaEta);
              h_l1jj_Mqq->Fill(Mjj);
              h_l1jj_VecPt->Fill((l1jets[l1jetIndex1]->vector() + l1jets[l1jetIndex2]->vector()).Pt());
              
              double genMjj = (genjets[genjetIndex1]->vector() + genjets[genjetIndex2]->vector()).M();
              double genDeltaEta = std::fabs(genjets[genjetIndex1]->vector().Rapidity() - genjets[genjetIndex2]->vector().Rapidity());
              
              h_JetsDeltaEtaResGenJet->Fill((DeltaEta - genDeltaEta)/genDeltaEta);
              h_JetsMjjResGenJet->Fill((Mjj - genMjj)/genMjj);
              h_LeadJetPtResGenJet->Fill((Pt1-genPt1)/genPt1);
              h_SubLeadJetPtResGenJet->Fill((Pt2-genPt2)/genPt2);
              
              double genAvePt = (genPt1 + genPt2)/2;
              double AvePt = (Pt1 + Pt2)/2;
              h_l1jj_AvePt->Fill(AvePt);
              h_AveJetPtResGenJet->Fill((AvePt-genAvePt)/genAvePt);
              
              h_PhiJetRes->Fill((l1jets[l1jetIndex1]->vector().Phi() - genjets[genjetIndex1]->vector().Phi())/genjets[genjetIndex1]->vector().Phi());
              h_PhiJetRes->Fill((l1jets[l1jetIndex2]->vector().Phi() - genjets[genjetIndex2]->vector().Phi())/genjets[genjetIndex2]->vector().Phi());
              
              h_EtaJetRes->Fill((l1jets[l1jetIndex1]->vector().Rapidity() - genjets[genjetIndex1]->vector().Rapidity())/genjets[genjetIndex1]->vector().Rapidity());
              h_EtaJetRes->Fill((l1jets[l1jetIndex2]->vector().Rapidity() - genjets[genjetIndex2]->vector().Rapidity())/genjets[genjetIndex2]->vector().Rapidity());
              
              h_JetEtaVsPhi->Fill(l1jets[l1jetIndex1]->vector().Phi(),l1jets[l1jetIndex1]->vector().Rapidity());
              h_JetEtaVsPhi->Fill(l1jets[l1jetIndex2]->vector().Phi(),l1jets[l1jetIndex2]->vector().Rapidity());
              
              h_l1j1_Eta->Fill(l1jets[l1jetIndex1]->vector().Rapidity());
              h_l1j2_Eta->Fill(l1jets[l1jetIndex2]->vector().Rapidity());
              h_l1j1_Phi->Fill(l1jets[l1jetIndex1]->vector().Phi());
              h_l1j2_Phi->Fill(l1jets[l1jetIndex2]->vector().Phi());

              
              bool largestAvePt =true;
              bool largestMjj=true;
              bool largestDeltaEta=true;
              bool largestMaxPt=true;
              
              for(unsigned i=0; i<n_l1jets_; i++){
                  for(unsigned j=i+1; j<n_l1jets_; j++){
                      
                      double MjjAll = (l1jets[i]->vector() + l1jets[j]->vector()).M();
                      double DeltaEtaAll = std::fabs(l1jets[i]->vector().Rapidity() - l1jets[j]->vector().Rapidity());
                      double AvePtAll = (l1jets[i]->vector().Pt() + l1jets[j]->vector().Pt())/2;
                      double PtMaxAll = std::max(l1jets[i]->vector().Pt() , l1jets[j]->vector().Pt());
                      
                      if(MjjAll > Mjj) largestMjj = false;
                      if(DeltaEtaAll > DeltaEta) largestDeltaEta =false;
                      if(AvePtAll > AvePt) largestAvePt = false;
                      if(PtMaxAll > std::max(l1jets[l1jetIndex1]->vector().Pt() , l1jets[l1jetIndex2]->vector().Pt())) largestMaxPt = false;
                    
                  }
              }
              
              h_VBFJetsWithMaxVariables->Fill(0);
              if(largestAvePt) h_VBFJetsWithMaxVariables->Fill(1);
              if(largestMjj) h_VBFJetsWithMaxVariables->Fill(2);
              if(largestDeltaEta) h_VBFJetsWithMaxVariables->Fill(3);
              if(largestMaxPt) h_VBFJetsWithMaxVariables->Fill(4);
              
              h_l1jjEta2D->Fill(l1jets[l1jetIndex1]->vector().Rapidity(), l1jets[l1jetIndex2]->vector().Rapidity());
              
              double l1Mjj = (l1jets[l1jetIndex1]->vector() + l1jets[l1jetIndex2]->vector()).M();
              double l1DeltaEta = std::fabs(l1jets[l1jetIndex1]->vector().Rapidity() - l1jets[l1jetIndex2]->vector().Rapidity());
              
              if(l1Mjj > 400. && l1DeltaEta > 3.) h_l1jjEta2D_Cut->Fill(l1jets[l1jetIndex1]->vector().Rapidity(), l1jets[l1jetIndex2]->vector().Rapidity());
          }
          
      }
  }
  
  

  return 0;
      
  }
  
  int VBFPlots::PostAnalysis() {
      
    h_MatchedToGenStats->Scale(100/h_MatchedToGenStats->GetEntries());
    h_PercentOppSides->Scale(100/h_PercentOppSides->GetEntries());
    h_PercentOppSidesVsSubPt->Divide(h_TotalDiJetsVsSubPt);
    h_PercentOppSidesVsSubPt->Scale(100);
    h_PercentSameSidesVsSubPt->Divide(h_TotalDiJetsVsSubPt);
    h_PercentSameSidesVsSubPt->Scale(100);
    
    return 0;
  }

  void VBFPlots::PrintInfo() {
    ;
  }
}

