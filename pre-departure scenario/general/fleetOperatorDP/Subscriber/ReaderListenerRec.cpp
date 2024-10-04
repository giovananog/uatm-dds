
#include "../../model/UATMTraits.h"
#include <tools/modeling/codegen/model/NullReaderListener.h>

#include <model/Sync.h>
#include <ace/Log_Msg.h>

#include "ReaderListenerRec.h"
#include <dds/DCPS/WaitSet.h>

ReaderListenerRec::ReaderListenerRec(OpenDDS::Model::ReaderCondSync& rcs)
  : rcs_(rcs) {}

void
ReaderListenerRec::on_data_available(DDS::DataReader_ptr reader) 
  {
    ACE_Guard<ACE_Thread_Mutex> g(mutex_);

    UATM::flightChangeRecDataReader_var reader_i =
      UATM::flightChangeRecDataReader::_narrow(reader);

    if (CORBA::is_nil(reader_i.in())) {
    ACE_ERROR((LM_ERROR,
               ACE_TEXT("ERROR: %N:%l: on_data_available_request() -")
               ACE_TEXT(" _narrow failed!\n")));
    ACE_OS::exit(-1);
  }

    UATM::flightChangeRec msg;
    DDS::SampleInfo info;

    while (true) {
      DDS::ReturnCode_t error = reader_i->take_next_sample(msg, info);
      if (error == DDS::RETCODE_OK) {
        if (info.valid_data) {
          if(msg.recommendation_id == 0) {
            break;
          }else {
          std::cout << "\n\n\n| plm flightChangeRec: " 
                    << "recommendation_id:" << msg.recommendation_id 
                    << ",flight_id:" << msg.flight_id 
                    << ",change_type:" << msg.change_type.in() 
                    << ",reason:" << msg.reason.in() 
                    << ",recommended_by:" << msg.recommended_by.in() 
                    << ",recommendation_time:" << msg.recommendation_time.in() << std::endl;        
          }     
        } else {
            // std::cout << "\n\n| flightChangeRec: signal\n\n";
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
  };
