
#include "../../model/UATMTraits.h"
#include <tools/modeling/codegen/model/NullReaderListener.h>
#include <model/Sync.h>
#include <ace/Log_Msg.h>
#include <dds/DCPS/WaitSet.h>
#include "ReaderListenerFlows.h"

ReaderListenerFlows::ReaderListenerFlows(OpenDDS::Model::ReaderCondSync &rcs)
    : rcs_(rcs) {}

void ReaderListenerFlows::on_data_available(DDS::DataReader_ptr reader)
{
  ACE_Guard<ACE_Thread_Mutex> g(mutex_);

  UATM::trafficFlowsInfoDataReader_var reader_i =
      UATM::trafficFlowsInfoDataReader::_narrow(reader);

  if (CORBA::is_nil(reader_i.in()))
  {
    ACE_ERROR((LM_ERROR,
               ACE_TEXT("ERROR: %N:%l: on_data_available_request() -")
                   ACE_TEXT(" _narrow failed!\n")));
    ACE_OS::exit(-1);
  }

  UATM::trafficFlowsInfo msg;
  DDS::SampleInfo info;

  while (true)
  {
    std::cout << "\n\n"
              << std::endl;
    DDS::ReturnCode_t error = reader_i->take_next_sample(msg, info);
    if (error == DDS::RETCODE_OK)
    {
      // std::cout << "SampleInfo.sample_rank = " << info.sample_rank << std::endl;
      if (info.valid_data)
      {
        std::cout << "\n-----------------UASP-----------------" << std::endl
                  << "        trafficFlowsInfo:" << std::endl
                  << "        -----------------" << std::endl
                  << "Flows ID: " << msg.flows_id << std::endl
                  << "Area: " << msg.area.in() << std::endl
                  << "Congestion Level: " << msg.congestion_level.in() << std::endl
                  << "Affected Routes: " << msg.affected_routes << std::endl
                  << "Time: \n"
                  << msg.timestamp.in() << std::endl;
      }
      else
      {
        rcs_.signal();
        break;
      }
    }
    else
    {
      if (error != DDS::RETCODE_NO_DATA)
      {
        ACE_ERROR((LM_ERROR,
                   ACE_TEXT("ERROR: %N:%l: on_data_available_request() -")
                       ACE_TEXT(" take_next_sample failed!\n")));
      }
      rcs_.signal();
      break;
    }
  }
};
