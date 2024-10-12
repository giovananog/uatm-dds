#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <string>
#include <vector>

struct FlightRoute
{
    int route_id;
    std::string origin_skyport_id;
    std::string destination_skyport_id;
    int available_capacity;
    bool available;
    std::string traffic_density;
};

std::vector<FlightRoute> readFlightRoutes(const std::string &filename);

#endif // FUNCTIONS_H
