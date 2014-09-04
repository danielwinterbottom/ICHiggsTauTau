#ifndef ICHiggsTauTau_Utilities_JsonTools_h
#define ICHiggsTauTau_Utilities_JsonTools_h

#include <string>
#include "Utilities/interface/json.h"

namespace ic {
/**
 * @brief Extracts a Json::Value from the specified input file
 *
 * @param file The full path to the input json file
 */
Json::Value ExtractJsonFromFile(std::string const& file);

/**
 * @brief Updates the values in one json from the values in another
 * @details All the values in *b* are entered into *a*, overwritting any already
 * defined in *a*.
 *
 * @param a The Json::Value to be updated
 * @param b The Json::Value containing the updates
 */
void UpdateJson(Json::Value& a, Json::Value const& b);
}
#endif
