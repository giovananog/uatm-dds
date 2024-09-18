 
#include "../model/UATMTraits.h"
#include < tools/modeling/codegen/model/NullReaderListener.h>

#include <model/Sync.h>
#include <ace/Log_Msg.h>

#include <dds/DCPS/WaitSet.h>
#include "ReaderListenerRest.h"

ReaderListenerRest::ReaderListenerRest(OpenDDS::Model::ReaderCondSync& rcs)
  : rcs_(rcs) {}

void
ReaderListenerRest::on_data_available(DDS::DataReader_ptr reader) 
  {
    ACE_Guard<ACE_Thread_Mutex> g(mutex_);

    UATM::airspaceRestrictionsDataReader_var reader_i =
      UATM::airspaceRestrictionsDataReader::_narrow(reader);

    if (CORBA::is_nil(reader_i.in())) {
    ACE_ERROR((LM_ERROR,
               ACE_TEXT("ERROR: %N:%l: on_data_available() -")
               ACE_TEXT(" _narrow failed!\n")));
    ACE_OS::exit(-1);
  }

    UATM::airspaceRestrictions msg;
    DDS::SampleInfo info;

    // Read until no more messages
    while (true) {
      DDS::ReturnCode_t error = reader_i->take_next_sample(msg, info);
      if (error == DDS::RETCODE_OK) {
        std::cout << "SampleInfo.sample_rank = " << info.sample_rank << std::endl;
        if (info.valid_data) {
          std::cout << "----------------------------------" << std::endl
                    << "        airspaceRestrictions:" << std::endl
                    << "        -----------------" << std::endl
                    << "Rest ID: " << msg.restriction_id << std::endl
                    << "Area: " << msg.area.in() << std::endl
                    << "Type: " << msg.restriction_type.in() << std::endl
                    << "Start Time: " << msg.start_time.in() << std::endl
                    << "End Time: " << msg.end_time.in() << std::endl
                    << "Approved by: " << msg.authority.in() << std::endl;
        } else {
            rcs_.signal();
            std::cout << "Received sample, but no valid data." << std::endl;
        }
        // break;
      } else {
        if (error != DDS::RETCODE_NO_DATA) {
        ACE_ERROR((LM_ERROR,
                   ACE_TEXT("ERROR: %N:%l: on_data_available_request() -")
                   ACE_TEXT(" take_next_sample failed!\n")));
        }
        rcs_.signal();
        break;
      }
    }
  };
