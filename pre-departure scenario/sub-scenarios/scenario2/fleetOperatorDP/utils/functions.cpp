#include "functions.h"
#include <iostream>
#include <fstream>
#include <vector>
#include "../../model/UATMTraits.h"
#include <unordered_set>
#include <sstream>

bool removeAssignedResources(const std::string &availabilityFile, const std::string &evtolID, const std::string &pilotID) {
    std::ifstream infile(availabilityFile);
    if (!infile.is_open()) {
        std::cerr << "Erro ao abrir o arquivo de disponibilidades: " << availabilityFile << std::endl;
        return false;
    }

    std::string line;
    std::stringstream newFileContent;
    bool evtolRemoved = false;
    bool pilotRemoved = false;

    while (std::getline(infile, line)) {
        std::istringstream iss(line);
        std::string token;
        std::string resource_id, resource_type;

        std::getline(iss, token, ',');
        resource_id = token.substr(token.find('=') + 1);
        std::getline(iss, token, ',');
        resource_type = token.substr(token.find('=') + 1);

        if (resource_id == evtolID && resource_type == "evtol") {
            evtolRemoved = true;
            continue;  
        } else if (resource_id == pilotID && resource_type == "pilot") {
            pilotRemoved = true;
            continue;  
        }

        newFileContent << line << "\n";
    }

    infile.close();

    if (evtolRemoved && pilotRemoved) {
        std::ofstream outfile(availabilityFile);
        if (!outfile.is_open()) {
            std::cerr << "Erro ao abrir o arquivo para escrita: " << availabilityFile << std::endl;
            return false;
        }
        outfile << newFileContent.str();
        outfile.close();
        return true;
    } else {
        std::cerr << "Erro: Um ou ambos os recursos não foram encontrados no arquivo de disponibilidades." << std::endl;
        return false;
    }
}

bool checkAvailability(const std::string &resourceFile, std::string &evtolID, std::string &pilotID)
{
    std::ifstream infile(resourceFile);
    if (!infile.is_open()) {
        std::cerr << "Erro ao abrir o arquivo: " << resourceFile << std::endl;
        return false;
    }

    std::string line;
    std::unordered_set<std::string> evtols;
    std::unordered_set<std::string> pilots;

    while (std::getline(infile, line))
    {
        std::istringstream iss(line);
        std::string key, value;
        std::string resource_id, available, skyport_id, resource_type;

        while (std::getline(iss, key, '=') && std::getline(iss, value, ','))
        {
            if (key == "resource_id") {
                resource_id = value;
            } else if (key == "available") {
                available = value;
            } else if (key == "skyport_id") {
                skyport_id = value;
            } else if (key == "resource_type") {
                resource_type = value;
            }
        }
        
        if (available == "1") {
            if (resource_type == "evtol") {
                evtols.insert(resource_id);
            } else if (resource_type == "pilot") {
                pilots.insert(resource_id);
            }
        }
    }

    if (!evtols.empty() && !pilots.empty())
    {
        evtolID = *evtols.begin(); 
        pilotID = *pilots.begin(); 
        return true;
    }

    return false;
}

bool findAndAssignFlight(const std::string &flightFile, const std::string &evtolID, const std::string &pilotID, std::string &flightID, std::string &weatherID, std::string &routeID)
{
    std::ifstream infile(flightFile);
    if (!infile.is_open()) {
        std::cerr << "Erro ao abrir o arquivo: " << flightFile << std::endl;
        return false;
    }

    std::string line;
    std::stringstream newFileContent;
    bool assigned = false;

    while (std::getline(infile, line)) {
        std::istringstream iss(line);
        std::string booking_id, costumer_id, flight_id, skyport_id, pilot_id, evtol_id, weather_id, route_id, tolpad_id;
        std::string token;

        std::getline(iss, token, ',');
        booking_id = token.substr(token.find(':') + 1);
        std::getline(iss, token, ',');
        costumer_id = token.substr(token.find(':') + 1);
        std::getline(iss, token, ',');
        flight_id = token.substr(token.find(':') + 1);
        std::getline(iss, token, ',');
        skyport_id = token.substr(token.find(':') + 1);
        std::getline(iss, token, ',');
        pilot_id = token.substr(token.find(':') + 1);
        std::getline(iss, token, ',');
        evtol_id = token.substr(token.find(':') + 1);
        std::getline(iss, token, ',');
        weather_id = token.substr(token.find(':') + 1);
        std::getline(iss, token, ',');
        route_id = token.substr(token.find(':') + 1);
        std::getline(iss, token);
        tolpad_id = token.substr(token.find(':') + 1);

        if (pilot_id.empty() && evtol_id.empty() && !assigned) {
            pilot_id = pilotID;
            evtol_id = evtolID;
            weather_id = weatherID;
            route_id = routeID;
            flightID = flight_id;
            assigned = true;
        }

        newFileContent << "booking_id:" << booking_id << ",costumer_id:" << costumer_id
                       << ",flight_id:" << flight_id << ",skyport_id:" << skyport_id
                       << ",pilot_id:" << pilot_id << ",evtol_id:" << evtol_id
                       << ",weather_id:" << weather_id << ",route_id:" << route_id
                       << ",tolpad_id:" << tolpad_id << "\n";
    }

    infile.close();

    if (assigned) {
        std::ofstream outfile(flightFile);
        if (!outfile.is_open()) {
            std::cerr << "Erro ao abrir o arquivo para escrita: " << flightFile << std::endl;
            return false;
        }
        outfile << newFileContent.str();
        outfile.close();
    }

    return assigned;
}

