#include "functionsBP.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <random>

// Function to generate a random origin skyport ID (either Skyport-1 or Skyport-2)
std::string generateOriginSkyportId() {
    return "Skyport-" + std::to_string(rand() % 2 + 1); // Randomly returns "Skyport-1" or "Skyport-2"
}

// Function to generate the destination skyport ID based on the origin
// If origin is "Skyport-1", the destination will be "Skyport-2" and vice versa
std::string generateDestinationSkyportId(std::string originSkyportID) {
    return originSkyportID == "Skyport-1" ? 
       "Skyport-2" : "Skyport-1"; // Returns the opposite skyport
}

// Function to generate a Poisson-distributed random number with a given lambda parameter
double generatePoisson(double lambda) {
    std::random_device rd;  // Random number generator seed
    std::mt19937 gen(rd()); // Mersenne Twister random number generator
    std::poisson_distribution<> d(lambda); // Poisson distribution with mean lambda
    return d(gen); // Generate and return a Poisson-distributed value
}
