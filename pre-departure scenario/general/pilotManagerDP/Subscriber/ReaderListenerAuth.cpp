#include "../../model/UATMTraits.h" // Custom UATM model traits header
#include <tools/modeling/codegen/model/NullReaderListener.h> // Null listener for code generation

#include <model/Sync.h> // Synchronization utilities
#include <ace/Log_Msg.h> // ACE logging utilities

#include <dds/DCPS/WaitSet.h> // DDS WaitSet for event management
#include "ReaderListenerAuth.h" // Listener class for flight authorization

// Constructor for the ReaderListenerAuth class, which initializes with a synchronization object
ReaderListenerAuth::ReaderListenerAuth(OpenDDS::Model::ReaderCondSync &rcs)
    : rcs_(rcs) {} // Initialize the ReaderCondSync object

// Method called when data is available in the DataReader
void ReaderListenerAuth::on_data_available(DDS::DataReader_ptr reader)
{
  // Lock the mutex to ensure thread-safety while accessing shared resources
  ACE_Guard<ACE_Thread_Mutex> g(mutex_);

  // Narrow the generic DataReader to the specific type: flightAuthorizationDataReader
  UATM::flightAuthorizationDataReader_var reader_i =
      UATM::flightAuthorizationDataReader::_narrow(reader);

  // Check if the narrowing was successful
  if (CORBA::is_nil(reader_i.in()))
  {
    // Log an error if narrowing failed
    ACE_ERROR((LM_ERROR,
               ACE_TEXT("ERROR: %N:%l: on_data_available_request() -")
                   ACE_TEXT(" _narrow failed!\n")));
    ACE_OS::exit(-1); // Exit the application with an error code
  }

  // Create a message object to store the data and a SampleInfo object for metadata
  UATM::flightAuthorization msg;
  DDS::SampleInfo info;

  // Start reading data in a loop
  while (true)
  {
    // Attempt to take the next sample from the DataReader
    DDS::ReturnCode_t error = reader_i->take_next_sample(msg, info);
    if (error == DDS::RETCODE_OK) // If the sample was successfully retrieved
    {
      if (info.valid_data) // Check if the sample contains valid data
      {
        // Output the flight authorization data to the console
        std::cout << "| plm flightAuthorization: "
                  << ",authorization_id" << msg.authorization_id.in()
                  << ",flight_id:" << msg.flight_id.in()
                  << ",authorization_status:" << msg.authorization_status
                  << ",authorization_time:" << msg.authorization_time.in()
                  << ",approved_departure_time:" << msg.approved_departure_time.in()
                  << ",approved_arrival_time:" << msg.approved_arrival_time.in()
                  << ",tolpad_id:" << msg.tolpad_id.in()
                  << ",pilot_id:" << msg.pilot_id.in()
                  << ",evtol_id:" << msg.evtol_id.in() << std::endl;
      }
      else // If the data is not valid, break out of the loop
      {
        rcs_.signal(); // Signal the condition variable to notify waiting threads
        break;
      }
    }
    else // If there was an error while retrieving the sample
    {
      if (error != DDS::RETCODE_NO_DATA) // If no data was returned, handle other errors
      {
        // Log an error if the take_next_sample failed
        ACE_ERROR((LM_ERROR,
                   ACE_TEXT("ERROR: %N:%l: on_data_available_request() -")
                       ACE_TEXT(" take_next_sample failed!\n")));
      }
      break; // Exit the loop
    }
  }
};
