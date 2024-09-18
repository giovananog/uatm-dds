  
#include "../model/UATMTraits.h"
#include < tools/modeling/codegen/model/NullReaderListener.h>

#include <model/Sync.h>
#include <ace/Log_Msg.h>

#include <dds/DCPS/WaitSet.h>
#include "ReaderListenerRequest.h"

ReaderListenerRequest::ReaderListenerRequest(OpenDDS::Model::ReaderCondSync& rcs)
  : rcs_(rcs) {}

void
ReaderListenerRequest::on_data_available(DDS::DataReader_ptr reader) 
  {
    ACE_Guard<ACE_Thread_Mutex> g(mutex_);

    UATM::acceptableRouteDataReader_var reader_i =
      UATM::acceptableRouteDataReader::_narrow(reader);

    if (CORBA::is_nil(reader_i.in())) {
    ACE_ERROR((LM_ERROR,
               ACE_TEXT("ERROR: %N:%l: on_data_available() -")
               ACE_TEXT(" _narrow failed!\n")));
    ACE_OS::exit(-1);
  }

    UATM::acceptableRoute msg;
    DDS::SampleInfo info;

    // Read until no more messages
    while (true) {
      DDS::ReturnCode_t error = reader_i->take_next_sample(msg, info);
      if (error == DDS::RETCODE_OK) {
        std::cout << "SampleInfo.sample_rank = " << info.sample_rank << std::endl;
        if (info.valid_data) {
          std::cout << "----------------------------------" << std::endl
                    << "        acceptableRoute:" << std::endl
                    << "        -----------------" << std::endl
                    << "Route ID: " << msg.route_id << std::endl
                    << "Origin: " << msg.origin.in() << std::endl
                    << "Destination: " << msg.destination.in() << std::endl
                    << "Estimated Time: " << msg.estimated_time.in() << std::endl
                    << "Timestmap: " << msg.timestamp.in() << std::endl
                    << "Approved by: " << msg.approved_by.in() << std::endl;
        } else {
            rcs_.signal();
            std::cout << "Received sample, but no valid data." << std::endl;
        }
      } else {
        if (error != DDS::RETCODE_NO_DATA) {
        ACE_ERROR((LM_ERROR,
                   ACE_TEXT("ERROR: %N:%l: on_data_available() -")
                   ACE_TEXT(" take_next_sample failed!\n")));
        }
        rcs_.signal();
        break;
      }
    }
  };
