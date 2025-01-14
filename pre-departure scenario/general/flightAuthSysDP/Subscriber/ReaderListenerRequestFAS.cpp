#include "../../model/UATMTraits.h" // Custom UATM model traits header
#include <tools/modeling/codegen/model/NullReaderListener.h> // Null listener for code generation
#include <model/Sync.h> // Synchronization utilities
#include <ace/Log_Msg.h> // ACE logging utilities
#include <dds/DCPS/WaitSet.h> // DDS WaitSet for event management

#include "ReaderListenerRequestFAS.h" // Reader listener for flight authorization request data

// Constructor for the ReaderListenerRequest class, initializes with the synchronization object
ReaderListenerRequest::ReaderListenerRequest(OpenDDS::Model::ReaderCondSync &rcs)
    : rcs_(rcs) {}

// This method is called when data is available from the reader
void ReaderListenerRequest::on_data_available(DDS::DataReader_ptr reader)
{
  // Synchronize access to shared resources using a thread mutex
  ACE_Guard<ACE_Thread_Mutex> g(mutex_);

  // Narrow the reader to a specific type (flightAuthorizationRequestDataReader)
  UATM::flightAuthorizationRequestDataReader_var reader_i =
      UATM::flightAuthorizationRequestDataReader::_narrow(reader);

  // Check if the narrow operation succeeded
  if (CORBA::is_nil(reader_i.in()))
  {
    ACE_ERROR((LM_ERROR,
               ACE_TEXT("ERROR: %N:%l: on_data_available() -")
                   ACE_TEXT(" _narrow failed!\n")));
    ACE_OS::exit(-1); // Exit the program if the narrow operation fails
  }

  // Declare the message and sample information
  UATM::flightAuthorizationRequest msg;
  DDS::SampleInfo info;

  // Loop to process available data samples
  while (true)
  {
    // Retrieve the next sample from the reader
    DDS::ReturnCode_t error = reader_i->take_next_sample(msg, info);

    // Check if the sample retrieval was successful
    if (error == DDS::RETCODE_OK)
    {
      // If valid data is received, process and print it
      if (info.valid_data)
      {
        // Print the details of the flight authorization request to the console
        std::cout << "| flightAuthorizationRequest: "
                  << "auth_request_id:" << msg.auth_request_id.in()
                  << ",flight_id:" << msg.flight_id.in()
                  << ",departure_skyport_id:" << msg.departure_skyport_id.in()
                  << ",destination_skyport_id:" << msg.destination_skyport_id.in()
                  << ",departure_time:" << msg.departure_time.in()
                  << ",pilot_id:" << msg.pilot_id.in()
                  << ",evtol_id:" << msg.evtol_id.in() << std::endl;

        // Open the file in append mode to log the data
        std::ofstream request_file("flightAuthSysDP/data/requests.txt", std::ios::app);
        if (request_file.is_open())
        {
          // Write the data to the file
          request_file << "auth_request_id:" << msg.auth_request_id.in() << ","
                       << "flight_id:" << msg.flight_id.in() << ","
                       << "departure_skyport_id:" << msg.departure_skyport_id.in() << ","
                       << "destination_skyport_id:" << msg.destination_skyport_id.in() << ","
                       << "departure_time:" << msg.departure_time.in() << ","
                       << "pilot_id:" << msg.pilot_id.in() << ","
                       << "evtol_id:" << msg.evtol_id.in() << "\n";
        }
        request_file.close(); // Close the file after writing
      }
      else
      {
        // Signal that data has been processed and exit the loop
        rcs_.signal();
        break;
      }
    }
    else
    {
      // Handle errors in sample retrieval (excluding no data error)
      if (error != DDS::RETCODE_NO_DATA)
      {
        ACE_ERROR((LM_ERROR,
                   ACE_TEXT("ERROR: %N:%l: on_data_available() -")
                       ACE_TEXT(" take_next_sample failed!\n")));
      }
      break; // Exit the loop if an error occurs
    }
  }
};
