#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <string>
#include <vector>

struct FlightRequest {
    std::string costumer_id;
    std::string skyport_id;
    std::string booking_id;
    std::string flight_id;
};

std::vector<FlightRequest> readRequestsFromFile(const std::string &filename);
void removeRequestFromFile(const std::string &filename, const std::string &costumer_id);

#endif // FUNCTIONS_H
