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
        std::string resource_id, available, skyport_id;

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
        }

        if (available == "1")
        {
            tolPads.insert(resource_id);
        }
    }

    if (!tolPads.empty())
    {
        tolPadID = *tolPads.begin();

        std::ofstream outfile(resourceFile);  
        for (const auto &line : lines)
        {
            if (line.find("resource_id=" + tolPadID) == std::string::npos)
            {
                outfile << line << std::endl; 
            }
        }
        return true;
    }

    return false;
}

void saveTolPadsToFile(const UATM::availabilityInfo& msg) {
    std::ofstream file("uaspManagerDP/data/tolpads.txt", std::ios_base::app); 
    if (!file.is_open()) {
        ACE_ERROR((LM_ERROR,
                   ACE_TEXT("ERROR: %N:%l: saveTolPadsToFile() -")
                   ACE_TEXT(" Failed to open file!\n")));
        return;
    }

    file << "resource_id=" << msg.resource_id.in() << ","
         << "skyport_id=" << msg.skyport_id.in() << ","
         << "available=" << msg.available << ","
         << "availability_time=" << msg.availability_time.in() << "\n";

    file.close();
}
