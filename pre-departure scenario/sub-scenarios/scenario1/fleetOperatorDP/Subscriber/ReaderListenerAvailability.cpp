#include "../../model/UATMTraits.h"
#include "ReaderListenerAvailability.h"
#include <model/Sync.h>
#include <ace/Log_Msg.h>
#include <dds/DCPS/WaitSet.h>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>


ReaderListenerAvailability::ReaderListenerAvailability(OpenDDS::Model::ReaderCondSync& rcs)
  : rcs_(rcs) {}

void ReaderListenerAvailability::updateAvailabilityFile(const UATM::availabilityInfo& msg) {
    std::string filename = "fleetOperatorDP/availabilities.txt"; 
    std::ifstream file(filename);
    std::string line;
    std::vector<std::string> lines;
    bool found = false;

    while (std::getline(file, line)) {
        if (!line.empty()) {
            if (line.find("resource_id=" + std::string(msg.resource_id.in())) != std::string::npos) {
                if (msg.available == 0) {
                    found = true;
                    continue; 
                }
            }
            lines.push_back(line); 
        }
    }
    file.close();

    if (msg.available == 1) {
        std::ostringstream new_line;
        new_line << "resource_id=" << msg.resource_id.in()
                 << ",resource_type=" << msg.resource_type.in()
                  << ",available=" << msg.available
                  << ",skyport_id=" << msg.skyport_id.in()
                  << ",availability_time=" << msg.availability_time.in();
        lines.push_back(new_line.str());
    }

    std::ofstream out_file(filename);
    for (const auto &l : lines) {
        out_file << l << "\n";
    }
    out_file.close();
}

void ReaderListenerAvailability::on_data_available(DDS::DataReader_ptr reader) {

    ACE_Guard<ACE_Thread_Mutex> g(mutex_);

    UATM::availabilityInfoDataReader_var reader_i =
      UATM::availabilityInfoDataReader::_narrow(reader);

    if (CORBA::is_nil(reader_i.in())) {
        ACE_ERROR((LM_ERROR,
                   ACE_TEXT("ERROR: %N:%l: on_data_available_request() -")
                   ACE_TEXT(" _narrow failed!\n")));
        ACE_OS::exit(-1);
    }

    UATM::availabilityInfo msg;
    DDS::SampleInfo info;

    while (true) {
        std::cout << "\n\n" << std::endl;                                              
        DDS::ReturnCode_t error = reader_i->take_next_sample(msg, info);
        if (error == DDS::RETCODE_OK) {
            if (info.valid_data) {
                std::cout << "------------------------fleetOperatorDP----------" << std::endl
                          << "        AvailabilityInfo:" << std::endl
                          << "        -----------------" << std::endl
                          << "Resource ID: " << msg.resource_id.in() << std::endl
                          << "Resource Type: " << msg.resource_type.in() << std::endl
                          << "Available: " << msg.available << std::endl
                          << "Skyport ID: " << msg.skyport_id.in() << std::endl
                          << "Availability Time: " << msg.availability_time.in() << std::endl; 
                updateAvailabilityFile(msg);
            } else {
                rcs_.signal();
                break;
            }
        } else {
            if (error != DDS::RETCODE_NO_DATA) {
                ACE_ERROR((LM_ERROR,
                           ACE_TEXT("ERROR: %N:%l: on_data_available_request() -")
                           ACE_TEXT(" take_next_sample failed!\n")));
            }
            break;
        }
    }
}
