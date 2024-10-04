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

    while (true) {
      DDS::ReturnCode_t error = reader_i->take_next_sample(msg, info);
      if (error == DDS::RETCODE_OK) {
        if (info.valid_data) {
          if (msg.acceptable_route_id == 0) {
            break;
          }else {
          std::cout << "| ansp acceptableRoute: " 
                    << "acceptable_route_id:" << msg.acceptable_route_id
                    << ",approved_by:" << msg.approved_by.in() 
                    << ",estimated_time:" << msg.estimated_time.in() 
                    << ",flight_id:" << msg.flight_id.in() 
                    << ",timestamp:" << msg.timestamp.in() << std::endl;
          }
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
