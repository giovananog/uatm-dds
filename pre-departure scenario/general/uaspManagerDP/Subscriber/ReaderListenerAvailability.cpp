#include "../../model/UATMTraits.h"  // Include UATM traits from a custom model
#include "../utils/functions.h"  // Include utility functions, possibly for shared operations
#include <tools/modeling/codegen/model/NullReaderListener.h>  // Include a NullReaderListener for code generation purposes
#include <model/Sync.h>  // Include synchronization utilities for thread safety
#include <ace/Log_Msg.h>  // Include ACE logging utilities for error messages and logging
#include <dds/DCPS/WaitSet.h>  // Include DDS WaitSet to handle events
#include "ReaderListenerAvailability.h"  // Include the header for the ReaderListenerAvailability class
#include <fstream>  // Include the file stream library to write data to files

// Constructor for the ReaderListenerAvailability class, which initializes the ReaderCondSync object
ReaderListenerAvailability::ReaderListenerAvailability(OpenDDS::Model::ReaderCondSync &rcs)
    : rcs_(rcs) {}

// The callback function that handles data availability for the DataReader
void ReaderListenerAvailability::on_data_available(DDS::DataReader_ptr reader)
{
    ACE_Guard<ACE_Thread_Mutex> g(mutex_);  // Lock the mutex to ensure thread-safety
    static bool signal_sent = false;  // Static flag to track if the signal has been sent

    // Narrow the DataReader to a specific type of DataReader (UATM::availabilityInfoDataReader)
    UATM::availabilityInfoDataReader_var reader_i =
        UATM::availabilityInfoDataReader::_narrow(reader);

    // Check if the _narrow operation was successful
    if (CORBA::is_nil(reader_i.in()))
    {
        ACE_ERROR((LM_ERROR,
                   ACE_TEXT("ERROR: %N:%l: on_data_available() -")
                       ACE_TEXT(" _narrow failed!\n")));
        ACE_OS::exit(-1);  // Exit with an error code if narrowing fails
    }

    UATM::availabilityInfo msg;  // Message to hold the data from the sample
    DDS::SampleInfo info;  // Information about the sample, such as validity and metadata

    // Process the data samples in a loop until no more valid data is available
    while (true)
    {
        DDS::ReturnCode_t error = reader_i->take_next_sample(msg, info);  // Take the next sample from the DataReader
        if (error == DDS::RETCODE_OK)  // If the sample was successfully retrieved
        {
            if (info.valid_data)  // Check if the data is valid
            {
                // Print the availability information to the console
                std::cout << "| AvailabilityInfo: "
                          << "resource_id:" << msg.resource_id.in()
                          << ",resource_type:" << msg.resource_type.in()
                          << ",available:" << msg.available
                          << ",skyport_id:" << msg.skyport_id.in()
                          << ",availability_time:" << msg.availability_time.in() << std::endl;

                // Open the file in append mode and write the availability data
                std::ofstream request_file("uaspManagerDP/data/tolpads.txt", std::ios_base::app);
                request_file << "resource_id=" << msg.resource_id.in() << ","
                             << "skyport_id=" << msg.skyport_id.in() << ","
                             << "available=" << msg.available << ","
                             << "availability_time=" << msg.availability_time.in() << "\n";
                request_file.close();  // Close the file after writing
            }
            else
            {
                // If no valid data is found, signal and exit the loop
                if (!signal_sent)
                {
                    rcs_.signal();  // Signal the condition variable to notify waiting threads
                    signal_sent = true;  // Set the flag to prevent multiple signals
                }
                break;  // Exit the loop if no valid data is found
            }
        }
        else
        {
            // Handle errors in taking the sample
            if (error != DDS::RETCODE_NO_DATA)  // If no data was retrieved but no error occurred, continue
            {
                ACE_ERROR((LM_ERROR,
                           ACE_TEXT("ERROR: %N:%l: on_data_available() -")
                               ACE_TEXT(" take_next_sample failed!\n")));
            }
            break;  // Exit the loop if an error occurred
        }
    }
}
