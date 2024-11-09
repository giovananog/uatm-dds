#include "../../model/UATMTraits.h"  // Include traits for the Urban Air Traffic Management (UATM) model
#include "ReaderListenerRequest.h"  // Include the custom reader listener class
#include <model/Sync.h>  // Include synchronization utilities for DDS
#include <ace/Log_Msg.h>  // Include logging utilities from ACE (Adaptive Communicative Environment)
#include <dds/DCPS/WaitSet.h>  // Include DDS WaitSet class for synchronization of multiple events
#include <fstream>  // Include file stream classes for file operations
#include <sstream>  // Include string stream classes for handling strings in streams
#include <string>  // Include string manipulation classes
#include <vector>  // Include vector container for dynamic arrays
#include "../utils/functions.h"  // Include utility functions (assumed to be in the 'utils' directory)

ReaderListenerRequest::ReaderListenerRequest(OpenDDS::Model::ReaderCondSync &rcs)
    : rcs_(rcs) {}  // Constructor initializes ReaderCondSync object

void ReaderListenerRequest::on_data_available(DDS::DataReader_ptr reader)
{
    ACE_Guard<ACE_Thread_Mutex> g(mutex_);  // Guard the mutex to ensure thread-safety when accessing shared resources
    static bool signal_sent = false;  // Flag to ensure that the signal is sent only once

    // Narrow the reader to the specific data type for flight assignment (flightAssignDataReader)
    UATM::flightAssignDataReader_var reader_i =
        UATM::flightAssignDataReader::_narrow(reader);

    // Check if the narrow operation was successful, exit if it failed
    if (CORBA::is_nil(reader_i.in()))
    {
        ACE_ERROR((LM_ERROR,
                   ACE_TEXT("ERROR: %N:%l: on_data_available() -")
                       ACE_TEXT(" _narrow failed!\n")));
        ACE_OS::exit(-1);  // Exit the program if narrowing the reader failed
    }

    UATM::flightAssign msg;  // Declare a message object to store data from the reader
    DDS::SampleInfo info;  // Declare a SampleInfo object to store metadata about the sample

    // Loop to process available data
    while (true)
    {
        // Take the next sample from the reader
        DDS::ReturnCode_t error = reader_i->take_next_sample(msg, info);
        if (error == DDS::RETCODE_OK)  // If data was successfully read
        {
            if (info.valid_data)  // If the data is valid
            {
                // Print the details of the flight assignment to the console
                std::cout << "| flightAssign: "
                          << "flight_assign_id:" << msg.flight_assign_id
                          << ",assign_time:" << msg.assign_time.in()
                          << ",flight_id:" << msg.flight_id.in()
                          << ",pilot_id:" << msg.pilot_id.in()
                          << ",evtol_id:" << msg.evtol_id.in()
                          << ",origin_skyport_id:" << msg.origin_skyport_id.in()
                          << ",destination_skyport_id:" << msg.destination_skyport_id.in() 
                          << std::endl;

                // Update the EVTOL (Electric Vertical Take-Off and Landing) status
                updateEvtolStatus(msg.evtol_id.in(), 0, msg.destination_skyport_id.in());
            }
            else  // If the data is invalid, send a signal if not already done
            {
                if (!signal_sent)
                {
                    rcs_.signal();  // Signal that data has been processed
                    signal_sent = true;  // Set the flag to avoid signaling more than once
                }
                break;  // Exit the loop after processing the sample
            }
        }
        else  // If there was an error while reading the sample
        {
            if (error != DDS::RETCODE_NO_DATA)  // If the error isn't "no data"
            {
                ACE_ERROR((LM_ERROR,
                           ACE_TEXT("ERROR: %N:%l: on_data_available() -")
                               ACE_TEXT(" take_next_sample failed!\n")));  // Log the error
            }
            break;  // Exit the loop after an error
        }
    }
};
