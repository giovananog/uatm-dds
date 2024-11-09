#include "functions.h"
#include <iostream>
#include <fstream>
#include <vector>
#include "../../model/UATMTraits.h"
#include <sstream>

// Reads flight request information from a file and returns a vector of flightRequestInfo objects.
std::vector<flightRequestInfo> readRequestsFromFile(const std::string& filename) {
    std::vector<flightRequestInfo> requests;
    std::ifstream request_file(filename);

    if (!request_file.is_open()) {
        ACE_ERROR((LM_ERROR,
                   ACE_TEXT("ERROR: Failed to open %s for reading!\n"), filename.c_str()));
        return requests;  
    }

    std::string line;
    while (std::getline(request_file, line)) {

        std::stringstream ss(line);
        flightRequestInfo request;

        std::string auth_request_id, flight_id, departure_skyport_id, destination_skyport_id, departure_time, pilot_id, evtol_id, sent_req;
        std::getline(ss, auth_request_id, ',');
        std::getline(ss, flight_id, ',');
        std::getline(ss, departure_skyport_id, ',');
        std::getline(ss, destination_skyport_id, ',');
        std::getline(ss, departure_time, ',');
        std::getline(ss, pilot_id, ',');
        std::getline(ss, evtol_id);

        request.auth_request_id = auth_request_id.substr(auth_request_id.find(':') + 1);
        request.flight_id = flight_id.substr(flight_id.find(':') + 1);
        request.departure_skyport_id = departure_skyport_id.substr(departure_skyport_id.find(':') + 1);
        request.destination_skyport_id = destination_skyport_id.substr(destination_skyport_id.find(':') + 1);
        request.departure_time = departure_time.substr(departure_time.find(':') + 1);
        request.pilot_id = pilot_id.substr(pilot_id.find(':') + 1);
        request.evtol_id = evtol_id.substr(evtol_id.find(':') + 1);
        
        requests.push_back(request);
    }

    request_file.close();  
    return requests;
}
