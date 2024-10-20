#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <string>
#include <vector>

struct FlightRequest {
    std::string costumer_id;
    std::string origin_skyport_id;
    std::string destination_skyport_id;
    std::string booking_id;
    std::string flight_id;
};

std::string generateOriginSkyportId();
std::string generateDestinationSkyportId(std::string originSkyportID);
double generatePoisson(double lambda);

#endif // FUNCTIONS_H
