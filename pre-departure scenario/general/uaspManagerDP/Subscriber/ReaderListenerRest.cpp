 
#include "../../model/UATMTraits.h"
#include <tools/modeling/codegen/model/NullReaderListener.h>

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

    while (true) {
      DDS::ReturnCode_t error = reader_i->take_next_sample(msg, info);
      if (error == DDS::RETCODE_OK) {
        if (info.valid_data) {
          std::cout << "| airspaceRestrictions: " 
                    << "restriction_id:" << msg.restriction_id 
                    << ",restriction_area:" << msg.restriction_area.in() 
                    << ",restriction_type:" << msg.restriction_type.in() 
                    << ",restriction_time:" << msg.restriction_time.in() 
                    << ",restriction_authority:" << msg.restriction_authority.in() << std::endl;

          std::ofstream outfile;
                outfile.open("uaspManagerDP/data/restrictions.txt", std::ios_base::app);
                
                outfile << "restriction_id:" << msg.restriction_id << ","
                        << "restriction_area:" << msg.restriction_area.in() << ","
                        << "restriction_type:" << msg.restriction_type << ","
                        << "restriction_time:" << msg.restriction_time << ","
                        << "restriction_authority:" << msg.restriction_authority.in() << std::endl;
                
                outfile.close();
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
        break;
      }
    }
  };
