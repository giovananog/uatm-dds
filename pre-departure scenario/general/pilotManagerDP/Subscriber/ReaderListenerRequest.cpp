#include "../../model/UATMTraits.h" // Include custom UATM model traits
#include <tools/modeling/codegen/model/NullReaderListener.h> // Null reader listener (possibly for code generation)
#include <model/Sync.h> // Synchronization utilities for thread safety
#include <ace/Log_Msg.h> // ACE logging utilities
#include <dds/DCPS/WaitSet.h> // DDS WaitSet for event handling
#include "ReaderListenerRequest.h" // The header for this class
#include <fstream> // File operations (likely for logging or data storage)
#include <sstream> // String stream operations (likely for formatting data)
#include <string> // String utilities
#include <vector> // For working with collections of data
#include "../utils/functions.h" // Utility functions, likely for system operations

// Constructor for ReaderListenerRequest, initializing with a synchronization object
ReaderListenerRequest::ReaderListenerRequest(OpenDDS::Model::ReaderCondSync &rcs)
    : rcs_(rcs) {} // Initialize the synchronization object (ReaderCondSync)

// Method called when data becomes available in the DataReader
void ReaderListenerRequest::on_data_available(DDS::DataReader_ptr reader)
{
    // Lock the mutex to ensure thread safety when accessing shared resources
    ACE_Guard<ACE_Thread_Mutex> g(mutex_);
    static bool signal_sent = false; // Static variable to track if the signal was already sent

    // Narrow the DataReader to a specific type: flightAssignDataReader
    UATM::flightAssignDataReader_var reader_i =
        UATM::flightAssignDataReader::_narrow(reader);

    // Check if narrowing was successful
    if (CORBA::is_nil(reader_i.in()))
    {
        // If the narrowing failed, log an error and exit the program
        ACE_ERROR((LM_ERROR,
                   ACE_TEXT("ERROR: %N:%l: on_data_available() -")
                       ACE_TEXT(" _narrow failed!\n")));
        ACE_OS::exit(-1); // Exit the application with an error code
    }

    // Create a message object to hold the data and a SampleInfo object to hold metadata
    UATM::flightAssign msg;
    DDS::SampleInfo info;

    // Loop to read data as long as it's available
    while (true)
    {
        // Attempt to take the next sample from the DataReader
        DDS::ReturnCode_t error = reader_i->take_next_sample(msg, info);
        if (error == DDS::RETCODE_OK) // If sample retrieval is successful
        {
            if (info.valid_data) // If the sample contains valid data
            {
                // Print the details of the flight assignment to the console
                std::cout << "| flightAssign: "
                          << "flight_assign_id:" << msg.flight_assign_id
                          << ",assign_time:" << msg.assign_time.in()
                          << ",flight_id:" << msg.flight_id.in()
                          << ",pilot_id:" << msg.pilot_id.in()
                          << ",evtol_id:" << msg.evtol_id.in() 
                          << ",origin_skyport_id:" << msg.origin_skyport_id.in() 
                          << ",destination_skyport_id:" << msg.destination_skyport_id.in() << std::endl;

                // Update the pilot's status based on the assignment details
                updatePilotStatus(msg.pilot_id.in(), 0, msg.destination_skyport_id.in());
            }
            else // If no valid data is present, signal and break the loop
            {
                if (!signal_sent)
                {
                    rcs_.signal(); // Signal the condition variable to notify waiting threads
                    signal_sent = true; // Mark that the signal has been sent
                }
                break; // Exit the loop if invalid data is encountered
            }
        }
        else // If there is an error retrieving the sample
        {
            if (error != DDS::RETCODE_NO_DATA) // If the error is not due to no data being available
            {
                // Log an error if taking the next sample failed
                ACE_ERROR((LM_ERROR,
                           ACE_TEXT("ERROR: %N:%l: on_data_available() -")
                               ACE_TEXT(" take_next_sample failed!\n")));
            }
            break; // Exit the loop if there's an error
        }
    }
}
