#include "functions.h"
#include <fstream>
#include <vector>
#include <sstream>
#include <ctime>
#include <chrono>
#include <iomanip>
#include <string>

// Function to read EVTOL data from a file and return a list of EVTOLs
std::vector<EVTOL> readEVTOLsFromFile(const std::string &filename)
{
    std::ifstream file(filename); // Open the file for reading
    std::string line;
    std::vector<EVTOL> evtols;

    // Read the file line by line
    while (std::getline(file, line))
    {
        if (!line.empty()) // Ignore empty lines
        {
            EVTOL evtol;
            std::istringstream ss(line); // Create a stringstream to parse the line

            std::string temp;
            // Read each part of the line, separated by '=' and ',' and populate the EVTOL object
            std::getline(ss, temp, '='); 
            std::getline(ss, evtol.evtol_id, ','); 

            std::getline(ss, temp, '='); 
            std::getline(ss, evtol.skyport_id, ','); 

            std::getline(ss, temp, '='); 
            std::getline(ss, temp);
            evtol.available = std::stoi(temp); // Convert availability status to integer

            // If EVTOL is available, add it to the list
            if (evtol.available == 1) 
            {
                evtols.push_back(evtol);
            }
        }
    }
    return evtols; // Return the list of available EVTOLs
}

// Function to update EVTOL status in the file, marking it as unavailable at the origin skyport
// and adding a new line with the destination skyport if necessary
void updateEvtolStatus(const std::string &evtol_id, int new_available, const std::string &destination_skyport_id) {
    std::string filename = "evtolManagerDP/data/evtols.txt"; // Path to the EVTOL file
    std::ifstream file(filename);
    std::string line;
    std::vector<std::string> lines;
    bool origin_updated = false;  // Flag to check if origin EVTOL is updated
    bool destination_added = false;  // Flag to check if destination EVTOL is added

    // Read the file and process each line
    while (std::getline(file, line)) {
        if (!line.empty()) {
            // If the line contains the specified EVTOL ID, update its status
            if (line.find("evtol_id=" + evtol_id) != std::string::npos) {
                std::istringstream ss(line);
                std::string token;
                std::string updated_line;

                // Loop through each token in the line and update the availability status
                while (std::getline(ss, token, ',')) {
                    if (token.find("available=") != std::string::npos) {
                        token = "available=0";  // Mark the EVTOL as unavailable
                        origin_updated = true;
                    }
                    updated_line += token + ",";
                }
                updated_line.pop_back(); // Remove the trailing comma
                lines.push_back(updated_line);
            } 
            else {
                lines.push_back(line); // Keep unchanged lines
            }
        }
    }
    file.close(); // Close the input file

    // If the origin EVTOL was updated, add the new line with the destination EVTOL
    if (origin_updated && !destination_added) {
        std::string new_line = "evtol_id=" + evtol_id + ",skyport_id=" + destination_skyport_id + ",available=1";
        lines.push_back(new_line); // Add the destination EVTOL
        destination_added = true;
    }

    // Write the updated lines back to the file
    std::ofstream out_file(filename);
    for (const auto &l : lines) {
        out_file << l << "\n";
    }
    out_file.close(); // Close the output file
}

// Function to get the current local time as a string in the format HH:MM:SS
std::string getCurrentTime() {
    std::time_t now = std::time(nullptr);  // Get the current time
    
    std::tm* local_time = std::localtime(&now);  // Convert time to local time
    
    std::ostringstream oss;
    // Format the time as HH:MM:SS
    oss << std::setw(2) << std::setfill('0') << local_time->tm_hour << ":"
        << std::setw(2) << std::setfill('0') << local_time->tm_min << ":"
        << std::setw(2) << std::setfill('0') << local_time->tm_sec;

    return oss.str(); // Return the formatted time as a string
}
