#include "../../model/UATMTraits.h" // Custom UATM model traits header
#include <tools/modeling/codegen/model/NullReaderListener.h> // Placeholder for a Null Reader Listener

#include <model/Sync.h> // Synchronization primitives
#include <ace/Log_Msg.h> // ACE logging utilities

#include <dds/DCPS/WaitSet.h> // For managing wait sets in DDS
#include "ReaderListenerAuth.h" // Listener for flight authorization data

// Constructor for the ReaderListenerAuth class, initializes the ReaderCondSync object
ReaderListenerAuth::ReaderListenerAuth(OpenDDS::Model::ReaderCondSync &rcs)
    : rcs_(rcs) {}

// This method is called when new data is available from the reader
void ReaderListenerAuth::on_data_available(DDS::DataReader_ptr reader)
{
  // Lock to ensure thread safety
  ACE_Guard<ACE_Thread_Mutex> g(mutex_);
  static bool signal_sent = false; // Ensure signal is sent only once

  // Narrow the generic DataReader pointer to a specific type (flightAuthorizationDataReader)
  UATM::flightAuthorizationDataReader_var reader_i =
      UATM::flightAuthorizationDataReader::_narrow(reader);

  // Check if the narrowing was successful
  if (CORBA::is_nil(reader_i.in()))
  {
    // Log error and exit if the narrowing failed
    ACE_ERROR((LM_ERROR,
               ACE_TEXT("ERROR: %N:%l: on_data_available_request() -")
                   ACE_TEXT(" _narrow failed!\n")));
    ACE_OS::exit(-1);
  }

  // Create a message object to hold the sample data
  UATM::flightAuthorization msg;
  DDS::SampleInfo info; // Holds information about the sample

  while (true)
  {
    // Take the next sample from the reader
    DDS::ReturnCode_t error = reader_i->take_next_sample(msg, info);
    
    // If data was successfully retrieved
    if (error == DDS::RETCODE_OK)
    {
      if (info.valid_data) // If the data is valid
      {
        // Print the authorization information to the console
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

        // Write the authorization information to a file
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
          outfile.close(); // Close the file after writing
        }
      }
      else // If no valid data is found
      {
        if (!signal_sent) // If the signal hasn't been sent yet
        {
          rcs_.signal(); // Signal the synchronization object
          signal_sent = true; // Ensure the signal is only sent once
        }
        break; // Exit the loop if no valid data is found
      }
    }
    else // If there was an error retrieving the sample
    {
      if (error != DDS::RETCODE_NO_DATA)
      {
        // Log the error if it wasn't a "no data" error
        ACE_ERROR((LM_ERROR,
                   ACE_TEXT("ERROR: %N:%l: on_data_available_request() -")
                       ACE_TEXT(" take_next_sample failed!\n")));
      }
      break; // Exit the loop if an error occurred
    }
  }
};
