#include "functions.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <ctime>
#include <chrono>
#include <iomanip>


std::vector<Skyport> readSkyportsFromFile(const std::string &filename)
{
    std::ifstream file(filename);
    std::string line;
    std::vector<Skyport> skyports;

    while (std::getline(file, line))
    {
        if (!line.empty())
        {
            Skyport skyport;
            std::istringstream ss(line);
            std::string token;

            while (std::getline(ss, token, ','))
            {
                std::istringstream key_value(token);
                std::string key, value;

                std::getline(key_value, key, '=');
                std::getline(key_value, value);

                if (key == "resource_id") {
                    skyport.resource_id = value;
                } else if (key == "available") {
                    skyport.available = std::stoi(value);
                } else if (key == "tolpads") {
                    skyport.tolpads = std::stoi(value);
                } 
            }

            if (skyport.available == 1)
            {
                skyports.push_back(skyport);
            }
        }
    }

    return skyports;
}

std::string getCurrentTime() {
    std::time_t now = std::time(nullptr);
    
    std::tm* local_time = std::localtime(&now);
    
    std::ostringstream oss;
    oss << std::setw(2) << std::setfill('0') << local_time->tm_hour << ":"
        << std::setw(2) << std::setfill('0') << local_time->tm_min << ":"
        << std::setw(2) << std::setfill('0') << local_time->tm_sec;

    return oss.str();
}
