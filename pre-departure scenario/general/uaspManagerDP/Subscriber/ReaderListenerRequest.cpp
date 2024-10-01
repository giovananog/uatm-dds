
#include "../../model/UATMTraits.h"
#include <tools/modeling/codegen/model/NullReaderListener.h>

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

    UATM::flightRequestInfoDataReader_var reader_i =
      UATM::flightRequestInfoDataReader::_narrow(reader);

    if (CORBA::is_nil(reader_i.in())) {
    ACE_ERROR((LM_ERROR,
               ACE_TEXT("ERROR: %N:%l: on_data_available() -")
               ACE_TEXT(" _narrow failed!\n")));
    ACE_OS::exit(-1);
  }

    UATM::flightRequestInfo msg;
    DDS::SampleInfo info;

    while (true) {
      DDS::ReturnCode_t error = reader_i->take_next_sample(msg, info);
      if (error == DDS::RETCODE_OK) {
        if (info.valid_data) {
          std::cout << "| flightRequestInfo: " 
                    << "request_id:" << msg.request_id.in()
                    << ",flight_id:" << msg.flight_id.in()
                    << ",departure_skyport_id:" << msg.departure_skyport_id.in()
                    << ",destination_skyport_id:" << msg.destination_skyport_id.in()
                    << ",departure_time:" << msg.departure_time.in()
                    << ",pilot_id:" << msg.pilot_id.in()
                    << ",evtol_id:" << msg.evtol_id.in() << std::endl;


          std::ofstream request_file("uaspManagerDP/data/requests.txt", std::ios_base::app);
          request_file << "request_id:" << msg.request_id.in() << ","
                             << "flight_id:" << msg.flight_id.in() << ","
                             << "departure_skyport_id:" << msg.departure_skyport_id.in() << ","
                             << "destination_skyport_id:" << msg.destination_skyport_id.in() << ","
                             << "departure_time:" << msg.departure_time.in() << ","
                             << "tolpad_id:" << "" << ","
                             << "pilot_id:" << msg.pilot_id.in() << ","
                             << "evtol_id:" << msg.evtol_id.in() << std::endl;
          request_file.close();

        } else {
            rcs_.signal();
            break;
        }
      } else {
        if (error != DDS::RETCODE_NO_DATA) {
        ACE_ERROR((LM_ERROR,
                   ACE_TEXT("ERROR: %N:%l: on_data_available() -")
                   ACE_TEXT(" take_next_sample failed!\n")));
        }
        break;
      }
    }
  };
