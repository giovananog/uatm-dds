#include "functions.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <ctime>
#include <chrono>
#include <iomanip>
#include <string>
#include <string.h>

std::vector<EVTOL> readEVTOLsFromFile(const std::string &filename)
{
  std::ifstream file(filename);
  std::string line;
  std::vector<EVTOL> evtols;

  while (std::getline(file, line))
  {
    if (!line.empty())
    {
      EVTOL evtol;
      std::istringstream ss(line);

      std::string temp;

      std::getline(ss, temp, '='); 
      std::getline(ss, evtol.evtol_id, ','); 

      std::getline(ss, temp, '='); 
      std::getline(ss, evtol.skyport_id, ','); 

      std::getline(ss, temp, '='); 
      std::getline(ss, temp);
      evtol.available = std::stoi(temp);

      if (evtol.available == 1) 
      {
        evtols.push_back(evtol);
      }
    }
  }
  return evtols;
}

void updateEvtolStatus(const std::string &evtol_id, int new_available) {
    std::string filename = "evtolManagerDP/data/evtols.txt"; 
    std::ifstream file(filename);
    std::string line;
    std::vector<std::string> lines;

    while (std::getline(file, line)) {
        if (!line.empty()) {
            if (line.find("evtol_id=" + evtol_id) != std::string::npos) {
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
