#include "functions.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

std::vector<WeatherInfo> readWeatherFromFile(const std::string &filename) {
    std::ifstream file(filename);
    std::string line;
    std::vector<WeatherInfo> weather_data;

    while (std::getline(file, line)) {
        if (!line.empty()) {
            WeatherInfo weather;
            std::istringstream ss(line);
            std::string token;

            while (std::getline(ss, token, ',')) {
                std::istringstream field_stream(token);
                std::string field_name;
                std::string field_value;

                std::getline(field_stream, field_name, '=');
                std::getline(field_stream, field_value);

                if (field_name == "weather_id") {
                    weather.weather_id = std::stoi(field_value);
                } else if (field_name == "location") {
                    weather.location = field_value;
                } else if (field_name == "temperature") {
                    weather.temperature = std::stod(field_value);
                } else if (field_name == "wind_speed") {
                    weather.wind_speed = std::stod(field_value);
                } else if (field_name == "weather_condition") {
                    weather.weather_condition = field_value;
                }
            }
            weather_data.push_back(weather);
        }
    }
    return weather_data;
}
