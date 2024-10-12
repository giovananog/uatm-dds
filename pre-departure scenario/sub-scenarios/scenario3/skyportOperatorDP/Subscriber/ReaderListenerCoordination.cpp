  
#include "../../model/UATMTraits.h"
#include <tools/modeling/codegen/model/NullReaderListener.h>

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

    while (true) {
      DDS::ReturnCode_t error = reader_i->take_next_sample(msg, info);
      if (error == DDS::RETCODE_OK) {
        std::cout << "SampleInfo.sample_rank = " << info.sample_rank << std::endl;
        if (info.valid_data) {
          std::cout << "----------------------------------" << std::endl
                    << "        flightCoordination:" << std::endl
                    << "        -----------------" << std::endl
                    << "Coord ID: " << msg.coordination_id.in() << std::endl
                    << "flight_id: " << msg.flight_id.in() << std::endl
                    << "skyport_id: " << msg.skyport_id.in() << std::endl
                    << "evtol_id: " << msg.evtol_id.in() << std::endl
                    << "pilot_id: " << msg.pilot_id.in() << std::endl
                    << "route_id: " << msg.route_id.in() << std::endl
                    << "weather_id: " << msg.weather_id.in() << std::endl;
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
        // rcs_.signal();
        break;
      }
    }
  };
