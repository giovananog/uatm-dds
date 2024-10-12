#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <string>
#include <vector>

struct WeatherInfo {
    int weather_id;
    std::string location;
    double temperature;
    double wind_speed;
    std::string weather_condition;
};

std::vector<WeatherInfo> readWeatherFromFile(const std::string &filename);

#endif // FUNCTIONS_H
