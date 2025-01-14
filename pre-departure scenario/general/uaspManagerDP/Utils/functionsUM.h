#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <string>
#include "../../model/UATMTraits.h"
#include <vector>

struct requestInfo
{
    std::string request_id;
    std::string flight_id;
    std::string departure_skyport_id;
    std::string destination_skyport_id;
    std::string departure_time;
    std::string tolpad_id;
    std::string pilot_id;
    std::string evtol_id;
};

struct TolPad
{
    std::string resource_id;
    std::string skyport_id;
    std::string available;
    std::string availability_time;
};

bool checkAvailability(const std::string &resourceFile, std::string &tolPadID);
std::vector<requestInfo> read_requests_from_file(const std::string &filename);
std::string getAndUpdateFlightIDWithEmptyTolPad(const std::string &filename, const std::string &tolPadID);
bool checkWeatherConditions(const std::string &weatherFile, const std::string &location, std::string &weatherID);
bool checkFlowConditions(const std::string &flowsFile, const std::string &area, std::string &flowsID);
bool checkRestrictionConditions(const std::string &restrictionsFile, const std::string &area, std::string &restrictionID);
bool removeAssignedTolPads(const std::string &tolPadsFile, const std::string &tolPadID);
std::vector<TolPad> read_tolpads(const std::string &resourceFile);
std::string getCurrentTime();

#endif // FUNCTIONS_H