void updateAvailabilityFile(const UATM::availabilityInfo& msg) {
    std::string filename = "fleetOperatorDP/data/availabilities.txt"; 
    std::ifstream file(filename);
    std::string line;
    std::vector<std::string> lines;
    bool found = false;

    while (std::getline(file, line)) {
        if (!line.empty()) {
            if (line.find("resource_id=" + std::string(msg.resource_id.in())) != std::string::npos) {
                if (msg.available == 0) {
                    found = true;
                    continue; 
                }
            }
            lines.push_back(line); 
        }
    }
    file.close();

    if (msg.available == 1) {
        std::ostringstream new_line;
        new_line << "resource_id=" << msg.resource_id.in()
                  << ",resource_type=" << msg.resource_type.in()
                  << ",available=" << msg.available
                  << ",skyport_id=" << msg.skyport_id.in()
                  << ",availability_time=" << msg.availability_time.in();
        lines.push_back(new_line.str());
    }

    std::ofstream out_file(filename);
    for (const auto &l : lines) {
        out_file << l << "\n";
    }
    out_file.close();
}

bool checkWeatherConditions(const std::string& weatherFile, const std::string& location, std::string &weatherID) {
    std::ifstream infile(weatherFile);
    if (!infile.is_open()) {
        std::cerr << "Erro ao abrir o arquivo de clima: " << weatherFile << std::endl;
        return false;
    }

    std::string line;
    while (std::getline(infile, line)) {
        std::istringstream iss(line);
        std::string token;
        std::string weather_id, location_name, temperature, wind_speed, weather_condition, observation_time;

        std::getline(iss, token, ',');
        weather_id = token.substr(token.find(':') + 1);
        std::getline(iss, token, ',');
        location_name = token.substr(token.find(':') + 1);
        std::getline(iss, token, ',');
        temperature = token.substr(token.find(':') + 1);
        std::getline(iss, token, ',');
        wind_speed = token.substr(token.find(':') + 1);
        std::getline(iss, token, ',');
        weather_condition = token.substr(token.find(':') + 1);
        std::getline(iss, token);
        observation_time = token.substr(token.find(':') + 1);
        
        std::string location_name_str = CORBA::string_dup(location_name.c_str());
        std::string weather_condition_str = CORBA::string_dup(weather_condition.c_str());

        if (CORBA::string_dup(location_name.c_str()) == location && (weather_condition_str== "Clear Sky" || weather_condition_str == "Partly Cloudy")) {
            weatherID = weather_id;
            return true; 
        }
    }

    return false; 
}

bool checkRouteAvailability(const std::string& routeFile, const std::string& origin, const std::string& destination, std::string &routeID) {
    std::ifstream infile(routeFile);
    if (!infile.is_open()) {
        std::cerr << "Erro ao abrir o arquivo de rotas: " << routeFile << std::endl;
        return false;
    }

    std::string line;
    while (std::getline(infile, line)) {
        std::istringstream iss(line);
        std::string token;
        std::string route_id, origin_skyport, destination_skyport, available, traffic_density;

        std::getline(iss, token, ',');
        route_id = token.substr(token.find(':') + 1);
        std::getline(iss, token, ',');
        origin_skyport = token.substr(token.find(':') + 1);
        std::getline(iss, token, ',');
        destination_skyport = token.substr(token.find(':') + 1);
        std::getline(iss, token, ',');
        available = token.substr(token.find(':') + 1);
        std::getline(iss, token);
        traffic_density = token.substr(token.find(':') + 1);

        if (CORBA::string_dup(origin_skyport.c_str()) == origin && CORBA::string_dup(destination_skyport.c_str()) == destination && available == "1") {
            routeID = route_id;
            return true; 
        }
    }

    return false; 
}

