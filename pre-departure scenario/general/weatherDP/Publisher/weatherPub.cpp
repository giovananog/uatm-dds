#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <thread>
#include <chrono>
#include <random>
#include <string>
#ifdef ACE_AS_STATIC_LIBS
#include <dds/DCPS/transport/tcp/Tcp.h>
#endif
#include <model/Sync.h>
#include <ace/Log_Msg.h>
#include "../../model/UATMTraits.h"
#include "../utils/functions.h"


int ACE_TMAIN(int argc, ACE_TCHAR **argv) {
    try {
        OpenDDS::Model::Application application(argc, argv);
        UATM::uatmDCPS::DefaultUATMType model3(application, argc, argv);

        using OpenDDS::Model::UATM::uatmDCPS::Elements;

        DDS::DataWriter_var writer = model3.writer(Elements::DataWriters::weatherInfoDW_WTR);
        UATM::weatherInfoDataWriter_var writer_var = UATM::weatherInfoDataWriter::_narrow(writer.in());

        if (CORBA::is_nil(writer_var.in())) {
            ACE_ERROR_RETURN((LM_ERROR,
                              ACE_TEXT("(%P|%t) ERROR: %N:%l: main() -")
                                  ACE_TEXT(" _narrow failed!\n")),
                             -1);
        }

        std::string filename = "weatherDP/data/weathers.txt";
        std::vector<WeatherInfo> weather_data = readWeatherFromFile(filename);
        std::random_device rd; 
        std::mt19937 gen(rd()); 
        std::uniform_int_distribution<> dis(0, weather_data.size() - 1); 

        OpenDDS::Model::WriterSync ws(writer);
        
        while (true) {
            int index = dis(gen);
            WeatherInfo selected_weather = weather_data[index];

            UATM::weatherInfo fr;
            fr.weather_id = selected_weather.weather_id;
            fr.location = CORBA::string_dup(selected_weather.location.c_str());
            fr.temperature = selected_weather.temperature;
            fr.wind_speed = selected_weather.wind_speed;
            fr.weather_condition = CORBA::string_dup(selected_weather.weather_condition.c_str());
            fr.observation_time = "time"; 

            DDS::ReturnCode_t error = writer_var->write(fr, DDS::HANDLE_NIL);

            if (error != DDS::RETCODE_OK) {
                ACE_ERROR((LM_ERROR,
                           ACE_TEXT("(%P|%t) ERROR: %N:%l: main() -")
                               ACE_TEXT(" write returned %d!\n"),
                           error));
            }

            std::this_thread::sleep_for(std::chrono::seconds(19));
        }
    }
    catch (const CORBA::Exception &e) {
        e._tao_print_exception("Exception caught in main():");
        return -1;
    }
    catch (const std::exception &ex) {
        ACE_ERROR_RETURN((LM_ERROR,
                          ACE_TEXT("(%P|%t) ERROR: %N:%l: main() -")
                              ACE_TEXT(" Exception caught: %C\n"),
                          ex.what()),
                         -1);
    }
    return 0;
}
