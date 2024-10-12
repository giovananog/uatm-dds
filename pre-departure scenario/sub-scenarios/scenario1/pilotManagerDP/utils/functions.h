#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <string>
#include <vector>

struct Pilot
{
  std::string pilot_id;
  std::string skyport_id;
  int available;
  int sent;
};

std::vector<Pilot> readPilotsFromFile(const std::string &filename);
void updatePilotInFile(const std::string &filename, const std::string &pilot_id);

#endif // FUNCTIONS_H
