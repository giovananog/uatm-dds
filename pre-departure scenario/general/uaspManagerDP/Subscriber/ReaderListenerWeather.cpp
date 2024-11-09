#include "../../model/UATMTraits.h"
#include <tools/modeling/codegen/model/NullReaderListener.h>

#include <model/Sync.h>
#include <ace/Log_Msg.h>

#include <dds/DCPS/WaitSet.h>
#include "ReaderListenerWeather.h"

// Constructor that takes a ReaderCondSync object by reference
ReaderListenerWeather::ReaderListenerWeather(OpenDDS::Model::ReaderCondSync &rcs)
    : rcs_(rcs) {}

// This method is triggered when data is available for the reader
void ReaderListenerWeather::on_data_available(DDS::DataReader_ptr reader)
{
  // Mutex lock to ensure thread-safety during the data processing
  ACE_Guard<ACE_Thread_Mutex> g(mutex_);
  static bool signal_sent = false;

  // Narrow the reader to a specific type (weatherInfoDataReader)
  UATM::weatherInfoDataReader_var reader_i =
      UATM::weatherInfoDataReader::_narrow(reader);

  // Check if the narrow operation was successful
  if (CORBA::is_nil(reader_i.in()))
  {
    ACE_ERROR((LM_ERROR,
               ACE_TEXT("ERROR: %N:%l: on_data_available_request() -")
                   ACE_TEXT(" _narrow failed!\n")));
    ACE_OS::exit(-1); // Exit if narrowing fails
  }

  // Declare message and sample info objects
  UATM::weatherInfo msg;
  DDS::SampleInfo info;

  // Continuously attempt to take data samples until no more data is available
  while (true)
  {
    DDS::ReturnCode_t error = reader_i->take_next_sample(msg, info);

    // If data is successfully retrieved, process it
    if (error == DDS::RETCODE_OK)
    {
      // Check if the data is valid
      if (info.valid_data)
      {
        // Print the received weather information to the console
        std::cout << "| weatherInfo: "
                  << "weather_id:" << msg.weather_id
                  << ",location:" << msg.location.in()
                  << ",temperature:" << msg.temperature
                  << ",wind_speed:" << msg.wind_speed
                  << ",weather_condition:" << msg.weather_condition.in()
                  << ",observation_time:" << msg.observation_time.in() << std::endl;

        // Open the file for appending the weather information
        std::ofstream outfile;
        outfile.open("uaspManagerDP/data/weather.txt", std::ios_base::app);

        // Append the weather details to the file
        outfile << "weather_id:" << msg.weather_id << ","
                << "location:" << msg.location.in() << ","
                << "temperature:" << msg.temperature << ","
                << "wind_speed:" << msg.wind_speed << ","
                << "weather_condition:" << msg.weather_condition.in() << ","
                << "observation_time:" << msg.observation_time.in() << std::endl;

        outfile.close(); // Close the file after writing
      }
      else
      {
        // If no valid data, signal the condition and break the loop
        if (!signal_sent)
        {
          rcs_.signal();
          signal_sent = true;
        }
        break; // Exit the loop when data is invalid or no more valid data is available
      }
    }
    else
    {
      // If an error occurs during data retrieval, log the error message
      if (error != DDS::RETCODE_NO_DATA)
      {
        ACE_ERROR((LM_ERROR,
                   ACE_TEXT("ERROR: %N:%l: on_data_available_request() -")
                       ACE_TEXT(" take_next_sample failed!\n")));
      }
      break; // Exit the loop if data retrieval failed
    }
  }
};
