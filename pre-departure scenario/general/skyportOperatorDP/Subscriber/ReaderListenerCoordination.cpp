#include "../../model/UATMTraits.h" // Custom UATM model traits
#include <tools/modeling/codegen/model/NullReaderListener.h> // Null reader listener (possibly for code generation)
#include <model/Sync.h> // Synchronization utilities for thread safety
#include <ace/Log_Msg.h> // ACE logging utilities
#include <dds/DCPS/WaitSet.h> // DDS WaitSet for event handling
#include "ReaderListenerCoordination.h" // Header for this class

// Constructor for ReaderListenerCoordination, initializing with a synchronization object
ReaderListenerCoordination::ReaderListenerCoordination(OpenDDS::Model::ReaderCondSync &rcs)
    : rcs_(rcs) {} // Initialize the synchronization object (ReaderCondSync)

// Method called when data becomes available in the DataReader
void ReaderListenerCoordination::on_data_available(DDS::DataReader_ptr reader)
{
    // Lock the mutex to ensure thread safety when accessing shared resources
    ACE_Guard<ACE_Thread_Mutex> g(mutex_);
    static bool signal_sent = false; // Flag to ensure the signal is sent only once

    // Narrow the DataReader to a specific type: flightCoordinationDataReader
    UATM::flightCoordinationDataReader_var reader_i =
        UATM::flightCoordinationDataReader::_narrow(reader);

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
    UATM::flightCoordination msg;
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
                // Print the details of the flight coordination to the console
                std::cout << "| flightCoordination: "
                          << "coordination_id:" << msg.coordination_id.in()
                          << ",flight_id:" << msg.flight_id.in()
                          << ",origin_skyport_id:" << msg.origin_skyport_id.in()
                          << ",destination_skyport_id:" << msg.destination_skyport_id.in()
                          << ",evtol_id:" << msg.evtol_id.in()
                          << ",pilot_id:" << msg.pilot_id.in()
                          << ",route_id:" << msg.route_id.in()
                          << ",weather_id:" << msg.weather_id.in() << std::endl;

                // Open a file for appending the coordination details
                std::ofstream outfile;
                outfile.open("skyportOperatorDP/data/coordinations.txt", std::ios_base::app);

                // Write the coordination information to the file
                outfile << "coordination_id:" << msg.coordination_id.in() << ","
                        << "flight_id:" << msg.flight_id.in() << ","
                        << "origin_skyport_id:" << msg.origin_skyport_id.in() << ","
                        << "destination_skyport_id:" << msg.destination_skyport_id.in() << ","
                        << "evtol_id:" << msg.evtol_id.in() << ","
                        << "pilot_id:" << msg.pilot_id.in() << ","
                        << "route_id:" << msg.route_id.in() << ","
                        << "weather_id:" << msg.weather_id.in() << std::endl;

                outfile.close(); // Close the file after writing

                break; // Exit the loop after processing valid data
            }
            else // If no valid data is present
            {
                // If the signal hasn't been sent yet, send the signal and set the flag
                if (!signal_sent)
                {
                    rcs_.signal(); // Signal the condition variable to notify waiting threads
                    signal_sent = true; // Set the flag to prevent resending the signal
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
