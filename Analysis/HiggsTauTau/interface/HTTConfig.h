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
    fall15,    //Strategy for running on fall15 samples
    mssmspring16,    //Strategy for running on spring16 samples, MSSM analysis
    smspring16    //Strategy for running on spring16 samples, SM analysis
	};
};
typedef safe_enum<strategy_def> strategy;

inline std::string Strategy2String(strategy const& in) {
	static std::map<strategy, std::string> conv = boost::assign::map_list_of
    (strategy::fall15, "fall15")
    (strategy::mssmspring16, "mssmspring16")
    (strategy::smspring16, "smspring16");
	if (conv.find(in) != conv.end()) {
		return (conv[in]);
	} else {
		std::cerr << "Enum2String Error, enum not recognised" << std::endl;
		throw;
	}
}

inline strategy String2Strategy(std::string const& in) {
	static std::map<std::string, strategy> conv = boost::assign::map_list_of
  ("fall15", strategy::fall15)
  ("mssmspring16", strategy::mssmspring16)
  ("smspring16", strategy::smspring16);
	if (conv.find(in) != conv.end()) {
		return (conv.find(in)->second);
	} else {
		std::cerr << "Enum2String Error, string " << in << " not recognised" << std::endl;
		throw;
	}
}

struct era_def {
	enum type {
		data_2015,
		data_2016
	};
};
typedef safe_enum<era_def> era;

inline std::string Era2String(era const& in) {
	static std::map<era, std::string> conv = boost::assign::map_list_of
		(era::data_2015, 	        "data_2015")
		(era::data_2016, 	        "data_2016");
	if (conv.find(in) != conv.end()) {
		return (conv[in]);
	} else {
		std::cerr << "Enum2String Error, enum not recognised" << std::endl;
		throw;
	}
}

inline era String2Era(std::string const& in) {
	static std::map<std::string, era> conv = boost::assign::map_list_of
	("data_2015", 	era::data_2015)
	("data_2016", 	era::data_2016);
	if (conv.find(in) != conv.end()) {
		return (conv.find(in)->second);
	} else {
		std::cerr << "String2Enum Error, string " << in << " not recognised" << std::endl;
		throw;
	}
}

struct mc_def {
	enum type {
    fall15_76X,      //76X MC 
    spring16_80X      //76X MC 
	};
};
typedef safe_enum <mc_def> mc;

inline std::string MC2String(mc const& in) {
	static std::map<mc, std::string> conv = boost::assign::map_list_of
    (mc::fall15_76X, "fall15_76X")
    (mc::spring16_80X, "spring16_80X");

	if (conv.find(in) != conv.end()) {
		return (conv[in]);
	} else {
		std::cerr << "Enum2String Error, enum not recognised" << std::endl;
		throw;
	}
}
inline mc String2MC(std::string const& in) {
	static std::map<std::string, mc> conv = boost::assign::map_list_of
  ("fall15_76X", mc::fall15_76X)
  ("spring16_80X", mc::spring16_80X);


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
    tpzee,
    tpzmm,
    wmnu
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
		(channel::tpzee, "tpzee")
		(channel::tpzmm, "tpzmm")
		(channel::wmnu, "wmnu");

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
	("tpzee", channel::tpzee)
	("tpzmm", channel::tpzmm)
	("wmnu", channel::wmnu);

	if (conv.find(in) != conv.end()) {
		return (conv.find(in)->second);
	} else {
		std::cerr << "String2Enum Error, string " << in << " not recognised" << std::endl;
		throw;
	}
}

struct mcorigin_def{
  enum type {
     promptE,
     promptMu,
     tauE,
     tauMu,
     tauHad,
     fake,
     hadE,
     hadMu
  };
};

typedef safe_enum<mcorigin_def> mcorigin;

inline unsigned MCOrigin2UInt(mcorigin const& in) {
  static std::map<mcorigin, unsigned> conv = boost::assign::map_list_of
    (mcorigin::promptE, 1)
    (mcorigin::promptMu, 2)
    (mcorigin::tauE,3)
    (mcorigin::tauMu,4)
    (mcorigin::tauHad,5)
    (mcorigin::fake,6)
    (mcorigin::hadE,7)
    (mcorigin::hadMu,8);

  if (conv.find(in) != conv.end()) {
    return (conv.find(in)->second);
  } else {
    std::cerr << "Enum2UInt error, enum not recognised " <<std::endl;
    throw;
  }
}

inline mcorigin UInt2MCOrigin(unsigned const&in){
   static std::map<unsigned, mcorigin> conv = boost::assign::map_list_of
   (1, mcorigin::promptE)
   (2, mcorigin::promptMu)
   (3, mcorigin::tauE)
   (4, mcorigin::tauMu)
   (5, mcorigin::tauHad)
   (6, mcorigin::fake)
   (7, mcorigin::hadE)
   (8, mcorigin::hadMu);

 if (conv.find(in) != conv.end()){
   return (conv.find(in)->second);
 } else {
   std::cerr << "UInt2Enum Error, unsigned " << in << " not recognised " <<std::endl;
   throw;
  }
} 

}

#endif
