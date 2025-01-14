#include "../../model/UATMTraits.h"  // Include UATM traits from a custom model
#include <tools/modeling/codegen/model/NullReaderListener.h>  // Include a NullReaderListener for code generation purposes

#include <model/Sync.h>  // Include synchronization utilities for thread safety
#include <ace/Log_Msg.h>  // Include ACE logging utilities for error messages and logging

#include <dds/DCPS/WaitSet.h>  // Include DDS WaitSet to handle events
#include "ReaderListenerRequestUASP.h"  // Include the header for the ReaderListenerRequest class

// Constructor for the ReaderListenerRequest class, which initializes the ReaderCondSync object
ReaderListenerRequest::ReaderListenerRequest(OpenDDS::Model::ReaderCondSync &rcs)
    : rcs_(rcs) {}

// The callback function that handles data availability for the DataReader
void ReaderListenerRequest::on_data_available(DDS::DataReader_ptr reader)
{
    ACE_Guard<ACE_Thread_Mutex> g(mutex_);  // Lock the mutex to ensure thread-safety
    static bool signal_sent = false;  // Static flag to track if the signal has been sent

    // Narrow the DataReader to a specific type of DataReader (UATM::flightRequestInfoDataReader)
    UATM::flightRequestInfoDataReader_var reader_i =
        UATM::flightRequestInfoDataReader::_narrow(reader);

    // Check if the _narrow operation was successful
    if (CORBA::is_nil(reader_i.in()))
    {
        ACE_ERROR((LM_ERROR,
                   ACE_TEXT("ERROR: %N:%l: on_data_available() -")
                       ACE_TEXT(" _narrow failed!\n")));
        ACE_OS::exit(-1);  // Exit with an error code if narrowing fails
    }

    UATM::flightRequestInfo msg;  // Message to hold the data from the sample
    DDS::SampleInfo info;  // Information about the sample, such as validity and metadata

    // Process the data samples in a loop until no more valid data is available
    while (true)
    {
        DDS::ReturnCode_t error = reader_i->take_next_sample(msg, info);  // Take the next sample from the DataReader
        if (error == DDS::RETCODE_OK)  // If the sample was successfully retrieved
        {
            if (info.valid_data)  // Check if the data is valid
            {
                // Print the flight request information to the console
                std::cout << "| flightRequestInfo: "
                          << "request_id:" << msg.request_id.in()
                          << ",flight_id:" << msg.flight_id.in()
                          << ",departure_skyport_id:" << msg.departure_skyport_id.in()
                          << ",destination_skyport_id:" << msg.destination_skyport_id.in()
                          << ",departure_time:" << msg.departure_time.in()
                          << ",pilot_id:" << msg.pilot_id.in()
                          << ",evtol_id:" << msg.evtol_id.in() << std::endl;

                // Open the file in append mode and write the flight request data
                std::ofstream request_file("uaspManagerDP/data/requests.txt", std::ios_base::app);
                request_file << "request_id:" << msg.request_id.in() << ","
                             << "flight_id:" << msg.flight_id.in() << ","
                             << "departure_skyport_id:" << msg.departure_skyport_id.in() << ","
                             << "destination_skyport_id:" << msg.destination_skyport_id.in() << ","
                             << "departure_time:" << msg.departure_time.in() << ","
                             << "tolpad_id:" << "" << ","
                             << "pilot_id:" << msg.pilot_id.in() << ","
                             << "evtol_id:" << msg.evtol_id.in() << "\n";
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
