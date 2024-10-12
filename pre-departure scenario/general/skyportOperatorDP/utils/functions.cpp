#include "functions.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <ctime>
#include <chrono>
#include <iomanip>

std::string getRandomValue(const std::vector<std::string> &values)
{
  int randomIndex = std::rand() % values.size();
  return values[randomIndex];
}

std::vector<std::string> areas = {"Downtown", "Midtown", "Uptown", "East Side", "West Side", "Suburbs", "Airport", "Highway", "City Center", "Industrial Area"};
std::vector<std::string> congestion_levels = {"Low", "Medium", "High"};
std::vector<std::string> restriction_areas = {"South Zone", "North Zone", "East Zone", "West Zone", "Central Zone"};
std::vector<std::string> restriction_types = {"No-Fly Zone", "Altitude Restriction", "Temporary Flight Restriction", "Drone Ban", "Emergency"};
std::vector<std::string> authorities = {"FAA", "ANAC", "CAA"};
std::vector<std::string> skyports = {"Skyport-1", "Skyport-2"};
std::vector<std::string> traffic_density_levels = {"Low", "Medium", "High"};


std::string getCurrentTime() {
    std::time_t now = std::time(nullptr);
    
    std::tm* local_time = std::localtime(&now);
    
    std::ostringstream oss;
    oss << std::setw(2) << std::setfill('0') << local_time->tm_hour << ":"
        << std::setw(2) << std::setfill('0') << local_time->tm_min << ":"
        << std::setw(2) << std::setfill('0') << local_time->tm_sec;

    return oss.str();
}

