#include "functions.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <ctime>
#include <chrono>
#include <iomanip>

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
      ss >> evtol.available; 
      ss.ignore(1); 

      std::getline(ss, temp, '='); 
      ss >> evtol.sent; 

      if (evtol.sent == 0)
      {
        evtols.push_back(evtol);
      }
    }
  }
  return evtols;
}

void updateEVTOLInFile(const std::string &filename, const std::string &evtol_id)
{
  std::ifstream file(filename);
  std::string line;
  std::vector<std::string> lines;

  while (std::getline(file, line))
  {
    if (line.find(evtol_id) != std::string::npos)
    {
      std::string updated_line = line;
      size_t pos = line.find("sent=0");
      if (pos != std::string::npos)
      {
        updated_line.replace(pos, 6, "sent=1");
      }
      lines.push_back(updated_line);
    }
    else
    {
      lines.push_back(line);
    }
  }
  file.close();

  std::ofstream outFile(filename);
  for (const auto &l : lines)
  {
    outFile << l << "\n";
  }
}

void updateEvtolStatus(const std::string &evtol_id, int new_available, int new_sent) {
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
                    if (token.find("sent=") != std::string::npos) {
                        token = "sent=" + std::to_string(new_sent);
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
