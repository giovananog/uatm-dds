#include "../../model/UATMTraits.h"
#include <tools/modeling/codegen/model/NullReaderListener.h>

#include <model/Sync.h>
#include <ace/Log_Msg.h>

#include <dds/DCPS/WaitSet.h>
#include "ReaderListenerRest.h"

// Constructor that takes a ReaderCondSync object by reference
ReaderListenerRest::ReaderListenerRest(OpenDDS::Model::ReaderCondSync &rcs)
    : rcs_(rcs) {}

// This method is triggered when data is available for the reader
void ReaderListenerRest::on_data_available(DDS::DataReader_ptr reader)
{
  // Mutex lock to ensure thread-safety during the data processing
  ACE_Guard<ACE_Thread_Mutex> g(mutex_);
  static bool signal_sent = false;

  // Narrow the reader to a specific type (airspaceRestrictionsDataReader)
  UATM::airspaceRestrictionsDataReader_var reader_i =
      UATM::airspaceRestrictionsDataReader::_narrow(reader);

  // Check if the narrow operation was successful
  if (CORBA::is_nil(reader_i.in()))
  {
    ACE_ERROR((LM_ERROR,
               ACE_TEXT("ERROR: %N:%l: on_data_available() -")
                   ACE_TEXT(" _narrow failed!\n")));
    ACE_OS::exit(-1); // Exit if narrowing fails
  }

  // Declare message and sample info objects
  UATM::airspaceRestrictions msg;
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
        // Print the received message details to the console
        std::cout << "| airspaceRestrictions: "
                  << "restriction_id:" << msg.restriction_id
                  << ",restriction_area:" << msg.restriction_area.in()
                  << ",restriction_type:" << msg.restriction_type.in()
                  << ",restriction_time:" << msg.restriction_time.in()
                  << ",restriction_authority:" << msg.restriction_authority.in() << std::endl;

        // Open the file for appending the restriction information
        std::ofstream outfile;
        outfile.open("uaspManagerDP/data/restrictions.txt", std::ios_base::app);

        // Append the restriction details to the file
        outfile << "restriction_id:" << msg.restriction_id << ","
                << "restriction_area:" << msg.restriction_area.in() << ","
                << "restriction_type:" << msg.restriction_type << ","
                << "restriction_time:" << msg.restriction_time << ","
                << "restriction_authority:" << msg.restriction_authority.in() << std::endl;

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
