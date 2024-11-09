#include "../../model/UATMTraits.h"  // UATM traits for Urban Air Traffic Management model
#include <tools/modeling/codegen/model/NullReaderListener.h>  // Header for a null reader listener

#include <model/Sync.h>  // Synchronization model for DDS
#include <ace/Log_Msg.h>  // ACE logging functionality

#include <dds/DCPS/WaitSet.h>  // DDS WaitSet for synchronization of read events
#include "ReaderListenerRequest.h"  // Header for the custom reader listener

// Constructor for ReaderListenerRequest, initializing with ReaderCondSync object
ReaderListenerRequest::ReaderListenerRequest(OpenDDS::Model::ReaderCondSync &rcs)
    : rcs_(rcs) {}

// Method that is called when new data is available to be read
void ReaderListenerRequest::on_data_available(DDS::DataReader_ptr reader)
{
  // Lock the mutex to ensure thread safety during data processing
  ACE_Guard<ACE_Thread_Mutex> g(mutex_);
  static bool signal_sent = false;  // Flag to track whether the signal has been sent

  // Narrow the reader to the specific type of data reader (acceptableRouteDataReader)
  UATM::acceptableRouteDataReader_var reader_i =
      UATM::acceptableRouteDataReader::_narrow(reader);

  // Check if the narrowing operation failed
  if (CORBA::is_nil(reader_i.in()))
  {
    // Log error if narrowing failed
    ACE_ERROR((LM_ERROR,
               ACE_TEXT("ERROR: %N:%l: on_data_available() -")
                   ACE_TEXT(" _narrow failed!\n")));
    ACE_OS::exit(-1);  // Exit the program with an error code
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
      // If the data is valid, print the message contents
      if (info.valid_data)
      {
        std::cout << "| ansp acceptableRoute: "
                  << "acceptable_route_id:" << msg.acceptable_route_id
                  << ",approved_by:" << msg.approved_by.in()
                  << ",estimated_time:" << msg.estimated_time.in()
                  << ",flight_id:" << msg.flight_id.in()
                  << ",timestamp:" << msg.timestamp.in() << std::endl;
        break;  // Exit the loop after processing a valid sample
      }
      else
      {
        // If data is invalid and the signal hasn't been sent yet, send the signal
        if (!signal_sent)
        {
          rcs_.signal();  // Signal the condition to notify the waiting thread
          signal_sent = true;  // Set the flag to true to prevent resending the signal
        }
        break;  // Exit the loop since no valid data was found
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
      break;  // Exit the loop when an error occurs
    }
  }
}
