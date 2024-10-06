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

  UATM::bookingFlightRequestDataReader_var reader_i =
      UATM::bookingFlightRequestDataReader::_narrow(reader);

  if (CORBA::is_nil(reader_i.in()))
  {
    ACE_ERROR((LM_ERROR,
               ACE_TEXT("ERROR: %N:%l: on_data_available() -")
                   ACE_TEXT(" _narrow failed!\n")));
    ACE_OS::exit(-1);
  }

  UATM::bookingFlightRequest msg;
  DDS::SampleInfo info;

  while (true)
  {
    DDS::ReturnCode_t error = reader_i->take_next_sample(msg, info);
    if (error == DDS::RETCODE_OK)
    {
      if (info.valid_data)
      {
        std::cout << "| bookingFlightRequest: "
                  << ",booking_id:" << msg.booking_id.in()
                  << ",costumer_id:" << msg.costumer_id.in()
                  << ",flight_id:" << msg.flight_id.in()
                  << ",skyport_id:" << msg.skyport_id.in() << std::endl;

        std::ofstream outfile("fleetOperatorDP/data/requests.txt", std::ios_base::app);
        if (outfile.is_open())
        {
          outfile << "booking_id:" << msg.booking_id.in() << ","
                  << "costumer_id:" << msg.costumer_id.in() << ","
                  << "flight_id:" << msg.flight_id.in() << ","
                  << "skyport_id:" << msg.skyport_id.in() << ","
                  << "pilot_id:" << "" << ","
                  << "evtol_id:" << "" << ","
                  << "weather_id:" << "" << ","
                  << "route_id:" << "" << ","
                  << "tolpad_id:" << "" << ","
                  << "status:" << 1 << "\n";
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
                   ACE_TEXT("ERROR: %N:%l: on_data_available() -")
                       ACE_TEXT(" take_next_sample failed!\n")));
      }
      break;
    }
  }
};
