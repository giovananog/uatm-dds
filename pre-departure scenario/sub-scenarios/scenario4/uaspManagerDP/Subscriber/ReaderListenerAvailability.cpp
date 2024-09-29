#include "../../model/UATMTraits.h"
#include "../utils/functions.h"
#include <tools/modeling/codegen/model/NullReaderListener.h>
#include <model/Sync.h>
#include <ace/Log_Msg.h>
#include <dds/DCPS/WaitSet.h>
#include "ReaderListenerAvailability.h"
#include <fstream>

ReaderListenerAvailability::ReaderListenerAvailability(OpenDDS::Model::ReaderCondSync& rcs)
  : rcs_(rcs) {}

void ReaderListenerAvailability::on_data_available(DDS::DataReader_ptr reader) 
{
    ACE_Guard<ACE_Thread_Mutex> g(mutex_);

    UATM::availabilityInfoDataReader_var reader_i =
      UATM::availabilityInfoDataReader::_narrow(reader);

    if (CORBA::is_nil(reader_i.in())) {
        ACE_ERROR((LM_ERROR,
                   ACE_TEXT("ERROR: %N:%l: on_data_available() -")
                   ACE_TEXT(" _narrow failed!\n")));
        ACE_OS::exit(-1);
    }

    UATM::availabilityInfo msg;
    DDS::SampleInfo info;

    while (true) {
        DDS::ReturnCode_t error = reader_i->take_next_sample(msg, info);
        if (error == DDS::RETCODE_OK) {
            if (info.valid_data) {
                std::cout << "----------------------------------" << std::endl
                          << "        availabilityInfo:" << std::endl
                          << "        -----------------" << std::endl
                          << "Resource ID: " << msg.resource_id.in() << std::endl
                          << "Resource Type: " << msg.resource_type.in() << std::endl
                          << "Status: " << msg.available << std::endl
                          << "Location: " << msg.skyport_id.in() << std::endl
                          << "Availability Time: " << msg.availability_time.in() << std::endl;

                saveTolPadsToFile(msg);
            } else {
                rcs_.signal();
                break;
            }
        } else {
            if (error != DDS::RETCODE_NO_DATA) {
                ACE_ERROR((LM_ERROR,
                           ACE_TEXT("ERROR: %N:%l: on_data_available() -")
                           ACE_TEXT(" take_next_sample failed!\n")));
            }
            break;
        }
    }
}
