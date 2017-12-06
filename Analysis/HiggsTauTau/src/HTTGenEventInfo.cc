#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTGenEventInfo.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPredicates.h"

std::vector<ic::GenParticle> FamilyTree (std::vector<ic::GenParticle> &v, ic::GenParticle p, std::vector<ic::GenParticle*> gen_particles){ 
  if(p.statusFlags()[13]){
      std::cout << p.pdgid() << "   " << gen_particles[p.mothers()[0]]->pdgid() << "    " << p.pt() << "    " << gen_particles[p.mothers()[0]]->pt() << std::endl;
      v.push_back(p);
  }
  else{
    for(size_t i = 0; i < p.daughters().size(); ++i ){
      ic::GenParticle d = *gen_particles[p.daughters().at(i)];
      FamilyTree(v,d, gen_particles);
    }
  }
  return v;
}

std::vector<std::pair<std::string,double>> SplitString(std::string instring){
  std::vector<std::pair<std::string,double>> out;
  std::stringstream ss(instring);   
  std::string splitstring;  
  while(std::getline(ss, splitstring, ',')){
    double val = std::stod(splitstring);
    if(splitstring.find(".") != std::string::npos) splitstring.replace(splitstring.find("."),1,"p");
    std::string weight_name = "wt_cp_"+splitstring;    
    out.push_back(std::make_pair(weight_name,val)); 
  }
  return out;
}
  
namespace ic {

  HTTGenEventInfo::HTTGenEventInfo(std::string const& name) : ModuleBase(name) {
  }

  HTTGenEventInfo::~HTTGenEventInfo() {
    ;
  }

  int HTTGenEventInfo::PreAnalysis() {
    return 0;
  }

  int HTTGenEventInfo::Execute(TreeEvent *event) {

    //std::vector<ic::GenParticle*> lhe_particles = event->GetPtrVec<ic::GenParticle>("lheParticles");
    //
    //std::vector<LHEParticle*> parts;
    //
    //for (unsigned i = 0; i < lheParticles.size(); ++i) {
    //   LHEParticle part;  
    //   part.p4 = ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float>>(lheParticles[i]->vector().Px(),lheParticles[i]->vector().Py(),lheParticles[i]->vector().Pz(),lheParticles[i]->vector().E());
    //   part.pdgId = lheParticles[i]->pdgid();
    //   parts.push_back(new LHEParticle(part));
    //}
    //
    //MadGraphTools* madGraphTools = new MadGraphTools((float)theta_sample_, process_dir_, param_card_sample_, 0.118, false);
    //double w0_ = madGraphTools->GetMatrixElementSquared(parts);
    //for(unsigned i=0; i<theta_vec_.size(); ++i){
    //  float theta_val_ = theta_vec_[i].second;
    //  std::string weight_name_ = theta_vec_[i].first;
    //  MadGraphTools* madGraphTools = new MadGraphTools((float)theta_val_, process_dir_, param_card_, 0.118, false);
    //  double weight_ = madGraphTools->GetMatrixElementSquared(parts);
    //  if(w0_!=0) weight_=weight_/w0_;
    //  else weight_=0;
    //  event->Add(weight_name_, weight_);
    //}   
    
    std::vector<ic::GenParticle*> gen_particles = event->GetPtrVec<ic::GenParticle>("genParticles");   
    
    unsigned n_incoming_quarks=0;
    unsigned n_incoming_gluons=0;
    unsigned n_outgoing_quarks=0;
    unsigned n_outgoing_gluons=0;
    
    for(unsigned i=0; i<gen_particles.size();++i){
      ic::GenParticle part = *gen_particles[i];
      
      unsigned genID = std::fabs(part.pdgid());
      
      if(genID==25 && gen_particles[i]->statusFlags()[IsFirstCopy]){
        std::cout << "-----------------------------" << std::endl;  
        std::vector<int> mother_ids = part.mothers();
        //loop over Higgs mothers
        for (unsigned j=0; j< mother_ids.size(); ++j){
          ic::GenParticle mother = *gen_particles[mother_ids[j]];
          unsigned motherID = std::fabs(mother.pdgid());
          std::cout << motherID << std::endl;
          //if(motherID == 21) n_incoming_gluons++;
          //if(motherID < 6) n_incoming_quarks++;
          ////if(gen_particles[mother.mothers()[0]]->pdgid()!=2212) std::cout << "incoming parton does not have proton mother! mother = " <<  motherID << "    " << gen_particles[mother.mothers()[0]]->pdgid() << std::endl;
          //std::vector<ic::GenParticle> family;
          //FamilyTree(family, mother, gen_particles);
          //mother = family[0];
          //std::vector<int> daughter_ids = mother.daughters();
          //for(unsigned k=0;k<daughter_ids.size();++k){
          //  ic::GenParticle daughter = *gen_particles[daughter_ids[k]];      
          //  unsigned daughterID = std::fabs(daughter.pdgid());
          //  if(daughterID == 21){ n_outgoing_gluons++; std::cout << "outgoing gluon has pT = " << daughter.vector().Pt() << std::endl;} 
          //  if(daughterID < 6) n_outgoing_quarks++;
          //}
        }
        break;
      }
      
    }
    if(n_outgoing_gluons>n_incoming_gluons){
      std::cout << "----------------------------------------" << std::endl;  
      std::cout << "n_incoming_gluons = " << n_incoming_gluons << std::endl;
      std::cout << "n_incoming_quarks = " << n_incoming_quarks << std::endl;
      std::cout << "n_outgoing_gluons = " << n_outgoing_gluons << std::endl;
      std::cout << "n_outgoing_quarks = " << n_outgoing_quarks << std::endl;  
    }

    event->Add("n_incoming_gluons", n_incoming_gluons);
    event->Add("n_incoming_quarks", n_incoming_quarks);
    event->Add("n_outgoing_gluons", n_outgoing_gluons);
    event->Add("n_outgoing_quarks", n_outgoing_quarks);
    
    return 0;
  }
  int HTTGenEventInfo::PostAnalysis() {

    return 0;
  }

  void HTTGenEventInfo::PrintInfo() {
    ;
  }

}
