#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTStatTools.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnRootTools.h"
#include <vector>
#include <map>
#include <string>
#include <iostream>
#include "boost/lexical_cast.hpp"
#include "boost/algorithm/string.hpp"
#include "boost/format.hpp"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPredicates.h"


namespace ic {

	void Pull::Print() const {
		std::cout << boost::format("%-60s %-4.2f +/- %-4.2f   %+-4.2f +/- %-4.2f   %+-4.2f +/- %-4.2f   %+-4.2f \n")
			% name % prefit %prefit_err % bonly % bonly_err % splusb % splusb_err % rho;
	}

	bool BvsSBComparator(Pull const& pull1, Pull const& pull2) {
		return ( fabs(pull1.bonly - pull1.splusb) > fabs(pull2.bonly - pull2.splusb) );
	}

	void PullsFromFile(std::string const& filename, std::vector<ic::Pull> & pullvec, bool verbose) {
		std::vector<std::string> lines = ParseFileLines(filename);
		for (unsigned i = 0; i < lines.size(); ++i) {
			boost::erase_all(lines[i],"*");
			boost::erase_all(lines[i],"!");
			std::vector<std::string> blocks;
			boost::split(blocks, lines[i], boost::is_any_of(" "), boost::token_compress_on);
			if (blocks.size() > 0) {
				if (blocks[0] == "name") continue;
			}
			if (blocks.size() == 15) {
				pullvec.push_back(ic::Pull());
				ic::Pull & new_pull = pullvec.back();
				new_pull.name = blocks[0];
				new_pull.prefit = boost::lexical_cast<double>(blocks[1]);
				new_pull.prefit_err = boost::lexical_cast<double>(blocks[3]);
				new_pull.bonly = boost::lexical_cast<double>(blocks[4]);
				new_pull.bonly_err = boost::lexical_cast<double>(blocks[6]);
				new_pull.splusb = boost::lexical_cast<double>(blocks[9]);
				new_pull.splusb_err = boost::lexical_cast<double>(blocks[11]);
				new_pull.rho = boost::lexical_cast<double>(blocks[14]);
				if (verbose) new_pull.Print();
			}
			for (unsigned j = 0; j < blocks.size(); ++j) {
			}
		}
	}

	std::ostream& operator<< (std::ostream &out, Nuisance &val) {
		out << boost::format("%-8s %-8s %-8s %-22s %-4i %-10s %-40s %-8s %-10.5g %-10i %-10i")
		% val.mass
		% val.era
		% val.channel
		% val.category
		% val.category_id
		% val.process
		% val.nuisance
		% val.type
		% val.value
		% bool(val.shape_down)
		% bool(val.shape_up);
		return out;
	}

	void Nuisance::PrintHeader(std::ostream &out) {
		out << "------------------------------------------------------------------------------------------------------------------------------------------------" << std::endl;
		out << boost::format("%-8s %-8s %-8s %-22s %-4i %-10s %-40s %-8s %-10.5g %-8i %-8i\n")
		% "Mass" % "Era" % "Channel" % "Category" % "ID" % "Process" % "Nuisance" % "Type" % "Value" % "DownShape" % "UpShape";
		out << "------------------------------------------------------------------------------------------------------------------------------------------------" << std::endl;
	}

	std::ostream& operator<< (std::ostream &out, Process &val) {
		out << boost::format("%-8s %-8s %-8s %-22s %-4i %-40s %-4i %-10.5g %-8i")
		% val.mass
		% val.era
		% val.channel
		% val.category
		% val.category_id
		% val.process
		% val.process_id
		% val.rate
		% bool(val.shape);
		return out;
	}

	void Process::PrintHeader(std::ostream &out) {
		out << "---------------------------------------------------------------------------------------------------------------------" << std::endl;
		out << boost::format("%-8s %-8s %-8s %-22s %-4i %-40s %-4i %-10.5g %-8i\n")
		% "Mass" % "Era" % "Channel" % "Category" % "ID" % "Process" % "PID" % "Rate" % "Shape";
		out << "---------------------------------------------------------------------------------------------------------------------" << std::endl;
	}

