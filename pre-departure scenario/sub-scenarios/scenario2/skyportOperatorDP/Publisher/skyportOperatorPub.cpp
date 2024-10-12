#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <chrono>
#include "../../model/UATMTraits.h"
#include "../utils/functions.h"
#include <model/Sync.h>


int ACE_TMAIN(int argc, ACE_TCHAR **argv)
{
    try
    {
        OpenDDS::Model::Application application(argc, argv);
        UATM::uatmDCPS::DefaultUATMType model3(application, argc, argv);

        using OpenDDS::Model::UATM::uatmDCPS::Elements;

        DDS::DataWriter_var writer = model3.writer(Elements::DataWriters::flightRoutesDW_SKO);
        UATM::flightRoutesInfoDataWriter_var writer_var = UATM::flightRoutesInfoDataWriter::_narrow(writer.in());

        if (CORBA::is_nil(writer_var.in()))
        {
            ACE_ERROR_RETURN((LM_ERROR,
                              ACE_TEXT("(%P|%t) ERROR: %N:%l: main() -")
                                  ACE_TEXT(" _narrow failed!\n")),
                             -1);
        }

        std::srand(static_cast<unsigned>(std::time(0)));

        while (true)
        {
            std::vector<FlightRoute> flightRoutes = readFlightRoutes("skyportOperatorDP/data/flight-routes.txt");

            if (flightRoutes.empty())
            {
                std::cerr << "Nenhuma rota encontrada no arquivo." << std::endl;
                return -1;
            }

            int randomRouteIndex = std::rand() % flightRoutes.size();
            FlightRoute &route = flightRoutes[randomRouteIndex];

            UATM::flightRoutesInfo fr;

            fr.route_id = route.route_id;
            fr.origin_skyport_id = CORBA::string_dup(route.origin_skyport_id.c_str());
            fr.destination_skyport_id = CORBA::string_dup(route.destination_skyport_id.c_str());
            fr.available_capacity = route.available_capacity;
            fr.available = route.available;
            fr.traffic_density = CORBA::string_dup(route.traffic_density.c_str());

            DDS::ReturnCode_t error = writer_var->write(fr, DDS::HANDLE_NIL);

            if (error != DDS::RETCODE_OK)
            {
                ACE_ERROR((LM_ERROR,
                           ACE_TEXT("(%P|%t) ERROR: %N:%l: main() -")
                               ACE_TEXT(" write returned %d!\n"),
                           error));
            }

            std::this_thread::sleep_for(std::chrono::seconds(10));
        }
    }
    catch (const CORBA::Exception &e)
    {
        e._tao_print_exception("Exception caught in main():");
        return -1;
    }
    catch (const std::exception &ex)
    {
        ACE_ERROR_RETURN((LM_ERROR,
                          ACE_TEXT("(%P|%t) ERROR: %N:%l: main() -")
                              ACE_TEXT(" Exception caught: %C\n"),
                          ex.what()),
                         -1);
    }

    return 0;
}
