#include "functions.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

std::vector<FlightRoute> readFlightRoutes(const std::string &filename)
{
    std::ifstream file(filename);
    std::vector<FlightRoute> routes;
    std::string line;

    while (std::getline(file, line))
    {
        if (!line.empty())
        {
            FlightRoute route;
            std::stringstream ss(line);
            std::string temp;

            std::getline(ss, temp, '=');
            std::getline(ss, temp, ',');
            route.route_id = std::stoi(temp);

            std::getline(ss, temp, '=');
            std::getline(ss, route.origin_skyport_id, ',');

            std::getline(ss, temp, '=');
            std::getline(ss, route.destination_skyport_id, ',');

            std::getline(ss, temp, '=');
            std::getline(ss, temp, ',');
            route.available_capacity = std::stoi(temp);

            std::getline(ss, temp, '=');
            std::getline(ss, temp, ',');
            route.available = std::stoi(temp);

            std::getline(ss, temp, '=');
            std::getline(ss, route.traffic_density, ',');

            routes.push_back(route);
        }
    }

    return routes;
}
