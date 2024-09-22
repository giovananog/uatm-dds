

#include "../../model/UATMTraits.h"
#include <tools/modeling/codegen/model/NullReaderListener.h>

#include <model/Sync.h>
#include <ace/Log_Msg.h>

#include <dds/DCPS/WaitSet.h>
#include "ReaderListenerRequest.h"

ReaderListenerRequest::ReaderListenerRequest(OpenDDS::Model::ReaderCondSync &rcs)
    : rcs_(rcs) {}

void ReaderListenerRequest::on_data_available(DDS::DataReader_ptr reader)
{
  ACE_Guard<ACE_Thread_Mutex> g(mutex_);

  UATM::tolPadRequestDataReader_var reader_i =
      UATM::tolPadRequestDataReader::_narrow(reader);

  if (CORBA::is_nil(reader_i.in()))
  {
    ACE_ERROR((LM_ERROR,
               ACE_TEXT("ERROR: %N:%l: on_data_available() -")
                   ACE_TEXT(" _narrow failed!\n")));
    ACE_OS::exit(-1);
  }

  UATM::tolPadRequest msg;
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
        std::cout << "----------------------------------" << std::endl
                  << "        tolPadRequest:" << std::endl
                  << "        -----------------" << std::endl
                  << "Asign ID: " << msg.assign_id << std::endl
                  << "Flight ID: " << msg.flight_id << std::endl
                  << "Tol Pad ID: " << msg.tol_pad_id << std::endl
                  << "Assign Time: " << msg.assign_time.in() << std::endl;
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
                   ACE_TEXT("ERROR: %N:%l: on_data_available() -")
                       ACE_TEXT(" take_next_sample failed!\n")));
      }
      rcs_.signal();
      break;
    }
  }
};
