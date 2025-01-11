#include "../../model/UATMTraits.h" // Custom UATM model traits header
#include "ReaderListenerRoute.h" // Header for flight routes info listener
#include <model/Sync.h> // Synchronization utilities
#include <ace/Log_Msg.h> // ACE logging utilities
#include <dds/DCPS/WaitSet.h> // DDS WaitSet for managing events
#include <fstream> // File handling

// Constructor for ReaderListenerRoute, initializes with ReaderCondSync object
ReaderListenerRoute::ReaderListenerRoute(OpenDDS::Model::ReaderCondSync &rcs)
    : rcs_(rcs) {}

// This method is called when new data becomes available from the reader
void ReaderListenerRoute::on_data_available(DDS::DataReader_ptr reader)
{
    // Lock to ensure thread safety
    ACE_Guard<ACE_Thread_Mutex> g(mutex_);
    static bool signal_sent = false; // Flag to ensure the signal is sent only once

    // Narrow the generic DataReader to the specific type (flightRoutesInfoDataReader)
    UATM::flightRoutesInfoDataReader_var reader_i =
        UATM::flightRoutesInfoDataReader::_narrow(reader);

    // Check if narrowing succeeded
    if (CORBA::is_nil(reader_i.in()))
    {
        // Log error and exit if narrowing failed
        ACE_ERROR((LM_ERROR,
                   ACE_TEXT("ERROR: %N:%l: on_data_available_request() -")
                       ACE_TEXT(" _narrow failed!\n")));
        ACE_OS::exit(-1);
    }

    // Create message and sample info objects
    UATM::flightRoutesInfo msg;
    DDS::SampleInfo info;

    while (true)
    {
        // Attempt to take the next sample from the reader
        DDS::ReturnCode_t error = reader_i->take_next_sample(msg, info);

        if (error == DDS::RETCODE_OK) // If the sample was successfully retrieved
        {
            if (info.valid_data) // If the sample contains valid data
            {
                // Output the flight route information to the console
                std::cout << "| flightRoutesInfo: "
                          << "flight_route_id:" << msg.flight_route_id
                          << ",origin_skyport_id:" << msg.origin_skyport_id.in()
                          << ",destination_skyport_id:" << msg.destination_skyport_id.in()
                          << ",available_capacity:" << msg.available_capacity
                          << ",available:" << msg.available
                          << ",traffic_density:" << msg.traffic_density << std::endl;

                // Open file in append mode and write the flight route information to it
                std::ofstream outfile;
                outfile.open("fleetOperatorDP/data/routes.txt", std::ios_base::app);

                // Append data to file
                outfile << "route_id:" << msg.flight_route_id << ","
                        << "origin_skyport_id:" << msg.origin_skyport_id.in() << ","
                        << "destination_skyport_id:" << msg.destination_skyport_id.in() << ","
                        << "available_capacity:" << msg.available_capacity << ","
                        << "available:" << msg.available << ","
                        << "traffic_density:" << msg.traffic_density << std::endl;

                outfile.close(); // Close the file after writing
            }
            else // If the sample doesn't contain valid data
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
}
