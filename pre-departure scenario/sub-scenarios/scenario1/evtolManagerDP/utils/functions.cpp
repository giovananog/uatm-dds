#include "functions.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

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