	std::ostream& operator<< (std::ostream &out, Observation &val) {
		out << boost::format("%-8s %-8s %-8s %-22s %-4i %-40s %-10.5g %-8i")
		% val.mass
		% val.era
		% val.channel
		% val.category
		% val.category_id
		% val.process
		% val.rate
		% bool(val.shape);
		return out;
	}

	void Observation::PrintHeader(std::ostream &out) {
		out << "----------------------------------------------------------------------------------------------------------------" << std::endl;
		out << boost::format("%-8s %-8s %-8s %-22s %-4i %-40s %-10.5g %-8i\n")
		% "Mass" % "Era" % "Channel" % "Category" % "ID" % "Process" % "Rate" % "Shape";
		out << "----------------------------------------------------------------------------------------------------------------" << std::endl;
	}

	CategoryKey Nuisance::GetKey() const { return CategoryKey(mass, era, channel, category); }
	CategoryKey Observation::GetKey() const { return CategoryKey(mass, era, channel, category); }
	CategoryKey Process::GetKey() const { return CategoryKey(mass, era, channel, category); }

	HTTSetup::HTTSetup() {
		ignore_nuisance_correlations_ = false;
	}

	HTTSetup & HTTSetup::PrintAll() {
		Observation::PrintHeader(std::cout);
		for (unsigned i = 0; i < obs_.size(); ++i) std::cout << obs_[i] << std::endl;
		Process::PrintHeader(std::cout);
		for (unsigned i = 0; i < processes_.size(); ++i) std::cout << processes_[i] << std::endl;
		Nuisance::PrintHeader(std::cout);
		for (unsigned i = 0; i < params_.size(); ++i) std::cout << params_[i] << std::endl;
		return *this;
	}

	HTTSetup HTTSetup::process(std::vector<std::string> const& process) const {
		HTTSetup result = *this;
		ic::erase_if(result.params_, [&] (Nuisance const& val) { return std::find(process.begin(), process.end(), val.process) == process.end(); });
		ic::erase_if(result.processes_, [&] (Process const& proc) { return std::find(process.begin(), process.end(), proc.process) == process.end(); });
		return result;
	}

	HTTSetup HTTSetup::era(std::vector<std::string> const& process) const {
		HTTSetup result = *this;
		ic::erase_if(result.params_, [&] (Nuisance const& val) { return std::find(process.begin(), process.end(), val.era) == process.end(); });
		ic::erase_if(result.processes_, [&] (Process const& proc) { return std::find(process.begin(), process.end(), proc.era) == process.end(); });
		ic::erase_if(result.obs_, [&] (Observation const& proc) { return std::find(process.begin(), process.end(), proc.era) == process.end(); });
		return result;
	}

	HTTSetup HTTSetup::category_id(std::vector<int> const& id) const {
		HTTSetup result = *this;
		ic::erase_if(result.params_, [&] (Nuisance const& val) { return std::find(id.begin(), id.end(), val.category_id) == id.end(); });
		ic::erase_if(result.processes_, [&] (Process const& proc) { return std::find(id.begin(), id.end(), proc.category_id) == id.end(); });
		ic::erase_if(result.obs_, [&] (Observation const& proc) { return std::find(id.begin(), id.end(), proc.category_id) == id.end(); });
		return result;
	}


	HTTSetup HTTSetup::nuisance(std::vector<std::string> const& nuisance) const {
		HTTSetup result = *this;
		ic::erase_if(result.params_, [&] (Nuisance const& val) { return std::find(nuisance.begin(), nuisance.end(), val.nuisance) == nuisance.end(); });
		return result;
	}

	HTTSetup HTTSetup::no_shapes() const {
		HTTSetup result = *this;
		ic::erase_if(result.params_, [&] (Nuisance const& val) { return val.type == "shape"; });
		return result;
	}

