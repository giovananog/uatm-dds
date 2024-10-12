  
#include "../../model/UATMTraits.h"
#include <tools/modeling/codegen/model/NullReaderListener.h>

#include <model/Sync.h>
#include <ace/Log_Msg.h>

#include <dds/DCPS/WaitSet.h>
#include "ReaderListenerAuth.h"

ReaderListenerAuth::ReaderListenerAuth(OpenDDS::Model::ReaderCondSync& rcs)
  : rcs_(rcs) {}

void
ReaderListenerAuth::on_data_available(DDS::DataReader_ptr reader) 
  {
    ACE_Guard<ACE_Thread_Mutex> g(mutex_);

    UATM::flightAuthorizationDataReader_var reader_i =
      UATM::flightAuthorizationDataReader::_narrow(reader);

    if (CORBA::is_nil(reader_i.in())) {
    ACE_ERROR((LM_ERROR,
               ACE_TEXT("ERROR: %N:%l: on_data_available_request() -")
               ACE_TEXT(" _narrow failed!\n")));
    ACE_OS::exit(-1);
  }

    UATM::flightAuthorization msg;
    DDS::SampleInfo info;

    while (true) {
      DDS::ReturnCode_t error = reader_i->take_next_sample(msg, info);
      if (error == DDS::RETCODE_OK) {
        std::cout << "SampleInfo.sample_rank = " << info.sample_rank << std::endl;
        if (info.valid_data) {
          std::cout << "----------------------------------" << std::endl
                    << "        flightAuthorization:" << std::endl
                    << "        -----------------" << std::endl
                    << "Auth ID: " << msg.authorization_id.in() << std::endl
                    << "Flight ID: " << msg.flight_id.in() << std::endl
                    << "Ap Route ID: " << msg.approved_route_id.in() << std::endl
                    << "authorization_status: " << msg.authorization_status << std::endl
                    << "Auth Time: " << msg.authorization_time.in() << std::endl
                    << "approved_departure_time: " << msg.approved_departure_time.in() << std::endl
                    << "approved_arrival_time: " << msg.approved_arrival_time.in() << std::endl
                    << "tolpad_id: " << msg.tolpad_id.in() << std::endl
                    << "pilot_id: " << msg.pilot_id.in() << std::endl
                    << "evtol_id: " << msg.evtol_id.in() << std::endl;
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
        // rcs_.signal();
        break;
      }
    }
  };
