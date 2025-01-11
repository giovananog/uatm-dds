#include "../../model/UATMTraits.h"  
#include <tools/modeling/codegen/model/NullReaderListener.h>  
#include <model/Sync.h>  
#include <ace/Log_Msg.h>  
#include <dds/DCPS/WaitSet.h>  
#include "ReaderListenerRequest.h"  

// Constructor for the ReaderListenerRequest class
ReaderListenerRequest::ReaderListenerRequest(OpenDDS::Model::ReaderCondSync &rcs)
    : rcs_(rcs) {}

// This method is called when data is available for reading from the DataReader
void ReaderListenerRequest::on_data_available(DDS::DataReader_ptr reader)
{
  // Lock the mutex to ensure thread safety
  ACE_Guard<ACE_Thread_Mutex> g(mutex_);
  static bool signal_sent = false;

  // Narrow the generic DataReader to a specific type for acceptableRouteData
  UATM::acceptableRouteDataReader_var reader_i =
      UATM::acceptableRouteDataReader::_narrow(reader);

  // Check if the narrowing failed
  if (CORBA::is_nil(reader_i.in()))
  {
    ACE_ERROR((LM_ERROR,
               ACE_TEXT("ERROR: %N:%l: on_data_available() -")
                   ACE_TEXT(" _narrow failed!\n")));
    ACE_OS::exit(-1); // Exit if the narrowing failed
  }

  // Declare a message object to store the data and a SampleInfo object
  UATM::acceptableRoute msg;
  DDS::SampleInfo info;

  // Loop to read available samples
  while (true)
  {
    // Take the next sample from the DataReader
    DDS::ReturnCode_t error = reader_i->take_next_sample(msg, info);
    
    if (error == DDS::RETCODE_OK)
    {
      // If valid data is found, print the message details
      if (info.valid_data)
      {
        std::cout << "| uss acceptableRoute: "
                  << "acceptable_route_id:" << msg.acceptable_route_id
                  << ",approved_by:" << msg.approved_by.in()
                  << ",estimated_time:" << msg.estimated_time.in()
                  << ",flight_id:" << msg.flight_id.in()
                  << ",timestamp:" << msg.timestamp.in() << std::endl;
        break; // Exit the loop once valid data is processed
      }
      else
      {
        // If no valid data is available, but the signal hasn't been sent yet, send the signal
        if (!signal_sent)
        {
          rcs_.signal();  // Signal the condition to indicate data is available
          signal_sent = true; // Set signal_sent to true to prevent multiple signals
        }
        break; // Exit the loop if invalid data is found
      }
    }
    else
    {
      // Handle errors when taking the sample
      if (error != DDS::RETCODE_NO_DATA)
      {
        ACE_ERROR((LM_ERROR,
                   ACE_TEXT("ERROR: %N:%l: on_data_available() -")
                       ACE_TEXT(" take_next_sample failed!\n")));
      }
      break; // Exit the loop if an error occurred
    }
  }
};
