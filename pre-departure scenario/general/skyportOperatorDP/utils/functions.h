#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <string>
#include <vector>

extern std::vector<std::string> areas;
extern std::vector<std::string> congestion_levels;
extern std::vector<std::string> restriction_areas;
extern std::vector<std::string> restriction_types;
extern std::vector<std::string> authorities;
extern std::vector<std::string> skyports;
extern std::vector<std::string> traffic_density_levels;

std::string getRandomValue(const std::vector<std::string> &values);

#endif // FUNCTIONS_H
