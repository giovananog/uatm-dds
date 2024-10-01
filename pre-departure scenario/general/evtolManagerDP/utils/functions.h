#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <string>
#include <vector>

struct EVTOL
{
  std::string evtol_id;
  std::string skyport_id;
  int available;
  int sent;
};

std::vector<EVTOL> readEVTOLsFromFile(const std::string &filename);
void updateEVTOLInFile(const std::string &filename, const std::string &evtol_id);
void updateEvtolStatus(const std::string &evtol_id, int new_available, int new_sent);

#endif // FUNCTIONS_H
