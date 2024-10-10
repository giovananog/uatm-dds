#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <string>
#include <vector>

struct Skyport
{
  std::string resource_id;
  int available;
  int tolpads;
  int sent;
};

std::vector<Skyport> readSkyportsFromFile(const std::string &filename);
std::string getCurrentTime();

#endif // FUNCTIONS_H
