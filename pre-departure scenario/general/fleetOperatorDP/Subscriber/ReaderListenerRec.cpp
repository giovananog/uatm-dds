#include "../../model/UATMTraits.h" // Custom UATM model traits header
#include <tools/modeling/codegen/model/NullReaderListener.h> // Null reader listener (possibly for code generation)
#include <model/Sync.h> // Synchronization utilities
#include <ace/Log_Msg.h> // ACE logging utilities
#include "ReaderListenerRec.h" // Header for flight change recommendation listener
#include <dds/DCPS/WaitSet.h> // DDS WaitSet for managing events

// Constructor for ReaderListenerRec, initializes with ReaderCondSync object
ReaderListenerRec::ReaderListenerRec(OpenDDS::Model::ReaderCondSync &rcs)
    : rcs_(rcs) {}

// This method is called when new data becomes available from the reader
void ReaderListenerRec::on_data_available(DDS::DataReader_ptr reader)
{
    // Lock to ensure thread safety
    ACE_Guard<ACE_Thread_Mutex> g(mutex_);
    static bool signal_sent = false; // Flag to ensure the signal is sent only once

    // Narrow the generic DataReader to the specific type (flightChangeRecDataReader)
    UATM::flightChangeRecDataReader_var reader_i =
        UATM::flightChangeRecDataReader::_narrow(reader);

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
    UATM::flightChangeRec msg;
    DDS::SampleInfo info;

    while (true)
    {
        // Attempt to take the next sample from the reader
        DDS::ReturnCode_t error = reader_i->take_next_sample(msg, info);
        
        if (error == DDS::RETCODE_OK) // If the sample was successfully retrieved
        {
            if (info.valid_data) // If the sample contains valid data
            {
                // Output the flight change recommendation to the console
                std::cout << "| plm flightChangeRec: "
                          << "recommendation_id:" << msg.recommendation_id
                          << ",flight_id:" << msg.flight_id
                          << ",change_type:" << msg.change_type.in()
                          << ",reason:" << msg.reason.in()
                          << ",recommended_by:" << msg.recommended_by.in()
                          << ",recommendation_time:" << msg.recommendation_time.in() << std::endl;
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
