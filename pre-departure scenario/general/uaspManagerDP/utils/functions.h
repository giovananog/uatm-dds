#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <string>
#include "../../model/UATMTraits.h"
#include <vector>

bool checkAvailability(const std::string &resourceFile, std::string &tolPadID);
std::vector<UATM::flightAuthorization> read_requests_from_file(const std::string& filename);
std::string getAndUpdateFlightIDWithEmptyTolPad(const std::string &filename, const std::string &tolPadID);
bool checkWeatherConditions(const std::string& weatherFile, const std::string& location, std::string& weatherID);
bool checkFlowConditions(const std::string& flowsFile, const std::string& area, std::string& flowsID);
bool checkRestrictionConditions(const std::string& restrictionsFile, const std::string& area, std::string& restrictionID);


#endif // FUNCTIONS_H
