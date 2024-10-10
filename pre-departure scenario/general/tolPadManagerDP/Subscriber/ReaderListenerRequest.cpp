#include "../../model/UATMTraits.h"
#include "../utils/functions.h"
#include <tools/modeling/codegen/model/NullReaderListener.h>
#include <model/Sync.h>
#include <ace/Log_Msg.h>
#include <dds/DCPS/WaitSet.h>
#include "ReaderListenerRequest.h"
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

ReaderListenerRequest::ReaderListenerRequest(OpenDDS::Model::ReaderCondSync &rcs)
    : rcs_(rcs) {} 

void ReaderListenerRequest::on_data_available(DDS::DataReader_ptr reader)
{
    ACE_Guard<ACE_Thread_Mutex> g(mutex_);

    static bool signal_sent = false;

    UATM::tolPadRequestDataReader_var reader_i =
        UATM::tolPadRequestDataReader::_narrow(reader);

    if (CORBA::is_nil(reader_i.in()))
    {
        ACE_ERROR((LM_ERROR,
                   ACE_TEXT("ERROR: %N:%l: on_data_available() -")
                       ACE_TEXT(" _narrow failed!\n")));
        ACE_OS::exit(-1);
    }

    UATM::tolPadRequest msg;
    DDS::SampleInfo info;

    while (true)
    {
        DDS::ReturnCode_t error = reader_i->take_next_sample(msg, info);
        if (error == DDS::RETCODE_OK)
        {
            if (info.valid_data)
            {
                std::cout << "| tolPadRequest: "
                          << ",assign_id:" << msg.assign_id
                          << ",flight_id:" << msg.flight_id
                          << ",tol_pad_id:" << msg.tol_pad_id.in()
                          << ",assign_time:" << msg.assign_time.in() << std::endl;
                
                updateTolPadStatus(msg.tol_pad_id.in(), 0);
                break;
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
}
