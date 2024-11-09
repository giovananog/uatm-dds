#include "functions.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <ctime>
#include <chrono>
#include <iomanip>

// Returns a random value from a given vector of strings.
std::string getRandomValue(const std::vector<std::string> &values)
{
  int randomIndex = std::rand() % values.size();
  return values[randomIndex];
}

// Generates a destination skyport ID based on the origin skyport ID.
std::string generateDestinationSkyportId(std::string originSkyportID) {
    return originSkyportID == "Skyport-1" ?
       "Skyport-2" : "Skyport-1";
}

// A list of predefined area names.
std::vector<std::string> areas = {"Downtown", "Midtown", "Uptown", "East Side", "West Side", "Suburbs", "Airport", "Highway", "City Center", "Industrial Area"};

// A list of predefined congestion levels.
std::vector<std::string> congestion_levels = {"Low", "Medium", "High"};

// A list of predefined restriction areas.
std::vector<std::string> restriction_areas = {"Skyport-2", "Skyport-1"};

// A list of predefined restriction types.
std::vector<std::string> restriction_types = {"No-Fly Zone", "Altitude Restriction", "Temporary Flight Restriction", "Drone Ban", "Emergency"};

// A list of predefined authorities.
std::vector<std::string> authorities = {"FAA", "ANAC", "CAA"};

// A list of predefined skyports.
std::vector<std::string> skyports = {"Skyport-1", "Skyport-2"};

// A list of predefined traffic density levels.
std::vector<std::string> traffic_density_levels = {"Low", "Medium", "High"};

// Returns the current time as a string in HH:MM:SS format.
std::string getCurrentTime() {
    std::time_t now = std::time(nullptr);
    
    std::tm* local_time = std::localtime(&now);
    
    std::ostringstream oss;
    oss << std::setw(2) << std::setfill('0') << local_time->tm_hour << ":"
        << std::setw(2) << std::setfill('0') << local_time->tm_min << ":"
        << std::setw(2) << std::setfill('0') << local_time->tm_sec;

    return oss.str();
}
