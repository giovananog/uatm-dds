#include "functions.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

void updatePilotStatus(const std::string &pilot_id, int new_available, int new_sent) {
    std::string filename = "pilotManagerDP/data/pilots.txt"; 
    std::ifstream file(filename);
    std::string line;
    std::vector<std::string> lines;

    while (std::getline(file, line)) {
        if (!line.empty()) {
            if (line.find("pilot_id=" + pilot_id) != std::string::npos) {
                std::istringstream ss(line);
                std::string token;
                std::string updated_line;

                while (std::getline(ss, token, ',')) {
                    if (token.find("available=") != std::string::npos) {
                        token = "available=" + std::to_string(new_available);
                    }
                    if (token.find("sent=") != std::string::npos) {
                        token = "sent=" + std::to_string(new_sent);
                    }
                    updated_line += token + ",";
                }
                updated_line.pop_back();
                lines.push_back(updated_line);
            } else {
                lines.push_back(line);
            }
        }
    }
    file.close();

    std::ofstream out_file(filename);
    for (const auto &l : lines) {
        out_file << l << "\n";
    }
    out_file.close();
}
