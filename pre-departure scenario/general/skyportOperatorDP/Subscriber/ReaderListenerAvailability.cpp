#include "../../model/UATMTraits.h" // Custom UATM model traits
#include <tools/modeling/codegen/model/NullReaderListener.h> // Null reader listener (possibly for code generation)
#include <model/Sync.h> // Synchronization utilities for thread safety
#include <ace/Log_Msg.h> // ACE logging utilities
#include <dds/DCPS/WaitSet.h> // DDS WaitSet for event handling
#include "ReaderListenerAvailability.h" // Header for this class

// Constructor for ReaderListenerAvailability, initializing with a synchronization object
ReaderListenerAvailability::ReaderListenerAvailability(OpenDDS::Model::ReaderCondSync &rcs)
    : rcs_(rcs) {} // Initialize the synchronization object (ReaderCondSync)

// Method called when data becomes available in the DataReader
void ReaderListenerAvailability::on_data_available(DDS::DataReader_ptr reader)
{
    // Lock the mutex to ensure thread safety when accessing shared resources
    ACE_Guard<ACE_Thread_Mutex> g(mutex_);

    // Narrow the DataReader to a specific type: availabilityInfoDataReader
    UATM::availabilityInfoDataReader_var reader_i =
        UATM::availabilityInfoDataReader::_narrow(reader);

    // Check if narrowing was successful
    if (CORBA::is_nil(reader_i.in()))
    {
        // If narrowing failed, log an error and exit
        ACE_ERROR((LM_ERROR,
                   ACE_TEXT("ERROR: %N:%l: on_data_available() -")
                       ACE_TEXT(" _narrow failed!\n")));
        ACE_OS::exit(-1); // Exit the application with an error code
    }

    // Create a message object to hold the data and a SampleInfo object to hold metadata
    UATM::availabilityInfo msg;
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
                // Print the details of the availability information to the console
                std::cout << "| AvailabilityInfo: "
                          << "resource_id:" << msg.resource_id.in()
                          << ",resource_type:" << msg.resource_type.in()
                          << ",available:" << msg.available
                          << ",skyport_id:" << msg.skyport_id.in()
                          << ",availability_time:" << msg.availability_time.in() << std::endl;
            }
            else // If no valid data is present, signal and break the loop
            {
                rcs_.signal(); // Signal the condition variable to notify waiting threads
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
