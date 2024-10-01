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
    UATM::uatmDCPS::DefaultUATMType model(application, argc, argv);
    UATM::uatmDCPS::DefaultUATMType model2(application, argc, argv);
    UATM::uatmDCPS::DefaultUATMType model3(application, argc, argv);

    using OpenDDS::Model::UATM::uatmDCPS::Elements;

    DDS::DataWriter_var writer_flows = model.writer(Elements::DataWriters::trafficFlowsDW_SKO);
    UATM::trafficFlowsInfoDataWriter_var writer_flows_var = UATM::trafficFlowsInfoDataWriter::_narrow(writer_flows.in());

    if (CORBA::is_nil(writer_flows_var.in()))
    {
      ACE_ERROR_RETURN((LM_ERROR,
                        ACE_TEXT("(%P|%t) ERROR: %N:%l: main() -")
                            ACE_TEXT(" _narrow failed!\n")),
                       -1);
    }

    DDS::DataWriter_var writer_rest = model2.writer(Elements::DataWriters::airspaceRestDW_SKO);
    UATM::airspaceRestrictionsDataWriter_var writer_rest_var = UATM::airspaceRestrictionsDataWriter::_narrow(writer_rest.in());

    if (CORBA::is_nil(writer_rest_var.in()))
    {
      ACE_ERROR_RETURN((LM_ERROR,
                        ACE_TEXT("(%P|%t) ERROR: %N:%l: main() -")
                            ACE_TEXT(" _narrow failed!\n")),
                       -1);
    }

    DDS::DataWriter_var writer = model3.writer(Elements::DataWriters::flightRoutesDW_SKO);
    UATM::flightRoutesInfoDataWriter_var writer_var = UATM::flightRoutesInfoDataWriter::_narrow(writer.in());

    if (CORBA::is_nil(writer_var.in()))
    {
      ACE_ERROR_RETURN((LM_ERROR,
                        ACE_TEXT("(%P|%t) ERROR: %N:%l: main() -")
                            ACE_TEXT(" _narrow failed!\n")),
                       -1);
    }

    while (true)
    {

      std::srand(std::time(nullptr));

        std::vector<TrafficFlow> trafficFlows = readTrafficFlows("skyportOperatorDP/data/traffic-flows.txt");
        std::vector<Restriction> restrictions = readRestrictions("skyportOperatorDP/data/airspace-rest.txt");
        std::vector<FlightRoute> flightRoutes = readFlightRoutes("skyportOperatorDP/data/flight-routes.txt");

        if (trafficFlows.empty() || restrictions.empty())
        {
          std::cerr << "Erro ao ler os arquivos de dados!" << std::endl;
          return 1;
        }

        OpenDDS::Model::WriterSync ws(writer_flows);
        {
          int randomFlowIndex = std::rand() % trafficFlows.size();
          TrafficFlow &flow = trafficFlows[randomFlowIndex];

          UATM::trafficFlowsInfo tf;

          tf.flows_id = 121;
          tf.area = CORBA::string_dup(flow.area.c_str());
          tf.congestion_level = CORBA::string_dup(flow.congestion_level.c_str());
          tf.timestamp = CORBA::string_dup(flow.timestamp.c_str());

          DDS::ReturnCode_t error = writer_flows_var->write(tf, DDS::HANDLE_NIL);

          if (error != DDS::RETCODE_OK)
          {
            ACE_ERROR((LM_ERROR,
                       ACE_TEXT("(%P|%t) ERROR: %N:%l: main() -")
                           ACE_TEXT(" write returned %d!\n"),
                       error));
          }
        }

        std::this_thread::sleep_for(std::chrono::seconds(9));

        OpenDDS::Model::WriterSync ws2(writer_rest);
        {
          int randomRestrictionIndex = std::rand() % restrictions.size();
          Restriction &restriction = restrictions[randomRestrictionIndex];

          UATM::airspaceRestrictions ar;

          ar.restriction_id = 212;
          ar.restriction_area = CORBA::string_dup(restriction.restriction_area.c_str());
          ar.restriction_type = CORBA::string_dup(restriction.restriction_type.c_str());
          ar.restriction_time = "a";
          ar.restriction_authority = CORBA::string_dup(restriction.restriction_authority.c_str());

          DDS::ReturnCode_t error = writer_rest_var->write(ar, DDS::HANDLE_NIL);

          if (error != DDS::RETCODE_OK)
          {
            ACE_ERROR((LM_ERROR,
                       ACE_TEXT("(%P|%t) ERROR: %N:%l: main() -")
                           ACE_TEXT(" write returned %d!\n"),
                       error));
          }
        }

        std::this_thread::sleep_for(std::chrono::seconds(7));

        OpenDDS::Model::WriterSync ws3(writer_rest);
        {

          if (flightRoutes.empty())
          {
            std::cerr << "Nenhuma rota encontrada no arquivo." << std::endl;
            return -1;
          }

          int randomRouteIndex = std::rand() % flightRoutes.size();
          FlightRoute &route = flightRoutes[randomRouteIndex];

          UATM::flightRoutesInfo fr;

          fr.flight_route_id = route.route_id;
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
