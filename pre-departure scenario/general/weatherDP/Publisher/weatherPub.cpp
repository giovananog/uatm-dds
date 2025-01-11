#include <iostream>
#include <random>
#include <string>
#include <thread>
#include <chrono>
#include <model/Sync.h>
#include <ace/Log_Msg.h>
#include "../../model/UATMTraits.h"
#include "../utils/functions.h"

// Security configurations
#if OPENDDS_CONFIG_SECURITY
#  include <dds/DCPS/security/framework/Properties.h>
#endif
#include <dds/DCPS/StaticIncludes.h>
#if OPENDDS_DO_MANUAL_STATIC_INCLUDES
#  ifndef OPENDDS_SAFETY_PROFILE
#    include <dds/DCPS/transport/udp/Udp.h>
#    include <dds/DCPS/transport/multicast/Multicast.h>
#    include <dds/DCPS/RTPS/RtpsDiscovery.h>
#    include <dds/DCPS/transport/shmem/Shmem.h>
#    if OPENDDS_CONFIG_SECURITY
#      include <dds/DCPS/security/BuiltInPlugins.h>
#    endif
#  endif
#  include <dds/DCPS/transport/rtps_udp/RtpsUdp.h>
#endif
#include <ace/Log_Msg.h>


int ACE_TMAIN(int argc, ACE_TCHAR **argv)
{
    try
    {
        // Set security for participant
        TheServiceParticipant->set_security(true); ///

        // Initialize the application and model
        OpenDDS::Model::Application application(argc, argv);             // Application initialization
        UATM::uatmDCPS::DefaultUATMType model3(application, argc, argv); // UATM model initialization

        using OpenDDS::Model::UATM::uatmDCPS::Elements;

        // Create a DataWriter for sending weather information
        DDS::DataWriter_var writer = model3.writer(Elements::DataWriters::weatherInfoDW_WTR);
        UATM::weatherInfoDataWriter_var writer_var = UATM::weatherInfoDataWriter::_narrow(writer.in());

        // Check if the writer was correctly initialized
        if (CORBA::is_nil(writer_var.in()))
        {
            ACE_ERROR_RETURN((LM_ERROR,
                              ACE_TEXT("(%P|%t) ERROR: %N:%l: main() -")
                                  ACE_TEXT(" _narrow failed!\n")),
                             -1);
        }

        // Set up a random number generator for weather data
        std::random_device rd;  // Initialize random device
        std::mt19937 gen(rd()); // Mersenne Twister generator
        int weather_id = 1;     // Initialize weather_id to 1

        // Set the simulation duration
        auto startTime = std::chrono::steady_clock::now(); // Record the start time
        double duration = 100.0;                           // Duration of simulation in seconds

        // Start the loop to simulate weather data
        while (true)
        {
            // Calculate elapsed time
            auto currentTime = std::chrono::steady_clock::now();
            std::chrono::duration<double> elapsedTime = currentTime - startTime;

            // Check if the simulation has run for the specified duration
            if (elapsedTime.count() >= duration)
            {
                break;
            }

            // Synchronize the writer before sending data
            OpenDDS::Model::WriterSync ws(writer);
            {
                // Generate random weather data
                std::string location = getRandomLocation(gen);                  // Random location
                double temperature = getRandomTemperature(gen);                 // Random temperature
                double wind_speed = getRandomWindSpeed(gen);                    // Random wind speed
                std::string weather_condition = getRandomWeatherCondition(gen); // Random weather condition

                // Create a weather information structure to hold the data
                UATM::weatherInfo fr;
                fr.weather_id = weather_id++;                                        // Increment weather_id
                fr.location = CORBA::string_dup(location.c_str());                   // Location of observation
                fr.temperature = temperature;                                        // Temperature at the location
                fr.wind_speed = wind_speed;                                          // Wind speed at the location
                fr.weather_condition = CORBA::string_dup(weather_condition.c_str()); // Condition of the weather
                fr.observation_time = CORBA::string_dup(getCurrentTime().c_str());   // Time of observation

                // Write the weather data to the DDS DataWriter
                DDS::ReturnCode_t error = writer_var->write(fr, DDS::HANDLE_NIL);

                // Check for any errors in the write operation
                if (error != DDS::RETCODE_OK)
                {
                    ACE_ERROR((LM_ERROR,
                               ACE_TEXT("(%P|%t) ERROR: %N:%l: main() -")
                                   ACE_TEXT(" write returned %d!\n"),
                               error));
                }

                // Sleep for 2 seconds before sending the next weather data
                std::this_thread::sleep_for(std::chrono::seconds(2));
            }
        }
    }
    catch (const CORBA::Exception &e)
    {
        // Catch CORBA exceptions and print the error message
        e._tao_print_exception("Exception caught in main():");
        return -1;
    }
    catch (const std::exception &ex)
    {
        // Catch standard exceptions and print the error message
        ACE_ERROR_RETURN((LM_ERROR,
                          ACE_TEXT("(%P|%t) ERROR: %N:%l: main() -")
                              ACE_TEXT(" Exception caught: %C\n"),
                          ex.what()),
                         -1);
    }
    return 0; // Return success
}
