#include "functions.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include "../../model/UATMTraits.h"


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
