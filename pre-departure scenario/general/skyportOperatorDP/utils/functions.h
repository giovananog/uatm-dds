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

struct TrafficFlow
{
  std::string flows_id;
  std::string area;
  std::string congestion_level;
  std::string affected_routes;
  std::string timestamp;
};

struct Restriction
{
  std::string restriction_id;
  std::string restriction_area;
  std::string restriction_type;
  std::string restriction_time;
  std::string restriction_authority;
};

std::vector<TrafficFlow> readTrafficFlows(const std::string &filename);
std::vector<Restriction> readRestrictions(const std::string &filename);
std::vector<FlightRoute> readFlightRoutes(const std::string &filename);

#endif // FUNCTIONS_H
