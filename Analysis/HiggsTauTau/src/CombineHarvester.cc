#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/CombineHarvester.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTAnalysisTools.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnRootTools.h"
#include <vector>
#include <map>
#include <string>
#include <iostream>
#include "Math/QuantFuncMathCore.h"
#include "TMath.h"
#include "boost/lexical_cast.hpp"
#include "boost/algorithm/string.hpp"
#include "boost/format.hpp"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPredicates.h"


namespace ch {

// 	/*

// 	TGraphAsymmErrors BuildPoissonErrors(TH1F const& hist) {
// 		TGraphAsymmErrors result(&hist);
// 		double alpha = 1 - 0.6827;
// 		for (int k = 0; k <result.GetN(); ++k) {
// 			double x;
// 			double y;
// 			result.GetPoint(k, x, y);
// 			// double err_y_up = 0.5 + std::sqrt(y+0.25);
// 			// double err_y_dn = -0.5 + std::sqrt(y+0.25);
// 			double L =  (y==0) ? 0  : (ROOT::Math::gamma_quantile(alpha/2,y,1.));
// 			double U =  ROOT::Math::gamma_quantile_c(alpha/2,y+1,1) ;
// 			result.SetPointEYhigh(k, U - y);
// 			result.SetPointEYlow(k, y - L);
// 		}
// 		return result;
// 	}


// 	void Pull::Print() const {
// 		std::cout << boost::format("%-60s %-4.2f +/- %-4.2f   %+-4.2f +/- %-4.2f   %+-4.2f +/- %-4.2f   %+-4.2f \n")
// 			% name % prefit %prefit_err % bonly % bonly_err % splusb % splusb_err % rho;
// 	}

// 	bool BvsSBComparator(Pull const& pull1, Pull const& pull2) {
// 		return ( fabs(pull1.bonly - pull1.splusb) > fabs(pull2.bonly - pull2.splusb) );
// 	}

// 	void PullsFromFile(std::string const& filename, std::vector<ic::Pull> & pullvec, bool verbose) {
// 		std::vector<std::string> lines = ParseFileLines(filename);
// 		for (unsigned i = 0; i < lines.size(); ++i) {
// 			boost::erase_all(lines[i],"*");
// 			boost::erase_all(lines[i],"!");
// 			std::vector<std::string> blocks;
// 			boost::split(blocks, lines[i], boost::is_any_of(" "), boost::token_compress_on);
// 			if (blocks.size() > 0) {
// 				if (blocks[0] == "name") continue;
// 			}
// 			if (blocks.size() == 15) {
// 				pullvec.push_back(ic::Pull());
// 				ic::Pull & new_pull = pullvec.back();
// 				new_pull.name = blocks[0];
// 				new_pull.prefit = boost::lexical_cast<double>(blocks[1]);
// 				new_pull.prefit_err = boost::lexical_cast<double>(blocks[3]);
// 				new_pull.bonly = boost::lexical_cast<double>(blocks[4]);
// 				new_pull.bonly_err = boost::lexical_cast<double>(blocks[6]);
// 				new_pull.splusb = boost::lexical_cast<double>(blocks[9]);
// 				new_pull.splusb_err = boost::lexical_cast<double>(blocks[11]);
// 				new_pull.rho = boost::lexical_cast<double>(blocks[14]);
// 				if (verbose) new_pull.Print();
// 			}
// 			for (unsigned j = 0; j < blocks.size(); ++j) {
// 			}
// 		}
// 	}
	std::ostream& Nuisance::PrintHeader(std::ostream &out) {
		out << "-------------------------------------------------------------------------------------------------------------------------------------------------------------------" << std::endl;
		out << boost::format("%-8s %-8s %-8s %-30s %-4i %-10s %-40s %-8s %-9s %-10i %-10i %-10i\n")
		% "mass" % "era" % "channel" % "category" % "id" % "process" % "nuisance" % "type" % "value" % "shape" % "shape_d" % "shape_u";
		out << "-------------------------------------------------------------------------------------------------------------------------------------------------------------------";
		return out;
	}

	std::ostream& operator<< (std::ostream &out, Nuisance &val) {
		std::string value_fmt;
		if (val.asymm) {
			value_fmt = (boost::format("%-4.4g/%-4.4g") % val.value_d % val.value_u).str();
		} else {
			value_fmt = (boost::format("%-9.4g") % val.value_u).str();
		}
		out << boost::format("%-8s %-8s %-8s %-30s %-4i %-10s %-40s %-8s %-9s %-10i %-10i %-10i")
		% val.category.mass
		% val.category.era
		% val.category.channel
		% val.category.category
		% val.category.category_id
		% val.process
		% val.nuisance
		% val.type
		% value_fmt
		% bool(val.shape)
		% bool(val.shape_d)
		% bool(val.shape_u);
		return out;
	}

	std::ostream& Process::PrintHeader(std::ostream &out) {
		out << "------------------------------------------------------------------------------------------------------------------------" << std::endl;
		out << boost::format("%-8s %-8s %-8s %-30s %-4i %-35s %-4i %-10.5g %-8i\n")
		% "mass" % "era" % "channel" % "category" % "id" % "process" % "pid" % "rate" % "shape";
		out << "------------------------------------------------------------------------------------------------------------------------";
		return out;
	}

	std::ostream& operator<< (std::ostream &out, Process &val) {
		out << boost::format("%-8s %-8s %-8s %-30s %-4i %-35s %-4i %-10.5g %-8i")
		% val.category.mass
		% val.category.era
		% val.category.channel
		% val.category.category
		% val.category.category_id
		% val.process
		% val.process_id
		% val.rate
		% bool(val.shape);
		return out;
	}


	std::ostream& Observation::PrintHeader(std::ostream &out) {
		out << "------------------------------------------------------------------------------------------------------------------------" << std::endl;
		out << boost::format("%-8s %-8s %-8s %-30s %-4i %-40s %-10.5g %-8i\n")
		% "mass" % "era" % "channel" % "category" % "id" % "process" % "rate" % "shape";
		out << "------------------------------------------------------------------------------------------------------------------------";
		return out;
	}
	std::ostream& operator<< (std::ostream &out, Observation &val) {
		out << boost::format("%-8s %-8s %-8s %-30s %-4i %-40s %-10.5g %-8i")
		% val.category.mass
		% val.category.era
		% val.category.channel
		% val.category.category
		% val.category.category_id
		% val.process
		% val.rate
		% bool(val.shape);
		return out;
	}


// 	CategoryKey Nuisance::GetKey() const { return CategoryKey(mass, era, channel, category); }
// 	CategoryKey Observation::GetKey() const { return CategoryKey(mass, era, channel, category); }
// 	CategoryKey Process::GetKey() const { return CategoryKey(mass, era, channel, category); }

// 	CombineHarvester::CombineHarvester() {
// 		ignore_nuisance_correlations_ = false;
// 	}

