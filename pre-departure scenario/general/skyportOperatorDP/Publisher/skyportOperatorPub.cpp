#include <iostream>               // Includes the standard input-output stream library for console output.
#include <vector>                 // Includes the vector container class from the C++ Standard Library.
#include <string>                 // Includes the string class to handle text-based data.
#include <cstdlib>                // Includes C standard library functions like random number generation.
#include <ctime>                  // Includes time-related functions, for instance, to generate random values.
#include <thread>                 // Includes functionality to work with threads, such as creating delays.
#include <chrono>                 // Includes time-related utilities, such as calculating elapsed time.
#include "../../model/UATMTraits.h"  // Includes a model-specific header (likely for types used in your application).
#include "../utils/functions.h"      // Includes utility functions used in the application.
#include <model/Sync.h>           // Includes synchronization-related functionality.

int ACE_TMAIN(int argc, ACE_TCHAR **argv)
{
    try
    {
        // Initializes three UATM models with application parameters (likely OpenDDS models).
        OpenDDS::Model::Application application(argc, argv);
        UATM::uatmDCPS::DefaultUATMType model(application, argc, argv);
        UATM::uatmDCPS::DefaultUATMType model2(application, argc, argv);
        UATM::uatmDCPS::DefaultUATMType model3(application, argc, argv);

        // Defines the specific data writers for traffic flows, airspace restrictions, and flight routes.
        DDS::DataWriter_var writer_flows = model.writer(Elements::DataWriters::trafficFlowsDW_SKO);
        UATM::trafficFlowsInfoDataWriter_var writer_flows_var = UATM::trafficFlowsInfoDataWriter::_narrow(writer_flows.in());

        DDS::DataWriter_var writer_rest = model2.writer(Elements::DataWriters::airspaceRestDW_SKO);
        UATM::airspaceRestrictionsDataWriter_var writer_rest_var = UATM::airspaceRestrictionsDataWriter::_narrow(writer_rest.in());

        DDS::DataWriter_var writer_routes = model3.writer(Elements::DataWriters::flightRoutesDW_SKO);
        UATM::flightRoutesInfoDataWriter_var writer_routes_var = UATM::flightRoutesInfoDataWriter::_narrow(writer_routes.in());

        // Checks if any of the writers are nil (failed narrowing).
        if (CORBA::is_nil(writer_flows_var.in()) || CORBA::is_nil(writer_rest_var.in()) || CORBA::is_nil(writer_routes_var.in()))
        {
            ACE_ERROR_RETURN((LM_ERROR, ACE_TEXT("(%P|%t) ERROR: Failed to narrow writer.\n")), -1);
        }

        std::srand(std::time(nullptr));  // Seeds the random number generator with the current time.

        // Initializes identifiers and start time for the data generation loop.
        int flows_id = 1;
        int rest_id = 1;
        int routes_id = 1;
        auto startTime = std::chrono::steady_clock::now();
        double duration = 100.0;  // Duration to run the loop in seconds.

        while (true)
        {
            // Calculates the elapsed time and checks if the loop duration has been reached.
            auto currentTime = std::chrono::steady_clock::now();
            std::chrono::duration<double> elapsedTime = currentTime - startTime;

            if (elapsedTime.count() >= duration)
            {
                // When the loop finishes, writes an empty file and exits the loop.
                std::ofstream outfile("skyportOperatorDP/data/coordinations.txt", std::ofstream::trunc);
                outfile.close();
                break;
            }

            OpenDDS::Model::WriterSync ws(writer_flows);  // Synchronizes the writer.
            {
                // Creates a traffic flow message and sends it to the DDS writer.
                UATM::trafficFlowsInfo tf;
                tf.flows_id = flows_id++;
                tf.area = CORBA::string_dup(getRandomValue(restriction_areas).c_str());
                tf.congestion_level = CORBA::string_dup(getRandomValue(congestion_levels).c_str());
                tf.affected_routes = "1";
                tf.timestamp = CORBA::string_dup(getCurrentTime().c_str());

                DDS::ReturnCode_t error = writer_flows_var->write(tf, DDS::HANDLE_NIL);
                if (error != DDS::RETCODE_OK)
                {
                    ACE_ERROR((LM_ERROR, ACE_TEXT("(%P|%t) ERROR: write for trafficFlows returned %d!\n"), error));
                }
            }

            std::this_thread::sleep_for(std::chrono::seconds(2));  // Pauses the thread for 2 seconds.

            OpenDDS::Model::WriterSync ws2(writer_rest);  // Synchronizes the airspace restrictions writer.
            {
                // Creates an airspace restriction message and sends it to the DDS writer.
                UATM::airspaceRestrictions ar;
                ar.restriction_id = rest_id++;
                ar.restriction_area = CORBA::string_dup(getRandomValue(restriction_areas).c_str());
                ar.restriction_type = "type";  // This could be randomized or updated for more variety.
                ar.restriction_time = CORBA::string_dup(getCurrentTime().c_str());
                ar.restriction_authority = CORBA::string_dup(getRandomValue(authorities).c_str());

                DDS::ReturnCode_t error = writer_rest_var->write(ar, DDS::HANDLE_NIL);
                if (error != DDS::RETCODE_OK)
                {
                    ACE_ERROR((LM_ERROR, ACE_TEXT("(%P|%t) ERROR: write for restrictions returned %d!\n"), error));
                }
            }

            std::this_thread::sleep_for(std::chrono::seconds(2));  // Pauses the thread for 2 seconds.

            OpenDDS::Model::WriterSync ws3(writer_routes);  // Synchronizes the flight routes writer.
            {
                // Creates a flight route message and sends it to the DDS writer.
                UATM::flightRoutesInfo fr;
                fr.flight_route_id = routes_id++;
                fr.origin_skyport_id = CORBA::string_dup(getRandomValue(skyports).c_str());
                fr.destination_skyport_id = CORBA::string_dup(generateDestinationSkyportId(std::string(fr.origin_skyport_id)).c_str());
                fr.available_capacity = std::rand() % 20;
                fr.available = std::rand() % 2;
                fr.traffic_density = CORBA::string_dup(getRandomValue(traffic_density_levels).c_str());

                DDS::ReturnCode_t error = writer_routes_var->write(fr, DDS::HANDLE_NIL);
                if (error != DDS::RETCODE_OK)
                {
                    ACE_ERROR((LM_ERROR, ACE_TEXT("(%P|%t) ERROR: write for flightRoutes returned %d!\n"), error));
                }
            }

            std::this_thread::sleep_for(std::chrono::seconds(2));  // Pauses the thread for 2 seconds.
        }
    }
    catch (const CORBA::Exception &e)
    {
        // Catches and logs any CORBA exceptions.
        e._tao_print_exception("Exception caught in main():");
        return -1;
    }
    catch (const std::exception &ex)
    {
        // Catches and logs any general exceptions.
        ACE_ERROR_RETURN((LM_ERROR, ACE_TEXT("(%P|%t) ERROR: main() - Exception caught: %C\n"), ex.what()), -1);
    }

    return 0;  // Successful execution of the program.
}
