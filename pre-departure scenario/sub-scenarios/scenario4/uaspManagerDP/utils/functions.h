#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <string>
#include "../../model/UATMTraits.h"
#include <vector>

bool checkAvailability(const std::string &resourceFile, std::string &tolPadID);
void saveTolPadsToFile(const UATM::availabilityInfo& msg);

#endif // FUNCTIONS_H
