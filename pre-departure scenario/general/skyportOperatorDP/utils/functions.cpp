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


std::vector<TrafficFlow> readTrafficFlows(const std::string &filename)
{
  std::ifstream file(filename);
  std::vector<TrafficFlow> flows;
  std::string line;

  while (std::getline(file, line))
  {
    if (!line.empty())
    {
      TrafficFlow flow;
      std::stringstream ss(line);
      std::string temp;

      std::getline(ss, temp, '=');
      std::getline(ss, temp, ',');
      flow.flows_id = temp;

      std::getline(ss, temp, '=');
      std::getline(ss, flow.area, ',');

      std::getline(ss, temp, '=');
      std::getline(ss, flow.congestion_level, ',');

      std::getline(ss, temp, '=');
      std::getline(ss, flow.timestamp, ',');

      flows.push_back(flow);
    }
  }

  return flows;
}

std::vector<Restriction> readRestrictions(const std::string &filename)
{
  std::ifstream file(filename);
  std::vector<Restriction> restrictions;
  std::string line;

  while (std::getline(file, line))
  {
    Restriction restriction;
    std::stringstream ss(line);
    std::string temp;

    std::getline(ss, temp, '=');
    std::getline(ss, temp, ',');
    restriction.restriction_id = temp;

    std::getline(ss, temp, '=');
    std::getline(ss, restriction.restriction_area, ',');

    std::getline(ss, temp, '=');
    std::getline(ss, restriction.restriction_type, ',');

    std::getline(ss, temp, '=');
    std::getline(ss, restriction.restriction_time, ',');

    std::getline(ss, temp, '=');
    std::getline(ss, restriction.restriction_authority, ',');

    restrictions.push_back(restriction);
  }

  return restrictions;
}
