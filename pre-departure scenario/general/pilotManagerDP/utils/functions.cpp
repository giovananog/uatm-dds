#include "functions.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <ctime>
#include <chrono>
#include <iomanip>

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

void updatePilotStatus(const std::string &pilot_id, int new_available) {
    std::string filename = "pilotManagerDP/data/pilots.txt"; 
    std::ifstream file(filename);
    std::string line;
    std::vector<std::string> lines;

    while (std::getline(file, line)) {
        if (!line.empty()) {
            if (line.find("pilot_id=" + pilot_id) != std::string::npos) {
                std::istringstream ss(line);
                std::string token;
                std::string updated_line;

                while (std::getline(ss, token, ',')) {
                    if (token.find("available=") != std::string::npos) {
                        token = "available=" + std::to_string(new_available);
                    }
                    updated_line += token + ",";
                }
                updated_line.pop_back();
                lines.push_back(updated_line);
            } else {
                lines.push_back(line);
            }
        }
    }
    file.close();

    std::ofstream out_file(filename);
    for (const auto &l : lines) {
        out_file << l << "\n";
    }
    out_file.close();
}

std::string getCurrentTime() {
    std::time_t now = std::time(nullptr);
    
    std::tm* local_time = std::localtime(&now);
    
    std::ostringstream oss;
    oss << std::setw(2) << std::setfill('0') << local_time->tm_hour << ":"
        << std::setw(2) << std::setfill('0') << local_time->tm_min << ":"
        << std::setw(2) << std::setfill('0') << local_time->tm_sec;

    return oss.str();
}
