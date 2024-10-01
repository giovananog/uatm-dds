#include "../../model/UATMTraits.h"
#include "ReaderListenerRequest.h"
#include <model/Sync.h>
#include <ace/Log_Msg.h>
#include <dds/DCPS/WaitSet.h>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "../utils/functions.h"

ReaderListenerRequest::ReaderListenerRequest(OpenDDS::Model::ReaderCondSync &rcs)
    : rcs_(rcs) {}


void ReaderListenerRequest::on_data_available(DDS::DataReader_ptr reader)
{
    ACE_Guard<ACE_Thread_Mutex> g(mutex_);

    UATM::flightAssignDataReader_var reader_i =
        UATM::flightAssignDataReader::_narrow(reader);

    if (CORBA::is_nil(reader_i.in()))
    {
        ACE_ERROR((LM_ERROR,
                   ACE_TEXT("ERROR: %N:%l: on_data_available() -")
                       ACE_TEXT(" _narrow failed!\n")));
        ACE_OS::exit(-1);
    }

    UATM::flightAssign msg;
    DDS::SampleInfo info;

    while (true)
    {
        DDS::ReturnCode_t error = reader_i->take_next_sample(msg, info);
        if (error == DDS::RETCODE_OK)
        {
            if (info.valid_data)
            {
                std::cout << "| flightAssign: " 
                          << "flight_assign_id:" << msg.flight_assign_id 
                          << ",assign_time:" << msg.assign_time.in() 
                          << ",flight_id:" << msg.flight_id.in() 
                          << ",pilot_id:" << msg.pilot_id.in() 
                          << ",evtol_id:" << msg.evtol_id.in() << std::endl;

                updateEvtolStatus(msg.evtol_id.in(), 0, 0); 
            }
            else
            {
                rcs_.signal();
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
}
