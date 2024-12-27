#include "../../model/UATMTraits.h"  // Include UATM traits from a custom model
#include <tools/modeling/codegen/model/NullReaderListener.h>  // Include a NullReaderListener for code generation purposes
#include <model/Sync.h>  // Include synchronization utilities for thread safety
#include <ace/Log_Msg.h>  // Include ACE logging utilities for error messages and logging
#include <dds/DCPS/WaitSet.h>  // Include DDS WaitSet to handle events
#include "ReaderListenerFlows.h"  // Include the header for the ReaderListenerFlows class

// Constructor for the ReaderListenerFlows class, which initializes the ReaderCondSync object
ReaderListenerFlows::ReaderListenerFlows(OpenDDS::Model::ReaderCondSync &rcs)
    : rcs_(rcs) {}

// The callback function that handles data availability for the DataReader
void ReaderListenerFlows::on_data_available(DDS::DataReader_ptr reader)
{
    ACE_Guard<ACE_Thread_Mutex> g(mutex_);  // Lock the mutex to ensure thread-safety
    static bool signal_sent = false;  // Static flag to track if the signal has been sent

    std::cout << "entrou no subscriber do trafficFlowsInfo" << std::endl;

    // Narrow the DataReader to a specific type of DataReader (UATM::trafficFlowsInfoDataReader)
    UATM::trafficFlowsInfoDataReader_var reader_i =
        UATM::trafficFlowsInfoDataReader::_narrow(reader);

    // Check if the _narrow operation was successful
    if (CORBA::is_nil(reader_i.in()))
    {
        ACE_ERROR((LM_ERROR,
                   ACE_TEXT("ERROR: %N:%l: on_data_available_request() -")
                       ACE_TEXT(" _narrow failed!\n")));
        ACE_OS::exit(-1);  // Exit with an error code if narrowing fails
    }

    UATM::trafficFlowsInfo msg;  // Message to hold the data from the sample
    DDS::SampleInfo info;  // Information about the sample, such as validity and metadata

    // Process the data samples in a loop until no more valid data is available
    while (true)
    {
        DDS::ReturnCode_t error = reader_i->take_next_sample(msg, info);  // Take the next sample from the DataReader
        if (error == DDS::RETCODE_OK)  // If the sample was successfully retrieved
        {
            if (info.valid_data)  // Check if the data is valid
            {
                // Print the traffic flow information to the console
                std::cout << "| trafficFlowsInfo: "
                          << "flows_id:" << msg.flows_id
                          << ",area:" << msg.area.in()
                          << ",congestion_level:" << msg.congestion_level.in()
                          << ",affected_routes:" << msg.affected_routes.in()
                          << ",timestamp:" << msg.timestamp.in() << std::endl;

                // Open the file in append mode and write the traffic flow data
                std::ofstream outfile;
                outfile.open("uaspManagerDP/data/flows.txt", std::ios_base::app);
                outfile << "flows_id:" << msg.flows_id << ","
                        << "area:" << msg.area.in() << ","
                        << "congestion_level:" << msg.congestion_level.in() << ","
                        << "timestamp:" << msg.timestamp.in() << ","
                        << "affected_routes:" << msg.affected_routes.in() << std::endl;

                outfile.close();  // Close the file after writing
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
                           ACE_TEXT("ERROR: %N:%l: on_data_available_request() -")
                               ACE_TEXT(" take_next_sample failed!\n")));
            }
            break;  // Exit the loop if an error occurred
        }
    }
}
