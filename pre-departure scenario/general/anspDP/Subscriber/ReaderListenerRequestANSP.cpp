#include "../../model/UATMTraits.h"  
#include <tools/modeling/codegen/model/NullReaderListener.h>  
#include <model/Sync.h>  
#include <ace/Log_Msg.h>  
#include <dds/DCPS/WaitSet.h>  
#include "ReaderListenerRequestANSP.h"  

// Constructor for ReaderListenerRequest, initializing with ReaderCondSync object
ReaderListenerRequest::ReaderListenerRequest(OpenDDS::Model::ReaderCondSync &rcs)
    : rcs_(rcs) {}

// Method that is called when new data is available to be read
void ReaderListenerRequest::on_data_available(DDS::DataReader_ptr reader)
{

  // Lock the mutex to ensure thread safety during data processing
  ACE_Guard<ACE_Thread_Mutex> g(mutex_);
  static bool signal_sent = false;

  // Narrow the reader to the specific type of data reader (acceptableRouteDataReader)
  UATM::acceptableRouteDataReader_var reader_i =
      UATM::acceptableRouteDataReader::_narrow(reader);

  // Check if the narrowing operation failed
  if (CORBA::is_nil(reader_i.in()))
  {
    ACE_ERROR((LM_ERROR,
               ACE_TEXT("ERROR: %N:%l: on_data_available() -")
                   ACE_TEXT(" _narrow failed!\n")));
    ACE_OS::exit(-1);  
  }

  UATM::acceptableRoute msg;  // Variable to store the incoming message
  DDS::SampleInfo info;  // Information about the sample (metadata)

  // Loop to keep reading data samples as long as they are available
  while (true)
  {
    // Attempt to take the next sample from the reader
    DDS::ReturnCode_t error = reader_i->take_next_sample(msg, info);

    // Check if the sample was successfully retrieved
    if (error == DDS::RETCODE_OK)
    {
      if (info.valid_data)
      {
        std::cout << "| ansp acceptableRoute: "
                  << "acceptable_route_id:" << msg.acceptable_route_id
                  << ",approved_by:" << msg.approved_by.in()
                  << ",estimated_time:" << msg.estimated_time.in()
                  << ",flight_id:" << msg.flight_id.in()
                  << ",timestamp:" << msg.timestamp.in() << std::endl;
        break;
      }
      else
      {
        // If data is invalid and the signal hasn't been sent yet, send the signal
        if (!signal_sent)
        {
          rcs_.signal();  // Signal the condition to notify the waiting thread
          signal_sent = true;  // Set the flag to true to prevent resending the signal
        }
        break;  
      }
    }
    else
    {
      // If there was an error reading the sample, log the error
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
