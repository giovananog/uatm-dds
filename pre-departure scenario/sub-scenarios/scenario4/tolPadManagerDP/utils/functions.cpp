#include "functions.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

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

      std::getline(ss, temp, '=');
      std::getline(ss, tolPad.resource_id, ',');

      std::getline(ss, temp, '=');
      std::getline(ss, tolPad.skyport_id, ',');

      std::getline(ss, temp, '=');
      std::getline(ss, temp, ',');
      tolPad.available = std::stoi(temp);

      std::getline(ss, temp, '=');
      std::getline(ss, temp, ',');
      tolPad.sent = std::stoi(temp);

      if (tolPad.sent == 0)
      {
        tolPad.sent = 1;
        tolPads.push_back(tolPad);
      }
    }
  }

  return tolPads;
}

void updateTolPadInFile(const std::string &filename, const std::string &resource_id)
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

void updateTolPadStatus(const std::string &resource_id, int new_available, int new_sent) {
    std::string filename = "tolPadManagerDP/data/tolpads.txt"; 
    std::ifstream file(filename);
    std::string line;
    std::vector<std::string> lines;

    while (std::getline(file, line)) {
        if (!line.empty()) {
            if (line.find("resource_id=" + resource_id) != std::string::npos) {
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
