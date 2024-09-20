#include "../../model/UATMTraits.h"

#include "ReaderListenerRoute.h"
#include <model/Sync.h>
#include <ace/Log_Msg.h>
#include <dds/DCPS/WaitSet.h>


// Construtor da classe
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

    // Read until no more messages
    while (true) {
      DDS::ReturnCode_t error = reader_i->take_next_sample(msg, info);
      if (error == DDS::RETCODE_OK) {
        std::cout << "SampleInfo.sample_rank = " << info.sample_rank << std::endl;
        if (info.valid_data) {
          std::cout << "----------------------------------" << std::endl
                    << "        flightRoutesInfo:" << std::endl
                    << "        -----------------" << std::endl
                    << "Flight Route ID: " << msg.flight_route_id << std::endl
                    << "Waypoints: " << msg.waypoints << std::endl
                    << "Fligt ID: " << msg.flight_id << std::endl
                    << "Estimated Time: " << msg.estimated_time.in() << std::endl
                    << "Status: " << msg.status << std::endl;
        } else {
            rcs_.signal();
            // break;
            std::cout << "Received sample, but no valid data." << std::endl;
        }
        // break;
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