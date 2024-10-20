#include "functions.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <random>

std::string generateOriginSkyportId() {
    return "Skyport-" + std::to_string(rand() % 2 + 1);
}

std::string generateDestinationSkyportId(std::string originSkyportID) {
    return originSkyportID == "Skyport-1" ?
       "Skyport-2" : "Skyport-1";
}

double generatePoisson(double lambda) {
    std::random_device rd; 
    std::mt19937 gen(rd());
    std::poisson_distribution<> d(lambda);
    return d(gen);
}