	CombineHarvester & CombineHarvester::PrintAll() {
		std::cout << Observation::PrintHeader << std::endl;
		for (unsigned i = 0; i < obs_.size(); ++i) std::cout << *(obs_[i]) << std::endl;
		std::cout << Process::PrintHeader << std::endl;
		for (unsigned i = 0; i < processes_.size(); ++i) std::cout << *(processes_[i]) << std::endl;
		std::cout << Nuisance::PrintHeader << std::endl;
		for (unsigned i = 0; i < nuisances_.size(); ++i) std::cout << *(nuisances_[i]) << std::endl;
		return *this;
	}

// 	CombineHarvester CombineHarvester::process(std::vector<std::string> const& process) const {
// 		CombineHarvester result = *this;
// 		ic::erase_if(result.nuisances_, [&] (Nuisance const& val) { return std::find(process.begin(), process.end(), val.process) == process.end(); });
// 		ic::erase_if(result.processes_, [&] (Process const& proc) { return std::find(process.begin(), process.end(), proc.process) == process.end(); });
// 		return result;
// 	}

// 	CombineHarvester CombineHarvester::era(std::vector<std::string> const& process) const {
// 		CombineHarvester result = *this;
// 		ic::erase_if(result.nuisances_, [&] (Nuisance const& val) { return std::find(process.begin(), process.end(), val.era) == process.end(); });
// 		ic::erase_if(result.processes_, [&] (Process const& proc) { return std::find(process.begin(), process.end(), proc.era) == process.end(); });
// 		ic::erase_if(result.obs_, [&] (Observation const& proc) { return std::find(process.begin(), process.end(), proc.era) == process.end(); });
// 		return result;
// 	}

// 	CombineHarvester CombineHarvester::category_id(std::vector<int> const& id) const {
// 		CombineHarvester result = *this;
// 		ic::erase_if(result.nuisances_, [&] (Nuisance const& val) { return std::find(id.begin(), id.end(), val.category_id) == id.end(); });
// 		ic::erase_if(result.processes_, [&] (Process const& proc) { return std::find(id.begin(), id.end(), proc.category_id) == id.end(); });
// 		ic::erase_if(result.obs_, [&] (Observation const& proc) { return std::find(id.begin(), id.end(), proc.category_id) == id.end(); });
// 		return result;
// 	}


// 	CombineHarvester CombineHarvester::nuisance(std::vector<std::string> const& nuisance) const {
// 		CombineHarvester result = *this;
// 		ic::erase_if(result.nuisances_, [&] (Nuisance const& val) { return std::find(nuisance.begin(), nuisance.end(), val.nuisance) == nuisance.end(); });
// 		return result;
// 	}

// 	CombineHarvester CombineHarvester::nuisance_pred(std::function<bool(Nuisance const&)> fn) const {
// 		CombineHarvester result = *this;
// 		ic::erase_if(result.nuisances_, std::not1(fn));
// 		return result;
// 	}


// 	CombineHarvester CombineHarvester::no_shapes() const {
// 		CombineHarvester result = *this;
// 		ic::erase_if(result.nuisances_, [&] (Nuisance const& val) { return val.type == "shape"; });
// 		return result;
// 	}

// 	CombineHarvester CombineHarvester::signals() const {
// 		CombineHarvester result = *this;
// 		ic::erase_if(result.nuisances_, [&] (Nuisance const& val) { return val.process_id > 0; });
// 		ic::erase_if(result.processes_, [&] (Process const& val) { return val.process_id > 0; });
// 		return result;
// 	}

// 	CombineHarvester CombineHarvester::backgrounds() const {
// 		CombineHarvester result = *this;
// 		ic::erase_if(result.nuisances_, [&] (Nuisance const& val) { return val.process_id <= 0; });
// 		ic::erase_if(result.processes_, [&] (Process const& val) { return val.process_id <= 0; });
// 		return result;
// 	}

// 	CombineHarvester CombineHarvester::key_match(CategoryKey const & keyval) const {
// 		CombineHarvester result = *this;
// 		ic::erase_if(result.nuisances_, [&] (Nuisance const& val) { return !(val.GetKey() == keyval); });
// 		ic::erase_if(result.processes_, [&] (Process const& proc) { return !(proc.GetKey() == keyval); });
// 		ic::erase_if(result.obs_, [&] (Observation const& proc) { return !(proc.GetKey() == keyval); });
// 		return result;
// 	}

// 	/*
// 	New logic here should be:
// 		loop through processes:
// 			find set of nuisance paramters that apply to this process
// 			loop through nuisance parameters:
// 			  work out product of scaling factors for this background
// 	*/
// 	double CombineHarvester::GetRate() {
// 		double total = 0.0;
// 		for (unsigned i = 0; i < processes_.size(); ++i) {
// 			total += processes_[i].rate;
// 		}
// 		return total;
// 	}

// 		double CombineHarvester::GetObservedRate() {
// 		double total = 0.0;
// 		for (unsigned i = 0; i < obs_.size(); ++i) {
// 			total += obs_[i].rate;
// 		}
// 		return total;
// 	}

// 	std::set<std::string> CombineHarvester::GetNuisanceSet() {
// 		std::set<std::string> result;
// 		for (unsigned i = 0; i < nuisances_.size(); ++i) {
// 			result.insert(nuisances_[i].nuisance);
// 		}
// 		return result;
// 	}



// 	/* New procedure
// 		Get set of all nuisance parameters and loop:
// 		  find current value and error
// 		  calculate dx for the effect of the error on each background:
// 		  	dx += process->GetRate() * get symm error

// 		  	e.g. nuisance value 1.10
// 		  	     param value 0.5 +/- 0.5

// 		  	     process rate is nominal * 1.10^0.5
// 		  	     process rate up is nominal * 1.10^1.0
// 		  	     process rate down is nominal * 1.10^0.0


// 		  	     nuisance value 1.10 up, 0.80 down
// 		  	     param value 0.5 +/- 0.8
// 		  	     process rate is nominal * 1.10^0.5
// 		  	     process rate up is nominal * 1.10^1.3
// 		  	     process rate down is 0.80^(0.3) <-- sign flip param


// 							potentially slow, calculating same process rate over and over

// 							the new GetShape should not automatically add errors, for toys we won't
// 							need this.

// 							ideally want a fast way to map from:

// 													(1 to many)
// 							nuisance name --> nuisances(1)

// 			(1 to many)				(1 to 1)
// nuis. name -->	nuisance(s) --> parameter value
// 								| (1 to 1)
// 								v
// 							process --> nuisance(s)
//  	                   (1 to many)

//  	                   can't use pointers if these stay in vectors,
//  	                   current looping is slow (a filter operation requires
//  	                   a complete traversal)

//  	                   map<nuis. name, parameter values>
//  	                   map<nuis. name, nuisances>
//  	                   map<key, process>
//  	                   map<key, nuisnaces>

//  	                   e.g. eff_e maps to W eff_e 1.05, QCD eff_e 1.10
//  	                       or W maps to W eff_e 1.05 W eff_m 1.10






// 	*/
// 	double CombineHarvester::GetUncertainty() {
// 		// Process::PrintHeader(std::cout);
// 		std::set<std::string> nuisances = this->GetNuisanceSet();
// 		std::vector<double> uncert_vec;
// 		for (auto & nu : nuisances) {
// 			// std::cout << "--- Nuisance: " << nu << std::endl;
// 			// std::cout << "--- Correlated between: " << std::endl;
// 			double dx = 0;
// 			for (unsigned i = 0; i < nuisances_.size(); ++i) {
// 				if (nuisances_[i].nuisance != nu) continue;
// 				for (unsigned j = 0; j < processes_.size(); ++j) {
// 					if (processes_[j].era 			!= nuisances_[i].era 			||
// 							processes_[j].channel 	!= nuisances_[i].channel 	||
// 							processes_[j].category 	!= nuisances_[i].category 	||
// 							processes_[j].process 	!= nuisances_[i].process) continue;
// 					// std::cout << processes_[j] << std::endl;
// 					if (ignore_nuisance_correlations_) {
// 						if (nuisances_[i].type == "lnN") {
// 							double dxx = ( (nuisances_[i].value-1.0) * processes_[j].rate );
// 							dx = sqrt(dx*dx +  dxx*dxx); 
// 						}
// 					} else {
// 						if (nuisances_[i].type == "lnN") dx += ( (nuisances_[i].value-1.0) * processes_[j].rate ); 
// 					} 
// 					if (nuisances_[i].type == "shape") {
// 						// Do we have the shapes?
// 						if (!nuisances_[i].shape_up || !nuisances_[i].shape_down) {
// 							std::cerr << "Warning in <CombineHarvester::GetUncertainty>: Shape uncertainty histograms not loaded for nuisance " << nuisances_[i].nuisance << std::endl;
// 							continue;
// 						}
// 						if (nuisances_[i].shape->Integral() == 0.) {
// 							std::cerr << "Warning in <CombineHarvester::GetUncertainty>: Shape uncertainty defined for empty histogram" << std::endl;
// 							continue;
// 						}
// 						// The yield uncertainty due to a shape variation could be
// 						// asymmetric - we take the mean variation here
// 						double y = nuisances_[i].shape->Integral();
// 						double y_up = nuisances_[i].shape_up->Integral()/nuisances_[i].value;
// 						double y_down = nuisances_[i].shape_down->Integral()/nuisances_[i].value;
// 						// std::cout << "Var+ = " << y_up-y << std::endl;
// 						// std::cout << "Var- = " << y_down-y << std::endl;
// 						double var = (fabs(y_up-y)+fabs(y-y_down))/2.0;
// 						// std::cout << "MeanAbsVar = " << var << std::endl;
// 						if ((y_up-y >= 0.)) 	dx += (var/y)*processes_[j].rate; 
// 						if ((y_up-y < 0.)) 	dx -= (var/y)*processes_[j].rate; 
// 					}
// 				}
// 			}
// 			uncert_vec.push_back(dx);
// 		}
// 		double result = 0.0;
// 		for (unsigned i = 0; i < uncert_vec.size(); ++i) {
// 			result += (uncert_vec[i] * uncert_vec[i]);
// 		}
// 		result = sqrt(result);
// 		return result;
// 	}

// 	TH1F 	CombineHarvester::GetShape() {
// 		TH1F shape = *(processes_[0].shape);
// 		// Don't count shape norm. uncertainty in the total
// 		double tot_uncert = this->no_shapes().GetUncertainty() / this->GetRate();
// 		for (unsigned i = 1; i < processes_.size(); ++i) shape.Add(processes_[i].shape);

// 		std::set<std::string> nuisances = this->GetNuisanceSet();
// 		std::map<int, std::vector<double>> uncert_map;
// 		for (auto& nu : nuisances) {
// 			std::map<int, double> dx;
// 			for (unsigned i = 0; i < nuisances_.size(); ++i) {
// 				if (nuisances_[i].nuisance != nu) continue;
// 				for (unsigned j = 0; j < processes_.size(); ++j) {
// 					if (processes_[j].era 			!= nuisances_[i].era 			||
// 							processes_[j].channel 	!= nuisances_[i].channel 	||
// 							processes_[j].category 	!= nuisances_[i].category 	||
// 							processes_[j].process 	!= nuisances_[i].process) continue;
// 					if (nuisances_[i].type == "shape") {
// 						// Do we have the shapes?
// 						if (!nuisances_[i].shape_up || !nuisances_[i].shape_down) {
// 							std::cerr << "Warning in <CombineHarvester::GetShape>: Shape uncertainty histograms not loaded for nuisance " << nuisances_[i].nuisance << std::endl;
// 							continue;
// 						}
// 						if (nuisances_[i].shape->Integral() == 0.) {
// 							std::cerr << "Warning in <CombineHarvester::GetShape>: Shape uncertainty defined for empty histogram" << std::endl;
// 							continue;
// 						}
// 						for (int k = 1; k <= nuisances_[i].shape->GetNbinsX(); ++k) {
// 							double y = nuisances_[i].shape->GetBinContent(k);
// 							double y_up = nuisances_[i].shape_up->GetBinContent(k)/nuisances_[i].value;
// 							double y_down = nuisances_[i].shape_down->GetBinContent(k)/nuisances_[i].value;
// 							double var = (fabs(y_up-y)+fabs(y-y_down))/2.0;
// 							if ((y_up-y >= 0.)) 	dx[k] += (var)*processes_[j].shape->Integral()/nuisances_[i].shape->Integral(); 
// 							if ((y_up-y < 0.)) 	dx[k] -= (var)*processes_[j].shape->Integral()/nuisances_[i].shape->Integral(); 
// 						}
// 					}
// 				}
// 			}
// 			for (auto it = dx.begin(); it != dx.end(); ++it) uncert_map[it->first].push_back(it->second);
// 		}
// 		std::map<int, double> result;

// 		for (int i = 1; i <= shape.GetNbinsX(); ++i) {
// 			uncert_map[i].push_back(shape.GetBinContent(i) * tot_uncert);
// 		}

// 		for (auto it = uncert_map.begin(); it != uncert_map.end(); ++it) {
// 			double binresult = 0.0;
// 			for (unsigned p = 0; p < it->second.size(); ++p) binresult += (it->second.at(p) * it->second.at(p));
// 			result[it->first] = sqrt(binresult);
// 		}
// 		for (int i = 1; i <= shape.GetNbinsX(); ++i) {
// 			shape.SetBinError(i, result[i]);
// 		}

// 		return shape;
// 	}

// 	TH1F 	CombineHarvester::GetObservedShape() {
// 		TH1F shape = *(obs_[0].shape);
// 		for (unsigned i = 1; i < obs_.size(); ++i) shape.Add(obs_[i].shape);
// 		return shape;
// 	}

// 	TGraphAsymmErrors CombineHarvester::GetObservedShapeErrors() {
// 		TGraphAsymmErrors shape = *(obs_[0].errors);
// 		for (int k = 0; k < shape.GetN(); ++k) {
// 			double x;
// 			double y;
// 			shape.GetPoint(k, x, y);
// 		}
// 		for (unsigned i = 1; i < obs_.size(); ++i) {
// 			TGraphAsymmErrors const* add = obs_[i].errors;
// 			for (int k = 0; k < add->GetN(); ++k) {
// 				double x1, x2;
// 				double y1, y2;
// 				shape.GetPoint(k, x1, y1);
// 				add->GetPoint(k, x2, y2);
// 				shape.SetPoint(k, x1, y1+y2);
// 				shape.SetPointEYlow(k, std::sqrt(std::pow(shape.GetErrorYlow(k),2.) + std::pow(add->GetErrorYlow(k),2.)));
// 				shape.SetPointEYhigh(k, std::sqrt(std::pow(shape.GetErrorYhigh(k),2.) + std::pow(add->GetErrorYhigh(k),2.)));
// 			}
// 		}

// 		return shape;
// 	}



// 	int CombineHarvester::ParsePulls(std::string const& filename) {
// 		PullsFromFile(filename, pulls_, false);
// 		return 0;
// 	}

// 	void CombineHarvester::ApplyPulls(bool use_b_only) {
// 		std::map<std::string, Pull> pmap;
// 		for (unsigned i = 0; i < pulls_.size(); ++i) pmap[pulls_[i].name] = pulls_[i];
		
// 		for (unsigned i = 0; i < nuisances_.size(); ++i) {
// 			auto it = pmap.find(nuisances_[i].nuisance);
// 			if (it == pmap.end()) continue;

// 			if (nuisances_[i].type == "lnN") {
// 				double yield_corr = (nuisances_[i].value - 1.0) * ((use_b_only) ? it->second.bonly : it->second.splusb);
// 				// std::cout << nuisances_[i] << ":: Pull(" << it->second.splusb << "," << it->second.splusb_err << ")" << std::endl;
// 				for (unsigned j = 0; j < processes_.size(); ++j) {
// 					if (processes_[j].era 			!= nuisances_[i].era 			||
// 							processes_[j].channel 	!= nuisances_[i].channel 	||
// 							processes_[j].category 	!= nuisances_[i].category 	||
// 							processes_[j].process 	!= nuisances_[i].process) continue;
// 					processes_[j].rate += yield_corr*processes_[j].rate;
// 					// Also scale the histogram if it exists
// 					if (processes_[j].shape) processes_[j].shape->Scale(1. + yield_corr);
// 				}
// 				nuisances_[i].value = ((nuisances_[i].value - 1.0) * it->second.bonly_err) + 1.0;				
// 			}

// 			if (nuisances_[i].type == "shape") {
// 				for (unsigned j = 0; j < processes_.size(); ++j) {
// 					if (processes_[j].era 			!= nuisances_[i].era 			||
// 							processes_[j].channel 	!= nuisances_[i].channel 	||
// 							processes_[j].category 	!= nuisances_[i].category 	||
// 							processes_[j].process 	!= nuisances_[i].process 	||
// 							!nuisances_[i].shape || !nuisances_[i].shape_down || !nuisances_[i].shape_up) continue;
// 					if (nuisances_[i].shape->Integral() == 0.) {
// 						std::cerr << "Warning in <CombineHarvester::ApplyPulls>: Shape uncertainty defined for empty histogram" << std::endl;
// 						continue;
// 					}					
// 					TH1F *central_new = (TH1F*)nuisances_[i].shape->Clone();
// 					TH1F *up_new = (TH1F*)nuisances_[i].shape->Clone();
// 					TH1F *down_new = (TH1F*)nuisances_[i].shape->Clone();

// 					// for (unsigned k = 1; k <= central_new->GetNbinsX(); ++k) {
// 					// 	double shift = 0.;
// 					// 	if (it->second.splusb >= 0) {
// 					// 		shift = nuisances_[i].shape_up->GetBinContent(k) - nuisances_[i].shape->GetBinContent(k);
// 					// 	} else {
// 					// 		shift = nuisances_[i].shape->GetBinContent(k) - nuisances_[i].shape_down->GetBinContent(k);
// 					// 	}
// 					// 	central_new->SetBinContent(k, nuisances_[i].shape->GetBinContent(k) + it->second.splusb * shift);
// 					// 	up_new->SetBinContent(k, nuisances_[i].shape->GetBinContent(k) + (it->second.splusb + 1. * it->second.splusb_err) * shift);
// 					// 	down_new->SetBinContent(k, nuisances_[i].shape->GetBinContent(k) + (it->second.splusb - 1. * it->second.splusb_err) * shift);
// 					// }

// 					VerticalMorph(central_new, nuisances_[i].shape_up, nuisances_[i].shape_down, (use_b_only ? it->second.bonly : it->second.splusb)/nuisances_[i].value);
// 					VerticalMorph(up_new, nuisances_[i].shape_up, nuisances_[i].shape_down,   (use_b_only ? (it->second.bonly + it->second.bonly_err) : (it->second.splusb + it->second.splusb_err))/nuisances_[i].value);
// 					VerticalMorph(down_new, nuisances_[i].shape_up, nuisances_[i].shape_down, (use_b_only ? (it->second.bonly - it->second.bonly_err) : (it->second.splusb - it->second.splusb_err))/nuisances_[i].value);
					
// 					// std::cout << "Applying pull for nuisance: " << nuisances_[i] << std::endl;
// 					// std::cout << "To Process: " << processes_[j] << std::endl;
// 					// std::cout << "New/Old rate ratio from morphing is " << central_new->Integral()/nuisances_[i].shape->Integral() << std::endl;
// 					TH1F *transformation = (TH1F*)central_new->Clone();
// 					transformation->Add(nuisances_[i].shape, -1); // Do shifted shape - original shape

// 					// !!!! Official code doesn't seem to do this.
// 					transformation->Scale(processes_[j].rate / nuisances_[i].shape->Integral()); // Scale transformation up to current rate
					
// 					processes_[j].shape->Add(transformation);
// 					// Scan and fix negative bins
// 					for (int k = 1; k <= processes_[j].shape->GetNbinsX(); ++k) {
// 						if (processes_[j].shape->GetBinContent(k) < 0.) processes_[j].shape->SetBinContent(k, 0.);
// 					}
// 					// Fix the normalisation of the new shape to the expected shift form the template morphing 
// 					// But again, this isn't what the offical post-fit code does.  Visually, it just adjusts the bin contents
// 					// and leaves it at that.  So change is in rate is absolute not fractional.
// 					processes_[j].shape->Scale( processes_[j].rate * (central_new->Integral()/nuisances_[i].shape->Integral()) / processes_[j].shape->Integral() );
// 					// processes_[j].shape->Scale( (processes_[j].rate + transformation->Integral()) / processes_[j].shape->Integral() );
					

// 					// std::cout << "Current process rate is " << processes_[j].rate << std::endl;
// 					processes_[j].rate = processes_[j].shape->Integral();
// 					// std::cout << "New process rate is " << processes_[j].rate << std::endl;
// 					if (transformation) delete transformation;
// 					nuisances_[i].shape = central_new;
// 					nuisances_[i].shape_up = up_new;
// 					nuisances_[i].shape_down = down_new;
// 				}
// 			}

// 		}
// 	}

// 	void CombineHarvester::WeightSoverB() {
// 		// First need to identify the unique set of (mass, era, channel, category) - these will be
// 		// reweighted individually. 
// 		std::vector<CategoryKey> keys;
// 		for (unsigned i = 0; i  < processes_.size(); ++i) {
// 			CategoryKey a(processes_[i].GetKey());
// 			if (std::find(keys.begin(), keys.end(), a) == keys.end()) keys.push_back(a);
// 		}
// 		for (unsigned i = 0; i < keys.size(); ++i) {
// 			std::cout << "Calculating S/B weight for: " << keys[i].channel << " " << keys[i].era << " " << keys[i].category << std::endl;

// 			TH1F sig_shape = this->key_match(keys[i]).signals().GetShape();
// 			TH1F bkg_shape = this->key_match(keys[i]).backgrounds().GetShape();

// 			// Find the range from the lowest edge containing 15.9% of the signal
// 			double xmin = sig_shape.GetXaxis()->GetXmin();
// 			double xmax = sig_shape.GetXaxis()->GetXmax();
// 			unsigned steps = 350;
// 			double step_size = (xmax-xmin)/double(steps);
// 			double sig_tot = sig_shape.Integral();
// 			double lower_limit = 0;
// 			double upper_limit = 0;
// 			for (unsigned j = 0; j < steps; ++j) {
// 				double integral = IntegrateFloatRange(&sig_shape, xmin, xmin+(step_size*double(j)));
// 				if (integral/sig_tot > 0.159) {
// 					lower_limit = xmin+(step_size*double(j));
// 					break;
// 				}
// 			}
// 			for (unsigned j = 0; j < steps; ++j) {
// 				double integral = IntegrateFloatRange(&sig_shape, xmax - (step_size*double(j)), xmax);
// 				if (integral/sig_tot > 0.159) {
// 					upper_limit = xmax - (step_size*double(j));
// 					break;
// 				}
// 			}
// 			std::cout << "Found 68\% limits at " << lower_limit << "," << upper_limit << std::endl;
// 			double signal_yield = IntegrateFloatRange(&sig_shape, lower_limit, upper_limit);
// 			double backgr_yield = IntegrateFloatRange(&bkg_shape, lower_limit, upper_limit);			
// 			double weight = signal_yield / backgr_yield;
// 			std::cout << "S/B: " << weight << std::endl;
// 			for (unsigned j = 0; j < obs_.size(); ++j) {
// 				if (obs_[j].GetKey() == keys[i]) {
// 					obs_[j].rate *= weight;
// 					if (obs_[j].shape) obs_[j].shape->Scale(weight);
// 					if (obs_[j].errors) {
// 						for (int k = 0; k < obs_[j].errors->GetN(); ++k) {
// 							double x;
// 							double y;
// 							obs_[j].errors->GetPoint(k, x, y);
// 							obs_[j].errors->SetPoint(k, x, y*weight);
// 							double err_y_up = weight * obs_[j].errors->GetErrorYhigh(k);
// 							double err_y_dn = weight * obs_[j].errors->GetErrorYlow(k);
// 							obs_[j].errors->SetPointEYhigh(k, err_y_up);
// 							obs_[j].errors->SetPointEYlow(k, err_y_dn);

// 						}
// 					}
// 				}
// 			}
// 			for (unsigned j = 0; j < processes_.size(); ++j) {
// 				if (processes_[j].GetKey() == keys[i]) {
// 					processes_[j].rate *= weight;
// 					if (processes_[j].shape) processes_[j].shape->Scale(weight);
// 				}
// 			}
// 		}
// 	}

// 	void CombineHarvester::VariableRebin(std::vector<double> bins) {
// 		for (unsigned i = 0; i < processes_.size(); ++i) {
// 			if (processes_[i].shape) {
// 				processes_[i].shape = (TH1F*)processes_[i].shape->Rebin(bins.size()-1,"",&(bins[0]));
// 			}
// 		}
// 		for (unsigned i = 0; i < obs_.size(); ++i) {
// 			if (obs_[i].shape) {
// 				obs_[i].shape = (TH1F*)obs_[i].shape->Rebin(bins.size()-1,"",&(bins[0]));
// 				// If we rebin then recreate the errors from scratch
// 				if (obs_[i].errors) delete obs_[i].errors;
// 				obs_[i].errors = new TGraphAsymmErrors(BuildPoissonErrors(*(obs_[i].shape)));
// 			}
// 		}
// 		for (unsigned i = 0; i < nuisances_.size(); ++i) {
// 			if (nuisances_[i].shape) nuisances_[i].shape = (TH1F*)nuisances_[i].shape->Rebin(bins.size()-1,"",&(bins[0]));
// 			if (nuisances_[i].shape_down) nuisances_[i].shape_down = (TH1F*)nuisances_[i].shape_down->Rebin(bins.size()-1,"",&(bins[0]));
// 			if (nuisances_[i].shape_up) nuisances_[i].shape_up = (TH1F*)nuisances_[i].shape_up->Rebin(bins.size()-1,"",&(bins[0]));
// 		}
// 	}


// 	int CombineHarvester::ParseDatacard(std::string const& filename) {
// 		std::vector<std::string> as_vec;
// 		boost::split(as_vec, filename, boost::is_any_of("/"));
// 		if (as_vec.size() > 0) {
// 			std::vector<std::string> tmp;
// 			boost::split(tmp, as_vec.back(), boost::is_any_of(".-_"));
// 			if (tmp.size() == 6) {
// 				std::cout << "Info in <CombineHarvester::ParseDatacard>: For file " << as_vec.back() 
// 					<< " interpret channel, id, era and mass as \"" << tmp[1] << "\", \"" << tmp[2] << "\", \"" << tmp[3]
// 					<< "\" and \"" << tmp[4] << "\" respectively" << std::endl;
// 				return this->ParseDatacard(filename, tmp[1], boost::lexical_cast<int>(tmp[2]), tmp[3], tmp[4]);
// 			} else {
// 				std::cout << "Warning in <CombineHarvester::ParseDatacard>: Unable to interpret channel and era from file name" << std::endl;
// 				return 1;
// 			}
// 		} else {
// 			std::cout << "Warning in <CombineHarvester::ParseDatacard>: Unable to interpret channel and era from file name" << std::endl;
// 			return 1;
// 		}
// 	}

// 	int CombineHarvester::ParseDatacard(std::string const& filename, std::string mass) {
// 		std::vector<std::string> as_vec;
// 		boost::split(as_vec, filename, boost::is_any_of("/"));
// 		if (as_vec.size() > 0) {
// 			std::vector<std::string> tmp;
// 			boost::split(tmp, as_vec.back(), boost::is_any_of(".-_"));
// 			if (tmp.size() == 5) {
// 				std::cout << "Info in <CombineHarvester::ParseDatacard>: For file " << as_vec.back() 
// 					<< " interpret channel, id, era and mass as \"" << tmp[1] << "\", \"" << tmp[2] << "\", \"" << tmp[3]
// 					<< "\" and \"" << mass << "\" respectively" << std::endl;
// 				return this->ParseDatacard(filename, tmp[1], boost::lexical_cast<int>(tmp[2]), tmp[3], mass);
// 			} else {
// 				std::cout << "Warning in <CombineHarvester::ParseDatacard>: Unable to interpret channel and era from file name" << std::endl;
// 				return 1;
// 			}
// 		} else {
// 			std::cout << "Warning in <CombineHarvester::ParseDatacard>: Unable to interpret channel and era from file name" << std::endl;
// 			return 1;
// 		}
// 	}

// //htt_mt.input_7TeV.root
// 	int CombineHarvester::ParseROOTFile(std::string const& filename) {
// 		// Assume filename is of the form htt_$CHANNEL.inputs-$ANALYSIS-$ERA.root
// 		std::vector<std::string> as_vec;
// 		boost::split(as_vec, filename, boost::is_any_of("/"));
// 		if (as_vec.size() > 0) {
// 			std::vector<std::string> tmp;
// 			boost::split(tmp, as_vec.back(), boost::is_any_of(".-_"));
// 			if (tmp.size() == 5) {
// 				std::cout << "Info in <CombineHarvester::ParseROOTFile>: For file " << as_vec.back() 
// 					<< " interpret channel and era as \"" << tmp[1] << "\" and \"" << tmp[3] << "\" respectively" << std::endl;
// 				return this->ParseROOTFile(filename, tmp[1], tmp[3]);
// 			} else {
// 				std::cout << "Warning in <CombineHarvester::ParseROOTFile>: Unable to interpret channel and era from file name" << std::endl;
// 				return 1;
// 			}
// 		} else {
// 			std::cout << "Warning in <CombineHarvester::ParseROOTFile>: Unable to interpret channel and era from file name" << std::endl;
// 			return 1;
// 		}
// 	}

// 	int CombineHarvester::ParseROOTFile(const std::string & filename, std::string const& channel, std::string era) {
// 		// Try and find shapes for each known process
// 		TFile *f = new TFile(filename.c_str());
// 		if (!f) return 1;
// 		f->cd();
// 		for (unsigned i = 0; i < processes_.size(); ++i) {
// 			if (processes_[i].channel == channel && processes_[i].era == era) {
// 				std::string cat = processes_[i].category;
// 				if (!gDirectory->cd(("/"+cat).c_str())) {
// 					std::cerr << "Warning, category " << cat << " not found in ROOT File" << std::endl;
// 					continue;
// 				} 
// 				std::string name = processes_[i].process;
// 				if (processes_[i].process_id <= 0) name += processes_[i].mass;
// 				TH1F* hist = (TH1F*)gDirectory->Get(name.c_str());
// 				if (!hist) {
// 					std::cerr << "Warning, histogram " << name << " not found in ROOT File" << std::endl;
// 					continue;
// 				} else {
// 					hist = (TH1F*)hist->Clone();
// 				}
// 				processes_[i].shape = hist;
// 				processes_[i].rate = hist->Integral();
// 			}
// 		}

// 		for (unsigned i = 0; i < obs_.size(); ++i) {
// 			if (obs_[i].channel == channel && obs_[i].era == era) {
// 				std::string cat = obs_[i].category;
// 				if (!gDirectory->cd(("/"+cat).c_str())) {
// 					std::cerr << "Warning, category " << cat << " not found in ROOT File" << std::endl;
// 					continue;
// 				} 
// 				std::string name = obs_[i].process;
// 				TH1F* hist = (TH1F*)gDirectory->Get(name.c_str());
// 				if (!hist) {
// 					std::cerr << "Warning, histogram " << name << " not found in ROOT File" << std::endl;
// 					continue;
// 				} else {
// 					hist = (TH1F*)hist->Clone();
// 				}
// 				obs_[i].shape = hist;
// 				// Create poisson errors
// 				obs_[i].errors = new TGraphAsymmErrors(BuildPoissonErrors(*hist));
// 			}
// 		}

// 		for (unsigned i = 0; i < nuisances_.size(); ++i) {
// 			if (nuisances_[i].type != "shape") continue;
// 			if (nuisances_[i].channel != channel || nuisances_[i].era != era) continue;

// 			std::string cat = nuisances_[i].category;
// 			if (!gDirectory->cd(("/"+cat).c_str())) {
// 				std::cerr << "Warning, category " << cat << " not found in ROOT File" << std::endl;
// 				continue;
// 			} 
// 			std::string name = nuisances_[i].process;
// 			if (nuisances_[i].process_id <= 0) name += nuisances_[i].mass;
// 			std::string up_name = name + "_" + nuisances_[i].nuisance + "Up";
// 			std::string down_name = name + "_" + nuisances_[i].nuisance + "Down";

// 			TH1F* hist = (TH1F*)gDirectory->Get(name.c_str());
// 			if (!hist) {
// 				std::cerr << "Warning, histogram " << name << " not found in ROOT File" << std::endl;
// 				continue;
// 			} else {
// 				nuisances_[i].shape = (TH1F*)hist->Clone();	
// 			}

// 			TH1F* up_hist = (TH1F*)gDirectory->Get(up_name.c_str());
// 			if (!up_hist) {
// 				std::cerr << "Warning, histogram " << up_name << " not found in ROOT File" << std::endl;
// 				continue;
// 			} else {
// 				nuisances_[i].shape_up = (TH1F*)up_hist->Clone();	
// 			}

// 			TH1F* down_hist = (TH1F*)gDirectory->Get(down_name.c_str());
// 			if (!down_hist) {
// 				std::cerr << "Warning, histogram " << down_name << " not found in ROOT File" << std::endl;
// 				continue;
// 			} else {
// 				nuisances_[i].shape_down = (TH1F*)down_hist->Clone();	
// 			}

// 		}
// 		return 0;

// 		// Try and find shapes for each known shape nuisance
// 	}

// algorithm for shapes
/*
	5 shapes * * ../common/htt_em.input_7TeV.root $CHANNEL/$PROCESS $CHANNEL/$PROCESS_$SYSTEMATIC
  6 shapes ggH * ../common/htt_em.input_7TeV.root $CHANNEL/$PROCESS$MASS $CHANNEL/$PROCESS$MASS_$SYSTEMATIC
  7 shapes qqH * ../common/htt_em.input_7TeV.root $CHANNEL/$PROCESS$MASS $CHANNEL/$PROCESS$MASS_$SYSTEMATIC
  8 shapes VH * ../common/htt_em.input_7TeV.root $CHANNEL/$PROCESS$MASS $CHANNEL/$PROCESS$MASS_$SYSTEMATIC

  process in list?
   yes? process,category in list?
     yes? go to file and load, subbing into expression

*/
  CombineHarvester::StrPairVec CombineHarvester::GenerateShapeMapAttempts(
  		std::string process, std::string category) {
  	CombineHarvester::StrPairVec result;
  	result.push_back(std::make_pair(process 	, category));
  	result.push_back(std::make_pair("*"				, category));
  	result.push_back(std::make_pair(process 	, "*"));
  	result.push_back(std::make_pair("*" 			, "*"));
  	return result;
  }

