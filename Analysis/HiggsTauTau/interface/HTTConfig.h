#ifndef ICHiggsTauTau_HiggsTauTau_HTTConfig_h
#define ICHiggsTauTau_HiggsTauTau_HTTConfig_h


#include <vector>
#include <set>
#include <map>
#include <string>
#include <iostream>
#include "boost/assign/list_of.hpp"

namespace ic {

inline std::string param_fmt() {
	return "%-25s %-40s\n";
}

inline std::string value_fmt() {
	return "%-20s %-9.2f +/-   %-9.2f";
}


// We define a "type-safe" enum class
// It stops us from doing something like Color c = Shape::square
template<typename def, typename inner = typename def::type>
class safe_enum : public def
{
  typedef typename def::type type;
  inner val;
 
public:
 
  safe_enum(type v) : val(v) {}
  inner underlying() const { return val; }
 
  bool operator == (const safe_enum & s) const { return this->val == s.val; }
  bool operator != (const safe_enum & s) const { return this->val != s.val; }
  bool operator <  (const safe_enum & s) const { return this->val <  s.val; }
  bool operator <= (const safe_enum & s) const { return this->val <= s.val; }
  bool operator >  (const safe_enum & s) const { return this->val >  s.val; }
  bool operator >= (const safe_enum & s) const { return this->val >= s.val; }
};


struct strategy_def {
	enum type {
		hcp2012,				// HCP 2012 strategy, MVA MET, cut-based VBF
		moriond2013,		// Moriond 2013 strategy, currently same as HCP
		paper2013				// Strategy for the final paper in 2013
	};
};
typedef safe_enum<strategy_def> strategy;

inline std::string Strategy2String(strategy const& in) {
	static std::map<strategy, std::string> conv = boost::assign::map_list_of
		(strategy::hcp2012, 		"hcp2012")
		(strategy::moriond2013, "moriond2013")
		(strategy::paper2013, 	"paper2013");
	if (conv.find(in) != conv.end()) {
		return (conv[in]);
	} else {
		std::cerr << "Enum2String Error, enum not recognised" << std::endl;
		throw;
	}
}

inline strategy String2Strategy(std::string const& in) {
	static std::map<std::string, strategy> conv = boost::assign::map_list_of
	("hcp2012", 		strategy::hcp2012)
	("moriond2013", strategy::moriond2013)
	("paper2013", 	strategy::paper2013);
	if (conv.find(in) != conv.end()) {
		return (conv.find(in)->second);
	} else {
		std::cerr << "Enum2String Error, string " << in << " not recognised" << std::endl;
		throw;
	}
}

struct era_def {
	enum type {
		data_2011,				// Entire 2011 dataset, mixture of prompt and re-reco
		data_2012_hcp,		// HCP dataset for 2012: A+B re-reco C prompt
		data_2012_moriond,// Moriond dataset for 2012: A+B re-reco, C+D prompt
		data_2012_donly, 	// 2012D prompt only
		data_2012_rereco,
		data_2015
	};
};
typedef safe_enum<era_def> era;

inline std::string Era2String(era const& in) {
	static std::map<era, std::string> conv = boost::assign::map_list_of
		(era::data_2011, 					"data_2011")
		(era::data_2012_hcp, 			"data_2012_hcp")
		(era::data_2012_moriond, 	"data_2012_moriond")
		(era::data_2012_donly, 		"data_2012_donly")
		(era::data_2012_rereco, 	"data_2012_rereco")
		(era::data_2015, 	        "data_2015");
	if (conv.find(in) != conv.end()) {
		return (conv[in]);
	} else {
		std::cerr << "Enum2String Error, enum not recognised" << std::endl;
		throw;
	}
}

inline era String2Era(std::string const& in) {
	static std::map<std::string, era> conv = boost::assign::map_list_of
	("data_2011",					era::data_2011)
	("data_2012_hcp", 		era::data_2012_hcp)
	("data_2012_moriond", era::data_2012_moriond)
	("data_2012_donly", 	era::data_2012_donly)
	("data_2012_rereco", 	era::data_2012_rereco)
	("data_2015", 	era::data_2015);
	if (conv.find(in) != conv.end()) {
		return (conv.find(in)->second);
	} else {
		std::cerr << "String2Enum Error, string " << in << " not recognised" << std::endl;
		throw;
	}
}

struct mc_def {
	enum type {
		fall11_42X,				// 42X MC
		summer12_53X,			// 53X MC
		summer12_52X			// 52X MC
	};
};
typedef safe_enum <mc_def> mc;

inline std::string MC2String(mc const& in) {
	static std::map<mc, std::string> conv = boost::assign::map_list_of
		(mc::fall11_42X, "fall11_42X")
		(mc::summer12_53X, "summer12_53X")
		(mc::summer12_52X, "summer12_52X");

	if (conv.find(in) != conv.end()) {
		return (conv[in]);
	} else {
		std::cerr << "Enum2String Error, enum not recognised" << std::endl;
		throw;
	}
}
inline mc String2MC(std::string const& in) {
	static std::map<std::string, mc> conv = boost::assign::map_list_of
	("fall11_42X",		mc::fall11_42X)
	("summer12_53X",	mc::summer12_53X)
	("summer12_52X",	mc::summer12_52X);

	if (conv.find(in) != conv.end()) {
		return (conv.find(in)->second);
	} else {
		std::cerr << "String2Enum Error, string " << in << " not recognised" << std::endl;
		throw;
	}
}

struct channel_def {
	enum type {
		et,							
		mt,		
		em,
		tt,
		zee,
		zmm,
		mtmet,
		etmet
	};
};
typedef safe_enum<channel_def> channel;

inline std::string Channel2String(channel const& in) {
	static std::map<channel, std::string> conv = boost::assign::map_list_of
		(channel::et, "et")
		(channel::mt, "mt")
		(channel::em, "em")
		(channel::tt, "tt")
		(channel::zee, "zee")
		(channel::zmm, "zmm")
		(channel::etmet, "etmet")
		(channel::mtmet, "mtmet");

	if (conv.find(in) != conv.end()) {
		return (conv[in]);
	} else {
		std::cerr << "Enum2String Error, enum not recognised" << std::endl;
		throw;
	}
}
inline channel String2Channel(std::string const& in) {
	static std::map<std::string, channel> conv = boost::assign::map_list_of
	("et", channel::et)
	("mt", channel::mt)
	("em", channel::em)
	("tt", channel::tt)
	("zee", channel::zee)
	("zmm", channel::zmm)
	("etmet", channel::etmet)
	("mtmet", channel::mtmet);

	if (conv.find(in) != conv.end()) {
		return (conv.find(in)->second);
	} else {
		std::cerr << "String2Enum Error, string " << in << " not recognised" << std::endl;
		throw;
	}
}

}

#endif
