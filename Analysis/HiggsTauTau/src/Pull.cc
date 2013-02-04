#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/Pull.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnRootTools.h"
#include <vector>
#include <map>
#include <string>
#include <iostream>
#include "boost/lexical_cast.hpp"
#include "boost/algorithm/string.hpp"
#include "boost/format.hpp"

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

}

