#include "../../model/UATMTraits.h"
#include <tools/modeling/codegen/model/NullReaderListener.h>
#include <model/Sync.h>
#include <ace/Log_Msg.h>
#include <dds/DCPS/WaitSet.h>

#include "ReaderListenerRequest.h"
#include "ReaderListenerAvailability.h"


ReaderListenerRequest::ReaderListenerRequest(OpenDDS::Model::ReaderCondSync& rcs)
  : rcs_(rcs) {}

void
ReaderListenerRequest::on_data_available(DDS::DataReader_ptr reader) 
  {
    ACE_Guard<ACE_Thread_Mutex> g(mutex_);

    UATM::bookingFlightRequestDataReader_var reader_i =
      UATM::bookingFlightRequestDataReader::_narrow(reader);

    if (CORBA::is_nil(reader_i.in())) {
    ACE_ERROR((LM_ERROR,
               ACE_TEXT("ERROR: %N:%l: on_data_available() -")
               ACE_TEXT(" _narrow failed!\n")));
    ACE_OS::exit(-1);
  }

    UATM::bookingFlightRequest msg;
    DDS::SampleInfo info;

    while (true) {
      DDS::ReturnCode_t error = reader_i->take_next_sample(msg, info);
      if (error == DDS::RETCODE_OK) {
        std::cout << "SampleInfo.sample_rank = " << info.sample_rank << std::endl;
        if (info.valid_data) {
          std::cout << "----------------------------------" << std::endl
                    << "        bookingFlightRequest:" << std::endl
                    << "        -----------------" << std::endl
                    << "Booking ID: " << msg.booking_id << std::endl
                    << "Flight ID: " << msg.flight_id << std::endl
                    << "Departure Time: " << msg.departure_time.in() << std::endl
                    << "Arrival Time: " << msg.arrival_time.in() << std::endl
                    << "Route ID: " << msg.route_id << std::endl
                    << "Request Status: " << msg.request_status << std::endl;

                  std::cout  << "\n ------Booking Flight Request Received----- " <<  std::endl;
                  for (const auto& availability : ReaderListenerAvailability::storedAvailabilities) {
                            std::cout  << "Available resource type: " << availability.resource_type <<  std::endl
                              << "Available resource ID: " << availability.resource_id <<  std::endl;
                }
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
        rcs_.signal();
        break;
      }
    }
  }
