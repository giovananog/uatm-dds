#include "functions.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <ctime>
#include <chrono>
#include <iomanip>

// Reads a list of pilots from a file and returns a vector of Pilot objects.
std::vector<Pilot> readPilotsFromFile(const std::string &filename)
{
  std::ifstream file(filename);
  std::string line;
  std::vector<Pilot> pilots;

  while (std::getline(file, line))
  {
    if (!line.empty())
    {
      Pilot pilot;
      std::istringstream ss(line);

      std::string temp;

      std::getline(ss, temp, '='); 
      std::getline(ss, pilot.pilot_id, ','); 

      std::getline(ss, temp, '='); 
      std::getline(ss, pilot.skyport_id, ','); 

      std::getline(ss, temp, '='); 
      std::getline(ss, temp);
      pilot.available = std::stoi(temp);

      if (pilot.available == 1)
      {
        pilots.push_back(pilot);
      }
    }
  }
  return pilots;
}

// Updates the pilot's availability status and modifies their skyport assignment.
void updatePilotStatus(const std::string &pilot_id, int new_available, const std::string &destination_skyport_id) {
    std::string filename = "pilotManagerDP/data/pilots.txt"; 
    std::ifstream file(filename);
    std::string line;
    std::vector<std::string> lines;
    bool origin_updated = false;  
    bool destination_added = false;  

    while (std::getline(file, line)) {
        if (!line.empty()) {
            if (line.find("pilot_id=" + pilot_id) != std::string::npos) 
            {
                std::istringstream ss(line);
                std::string token;
                std::string updated_line;

                while (std::getline(ss, token, ',')) {
                    if (token.find("available=") != std::string::npos) {
                        token = "available=0";  
                        origin_updated = true;
                    }
                    updated_line += token + ",";
                }
                updated_line.pop_back();
                lines.push_back(updated_line);
            } 
            else {
                lines.push_back(line);
            }
        }
    }
    file.close();

    if (origin_updated && !destination_added) {
        std::string new_line = "pilot_id=" + pilot_id + ",skyport_id=" + destination_skyport_id + ",available=1";
        lines.push_back(new_line);
        destination_added = true;
    }

    std::ofstream out_file(filename);
    for (const auto &l : lines) {
        out_file << l << "\n";
    }
    out_file.close();
}

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