	HTTSetup HTTSetup::signals() const {
		HTTSetup result = *this;
		ic::erase_if(result.params_, [&] (Nuisance const& val) { return val.process_id > 0; });
		ic::erase_if(result.processes_, [&] (Process const& val) { return val.process_id > 0; });
		return result;
	}

	HTTSetup HTTSetup::backgrounds() const {
		HTTSetup result = *this;
		ic::erase_if(result.params_, [&] (Nuisance const& val) { return val.process_id <= 0; });
		ic::erase_if(result.processes_, [&] (Process const& val) { return val.process_id <= 0; });
		return result;
	}

	HTTSetup HTTSetup::key_match(CategoryKey const & keyval) const {
		HTTSetup result = *this;
		ic::erase_if(result.params_, [&] (Nuisance const& val) { return !(val.GetKey() == keyval); });
		ic::erase_if(result.processes_, [&] (Process const& proc) { return !(proc.GetKey() == keyval); });
		ic::erase_if(result.obs_, [&] (Observation const& proc) { return !(proc.GetKey() == keyval); });
		return result;
	}

	double HTTSetup::GetRate() {
		double total = 0.0;
		for (unsigned i = 0; i < processes_.size(); ++i) {
			total += processes_[i].rate;
		}
		return total;
	}

		double HTTSetup::GetObservedRate() {
		double total = 0.0;
		for (unsigned i = 0; i < obs_.size(); ++i) {
			total += obs_[i].rate;
		}
		return total;
	}

	std::set<std::string> HTTSetup::GetNuisanceSet() {
		std::set<std::string> result;
		for (unsigned i = 0; i < params_.size(); ++i) {
			result.insert(params_[i].nuisance);
		}
		return result;
	}


	double HTTSetup::GetUncertainty() {
		std::set<std::string> nuisances = this->GetNuisanceSet();
		std::vector<double> uncert_vec;
		for (auto & nu : nuisances) {
			double dx = 0;
			for (unsigned i = 0; i < params_.size(); ++i) {
				if (params_[i].nuisance != nu) continue;
				for (unsigned j = 0; j < processes_.size(); ++j) {
					if (processes_[j].era 			!= params_[i].era 			||
							processes_[j].channel 	!= params_[i].channel 	||
							processes_[j].category 	!= params_[i].category 	||
							processes_[j].process 	!= params_[i].process) continue;

					if (ignore_nuisance_correlations_) {
						if (params_[i].type == "lnN") {
							double dxx = ( (params_[i].value-1.0) * processes_[j].rate );
							dx = sqrt(dx*dx +  dxx*dxx); 
						}
					} else {
						if (params_[i].type == "lnN") dx += ( (params_[i].value-1.0) * processes_[j].rate ); 
					} 
					if (params_[i].type == "shape") {
						// Do we have the shapes?
						if (!params_[i].shape_up || !params_[i].shape_down) {
							std::cerr << "Warning in <HTTSetup::GetUncertainty>: Shape uncertainty histograms not loaded for nuisance " << params_[i].nuisance << std::endl;
							continue;
						}
						// The yield uncertainty due to a shape variation could be
						// asymmetric - we take the mean variation here
						double y = params_[i].shape->Integral();
						double y_up = params_[i].shape_up->Integral();
						double y_down = params_[i].shape_down->Integral();
						// std::cout << "Var+ = " << y_up-y << std::endl;
						// std::cout << "Var- = " << y_down-y << std::endl;
						double var = (fabs(y_up-y)+fabs(y-y_down))/2.0;
						// std::cout << "MeanAbsVar = " << var << std::endl;
						if ((y_up-y >= 0.)) 	dx += (var/y)*processes_[j].rate; 
						if ((y_up-y < 0.)) 	dx -= (var/y)*processes_[j].rate; 
					}
				}
			}
			uncert_vec.push_back(dx);
		}
		double result = 0.0;
		for (unsigned i = 0; i < uncert_vec.size(); ++i) {
			result += (uncert_vec[i] * uncert_vec[i]);
		}
		result = sqrt(result);
		return result;
	}

