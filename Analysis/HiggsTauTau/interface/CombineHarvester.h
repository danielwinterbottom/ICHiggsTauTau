#ifndef ICHiggsTauTau_HiggsTauTau_CombineHarvester_h
#define ICHiggsTauTau_HiggsTauTau_CombineHarvester_h

// #include <vector>
// #include <set>
#include <map>
#include <string>
#include <iostream>
#include <memory>
#include "TH1F.h"
#include "TGraphAsymmErrors.h"
#include "TFile.h"

namespace ch {

	struct Category {
		std::string 	mass;
		std::string   era;
		std::string 	channel;
		std::string 	category;
		int 					category_id;
		Category() {;}
		Category(std::string m, std::string e, std::string ch, std::string ca, int id) :
			mass (m), era(e), channel(ch), category(ca), category_id(id) {
		}
		bool operator== (const Category& other) const {
			return (
				category_id == other.category_id &&
				mass        == other.mass && 
				era         == other.era && 
				channel     == other.channel && 
				category    == other.category);
		}
	};

	struct Nuisance {
		Category      category;

		std::string 	nuisance;
		std::string   process;
		int 					process_id;
		std::string 	type;
		double 				value_u;
		double 				value_d;
		bool 					asymm;
		TH1*				  shape;
		TH1*				  shape_u;
		TH1*				  shape_d;

		friend std::ostream& operator<< (std::ostream &out, Nuisance &val);
		static std::ostream& PrintHeader(std::ostream &out);
	};

	struct Parameter {
		std::string name;
		double 			value;
		double 			error_u;
		double 			error_d;

		friend std::ostream& operator<< (std::ostream &out, Parameter &val);
		static std::ostream& PrintHeader(std::ostream &out);
	};

	struct Observation {
		Category            category;

		std::string   			process;
		double 							rate;
		TH1*								shape;
		TGraphAsymmErrors* 	errors;

		friend std::ostream& operator<< (std::ostream &out, Observation &val);
		static std::ostream& PrintHeader(std::ostream &out);
	};

	struct Process {
		Category      category;

		std::string   process;
		int 					process_id;
		double 				rate;
		TH1*	  			shape;

		friend std::ostream& operator<< (std::ostream &out, Process &val);
		static std::ostream& PrintHeader(std::ostream &out);
	};

// struct Pull {
// 	std::string name;
// 	double 			prefit;
// 	double 			prefit_err;
// 	double 			bonly;
// 	double 			bonly_err;
// 	double 			splusb;
// 	double 			splusb_err;
// 	double 			rho;
// 	void Print() const;
// };

	/*

		CombineHarvester h;
		h.ParseDatacard("datacard.txt");

		CombineHarvester p = h; // copies all underlying
		CombineHarvester p = h.processes{"VH"}; // h.processes makes a "shallow" copy

		Have a private copy constructor and assignment operator




	*/

class CombineHarvester {
	private:
		CombineHarvester(CombineHarvester const& rhs);
		CombineHarvester& operator=(CombineHarvester rhs);
		std::vector<std::shared_ptr<Nuisance>> nuisances_;
		std::vector<std::shared_ptr<Process>> processes_;
		std::vector<std::shared_ptr<Observation>> obs_;
		std::map<std::string, std::shared_ptr<Parameter>> params_; 
		// std::vector<Pull> pulls_;
		// bool ignore_nuisance_correlations_;
		struct ShapeMap {
			std::string process;
			std::string category;
			TFile *file;
			std::string pattern;
			std::string syst_pattern;
		};

		typedef std::vector<std::pair<std::string, std::string>> StrPairVec;
		StrPairVec GenerateShapeMapAttempts(std::string process, std::string category);

	public:
		CombineHarvester() {;}
		int ParseDatacard(std::string const& filename,  
			std::string mass, 
			std::string era,
			std::string const& channel, 
			int category_id);
		CombineHarvester & PrintAll();
		// int ParseROOTFile(std::string const& filename, std::string const& channel, std::string era);
		// int ParseDatacard(std::string const& filename);
		// int ParseDatacard(std::string const& filename, std::string mass);
		// int ParseROOTFile(std::string const& filename);
		// int ParsePulls(std::string const& filename);
		// void ApplyPulls(bool use_b_only = false);
		// void WeightSoverB();
		// inline void AddProcess(Process proc) { processes_.push_back(proc); }
		// void VariableRebin(std::vector<double> bins);
		// CombineHarvester process(std::vector<std::string> const& process) const;
		// CombineHarvester era(std::vector<std::string> const& era) const;
		// CombineHarvester key_match(CategoryKey const & keyval) const;
		// CombineHarvester category_id(std::vector<int> const& id) const;
		// CombineHarvester no_shapes() const;
		// CombineHarvester backgrounds() const;
		// CombineHarvester nuisance(std::vector<std::string> const& nuisance) const;
		// CombineHarvester nuisance_pred(std::function<bool(Nuisance const&)> fn) const;
		// double GetRate();
		// double GetObservedRate();
		// double GetUncertainty();
		// inline void SetIgnoreNuisanceCorrelations(bool const& val) {ignore_nuisance_correlations_ = val; }
		// TH1F 	GetShape();
		// TH1F 	GetObservedShape();
		// TGraphAsymmErrors GetObservedShapeErrors();
		// std::set<std::string> GetNuisanceSet();
		// bool HasProcess(std::string const& process) const;
		// void ScaleProcessByEra(std::string const& process, std::string const& era, double scale);
		// std::pair<double, int> GetPullsChi2(bool splusb) const;
};

// void PullsFromFile(std::string const& filename, std::vector<ic::Pull> & pullvec, bool verbose);

// bool BvsSBComparator(Pull const& pull1, Pull const& pull2);

// TGraphAsymmErrors BuildPoissonErrors(TH1F const& hist);

}

#endif