#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <string>
#include <vector>
#include "../../model/UATMTraits.h"

struct BookingData {
    std::string booking_id;
    std::string costumer_id;
    std::string flight_id;
    std::string skyport_id;
    std::string pilot_id;
    std::string evtol_id;
    std::string weather_id;
    std::string route_id;
    std::string tolpad_id;
    int status;
    int sent_coord;
    int sent_auth;
};

struct Route {
    std::string route_id;
    std::string origin_skyport_id;
    std::string destination_skyport_id;
    int available_capacity;
    bool available;
    std::string traffic_density;
};


void updateAvailabilityFile(const UATM::availabilityInfo& msg);
bool removeAssignedResources(const std::string &availabilityFile, const std::string &evtolID, const std::string &pilotID);
bool checkAvailability(const std::string &resourceFile, std::string &evtolID, std::string &pilotID);
bool findAndAssignFlight(const std::string &flightFile, const std::string &evtolID, const std::string &pilotID, std::string &flightID, std::string &weatherID, std::string &routeID, bool &status_send);
bool checkWeatherConditions(const std::string& weatherFile, const std::string& location, std::string &weatherID);
bool checkRouteAvailability(const std::string& routeFile, const std::string& origin, const std::string& destination, std::string &routeID);
std::vector<BookingData> readBookingsFromFile(const std::string &filename);
bool canSendCoordination(const BookingData &booking);
bool canSendAuthorization(const BookingData &booking);
Route* findRouteById(std::vector<Route>& routes, const std::string& route_id);
std::vector<Route> readRoutesFromFile(const std::string& filename);
void updateSentCoord(const std::string &filename, std::string &booking_id);
void updateSentAuth(const std::string &filename, std::string &flight_id);

#endif // FUNCTIONS_H
