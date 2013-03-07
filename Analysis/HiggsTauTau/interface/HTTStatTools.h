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
	TH1F *				shape_up;
	TH1F *				shape_down;
	friend std::ostream& operator<< (std::ostream &out, Nuisance &val);
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
	static void PrintHeader(std::ostream &out);
};

class HTTSetup {
	private:
		std::vector<Nuisance> params_;
		std::vector<Process> processes_;
		std::vector<Observation> obs_;
		std::vector<Pull> pulls_;

	public:
		int ParseDatacard(std::string const& filename, std::string const& channel, int category_id, std::string era, std::string mass);
		int ParseROOTFile(std::string const& filename, std::string const& channel, std::string era);
		int ParseDatacard(std::string const& filename);
		int ParseROOTFile(std::string const& filename);
		int ParsePulls(std::string const& filename);
		void ApplyPulls();
		HTTSetup & PrintAll();
		HTTSetup process(std::vector<std::string> const& process) const;
		HTTSetup nuisance(std::vector<std::string> const& nuisance) const;
		double GetRate();
		double GetUncertainty();
		std::set<std::string> GetNuisanceSet();
};

void PullsFromFile(std::string const& filename, std::vector<ic::Pull> & pullvec, bool verbose);

bool BvsSBComparator(Pull const& pull1, Pull const& pull2);

}

#endif