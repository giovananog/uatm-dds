#include "functions.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

std::vector<FlightRequest> readRequestsFromFile(const std::string &filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Erro ao abrir o arquivo: " << filename << std::endl;
        return {};  
    }

    std::string line;
    std::vector<FlightRequest> requests;

    while (std::getline(file, line)) {
        if (!line.empty()) {
            FlightRequest fr;
            std::istringstream ss(line);
            std::string pair;

            while (std::getline(ss, pair, ',')) {
                std::string key, value;
                std::istringstream pairStream(pair);
                std::getline(pairStream, key, '=');
                std::getline(pairStream, value, '=');

                if (key == "costumer_id") {
                    fr.costumer_id = value;
                } else if (key == "skyport_id") {
                    fr.skyport_id = value;
                } else if (key == "booking_id") {
                    fr.booking_id = value;
                } else if (key == "flight_id") {
                    fr.flight_id = value;
                }
            }

            requests.push_back(fr);
        }
    }

    return requests;
}

void removeRequestFromFile(const std::string &filename, const std::string &costumer_id) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Erro ao abrir o arquivo: " << filename << std::endl;
        return;
    }

    std::string line;
    std::vector<std::string> lines;

    while (std::getline(file, line)) {
        if (line.find(costumer_id) == std::string::npos) {
            lines.push_back(line);
        }
    }

    file.close();

    std::ofstream outFile(filename);
    if (!outFile.is_open()) {
        std::cerr << "Erro ao abrir o arquivo para escrita: " << filename << std::endl;
        return;
    }

    for (const auto &l : lines) {
        outFile << l << "\n";
    }
}