	TH1F 	HTTSetup::GetShape() {
		TH1F shape = *(processes_[0].shape);
		// Don't count shape norm. uncertainty in the total
		double tot_uncert = this->no_shapes().GetUncertainty() / this->GetRate();
		for (unsigned i = 1; i < processes_.size(); ++i) shape.Add(processes_[i].shape);

		std::set<std::string> nuisances = this->GetNuisanceSet();
		std::map<int, std::vector<double>> uncert_map;
		for (auto& nu : nuisances) {
			std::map<int, double> dx;
			for (unsigned i = 0; i < params_.size(); ++i) {
				if (params_[i].nuisance != nu) continue;
				for (unsigned j = 0; j < processes_.size(); ++j) {
					if (processes_[j].era 			!= params_[i].era 			||
							processes_[j].channel 	!= params_[i].channel 	||
							processes_[j].category 	!= params_[i].category 	||
							processes_[j].process 	!= params_[i].process) continue;
					if (params_[i].type == "shape") {
						// Do we have the shapes?
						if (!params_[i].shape_up || !params_[i].shape_down) {
							std::cerr << "Warning in <HTTSetup::GetUncertainty>: Shape uncertainty histograms not loaded for nuisance " << params_[i].nuisance << std::endl;
							continue;
						}
						for (int k = 1; k <= params_[i].shape->GetNbinsX(); ++k) {
							double y = params_[i].shape->GetBinContent(k);
							double y_up = params_[i].shape_up->GetBinContent(k);
							double y_down = params_[i].shape_down->GetBinContent(k);
							double var = (fabs(y_up-y)+fabs(y-y_down))/2.0;
							if ((y_up-y >= 0.)) 	dx[k] += (var)*processes_[j].shape->Integral()/params_[i].shape->Integral(); 
							if ((y_up-y < 0.)) 	dx[k] -= (var)*processes_[j].shape->Integral()/params_[i].shape->Integral(); 
						}
					}
				}
			}
			for (auto it = dx.begin(); it != dx.end(); ++it) uncert_map[it->first].push_back(it->second);
		}
		std::map<int, double> result;

		for (int i = 1; i <= shape.GetNbinsX(); ++i) {
			uncert_map[i].push_back(shape.GetBinContent(i) * tot_uncert);
		}

		for (auto it = uncert_map.begin(); it != uncert_map.end(); ++it) {
			double binresult = 0.0;
			for (unsigned p = 0; p < it->second.size(); ++p) binresult += (it->second.at(p) * it->second.at(p));
			result[it->first] = sqrt(binresult);
		}
		for (int i = 1; i <= shape.GetNbinsX(); ++i) {
			shape.SetBinError(i, result[i]);
		}

		return shape;
	}

	TH1F 	HTTSetup::GetObservedShape() {
		TH1F shape = *(obs_[0].shape);
		for (unsigned i = 1; i < obs_.size(); ++i) shape.Add(obs_[i].shape);
		return shape;
	}


	int HTTSetup::ParsePulls(std::string const& filename) {
		PullsFromFile(filename, pulls_, false);
		return 0;
	}

