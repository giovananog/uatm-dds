#include <iostream>
#include <vector>
#include <string>
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

        DDS::DataWriter_var writer_rest = model2.writer(Elements::DataWriters::airspaceRestDW_SKO);
        UATM::airspaceRestrictionsDataWriter_var writer_rest_var = UATM::airspaceRestrictionsDataWriter::_narrow(writer_rest.in());

        DDS::DataWriter_var writer_routes = model3.writer(Elements::DataWriters::flightRoutesDW_SKO);
        UATM::flightRoutesInfoDataWriter_var writer_routes_var = UATM::flightRoutesInfoDataWriter::_narrow(writer_routes.in());

        if (CORBA::is_nil(writer_flows_var.in()) || CORBA::is_nil(writer_rest_var.in()) || CORBA::is_nil(writer_routes_var.in())) {
            ACE_ERROR_RETURN((LM_ERROR, ACE_TEXT("(%P|%t) ERROR: Failed to narrow writer.\n")), -1);
        }

        std::srand(std::time(nullptr));  

        int flows_id = 1;
        int rest_id = 1;
        int routes_id = 1;

        while (flows_id != 2)
        {
            OpenDDS::Model::WriterSync ws(writer_flows);
            {

                UATM::trafficFlowsInfo tf;
                tf.flows_id = flows_id++;
                tf.area = CORBA::string_dup(getRandomValue(areas).c_str());
                tf.congestion_level = CORBA::string_dup(getRandomValue(congestion_levels).c_str());
                tf.affected_routes = "1";
                tf.timestamp = CORBA::string_dup("time");  

                DDS::ReturnCode_t error = writer_flows_var->write(tf, DDS::HANDLE_NIL);
                if (error != DDS::RETCODE_OK) {
                    ACE_ERROR((LM_ERROR, ACE_TEXT("(%P|%t) ERROR: write for trafficFlows returned %d!\n"), error));
                }
            }

            std::this_thread::sleep_for(std::chrono::seconds(9));

            OpenDDS::Model::WriterSync ws2(writer_rest);
            {

                UATM::airspaceRestrictions ar;
                ar.restriction_id = rest_id++;  
                ar.restriction_area = CORBA::string_dup(getRandomValue(restriction_areas).c_str());
                // ar.restriction_type = CORBA::string_dup(getRandomValue(restriction_types).c_str());
                ar.restriction_type = "type";
                ar.restriction_time = "time";  
                ar.restriction_authority = CORBA::string_dup(getRandomValue(authorities).c_str());

                DDS::ReturnCode_t error = writer_rest_var->write(ar, DDS::HANDLE_NIL);
                if (error != DDS::RETCODE_OK) {
                    ACE_ERROR((LM_ERROR, ACE_TEXT("(%P|%t) ERROR: write for restrictions returned %d!\n"), error));
                }
            }

            std::this_thread::sleep_for(std::chrono::seconds(7));

                OpenDDS::Model::WriterSync ws3(writer_routes);
            {

                UATM::flightRoutesInfo fr;
                fr.flight_route_id = routes_id++;  
                fr.origin_skyport_id = CORBA::string_dup(getRandomValue(skyports).c_str());
                fr.destination_skyport_id = CORBA::string_dup(getRandomValue(skyports).c_str());
                fr.available_capacity = std::rand() % 20;  
                fr.available = std::rand() % 2;  
                fr.traffic_density = CORBA::string_dup(getRandomValue(traffic_density_levels).c_str());

                DDS::ReturnCode_t error = writer_routes_var->write(fr, DDS::HANDLE_NIL);
                if (error != DDS::RETCODE_OK) {
                    ACE_ERROR((LM_ERROR, ACE_TEXT("(%P|%t) ERROR: write for flightRoutes returned %d!\n"), error));
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
        ACE_ERROR_RETURN((LM_ERROR, ACE_TEXT("(%P|%t) ERROR: main() - Exception caught: %C\n"), ex.what()), -1);
    }

    return 0;
}
