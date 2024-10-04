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
      DDS::ReturnCode_t error = reader_i->take_next_sample(msg, info);
      if (error == DDS::RETCODE_OK) {
        if (info.valid_data) {
          std::cout << "| weatherInfo: " 
                    << "weather_id:" << msg.weather_id 
                    << ",location:" << msg.location.in() 
                    << ",temperature:" << msg.temperature 
                    << ",wind_speed:" << msg.wind_speed 
                    << ",weather_condition:" << msg.weather_condition.in() 
                    << ",observation_time:" << msg.observation_time.in() << std::endl;

          std::ofstream outfile;
                outfile.open("fleetOperatorDP/data/weather.txt", std::ios_base::app);
                
                outfile << "weather_id:" << msg.weather_id << ","
                        << "location:" << msg.location.in() << ","
                        << "temperature:" << msg.temperature << ","
                        << "wind_speed:" << msg.wind_speed << ","
                        << "weather_condition:" << msg.weather_condition.in() << ","
                        << "observation_time:" << msg.observation_time.in() << std::endl;
                
                outfile.close();
        } else {
            // std::cout << "\n\n| weatherInfo: signal\n\n";
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
