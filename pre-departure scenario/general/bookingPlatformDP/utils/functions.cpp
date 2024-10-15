#include "functions.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

std::string generateOriginSkyportId() {
    return "Skyport-" + std::to_string(rand() % 2 + 1);
}

std::string generateDestinationCustomerId(std::string originSkyportID) {
    return originSkyportID == "1" ?
       "Skyport-2" : "Skyport-1";
}