	void HTTSetup::ApplyPulls(bool use_b_only) {
		std::map<std::string, Pull> pmap;
		for (unsigned i = 0; i < pulls_.size(); ++i) pmap[pulls_[i].name] = pulls_[i];
		
		for (unsigned i = 0; i < params_.size(); ++i) {
			auto it = pmap.find(params_[i].nuisance);
			if (it == pmap.end()) continue;

			if (params_[i].type == "lnN") {
				double yield_corr = (params_[i].value - 1.0) * ((use_b_only) ? it->second.bonly : it->second.splusb);
				// std::cout << params_[i] << ":: Pull(" << it->second.splusb << "," << it->second.splusb_err << ")" << std::endl;
				for (unsigned j = 0; j < processes_.size(); ++j) {
					if (processes_[j].era 			!= params_[i].era 			||
							processes_[j].channel 	!= params_[i].channel 	||
							processes_[j].category 	!= params_[i].category 	||
							processes_[j].process 	!= params_[i].process) continue;
					processes_[j].rate += yield_corr*processes_[j].rate;
					// Also scale the histogram if it exists
					if (processes_[j].shape) processes_[j].shape->Scale(1. + yield_corr);
				}
				params_[i].value = ((params_[i].value - 1.0) * it->second.bonly_err) + 1.0;				
			}

			if (params_[i].type == "shape") {
				for (unsigned j = 0; j < processes_.size(); ++j) {
					if (processes_[j].era 			!= params_[i].era 			||
							processes_[j].channel 	!= params_[i].channel 	||
							processes_[j].category 	!= params_[i].category 	||
							processes_[j].process 	!= params_[i].process 	||
							!params_[i].shape || !params_[i].shape_down || !params_[i].shape_up) continue;
					TH1F *central_new = (TH1F*)params_[i].shape->Clone();
					TH1F *up_new = (TH1F*)params_[i].shape->Clone();
					TH1F *down_new = (TH1F*)params_[i].shape->Clone();

					// for (unsigned k = 1; k <= central_new->GetNbinsX(); ++k) {
					// 	double shift = 0.;
					// 	if (it->second.splusb >= 0) {
					// 		shift = params_[i].shape_up->GetBinContent(k) - params_[i].shape->GetBinContent(k);
					// 	} else {
					// 		shift = params_[i].shape->GetBinContent(k) - params_[i].shape_down->GetBinContent(k);
					// 	}
					// 	central_new->SetBinContent(k, params_[i].shape->GetBinContent(k) + it->second.splusb * shift);
					// 	up_new->SetBinContent(k, params_[i].shape->GetBinContent(k) + (it->second.splusb + 1. * it->second.splusb_err) * shift);
					// 	down_new->SetBinContent(k, params_[i].shape->GetBinContent(k) + (it->second.splusb - 1. * it->second.splusb_err) * shift);
					// }

					VerticalMorph(central_new, params_[i].shape_up, params_[i].shape_down, (use_b_only ? it->second.bonly : it->second.splusb));
					VerticalMorph(up_new, params_[i].shape_up, params_[i].shape_down,   (use_b_only ? (it->second.bonly + it->second.bonly_err) : (it->second.splusb + it->second.splusb_err)));
					VerticalMorph(down_new, params_[i].shape_up, params_[i].shape_down, (use_b_only ? (it->second.bonly - it->second.bonly_err) : (it->second.splusb - it->second.splusb_err)));
					
					// std::cout << "Applying pull for nuisance: " << params_[i] << std::endl;
					// std::cout << "To Process: " << processes_[j] << std::endl;
					// std::cout << "New/Old rate ratio from morphing is " << central_new->Integral()/params_[i].shape->Integral() << std::endl;
					TH1F *transformation = (TH1F*)central_new->Clone();
					transformation->Add(params_[i].shape, -1); // Do shifted shape - original shape

					// !!!! Official code doesn't seem to do this.
					transformation->Scale(processes_[j].rate / params_[i].shape->Integral()); // Scale transformation up to current rate
					
					processes_[j].shape->Add(transformation);
					// Scan and fix negative bins
					for (int k = 1; k <= processes_[j].shape->GetNbinsX(); ++k) {
						if (processes_[j].shape->GetBinContent(k) < 0.) processes_[j].shape->SetBinContent(k, 0.);
					}
					// Fix the normalisation of the new shape to the expected shift form the template morphing 
					// But again, this isn't what the offical post-fit code does.  Visually, it just adjusts the bin contents
					// and leaves it at that.  So change is in rate is absolute not fractional.
					processes_[j].shape->Scale( processes_[j].rate * (central_new->Integral()/params_[i].shape->Integral()) / processes_[j].shape->Integral() );
					// processes_[j].shape->Scale( (processes_[j].rate + transformation->Integral()) / processes_[j].shape->Integral() );
					

					// std::cout << "Current process rate is " << processes_[j].rate << std::endl;
					processes_[j].rate = processes_[j].shape->Integral();
					// std::cout << "New process rate is " << processes_[j].rate << std::endl;
					if (transformation) delete transformation;
					params_[i].shape = central_new;
					params_[i].shape_up = up_new;
					params_[i].shape_down = down_new;
				}
			}

		}
	}

