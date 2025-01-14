#include "functionsTPM.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <ctime>
#include <chrono>
#include <iomanip>

// Reads tolpad data from a file and returns a vector of available tolpad objects.
std::vector<TolPad> readTolPadsFromFile(const std::string &filename)
{
  std::ifstream file(filename);
  std::string line;
  std::vector<TolPad> tolPads;

  while (std::getline(file, line))
  {
    if (!line.empty())
    {
      TolPad tolPad;
      std::istringstream ss(line);
      std::string temp;

      // Extract resource_id from line.
      std::getline(ss, temp, '=');
      std::getline(ss, tolPad.resource_id, ',');

      // Extract skyport_id from line.
      std::getline(ss, temp, '=');
      std::getline(ss, tolPad.skyport_id, ',');

      // Extract available status from line.
      std::getline(ss, temp, '=');
      std::getline(ss, temp, ',');
      tolPad.available = std::stoi(temp);

      // Add to tolPads if available.
      if (tolPad.available == 1)
      {
        tolPads.push_back(tolPad);
      }
    }
  }

  return tolPads;
}

// Updates the status of a specific tolpad based on its resource_id.
void updateTolPadStatus(const std::string &resource_id, int new_available) {
    std::string filename = "tolPadManagerDP/data/tolpads.txt"; 
    std::ifstream file(filename);
    std::string line;
    std::vector<std::string> lines;

    // Read lines and update availability for the matching tolpad.
    while (std::getline(file, line)) {
        if (!line.empty()) {
            if (line.find("resource_id=" + resource_id) != std::string::npos) {
                std::istringstream ss(line);
                std::string token;
                std::string updated_line;

                // Loop through tokens to update the "available" field.
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

    // Write updated lines back to the file.
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
