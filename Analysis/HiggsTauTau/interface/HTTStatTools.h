#ifndef ICHiggsTauTau_HiggsTauTau_HTTStatTools_h
#define ICHiggsTauTau_HiggsTauTau_HTTStatTools_h

#include <vector>
#include <set>
#include <map>
#include <string>
#include <iostream>
#include "TH1F.h"
#include "boost/assign/list_of.hpp"

namespace ic {

	// How to handle shape nuisances
	/*
		Each nuisance has the central, up and down shapes.  These never change or get scaled
		When determining the uncertainty, take the average fractional change in yield.

		Applying pulls.
		For each pull we apply, we must update the process rate and it's shape (if loaded).
		For lnN this is simple, just scale the shape the same way as the rate
		For shape, Determine the absolute shift for each bin using the nuisance shapes.  
		This "transformation" TH1F should then be scaled by a factor that takes us from the
		nuisance central rate to the process central rate.  The transformation is then applied
		to the process shape.  By construction the relative change in rate should be that 
		from simply integrating the post and pre-transformed shapes in the nuisance: to be checked
		We must finally switch the nuisance shapes for the post-fit versions. 

		Bin-wise uncertainty with shapes.  Taken as average fractional on bin yield, 
		should be simple enough.
	*/

struct Pull {
	std::string name;
	double 			prefit;
	double 			prefit_err;
	double 			bonly;
	double 			bonly_err;
	double 			splusb;
	double 			splusb_err;
	double 			rho;
	void Print() const;
};

struct CategoryKey {
	std::string 	mass;
	std::string   era;
	std::string 	channel;
	std::string 	category;
	CategoryKey(std::string m, std::string e, std::string ch, std::string ca) {
		mass = m;
		era = e;
		channel = ch;
		category = ca;
	}
	bool operator== (const CategoryKey& other) const {
		return (mass==other.mass && era==other.era && channel==other.channel && category==other.category);
	}
};

struct Nuisance {
	// Common attributes
	std::string 	mass;
	std::string   era;
	std::string 	channel;
	std::string 	category;
	int 					category_id;
	std::string   process;

	// Filterable attribtures
	std::string 	nuisance;
	std::string 	type;

	int 					process_id;
	double 				value;
	TH1F *				shape;
	TH1F *				shape_up;
	TH1F *				shape_down;
	friend std::ostream& operator<< (std::ostream &out, Nuisance &val);
	CategoryKey GetKey() const;
	static void PrintHeader(std::ostream &out);
};

struct Process {
	// Common attributes
	std::string 	mass;
	std::string   era;
	std::string 	channel;
	std::string 	category;
	int 					category_id;
	std::string   process;

	// Filterable attribtures
	int 					process_id;

	double 				rate;
	TH1F *				shape;
	friend std::ostream& operator<< (std::ostream &out, Process &val);
	CategoryKey GetKey() const;
	static void PrintHeader(std::ostream &out);
};

struct Observation {
	// Common attributes 
	std::string 	mass;
	std::string   era;
	std::string 	channel;
	std::string 	category;
	int 					category_id;
	std::string   process;

	double 				rate;
	TH1F *				shape;
	friend std::ostream& operator<< (std::ostream &out, Observation &val);
	CategoryKey GetKey() const;
	static void PrintHeader(std::ostream &out);
};

class HTTSetup {
	private:
		std::vector<Nuisance> params_;
		std::vector<Process> processes_;
		std::vector<Observation> obs_;
		std::vector<Pull> pulls_;
		bool ignore_nuisance_correlations_;

	public:
		int ParseDatacard(std::string const& filename, std::string const& channel, int category_id, std::string era, std::string mass);
		int ParseROOTFile(std::string const& filename, std::string const& channel, std::string era);
		int ParseDatacard(std::string const& filename);
		int ParseROOTFile(std::string const& filename);
		int ParsePulls(std::string const& filename);
		HTTSetup();
		void ApplyPulls();
		void WeightSoverB();
		inline void AddProcess(Process proc) { processes_.push_back(proc); }
		void VariableRebin(std::vector<double> bins);
		HTTSetup & PrintAll();
		HTTSetup process(std::vector<std::string> const& process) const;
		HTTSetup key_match(CategoryKey const & keyval) const;
		HTTSetup category_id(std::vector<int> const& id) const;
		HTTSetup no_shapes() const;
		HTTSetup signals() const;
		HTTSetup backgrounds() const;
		HTTSetup nuisance(std::vector<std::string> const& nuisance) const;
		double GetRate();
		double GetObservedRate();
		double GetUncertainty();
		inline void SetIgnoreNuisanceCorrelations(bool const& val) {ignore_nuisance_correlations_ = val; }
		TH1F 	GetShape();
		TH1F 	GetObservedShape();
		std::set<std::string> GetNuisanceSet();
};

void PullsFromFile(std::string const& filename, std::vector<ic::Pull> & pullvec, bool verbose);

bool BvsSBComparator(Pull const& pull1, Pull const& pull2);

}

#endif