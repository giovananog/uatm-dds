#ifdef ACE_AS_STATIC_LIBS
#include <C:/Users/ongio_1lak36v/Downloads/OpenDDS-3.29.1/dds/DCPS/transport/tcp/Tcp.h>
#endif
  
#include "../model/UATMTraits.h"
#include <C:/Users/ongio_1lak36v/Downloads/OpenDDS-3.29.1/tools/modeling/codegen/model/NullReaderListener.h>

#include <model/Sync.h>
#include <ace/Log_Msg.h>
#include <dds/DCPS/WaitSet.h>


class ReaderListenerAvailability : public OpenDDS::Model::NullReaderListener {
  public:
    ReaderListenerAvailability(OpenDDS::Model::ReaderCondSync& rcs) : rcs_(rcs) {}
    virtual void on_data_available(DDS::DataReader_ptr reader);
  private:
    OpenDDS::Model::ReaderCondSync& rcs_;
    ACE_Thread_Mutex mutex_;
};

void
ReaderListenerAvailability::on_data_available(DDS::DataReader_ptr reader) 
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
        std::cout << "SampleInfo.sample_rank = " << info.sample_rank << std::endl;
        if (info.valid_data) {
          std::cout << "----------------------------------" << std::endl
                    << "        AvailabilityInfo:" << std::endl
                    << "        -----------------" << std::endl
                    << "Resource ID: " << msg.resource_id << std::endl
                    << "Resource Type: " << msg.resource_type.in() << std::endl
                    << "Status: " << msg.status << std::endl
                    << "Location: " << msg.location.in() << std::endl
                    << "Availability Time: " << msg.availability_time.in() << std::endl;
        } else {
            rcs_.signal();
            std::cout << "Received sample, but no valid data." << std::endl;
        }
        // break;
      } else {
        if (error != DDS::RETCODE_NO_DATA) {
        ACE_ERROR((LM_ERROR,
                   ACE_TEXT("ERROR: %N:%l: on_data_available() -")
                   ACE_TEXT(" take_next_sample failed!\n")));
        }
        break;
      }
    }
  };

class ReaderListenerRequest : public OpenDDS::Model::NullReaderListener {
  public:
    ReaderListenerRequest(OpenDDS::Model::ReaderCondSync& rcs) : rcs_(rcs) {}
    virtual void on_data_available(DDS::DataReader_ptr reader);
  private:
    OpenDDS::Model::ReaderCondSync& rcs_;
    ACE_Thread_Mutex mutex_;
};

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
        } else {
            rcs_.signal();
            std::cout << "Received sample, but no valid data." << std::endl;
        }
        // break;
      } else {
        if (error != DDS::RETCODE_NO_DATA) {
        ACE_ERROR((LM_ERROR,
                   ACE_TEXT("ERROR: %N:%l: on_data_available() -")
                   ACE_TEXT(" take_next_sample failed!\n")));
        }
        break;
      }
    }
  };

int ACE_TMAIN(int argc, ACE_TCHAR* argv[])
{
  try {
    OpenDDS::Model::Application application(argc, argv);
    UATM::uatmDCPS::DefaultUATMType model(application, argc, argv);
    UATM::uatmDCPS::DefaultUATMType model2(application, argc, argv);

    using OpenDDS::Model::UATM::uatmDCPS::Elements;


    ACE_SYNCH_MUTEX lock;
    ACE_Condition<ACE_SYNCH_MUTEX> condition(lock);


    DDS::DataReader_var reader_availability = model.reader(Elements::DataReaders::availabilityDR_FOP);
    OpenDDS::Model::ReaderCondSync rcs(reader_availability, condition);
    DDS::DataReaderListener_var listener(new ReaderListenerAvailability(rcs));
    reader_availability->set_listener(listener, OpenDDS::DCPS::DEFAULT_STATUS_MASK);
    listener->on_data_available(reader_availability);
    
    DDS::DataReader_var reader_request = model2.reader(Elements::DataReaders::flightRequestDR_FOP);    
    OpenDDS::Model::ReaderCondSync rcs2(reader_request, condition);
    DDS::DataReaderListener_var listener2(new ReaderListenerRequest(rcs2));
    reader_request->set_listener(listener2, OpenDDS::DCPS::DEFAULT_STATUS_MASK);
    listener2->on_data_available(reader_request);

  } catch (const CORBA::Exception& e) {
    e._tao_print_exception("Exception caught in main():");
    return -1;

  } catch( const std::exception& ex) {
    ACE_ERROR_RETURN((LM_ERROR,
                      ACE_TEXT("(%P|%t) ERROR: %N:%l: main() -")
                      ACE_TEXT(" Exception caught: %C\n"),
                      ex.what()),
                     -1);
  }
  return 0;
}