#ifndef ICHiggsTauTau_HiggsTauTau_Pull_h
#define ICHiggsTauTau_HiggsTauTau_Pull_h

#include <vector>
#include <set>
#include <map>
#include <string>
#include <iostream>
#include "boost/assign/list_of.hpp"

namespace ic {

struct Pull {
	std::string name;
	double prefit;
	double prefit_err;
	double bonly;
	double bonly_err;
	double splusb;
	double splusb_err;
	double rho;

	void Print() const;
};


void PullsFromFile(std::string const& filename, std::vector<ic::Pull> & pullvec, bool verbose);

bool BvsSBComparator(Pull const& pull1, Pull const& pull2);

}

#endif