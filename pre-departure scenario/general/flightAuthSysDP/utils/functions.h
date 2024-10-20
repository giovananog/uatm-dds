#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "../../model/UATMTraits.h"
#include <string>
#include <vector>

struct flightRequestInfo {
    std::string auth_request_id;
    std::string flight_id;
    std::string departure_skyport_id;
    std::string destination_skyport_id;
    std::string departure_time;
    std::string pilot_id;
    std::string evtol_id;
};

std::vector<flightRequestInfo> readRequestsFromFile(const std::string& filename);

#endif // FUNCTIONS_H
