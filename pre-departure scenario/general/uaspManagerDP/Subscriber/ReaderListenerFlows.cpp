
#include "../../model/UATMTraits.h"
#include <tools/modeling/codegen/model/NullReaderListener.h>
#include <model/Sync.h>
#include <ace/Log_Msg.h>
#include <dds/DCPS/WaitSet.h>
#include "ReaderListenerFlows.h"


ReaderListenerFlows::ReaderListenerFlows(OpenDDS::Model::ReaderCondSync& rcs)
  : rcs_(rcs) {}

void
ReaderListenerFlows::on_data_available(DDS::DataReader_ptr reader) 
  {
    ACE_Guard<ACE_Thread_Mutex> g(mutex_);

    UATM::trafficFlowsInfoDataReader_var reader_i =
      UATM::trafficFlowsInfoDataReader::_narrow(reader);

    if (CORBA::is_nil(reader_i.in())) {
    ACE_ERROR((LM_ERROR,
               ACE_TEXT("ERROR: %N:%l: on_data_available_request() -")
               ACE_TEXT(" _narrow failed!\n")));
    ACE_OS::exit(-1);
  }

    UATM::trafficFlowsInfo msg;
    DDS::SampleInfo info;

    while (true) {
      DDS::ReturnCode_t error = reader_i->take_next_sample(msg, info);
      if (error == DDS::RETCODE_OK) {
        if (info.valid_data) {
          std::cout << "| trafficFlowsInfo: " 
                    << "flows_id:" << msg.flows_id 
                    << ",area:" << msg.area.in() 
                    << ",congestion_level:" << msg.congestion_level.in() 
                    << ",affected_routes:" << msg.affected_routes.in() 
                    << ",timestamp:" << msg.timestamp.in() << std::endl;

           std::ofstream outfile;
                outfile.open("uaspManagerDP/data/flows.txt", std::ios_base::app);
                
                outfile << "flows_id:" << msg.flows_id << ","
                        << "area:" << msg.area.in() << ","
                        << "congestion_level:" << msg.congestion_level << ","
                        << "timestamp:" << msg.timestamp << ","
                        << "affected_routes:" << msg.affected_routes.in() << std::endl;
                
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