	void HTTSetup::WeightSoverB() {
		// First need to identify the unique set of (mass, era, channel, category) - these will be
		// reweighted individually. 
		std::vector<CategoryKey> keys;
		for (unsigned i = 0; i  < processes_.size(); ++i) {
			CategoryKey a(processes_[i].GetKey());
			if (std::find(keys.begin(), keys.end(), a) == keys.end()) keys.push_back(a);
		}
		for (unsigned i = 0; i < keys.size(); ++i) {
			double signal_yield = this->key_match(keys[i]).signals().GetRate();
			// std::cout << "Signal rate: " << signal_yield << std::endl;
			double backgr_yield = this->key_match(keys[i]).backgrounds().GetRate();
			// std::cout << "Background rate: " << backgr_yield << std::endl;
			double weight = signal_yield / backgr_yield;
			for (unsigned j = 0; j < obs_.size(); ++j) {
				if (obs_[j].GetKey() == keys[i]) {
					obs_[j].rate *= weight;
					if (obs_[j].shape) obs_[j].shape->Scale(weight);
				}
			}
			for (unsigned j = 0; j < processes_.size(); ++j) {
				if (processes_[j].GetKey() == keys[i]) {
					processes_[j].rate *= weight;
					if (processes_[j].shape) processes_[j].shape->Scale(weight);
				}
			}
		}
	}

	void HTTSetup::VariableRebin(std::vector<double> bins) {
		for (unsigned i = 0; i < processes_.size(); ++i) {
			if (processes_[i].shape) {
				processes_[i].shape = (TH1F*)processes_[i].shape->Rebin(bins.size()-1,"",&(bins[0]));
			}
		}
		for (unsigned i = 0; i < obs_.size(); ++i) {
			if (obs_[i].shape) obs_[i].shape = (TH1F*)obs_[i].shape->Rebin(bins.size()-1,"",&(bins[0]));
		}
		for (unsigned i = 0; i < params_.size(); ++i) {
			if (params_[i].shape) params_[i].shape = (TH1F*)params_[i].shape->Rebin(bins.size()-1,"",&(bins[0]));
			if (params_[i].shape_down) params_[i].shape_down = (TH1F*)params_[i].shape_down->Rebin(bins.size()-1,"",&(bins[0]));
			if (params_[i].shape_up) params_[i].shape_up = (TH1F*)params_[i].shape_up->Rebin(bins.size()-1,"",&(bins[0]));
		}
	}


	int HTTSetup::ParseDatacard(std::string const& filename) {
		std::vector<std::string> as_vec;
		boost::split(as_vec, filename, boost::is_any_of("/"));
		if (as_vec.size() > 0) {
			std::vector<std::string> tmp;
			boost::split(tmp, as_vec.back(), boost::is_any_of(".-_"));
			if (tmp.size() == 6) {
				std::cout << "Info in <HTTSetup::ParseDatacard>: For file " << as_vec.back() 
					<< " interpret channel, id, era and mass as \"" << tmp[1] << "\", \"" << tmp[2] << "\", \"" << tmp[3]
					<< "\" and \"" << tmp[4] << "\" respectively" << std::endl;
				return this->ParseDatacard(filename, tmp[1], boost::lexical_cast<int>(tmp[2]), tmp[3], tmp[4]);
			} else {
				std::cout << "Warning in <HTTSetup::ParseDatacard>: Unable to interpret channel and era from file name" << std::endl;
				return 1;
			}
		} else {
			std::cout << "Warning in <HTTSetup::ParseDatacard>: Unable to interpret channel and era from file name" << std::endl;
			return 1;
		}
	}

