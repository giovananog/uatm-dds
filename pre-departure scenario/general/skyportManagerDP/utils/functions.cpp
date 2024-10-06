#include "functions.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <ctime>
#include <chrono>
#include <iomanip>


std::vector<Skyport> readSkyportsFromFile(const std::string &filename)
{
    std::ifstream file(filename);
    std::string line;
    std::vector<Skyport> skyports;

    while (std::getline(file, line))
    {
        if (!line.empty())
        {
            Skyport skyport;
            std::istringstream ss(line);
            std::string token;

            while (std::getline(ss, token, ','))
            {
                std::istringstream key_value(token);
                std::string key, value;

                std::getline(key_value, key, '=');
                std::getline(key_value, value);

                if (key == "resource_id") {
                    skyport.resource_id = value;
                } else if (key == "available") {
                    skyport.available = std::stoi(value);
                } else if (key == "tolpads") {
                    skyport.tolpads = std::stoi(value);
                } else if (key == "sent") {
                    skyport.sent = std::stoi(value);
                }
            }

            if (skyport.sent == 0)
            {
                skyports.push_back(skyport);
            }
        }
    }

    return skyports;
}

void updateSkyportInFile(const std::string &filename, const std::string &resource_id)
{
  std::ifstream file(filename);
  std::string line;
  std::vector<std::string> lines;

  while (std::getline(file, line))
  {
    if (line.find(resource_id) != std::string::npos)
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

std::string getCurrentTime() {
    std::time_t now = std::time(nullptr);
    
    std::tm* local_time = std::localtime(&now);
    
    std::ostringstream oss;
    oss << std::setw(2) << std::setfill('0') << local_time->tm_hour << ":"
        << std::setw(2) << std::setfill('0') << local_time->tm_min << ":"
        << std::setw(2) << std::setfill('0') << local_time->tm_sec;

    return oss.str();
}
