  
#include "../model/UATMTraits.h"
#include < tools/modeling/codegen/model/NullReaderListener.h>

#include <model/Sync.h>
#include <ace/Log_Msg.h>
#include <dds/DCPS/WaitSet.h>

#include "ReaderListenerCoordination.h"

ReaderListenerCoordination::ReaderListenerCoordination(OpenDDS::Model::ReaderCondSync& rcs)
  : rcs_(rcs) {}

void
ReaderListenerCoordination::on_data_available(DDS::DataReader_ptr reader) 
  {
    ACE_Guard<ACE_Thread_Mutex> g(mutex_);

    UATM::flightCoordinationDataReader_var reader_i =
      UATM::flightCoordinationDataReader::_narrow(reader);

    if (CORBA::is_nil(reader_i.in())) {
    ACE_ERROR((LM_ERROR,
               ACE_TEXT("ERROR: %N:%l: on_data_available() -")
               ACE_TEXT(" _narrow failed!\n")));
    ACE_OS::exit(-1);
  }

    UATM::flightCoordination msg;
    DDS::SampleInfo info;

    // Read until no more messages
    while (true) {
      DDS::ReturnCode_t error = reader_i->take_next_sample(msg, info);
      if (error == DDS::RETCODE_OK) {
        std::cout << "SampleInfo.sample_rank = " << info.sample_rank << std::endl;
        if (info.valid_data) {
          std::cout << "----------------------------------" << std::endl
                    << "        flightCoordination:" << std::endl
                    << "        -----------------" << std::endl
                    << "Coord ID: " << msg.coordination_id << std::endl
                    << "Flight ID: " << msg.flight_id << std::endl
                    << "Involved parties: " << msg.involved_parties << std::endl
                    << "Coord Details: " << msg.coordination_details << std::endl
                    << "Rec time: " << msg.recommendation_time.in() << std::endl;
        } else {
            std::cout << "Received sample, but no valid data." << std::endl;
            rcs_.signal();
            // break;
        }
        // break;
      } else {
        if (error != DDS::RETCODE_NO_DATA) {
        ACE_ERROR((LM_ERROR,
                   ACE_TEXT("ERROR: %N:%l: on_data_available() -")
                   ACE_TEXT(" take_next_sample failed!\n")));
        }
        rcs_.signal();
        // break;
      }
    }
  };