	int HTTSetup::ParseDatacard(std::string const& filename, std::string mass) {
		std::vector<std::string> as_vec;
		boost::split(as_vec, filename, boost::is_any_of("/"));
		if (as_vec.size() > 0) {
			std::vector<std::string> tmp;
			boost::split(tmp, as_vec.back(), boost::is_any_of(".-_"));
			if (tmp.size() == 5) {
				std::cout << "Info in <HTTSetup::ParseDatacard>: For file " << as_vec.back() 
					<< " interpret channel, id, era and mass as \"" << tmp[1] << "\", \"" << tmp[2] << "\", \"" << tmp[3]
					<< "\" and \"" << mass << "\" respectively" << std::endl;
				return this->ParseDatacard(filename, tmp[1], boost::lexical_cast<int>(tmp[2]), tmp[3], mass);
			} else {
				std::cout << "Warning in <HTTSetup::ParseDatacard>: Unable to interpret channel and era from file name" << std::endl;
				return 1;
			}
		} else {
			std::cout << "Warning in <HTTSetup::ParseDatacard>: Unable to interpret channel and era from file name" << std::endl;
			return 1;
		}
	}

//htt_mt.input_7TeV.root
	int HTTSetup::ParseROOTFile(std::string const& filename) {
		// Assume filename is of the form htt_$CHANNEL.inputs-$ANALYSIS-$ERA.root
		std::vector<std::string> as_vec;
		boost::split(as_vec, filename, boost::is_any_of("/"));
		if (as_vec.size() > 0) {
			std::vector<std::string> tmp;
			boost::split(tmp, as_vec.back(), boost::is_any_of(".-_"));
			if (tmp.size() == 5) {
				std::cout << "Info in <HTTSetup::ParseROOTFile>: For file " << as_vec.back() 
					<< " interpret channel and era as \"" << tmp[1] << "\" and \"" << tmp[3] << "\" respectively" << std::endl;
				return this->ParseROOTFile(filename, tmp[1], tmp[3]);
			} else {
				std::cout << "Warning in <HTTSetup::ParseROOTFile>: Unable to interpret channel and era from file name" << std::endl;
				return 1;
			}
		} else {
			std::cout << "Warning in <HTTSetup::ParseROOTFile>: Unable to interpret channel and era from file name" << std::endl;
			return 1;
		}
	}

	int HTTSetup::ParseROOTFile(const std::string & filename, std::string const& channel, std::string era) {
		// Try and find shapes for each known process
		TFile *f = new TFile(filename.c_str());
		if (!f) return 1;
		f->cd();
		for (unsigned i = 0; i < processes_.size(); ++i) {
			if (processes_[i].channel == channel && processes_[i].era == era) {
				std::string cat = processes_[i].category;
				if (!gDirectory->cd(("/"+cat).c_str())) {
					std::cerr << "Warning, category " << cat << " not found in ROOT File" << std::endl;
					continue;
				} 
				std::string name = processes_[i].process;
				if (processes_[i].process_id <= 0) name += processes_[i].mass;
				TH1F* hist = (TH1F*)gDirectory->Get(name.c_str())->Clone();
				if (!hist) {
					std::cerr << "Warning, histogram " << name << " not found in ROOT File" << std::endl;
					continue;
				}
				processes_[i].shape = hist;
			}
		}

		for (unsigned i = 0; i < obs_.size(); ++i) {
			if (obs_[i].channel == channel && obs_[i].era == era) {
				std::string cat = obs_[i].category;
				if (!gDirectory->cd(("/"+cat).c_str())) {
					std::cerr << "Warning, category " << cat << " not found in ROOT File" << std::endl;
					continue;
				} 
				std::string name = obs_[i].process;
				TH1F* hist = (TH1F*)gDirectory->Get(name.c_str())->Clone();
				if (!hist) {
					std::cerr << "Warning, histogram " << name << " not found in ROOT File" << std::endl;
					continue;
				}
				obs_[i].shape = hist;
			}
		}

		for (unsigned i = 0; i < params_.size(); ++i) {
			if (params_[i].type != "shape") continue;
			if (params_[i].channel != channel || params_[i].era != era) continue;

			std::string cat = params_[i].category;
			if (!gDirectory->cd(("/"+cat).c_str())) {
				std::cerr << "Warning, category " << cat << " not found in ROOT File" << std::endl;
				continue;
			} 
			std::string name = params_[i].process;
			if (params_[i].process_id <= 0) name += params_[i].mass;
			std::string up_name = name + "_" + params_[i].nuisance + "Up";
			std::string down_name = name + "_" + params_[i].nuisance + "Down";

			TH1F* hist = (TH1F*)gDirectory->Get(name.c_str());
			if (!hist) {
				std::cerr << "Warning, histogram " << name << " not found in ROOT File" << std::endl;
				continue;
			} else {
				params_[i].shape = (TH1F*)hist->Clone();	
			}

			TH1F* up_hist = (TH1F*)gDirectory->Get(up_name.c_str());
			if (!up_hist) {
				std::cerr << "Warning, histogram " << up_name << " not found in ROOT File" << std::endl;
				continue;
			} else {
				params_[i].shape_up = (TH1F*)up_hist->Clone();	
			}

			TH1F* down_hist = (TH1F*)gDirectory->Get(down_name.c_str());
			if (!down_hist) {
				std::cerr << "Warning, histogram " << down_name << " not found in ROOT File" << std::endl;
				continue;
			} else {
				params_[i].shape_down = (TH1F*)down_hist->Clone();	
			}

		}
		return 0;

		// Try and find shapes for each known shape nuisance
	}

