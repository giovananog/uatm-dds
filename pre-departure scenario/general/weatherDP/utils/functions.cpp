#include "functions.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <random>
#include <string>
#include <chrono>


std::string getRandomLocation(std::mt19937& gen) {
    std::uniform_int_distribution<> dis(1, 2);
    return "Skyport-" + std::to_string(dis(gen));
}

double getRandomTemperature(std::mt19937& gen) {
    std::uniform_real_distribution<> dis(15.0, 35.0); 
    return dis(gen);
}

double getRandomWindSpeed(std::mt19937& gen) {
    std::uniform_real_distribution<> dis(0.0, 20.0); 
    return dis(gen);
}

std::string getRandomWeatherCondition(std::mt19937& gen) {
    std::vector<std::string> conditions = {"Clear Sky", "Partly Cloudy", "Rainy", "Stormy"};
    std::uniform_int_distribution<> dis(0, conditions.size() - 1);
    return conditions[dis(gen)];
}

