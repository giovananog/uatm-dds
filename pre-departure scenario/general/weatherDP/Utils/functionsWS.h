#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <string>
#include <random>
#include <vector>

std::string getRandomLocation(std::mt19937& gen);
double getRandomTemperature(std::mt19937& gen);
double getRandomWindSpeed(std::mt19937& gen);
std::string getRandomWeatherCondition(std::mt19937& gen);
std::string getCurrentTime();

#endif // FUNCTIONS_H
