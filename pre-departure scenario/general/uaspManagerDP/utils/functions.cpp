#include "functions.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include "../../model/UATMTraits.h"
#include <unordered_set>

bool checkAvailability(const std::string &resourceFile, std::string &tolPadID)
{
    std::ifstream infile(resourceFile);
    std::string line;
    std::unordered_set<std::string> tolPads;
    std::vector<std::string> lines;

    while (std::getline(infile, line))
    {
        lines.push_back(line);
        std::string resource_id, available, skyport_id, availability_time;

        std::istringstream iss(line);
        std::string token;

        while (std::getline(iss, token, ','))
        {
            std::string key = token.substr(0, token.find('='));
            std::string value = token.substr(token.find('=') + 1);

            if (key == "resource_id")
            {
                resource_id = value;
            }
            else if (key == "available")
            {
                available = value;
            }
            else if (key == "skyport_id")
            {
                skyport_id = value;
            }
            else if (key == "availability_time")
            {
                availability_time = value;
            }
        }

        if (available == "1")
        {
            tolPads.insert(resource_id);
        }
    }

    infile.close();

    if (!tolPads.empty())
    {
        tolPadID = *tolPads.begin();
    }

    return true;

return false;
}

std::vector<UATM::flightAuthorization> read_requests_from_file(const std::string &filename)
{
    std::ifstream file(filename);
    std::vector<UATM::flightAuthorization> authorizations;

    if (!file.is_open())
    {
        std::cout << "Failed to open file for reading!" << std::endl;
        return authorizations;
    }

    std::string line;
    while (std::getline(file, line))
    {
        UATM::flightAuthorization fa;
        std::stringstream ss(line);

        std::string request_id, flight_id_str, departure_skyport_id, destination_skyport_id;
        std::string departure_time, approved_departure_time, approved_arrival_time, tolpad_id, pilot_id, evtol_id;

        std::getline(ss, request_id, ',');
        std::getline(ss, flight_id_str, ',');
        std::getline(ss, departure_skyport_id, ',');
        std::getline(ss, destination_skyport_id, ',');
        std::getline(ss, departure_time, ',');
        std::getline(ss, tolpad_id, ',');
        std::getline(ss, pilot_id, ',');
        std::getline(ss, evtol_id, ',');

        fa.authorization_id = request_id.c_str();
        fa.flight_id = flight_id_str.c_str();
        fa.approved_route_id = departure_skyport_id.c_str();
        fa.authorization_status = destination_skyport_id.c_str();
        fa.tolpad_id = tolpad_id.c_str();
        fa.pilot_id = pilot_id.c_str();
        fa.evtol_id = evtol_id.c_str();

        authorizations.push_back(fa);
    }

    file.close();
    return authorizations;
}

std::string getAndUpdateFlightIDWithEmptyTolPad(const std::string &filename, const std::string &tolPadID)
{
    std::ifstream request_file(filename);
    std::string line;
    std::string flight_id;
    std::vector<std::string> lines;

    if (!request_file.is_open())
    {
        std::cerr << "Erro ao abrir o arquivo de requests." << std::endl;
        return "";
    }

    bool updated = false;

    while (std::getline(request_file, line))
    {
        std::istringstream line_stream(line);
        std::string field;
        std::string temp_flight_id;
        std::string tolpad_id;

        while (std::getline(line_stream, field, ','))
        {
            if (field.find("flight_id:") != std::string::npos)
            {
                temp_flight_id = field.substr(field.find(":") + 1);
            }
            if (field.find("tolpad_id:") != std::string::npos)
            {
                tolpad_id = field.substr(field.find(":") + 1);
            }
        }

        if (tolpad_id.empty() && !updated)
        {
            flight_id = temp_flight_id;
            size_t pos = line.find("tolpad_id:");
            if (pos != std::string::npos)
            {
                if (pos + 10 < line.length())
                {
                    line.replace(pos + 10, tolpad_id.length(), tolPadID); 
                    updated = true;
                } 
            } 
        }

        lines.push_back(line);
    }

    request_file.close();

    if (updated)
    {
        std::ofstream outFile(filename);
        if (outFile.is_open())
        {
            for (const auto &l : lines)
            {
                outFile << l << "\n";
            }
            outFile.close();
        }
        else
        {
            std::cout << "Erro ao reescrever o arquivo de requests." << std::endl;
            return "";
        }
    }

    return flight_id;
}

bool checkWeatherConditions(const std::string &weatherFile, const std::string &location, std::string &weatherID)
{
    std::ifstream infile(weatherFile);
    if (!infile.is_open())
    {
        std::cerr << "Erro ao abrir o arquivo de clima: " << weatherFile << std::endl;
        return false;
    }

    std::string line;
    std::string latestWeatherID;
    bool foundLocation = false;

    while (std::getline(infile, line))
    {
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

        if (CORBA::string_dup(location_name.c_str()) == location && (weather_condition_str == "Clear Sky" || weather_condition_str == "Partly Cloudy"))
        {
            latestWeatherID = weather_id;
            foundLocation = true;
        }
    }

    infile.close();

    if (foundLocation)
    {
        weatherID = latestWeatherID;
        return true;
    }

    return false;
}

bool checkFlowConditions(const std::string &flowsFile, const std::string &area, std::string &flowsID)
{
    std::ifstream infile(flowsFile);
    if (!infile.is_open())
    {
        std::cerr << "Erro ao abrir o arquivo de flows: " << flowsFile << std::endl;
        return false;
    }

    std::string line;
    std::string latestFlowsID;
    bool foundArea = false;

    while (std::getline(infile, line))
    {
        std::istringstream iss(line);
        std::string token;
        std::string flows_id, area_name, congestion_level, timestamp, affected_routes;

        std::getline(iss, token, ',');
        flows_id = token.substr(token.find(':') + 1);
        std::getline(iss, token, ',');
        area_name = token.substr(token.find(':') + 1);
        std::getline(iss, token, ',');
        congestion_level = token.substr(token.find(':') + 1);
        std::getline(iss, token);
        timestamp = token.substr(token.find(':') + 1);

        if (area_name == area && std::stoi(congestion_level) < 3)
        {
            latestFlowsID = flows_id;
            foundArea = true;
        }
    }

    infile.close();

    if (foundArea)
    {
        flowsID = latestFlowsID;
        return true;
    }

    return false;
}

bool checkRestrictionConditions(const std::string &restrictionsFile, const std::string &area, std::string &restrictionID)
{
    std::ifstream infile(restrictionsFile);
    if (!infile.is_open())
    {
        std::cerr << "Erro ao abrir o arquivo de restrictions: " << restrictionsFile << std::endl;
        return false;
    }

    std::string line;
    std::string latestRestrictionID;
    bool foundArea = false;

    while (std::getline(infile, line))
    {
        std::istringstream iss(line);
        std::string token;
        std::string restriction_id, restriction_area, restriction_type, restriction_time, restriction_authority;

        std::getline(iss, token, ',');
        restriction_id = token.substr(token.find(':') + 1);
        std::getline(iss, token, ',');
        restriction_area = token.substr(token.find(':') + 1);
        std::getline(iss, token, ',');
        restriction_type = token.substr(token.find(':') + 1);
        std::getline(iss, token);
        restriction_time = token.substr(token.find(':') + 1);

        if (restriction_area == area && restriction_type != "No-Fly Zone")
        {
            latestRestrictionID = restriction_id;
            foundArea = true;
        }
    }

    infile.close();

    if (foundArea)
    {
        restrictionID = latestRestrictionID;
        return true;
    }

    return false;
}
