#include "../../model/UATMTraits.h"
#include "ReaderListenerRoute.h"
#include <model/Sync.h>
#include <ace/Log_Msg.h>
#include <dds/DCPS/WaitSet.h>
#include <fstream>  

ReaderListenerRoute::ReaderListenerRoute(OpenDDS::Model::ReaderCondSync& rcs)
  : rcs_(rcs) {}

void
ReaderListenerRoute::on_data_available(DDS::DataReader_ptr reader) 
{
    ACE_Guard<ACE_Thread_Mutex> g(mutex_);

    UATM::flightRoutesInfoDataReader_var reader_i =
      UATM::flightRoutesInfoDataReader::_narrow(reader);

    if (CORBA::is_nil(reader_i.in())) {
        ACE_ERROR((LM_ERROR,
                   ACE_TEXT("ERROR: %N:%l: on_data_available_request() -")
                   ACE_TEXT(" _narrow failed!\n")));
        ACE_OS::exit(-1);
    }

    UATM::flightRoutesInfo msg;
    DDS::SampleInfo info;

    while (true) {
                                                     
        DDS::ReturnCode_t error = reader_i->take_next_sample(msg, info);
        if (error == DDS::RETCODE_OK) {
            if (info.valid_data) {
                std::cout << "| flightRoutesInfo: " 
                          << "flight_route_id:" << msg.flight_route_id 
                          << ",origin_skyport_id:" << msg.origin_skyport_id.in() 
                          << ",destination_skyport_id:" << msg.destination_skyport_id.in() 
                          << ",available_capacity:" << msg.available_capacity 
                          << ",available:" << msg.available 
                          << ",traffic_density:" << msg.traffic_density << std::endl;

                std::ofstream outfile;
                outfile.open("fleetOperatorDP/data/routes.txt", std::ios_base::app);
                
                outfile << "route_id:" << msg.flight_route_id << ","
                        << "origin_skyport_id:" << msg.origin_skyport_id.in() << ","
                        << "destination_skyport_id:" << msg.destination_skyport_id.in() << ","
                        << "available_capacity:" << msg.available_capacity << ","
                        << "available:" << msg.available << ","
                        << "traffic_density:" << msg.traffic_density << std::endl;
                
                outfile.close();
            } else {
                rcs_.signal();
                break;
            }
        } else {
            if (error != DDS::RETCODE_NO_DATA) {
                ACE_ERROR((LM_ERROR,
                           ACE_TEXT("ERROR: %N:%l: on_data_available_request() -")
                           ACE_TEXT(" take_next_sample failed!\n")));
            }
            break;
        }
    }
}
