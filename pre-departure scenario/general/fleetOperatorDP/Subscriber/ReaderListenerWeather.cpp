#include "../../model/UATMTraits.h"

#include "ReaderListenerWeather.h"
#include <model/Sync.h>
#include <ace/Log_Msg.h>
#include <dds/DCPS/WaitSet.h>

ReaderListenerWeather::ReaderListenerWeather(OpenDDS::Model::ReaderCondSync& rcs)
  : rcs_(rcs) {}

void
ReaderListenerWeather::on_data_available(DDS::DataReader_ptr reader) 
  {
    ACE_Guard<ACE_Thread_Mutex> g(mutex_);

    UATM::weatherInfoDataReader_var reader_i =
      UATM::weatherInfoDataReader::_narrow(reader);

    if (CORBA::is_nil(reader_i.in())) {
    ACE_ERROR((LM_ERROR,
               ACE_TEXT("ERROR: %N:%l: on_data_available_request() -")
               ACE_TEXT(" _narrow failed!\n")));
    ACE_OS::exit(-1);
  }

    UATM::weatherInfo msg;
    DDS::SampleInfo info;

    while (true) {
      std::cout << "\n\n" << std::endl;                                              
      DDS::ReturnCode_t error = reader_i->take_next_sample(msg, info);
      if (error == DDS::RETCODE_OK) {
        // std::cout << "SampleInfo.sample_rank = " << info.sample_rank << std::endl;
        if (info.valid_data) {
          std::cout << "--------------------fleetOperatorDP--------------" << std::endl
                    << "        weatherInfo:" << std::endl
                    << "        -----------------" << std::endl
                    << "Weather Info ID: " << msg.weather_id << std::endl
                    << "Location: " << msg.location.in() << std::endl
                    << "Temperature: " << msg.temperature << std::endl
                    << "Wind Speed: " << msg.wind_speed << std::endl
                    << "Weather Condition: " << msg.weather_condition.in() << std::endl
                    << "Observation Time: " << msg.observation_time.in() << std::endl;
        } else {
            rcs_.signal();
        }
      } else {
        if (error != DDS::RETCODE_NO_DATA) {
        ACE_ERROR((LM_ERROR,
                   ACE_TEXT("ERROR: %N:%l: on_data_available_request() -")
                   ACE_TEXT(" take_next_sample failed!\n")));
        }
        rcs_.signal();
        break;
      }
    }
  };
