#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <string>
#include <vector>
#include "../../model/UATMTraits.h"

bool removeAssignedResources(const std::string &availabilityFile, const std::string &evtolID, const std::string &pilotID);
bool checkAvailability(const std::string &resourceFile, std::string &evtolID, std::string &pilotID);
bool findAndAssignFlight(const std::string &flightFile, const std::string &evtolID, const std::string &pilotID, std::string &flightID);
void updateAvailabilityFile(const UATM::availabilityInfo& msg);

#endif // FUNCTIONS_H
