#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <string>
#include <vector>

struct EVTOL
{
  std::string evtol_id;
  std::string skyport_id;
  int available;
};

std::vector<EVTOL> readEVTOLsFromFile(const std::string &filename);
void updateEvtolStatus(const std::string &evtol_id, int new_available);
std::string getCurrentTime();

#endif // FUNCTIONS_H
