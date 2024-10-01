#include "../../model/UATMTraits.h"
#include "../utils/functions.h"
#include <tools/modeling/codegen/model/NullReaderListener.h>
#include <model/Sync.h>
#include <ace/Log_Msg.h>
#include <dds/DCPS/WaitSet.h>
#include "ReaderListenerAvailability.h"
#include <fstream>

ReaderListenerAvailability::ReaderListenerAvailability(OpenDDS::Model::ReaderCondSync &rcs)
    : rcs_(rcs) {}

void ReaderListenerAvailability::on_data_available(DDS::DataReader_ptr reader)
{
    ACE_Guard<ACE_Thread_Mutex> g(mutex_);

    UATM::availabilityInfoDataReader_var reader_i =
        UATM::availabilityInfoDataReader::_narrow(reader);

    if (CORBA::is_nil(reader_i.in()))
    {
        ACE_ERROR((LM_ERROR,
                   ACE_TEXT("ERROR: %N:%l: on_data_available() -")
                       ACE_TEXT(" _narrow failed!\n")));
        ACE_OS::exit(-1);
    }

    UATM::availabilityInfo msg;
    DDS::SampleInfo info;

    while (true)
    {
        DDS::ReturnCode_t error = reader_i->take_next_sample(msg, info);
        if (error == DDS::RETCODE_OK)
        {
            if (info.valid_data)
            {
                std::cout << "| AvailabilityInfo: "
                          << "resource_id:" << msg.resource_id.in()
                          << ",resource_type:" << msg.resource_type.in()
                          << ",available:" << msg.available
                          << ",skyport_id:" << msg.skyport_id.in()
                          << ",availability_time:" << msg.availability_time.in() << std::endl;

                std::ofstream request_file("uaspManagerDP/data/tolpads.txt", std::ios_base::app);
                request_file << "resource_id=" << msg.resource_id.in() << ","
                             << "skyport_id=" << msg.skyport_id.in() << ","
                             << "available=" << msg.available << ","
                             << "availability_time=" << msg.availability_time.in() << "\n";
                request_file.close();
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
