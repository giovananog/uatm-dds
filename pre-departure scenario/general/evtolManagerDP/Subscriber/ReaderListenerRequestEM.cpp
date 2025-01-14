#include "../../model/UATMTraits.h"  
#include "ReaderListenerRequestEM.h"  
#include <model/Sync.h>  
#include <ace/Log_Msg.h>  
#include <dds/DCPS/WaitSet.h>  
#include "../Utils/functionsEM.h"  

// Constructor initializes ReaderCondSync object
ReaderListenerRequest::ReaderListenerRequest(OpenDDS::Model::ReaderCondSync &rcs)
    : rcs_(rcs) {}  

void ReaderListenerRequest::on_data_available(DDS::DataReader_ptr reader)
{
    ACE_Guard<ACE_Thread_Mutex> g(mutex_);  
    static bool signal_sent = false;  

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
        if (error == DDS::RETCODE_OK)  
        {
            if (info.valid_data)  
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
            else  
            {
                if (!signal_sent)
                {
                    rcs_.signal();  
                    signal_sent = true;  
                }
                break;  
            }
        }
        else  
        {
            if (error != DDS::RETCODE_NO_DATA)  
            {
                ACE_ERROR((LM_ERROR,
                           ACE_TEXT("ERROR: %N:%l: on_data_available() -")
                               ACE_TEXT(" take_next_sample failed!\n")));  
            }
            break;  
        }
    }
};
