#include "functions.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

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
      ss >> pilot.available; 
      ss.ignore(1); 

      std::getline(ss, temp, '='); 
      ss >> pilot.sent; 

      if (pilot.sent == 0)
      {
        pilots.push_back(pilot);
      }
    }
  }
  return pilots;
}

void updatePilotInFile(const std::string &filename, const std::string &pilot_id)
{
  std::ifstream file(filename);
  std::string line;
  std::vector<std::string> lines;

  while (std::getline(file, line))
  {
    if (line.find(pilot_id) != std::string::npos)
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
