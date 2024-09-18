#include "../model/UATMTraits.h"
#include "ReaderListenerAvailability.h"
#include <model/Sync.h>
#include <ace/Log_Msg.h>
#include <dds/DCPS/WaitSet.h>

// Construtor da classe
ReaderListenerAvailability::ReaderListenerAvailability(OpenDDS::Model::ReaderCondSync& rcs)
  : rcs_(rcs) {}

void
ReaderListenerAvailability::on_data_available(DDS::DataReader_ptr reader) 
  {
    ACE_Guard<ACE_Thread_Mutex> g(mutex_);

    UATM::availabilityInfoDataReader_var reader_i =
      UATM::availabilityInfoDataReader::_narrow(reader);

    if (CORBA::is_nil(reader_i.in())) {
    ACE_ERROR((LM_ERROR,
               ACE_TEXT("ERROR: %N:%l: on_data_available_request() -")
               ACE_TEXT(" _narrow failed!\n")));
    ACE_OS::exit(-1);
  }

    UATM::availabilityInfo msg;
    DDS::SampleInfo info;

    // Read until no more messages
    while (true) {
      DDS::ReturnCode_t error = reader_i->take_next_sample(msg, info);
      if (error == DDS::RETCODE_OK) {
        std::cout << "SampleInfo.sample_rank = " << info.sample_rank << std::endl;
        if (info.valid_data) {
          std::cout << "----------------------------------" << std::endl
                    << "        AvailabilityInfo:" << std::endl
                    << "        -----------------" << std::endl
                    << "Resource ID: " << msg.resource_id << std::endl
                    << "Resource Type: " << msg.resource_type.in() << std::endl
                    << "Status: " << msg.status << std::endl
                    << "Location: " << msg.location.in() << std::endl
                    << "Availability Time: " << msg.availability_time.in() << std::endl;
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
        rcs_.signal();
        break;
      }
    }
  }