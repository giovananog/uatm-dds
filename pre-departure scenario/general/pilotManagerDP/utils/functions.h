#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <string>
#include <vector>

struct Pilot
{
  std::string pilot_id;
  std::string skyport_id;
  int available;
};

std::vector<Pilot> readPilotsFromFile(const std::string &filename);
void updatePilotStatus(const std::string &pilot_id, int new_available);
std::string getCurrentTime();

#endif // FUNCTIONS_H
