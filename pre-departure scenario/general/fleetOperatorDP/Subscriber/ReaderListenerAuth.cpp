
#include "../../model/UATMTraits.h"
#include <tools/modeling/codegen/model/NullReaderListener.h>

#include <model/Sync.h>
#include <ace/Log_Msg.h>

#include <dds/DCPS/WaitSet.h>
#include "ReaderListenerAuth.h"

ReaderListenerAuth::ReaderListenerAuth(OpenDDS::Model::ReaderCondSync &rcs)
    : rcs_(rcs) {}

void ReaderListenerAuth::on_data_available(DDS::DataReader_ptr reader)
{
  ACE_Guard<ACE_Thread_Mutex> g(mutex_);

  UATM::flightAuthorizationDataReader_var reader_i =
      UATM::flightAuthorizationDataReader::_narrow(reader);

  if (CORBA::is_nil(reader_i.in()))
  {
    ACE_ERROR((LM_ERROR,
               ACE_TEXT("ERROR: %N:%l: on_data_available_request() -")
                   ACE_TEXT(" _narrow failed!\n")));
    ACE_OS::exit(-1);
  }

  UATM::flightAuthorization msg;
  DDS::SampleInfo info;

  while (true)
  {
    DDS::ReturnCode_t error = reader_i->take_next_sample(msg, info);
    if (error == DDS::RETCODE_OK)
    {
      if (info.valid_data)
      {
        std::cout << "| fop flightAuthorization: "
                  << ",authorization_id" << msg.authorization_id.in()
                  << ",flight_id:" << msg.flight_id.in()
                  << ",authorization_status:" << msg.authorization_status
                  << ",authorization_time:" << msg.authorization_time.in()
                  << ",approved_departure_time:" << msg.approved_departure_time.in()
                  << ",approved_arrival_time:" << msg.approved_arrival_time.in()
                  << ",tolpad_id:" << msg.tolpad_id.in()
                  << ",pilot_id:" << msg.pilot_id.in()
                  << ",evtol_id:" << msg.evtol_id.in() << std::endl;

        std::ofstream outfile("fleetOperatorDP/data/authorizations.txt", std::ios_base::app);
        if (outfile.is_open())
        {
          outfile << "authorization_id:" << msg.authorization_id.in()
                  << ",flight_id:" << msg.flight_id.in()
                  << ",authorization_status:" << msg.authorization_status
                  << ",authorization_time:" << msg.authorization_time.in()
                  << ",approved_departure_time:" << msg.approved_departure_time.in()
                  << ",approved_arrival_time:" << msg.approved_arrival_time.in()
                  << ",tolpad_id:" << msg.tolpad_id.in()
                  << ",pilot_id:" << msg.pilot_id.in()
                  << ",evtol_id:" << msg.evtol_id.in() << "\n";
          outfile.close();
        }
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
      break;
    }
  }
};
