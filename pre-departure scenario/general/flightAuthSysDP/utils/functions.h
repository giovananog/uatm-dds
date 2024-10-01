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
    int sent_req;
};

std::vector<flightRequestInfo> readRequestsFromFile(const std::string& filename);
void updateSentReq(const std::string &filename, std::string &flight_id);


#endif // FUNCTIONS_H
