#include <string>
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/CombineHarvester.h"

int main() {

	ch::CombineHarvester h;
	h.ParseDatacard("paper_final/125/htt_mt_1_7TeV.txt", "125", "7TeV", "mt", 1);
	h.PrintAll();
}