	int HTTSetup::ParseDatacard(const std::string & filename, std::string const& channel, int category_id, std::string era, std::string mass) {

		std::vector<std::string> lines = ParseFileLines(filename);

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
		for (unsigned i = 0; i < words.size(); ++i) {
			// Ignore line if it only has one word
			if (words[i].size() <= 1) continue;

			// Want to check this line and the previous one, so need i >= 1.
			// If the first word on this line is "observation" and "bin" on
			// the previous line then we've found the entries for data, and
			// can add Observation objects
			if (i >= 1) {
				if (	words[i][0] 		== "observation" && 
							words[i-1][0] 	== "bin" && 
							words[i].size() == words[i-1].size()) {
					for (unsigned p = 1; p < words[i].size(); ++p) {
						obs_.push_back(Observation());
						obs_.back().channel = channel;
						obs_.back().category_id = category_id;
						obs_.back().era = era;
						obs_.back().category = words[i-1][p];
						obs_.back().process = "data_obs";
						obs_.back().rate = boost::lexical_cast<double>(words[i][p]);
						obs_.back().mass = mass;
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
						processes_.push_back(Process());
						processes_.back().channel = channel;
						processes_.back().category_id = category_id;
						processes_.back().era = era;
						processes_.back().category = words[i-3][p];
						processes_.back().process = words[i-1][p];
						processes_.back().process_id = boost::lexical_cast<int>(words[i-2][p]);
						processes_.back().rate = boost::lexical_cast<double>(words[i][p]);
						processes_.back().mass = mass;
					}
					r = i;
					start_nuisance_scan = true;
				}
			}

			if (start_nuisance_scan && words[i].size()-1 == words[r].size()) {
				for (unsigned p = 2; p < words[i].size(); ++p) {
					if (words[i][p] == "-") continue;
					if (words[i][0].at(0) == '#') continue;
					params_.push_back(Nuisance());
					params_.back().channel = channel;
					params_.back().category_id = category_id;
					params_.back().category = words[r-3][p-1];
					params_.back().era = era;
					params_.back().process = words[r-1][p-1];
					params_.back().process_id = boost::lexical_cast<int>(words[r-2][p-1]);
					params_.back().nuisance = words[i][0];
					params_.back().type = words[i][1];
					params_.back().value = boost::lexical_cast<double>(words[i][p]);
					params_.back().mass = mass;
				}
			}
		}
		return 0;
	}

	bool HTTSetup::HasProcess(std::string const& process) const {
		for (unsigned i = 0; i < processes_.size(); ++i) {
			if (processes_[i].process == process) return true;
		}
		return false;
	}


}