#include "../../model/UATMTraits.h"  // Custom traits related to UATM model
#include "../utils/functions.h"  // Utility functions for various tasks
#include <tools/modeling/codegen/model/NullReaderListener.h>  // A null listener, likely for code generation
#include <model/Sync.h>  // Synchronization utilities for multithreading
#include <ace/Log_Msg.h>  // ACE logging utilities
#include <dds/DCPS/WaitSet.h>  // DDS WaitSet for handling events
#include "ReaderListenerRequest.h"  // Header for ReaderListenerRequest class
#include <fstream>  // File handling for logging or reading data
#include <sstream>  // String stream for data handling
#include <string>  // String utilities
#include <vector>  // Vector container for managing lists of data

// Constructor for the ReaderListenerRequest class
ReaderListenerRequest::ReaderListenerRequest(OpenDDS::Model::ReaderCondSync &rcs)
    : rcs_(rcs) {}  // Initialize the synchronization object

// Function called when new data is available from the DataReader
void ReaderListenerRequest::on_data_available(DDS::DataReader_ptr reader)
{
    ACE_Guard<ACE_Thread_Mutex> g(mutex_);  // Locking the mutex for thread safety

    static bool signal_sent = false;  // Static flag to send a signal only once

    // Narrow the reader to the specific type (tolPadRequestDataReader)
    UATM::tolPadRequestDataReader_var reader_i =
        UATM::tolPadRequestDataReader::_narrow(reader);

    // Check if narrowing succeeded
    if (CORBA::is_nil(reader_i.in()))
    {
        ACE_ERROR((LM_ERROR,
                   ACE_TEXT("ERROR: %N:%l: on_data_available() -")
                       ACE_TEXT(" _narrow failed!\n")));  // Log error if narrowing fails
        ACE_OS::exit(-1);  // Exit if narrowing fails
    }

    UATM::tolPadRequest msg;  // Declare the message object to hold the incoming data
    DDS::SampleInfo info;  // Declare the sample info object

    // Loop to take samples from the reader
    while (true)
    {
        DDS::ReturnCode_t error = reader_i->take_next_sample(msg, info);  // Take the next sample from the reader
        if (error == DDS::RETCODE_OK)  // If the sample is taken successfully
        {
            if (info.valid_data)  // Check if the data is valid
            {
                // Output the message content to the console
                std::cout << "| tolPadRequest: "
                          << ",assign_id:" << msg.assign_id
                          << ",flight_id:" << msg.flight_id
                          << ",tol_pad_id:" << msg.tol_pad_id.in()
                          << ",skyport_id:" << msg.skyport_id.in()
                          << ",assign_time:" << msg.assign_time.in() << std::endl;

                // Update the toll pad status (pass 1 to mark as active)
                updateTolPadStatus(msg.tol_pad_id.in(), 1);
                break;  // Exit the loop after processing one valid sample
            }
            else  // If the data is not valid
            {
                if (!signal_sent)  // If the signal has not been sent yet
                {
                    rcs_.signal();  // Signal the synchronization object
                    signal_sent = true;  // Set the flag to true to prevent signaling again
                }
                break;  // Exit the loop when invalid data is encountered
            }
        }
        else  // If an error occurred while taking the sample
        {
            if (error != DDS::RETCODE_NO_DATA)  // If the error is not "no data"
            {
                ACE_ERROR((LM_ERROR,
                           ACE_TEXT("ERROR: %N:%l: on_data_available() -")
                               ACE_TEXT(" take_next_sample failed!\n")));  // Log the error
            }
            break;  // Exit the loop in case of error
        }
    }
}
