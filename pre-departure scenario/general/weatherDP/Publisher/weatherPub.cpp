#include <iostream>
#include <random>
#include <string>
#include <thread>
#include <chrono>
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

        std::random_device rd; 
        std::mt19937 gen(rd()); 

        int weather_id = 1;  

        OpenDDS::Model::WriterSync ws(writer);
        
        while (true) {
            std::string location = getRandomLocation(gen);
            double temperature = getRandomTemperature(gen);
            double wind_speed = getRandomWindSpeed(gen);
            std::string weather_condition = getRandomWeatherCondition(gen);

            UATM::weatherInfo fr;
            fr.weather_id = weather_id++;  
            fr.location = CORBA::string_dup(location.c_str());
            fr.temperature = temperature;
            fr.wind_speed = wind_speed;
            fr.weather_condition = CORBA::string_dup(weather_condition.c_str());
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
