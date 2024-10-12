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
void updateSkyportInFile(const std::string &filename, const std::string &resource_id);

#endif // FUNCTIONS_H
