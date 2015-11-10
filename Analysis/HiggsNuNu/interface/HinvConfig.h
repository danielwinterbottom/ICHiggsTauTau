#ifndef ICHiggsTauTau_HiggsNuNu_HinvConfig_h
#define ICHiggsTauTau_HiggsNuNu_HinvConfig_h


#include <vector>
#include <set>
#include <map>
#include <string>
#include <iostream>
#include "boost/assign/list_of.hpp"

namespace ic {


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
		ichep2012,			// ICHEP 2012 anaylsis strategy, e.g. PF MET, VBF MVA
		hcp2012,				// HCP 2012 strategy, MVA MET, cut-based VBF
		moriond2013			// Moriond 2013 strategy, currently same as HCP
	};
};
typedef safe_enum<strategy_def> strategy;

inline std::string Strategy2String(strategy const& in) {
	static std::map<strategy, std::string> conv = boost::assign::map_list_of
		(strategy::ichep2012, 	"ichep2012")
		(strategy::hcp2012, 		"hcp2012")
		(strategy::moriond2013, "moriond2013");
	if (conv.find(in) != conv.end()) {
		return (conv[in]);
	} else {
		std::cerr << "Enum2String Error, enum not recognised" << std::endl;
		throw;
	}
}

inline strategy String2Strategy(std::string const& in) {
	static std::map<std::string, strategy> conv = boost::assign::map_list_of
	("ichep2012", 	strategy::ichep2012)
	("hcp2012", 		strategy::hcp2012)
	("moriond2013", strategy::moriond2013);
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
		data_2012_ichep,	// ICHEP dataset for 2012: A+B re-reco
		data_2012_hcp,		// HCP dataset for 2012: A+B re-reco C prompt
		data_2012_moriond,// Moriond dataset for 2012: A+B re-reco, C+D prompt
		data_2012_rereco,// Rereco dataset for 2012
		data_2012_donly, 	// 2012D prompt only
		data_2015_50ns, 	// 2015 50ns
		data_2015_25ns 	// 2015 25ns

	};
};
typedef safe_enum<era_def> era;

inline std::string Era2String(era const& in) {
	static std::map<era, std::string> conv = boost::assign::map_list_of
		(era::data_2011, 					"data_2011")
		(era::data_2012_ichep, 		"data_2012_ichep")
		(era::data_2012_hcp, 			"data_2012_hcp")
		(era::data_2012_moriond, 	"data_2012_moriond")
		(era::data_2012_rereco, 	"data_2012_rereco")
		(era::data_2012_donly, 		"data_2012_donly")
		(era::data_2015_50ns, 		"data_2015_50ns")
		(era::data_2015_25ns, 		"data_2015_25ns");
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
	("data_2012_ichep", 	era::data_2012_ichep)
	("data_2012_hcp", 		era::data_2012_hcp)
	("data_2012_moriond", era::data_2012_moriond)
	("data_2012_rereco", era::data_2012_rereco)
	("data_2012_donly", 	era::data_2012_donly)
	("data_2015_50ns", 	era::data_2015_50ns)
	("data_2015_25ns", 	era::data_2015_25ns);
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
		summer12_52X,			// 52X MC
		phys14_72X,			// 72X MC
		spring15_74X			// 74X MC
	};
};
typedef safe_enum <mc_def> mc;

inline std::string MC2String(mc const& in) {
	static std::map<mc, std::string> conv = boost::assign::map_list_of
		(mc::fall11_42X, "fall11_42X")
		(mc::summer12_53X, "summer12_53X")
		(mc::summer12_52X, "summer12_52X")
		(mc::phys14_72X, "phys14_72X")
		(mc::spring15_74X, "spring15_74X");

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
	("summer12_52X",	mc::summer12_52X)
	("phys14_72X",	        mc::phys14_72X)
	("spring15_74X",	mc::spring15_74X);

	if (conv.find(in) != conv.end()) {
		return (conv.find(in)->second);
	} else {
		std::cerr << "String2Enum Error, string " << in << " not recognised" << std::endl;
		throw;
	}
}

struct channel_def {
	enum type {
		nunu,							
		munu,
		enu,
		taunu,
		ee,
		mumu,
		emu,
		tautau,
		etau,
		mutau,
		nunuiglep,
		nunulowmet,
		nunulowmetiglep
	};
};

 typedef safe_enum<channel_def> channel;
 
 inline std::string Channel2String(channel const& in) {
   static std::map<channel, std::string> conv = boost::assign::map_list_of
     (channel::nunu, "nunu")
     (channel::munu, "munu")
     (channel::enu, "enu")
     (channel::taunu, "taunu")
     (channel::ee, "ee")
     (channel::mumu, "mumu")
     (channel::emu, "emu")
     (channel::tautau, "tautau")
     (channel::etau, "etau")
     (channel::mutau, "mutau")
     (channel::nunuiglep, "nunuiglep")
     (channel::nunulowmet, "nunulowmet")
     (channel::nunulowmetiglep, "nunulowmetiglep")
     ;

   if (conv.find(in) != conv.end()) {
     return (conv[in]);
   } else {
     std::cerr << "Enum2String Error, enum not recognised" << std::endl;
     throw;
   }
 }
 inline channel String2Channel(std::string const& in) {
   static std::map<std::string, channel> conv = boost::assign::map_list_of
     ("nunu", channel::nunu)
     ("munu", channel::munu)
     ("enu", channel::enu)
     ("taunu", channel::taunu)
     ("ee", channel::ee)
     ("mumu", channel::mumu)
     ("emu", channel::emu)
     ("tautau", channel::tautau)
     ("etau", channel::etau)
     ("mutau", channel::mutau)
     ("nunuiglep", channel::nunuiglep)
     ("nunulowmet", channel::nunulowmet)
     ("nunulowmetiglep", channel::nunulowmetiglep)
     ;
   
   if (conv.find(in) != conv.end()) {
     return (conv.find(in)->second);
   } else {
     std::cerr << "String2Enum Error, string " << in << " not recognised" << std::endl;
     throw;
   }
 }

}

#endif