	int CombineHarvester::ParseDatacard(std::string const& filename,  
			std::string mass, 
			std::string era,
			std::string const& channel, 
			int category_id) {

		std::vector<std::string> lines = ic::ParseFileLines(filename);

		bool start_nuisance_scan = false;
		unsigned r = 0;

		std::vector<std::vector<std::string>> words;
		
		// Loop through lines, trimming whitespace at the beginning or end
		// then splitting each line into a vector of words (using any amount
		// of whitespace as the separator).  We skip any line of zero length
		// or which starts with a "#" or "-" character.
		for (unsigned i = 0; i < lines.size(); ++i) {
			boost::trim(lines[i]);
			if (lines[i].size() == 0) continue;
			if (lines[0] == "#" || lines[0] == "-") continue;
			words.push_back(std::vector<std::string>());
			boost::split(words.back(), lines[i], boost::is_any_of("\t "), boost::token_compress_on);
		}

		// Loop through the vector of word vectors
		std::vector<ShapeMap> shape_map;
		for (unsigned i = 0; i < words.size(); ++i) {
			// Ignore line if it only has one word
			if (words[i].size() <= 1) continue;

			// Want to check this line and the previous one, so need i >= 1.
			// If the first word on this line is "observation" and "bin" on
			// the previous line then we've found the entries for data, and
			// can add Observation objects


			if (words[i][0] == "shapes") {
				shape_map.push_back(ShapeMap());
				shape_map.back().process = words[i][1];
				shape_map.back().category = words[i][2];
				std::string dc_path;
				std::size_t slash = filename.find_last_of('/');
				if (slash != filename.npos) {
					dc_path = filename.substr(0, slash) + "/" + words[i][3];
				} else {
					dc_path = words[i][3];
				}
				shape_map.back().file = new TFile(dc_path.c_str());
				shape_map.back().pattern = words[i][4];
				if (words[i].size() > 5) shape_map.back().syst_pattern = words[i][5];
			}

						// std::vector<std::pair<std::string,std::string>> attempts= { 
	
		

			if (i >= 1) {
				if (	words[i][0] 		== "observation" && 
							words[i-1][0] 	== "bin" && 
							words[i].size() == words[i-1].size()) {
					for (unsigned p = 1; p < words[i].size(); ++p) {
						obs_.emplace_back(new Observation());
						Observation &tmp = *(obs_.back());
						tmp.category = Category(mass, era, channel, words[i-1][p], category_id);
						tmp.process  = "data_obs";
						tmp.rate 		 = boost::lexical_cast<double>(words[i][p]);
						StrPairVec attempts = this->GenerateShapeMapAttempts(tmp.process, tmp.category.category);
						for (unsigned a = 0; a < attempts.size(); ++a) {
							bool success = false;
							for (unsigned m = 0; m < shape_map.size(); ++m) {
								if ((attempts[a].first == shape_map[m].process) && 
									(attempts[a].second == shape_map[m].category)) {
									std::string hist_path = shape_map[m].pattern;
									boost::replace_all(hist_path, "$CHANNEL", tmp.category.category);
									boost::replace_all(hist_path, "$PROCESS", tmp.process);
									boost::replace_all(hist_path, "$MASS", tmp.category.mass);
									shape_map[m].file->cd();
									tmp.shape = (TH1*)(gDirectory->Get(hist_path.c_str())->Clone());
									success = true;
									break;
								}
							}
							if (success) break;
						}
					}
				}
			}

			// Similarly look for the lines indicating the different signal
			// and background processes
			// Once these are found save in line index for the rate line as r
			// to we can refer back to these later, then assume that every
			// line that follows is a nuisance parameter
			if (i >= 3) {
				if (	words[i][0] 	== "rate" && 
							words[i-1][0] == "process" && 
							words[i-2][0] == "process" &&
							words[i-3][0] == "bin" && 
							words[i].size() == words[i-1].size() &&
							words[i].size() == words[i-2].size() &&
							words[i].size() == words[i-3].size()) {
					for (unsigned p = 1; p < words[i].size(); ++p) {
						processes_.emplace_back(new Process());
						Process &tmp = *(processes_.back());
						tmp.category = Category(mass, era, channel, words[i-3][p], category_id);
						tmp.process = words[i-1][p];
						tmp.process_id = boost::lexical_cast<int>(words[i-2][p]);
						tmp.rate = boost::lexical_cast<double>(words[i][p]);
						StrPairVec attempts = this->GenerateShapeMapAttempts(tmp.process, tmp.category.category);
						for (unsigned a = 0; a < attempts.size(); ++a) {
							bool success = false;
							for (unsigned m = 0; m < shape_map.size(); ++m) {
								if ((attempts[a].first == shape_map[m].process) && 
									(attempts[a].second == shape_map[m].category)) {
									std::string hist_path = shape_map[m].pattern;
									boost::replace_all(hist_path, "$CHANNEL", tmp.category.category);
									boost::replace_all(hist_path, "$PROCESS", tmp.process);
									boost::replace_all(hist_path, "$MASS", tmp.category.mass);
									shape_map[m].file->cd();
									tmp.shape = (TH1*)(gDirectory->Get(hist_path.c_str())->Clone());
									std::cout << tmp.shape->Integral() << std::endl;
									success = true;
									break;
								}
							}
							if (success) break;
						}
					}
					r = i;
					start_nuisance_scan = true;
				}
			}

			if (start_nuisance_scan && words[i].size()-1 == words[r].size()) {
				for (unsigned p = 2; p < words[i].size(); ++p) {
					if (words[i][p] == "-") continue;
					if (words[i][0].at(0) == '#') continue;
					nuisances_.emplace_back(new Nuisance());
					Nuisance &tmp = *(nuisances_.back());
					tmp.category = Category(mass, era, channel, words[r-3][p-1], category_id);
					tmp.process = words[r-1][p-1];
					tmp.process_id = boost::lexical_cast<int>(words[r-2][p-1]);
					tmp.nuisance = words[i][0];
					tmp.type = words[i][1];
					std::size_t slash_pos = words[i][p].find("/");
					if (slash_pos != words[i][p].npos) {
						// Assume asymmetric of form kDown/kUp
						tmp.value_d = boost::lexical_cast<double>(words[i][p].substr(0,slash_pos));
						tmp.value_u = boost::lexical_cast<double>(words[i][p].substr(slash_pos+1));
						tmp.asymm = true;
					} else {
						tmp.value_u = boost::lexical_cast<double>(words[i][p]);
						tmp.asymm = false;
					}
					if (tmp.type == "shape") {
						StrPairVec attempts = this->GenerateShapeMapAttempts(tmp.process, tmp.category.category);
						for (unsigned a = 0; a < attempts.size(); ++a) {
							bool success = false;
							for (unsigned m = 0; m < shape_map.size(); ++m) {
								if ((attempts[a].first == shape_map[m].process) && 
									(attempts[a].second == shape_map[m].category)) {
									std::string hist_path = shape_map[m].pattern;
									boost::replace_all(hist_path, "$CHANNEL", tmp.category.category);
									boost::replace_all(hist_path, "$PROCESS", tmp.process);
									boost::replace_all(hist_path, "$MASS", tmp.category.mass);
									std::string hist_u = shape_map[m].syst_pattern;
									boost::replace_all(hist_u, "$CHANNEL", tmp.category.category);
									boost::replace_all(hist_u, "$PROCESS", tmp.process);
									boost::replace_all(hist_u, "$MASS", tmp.category.mass);
									std::string hist_d = hist_u;
									boost::replace_all(hist_u, "$SYSTEMATIC", tmp.nuisance+"Up");
									boost::replace_all(hist_d, "$SYSTEMATIC", tmp.nuisance+"Down");
									std::cout << hist_u << std::endl;
									shape_map[m].file->cd();
									tmp.shape = (TH1*)(gDirectory->Get(hist_path.c_str())->Clone());
									tmp.shape_u = (TH1*)(gDirectory->Get(hist_u.c_str())->Clone());
									tmp.shape_d = (TH1*)(gDirectory->Get(hist_d.c_str())->Clone());
									std::cout << tmp.shape->Integral() << std::endl;
									std::cout << tmp.shape_u->Integral() << std::endl;
									std::cout << tmp.shape_d->Integral() << std::endl;
									success = true;
									break;
								}
							}
							if (success) break;
						}
						tmp.asymm = true;
						tmp.value_d = tmp.shape_d->Integral() / tmp.shape->Integral();
						tmp.value_u = tmp.shape_u->Integral() / tmp.shape->Integral();
						tmp.shape->Scale(1.0/tmp.shape->Integral());
						tmp.shape_d->Scale(1.0/tmp.shape_d->Integral());
						tmp.shape_u->Scale(1.0/tmp.shape_u->Integral());
					}
					if (!params_.count(tmp.nuisance)) {
						std::shared_ptr<Parameter> param = std::make_shared<Parameter>(Parameter());
						(*param).name = tmp.nuisance;
						(*param).value = 0.0;
						(*param).error_u = 1.0;
						(*param).error_d = -1.0;
						params_.insert(std::make_pair(tmp.nuisance, param));
					}
				}
			}
		}
		return 0;
	}

// 	bool CombineHarvester::HasProcess(std::string const& process) const {
// 		for (unsigned i = 0; i < processes_.size(); ++i) {
// 			if (processes_[i].process == process) return true;
// 		}
// 		return false;
// 	}

// 	void CombineHarvester::ScaleProcessByEra(std::string const& process, std::string const& era, double scale) {
// 		for (unsigned i = 0; i < processes_.size(); ++i) {
// 			if (processes_[i].process == process && processes_[i].era == era) {
// 				processes_[i].rate *= scale;
// 				if (processes_[i].shape) processes_[i].shape->Scale(scale);
// 			} 
// 		}
// 	}

// 	std::pair<double, int> CombineHarvester::GetPullsChi2(bool splusb) const {
// 		double tot = 0.0;
// 		for (unsigned i = 0; i < pulls_.size(); ++i) {
// 			tot += splusb ? (pulls_[i].splusb*pulls_[i].splusb) : (pulls_[i].bonly*pulls_[i].bonly);
// 		}
// 		return std::make_pair(tot, pulls_.size());
// 	}






}