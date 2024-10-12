#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <string>
#include <vector>

struct TolPad
{
  std::string resource_id;
  std::string skyport_id;
  int available;
  int sent;
};

std::vector<TolPad> readTolPadsFromFile(const std::string &filename);
void updateTolPadInFile(const std::string &filename, const std::string &resource_id);
void updateTolPadStatus(const std::string &resource_id, int new_available, int new_sent);

#endif // FUNCTIONS_H
