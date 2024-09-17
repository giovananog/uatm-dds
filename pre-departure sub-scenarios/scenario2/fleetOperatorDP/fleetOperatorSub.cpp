#ifdef ACE_AS_STATIC_LIBS
#include <dds/DCPS/transport/tcp/Tcp.h>
#endif
  
#include "../model/UATMTraits.h"
#include < tools/modeling/codegen/model/NullReaderListener.h>

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
               ACE_TEXT("ERROR: %N:%l: on_data_available_request() -")
               ACE_TEXT(" _narrow failed!\n")));
    ACE_OS::exit(-1);
  }

    UATM::availabilityInfo msg;
    DDS::SampleInfo info;

    // Read until no more messages
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
            // break;
            std::cout << "Received sample, but no valid data." << std::endl;
        }
        // break;
      } else {
        if (error != DDS::RETCODE_NO_DATA) {
        ACE_ERROR((LM_ERROR,
                   ACE_TEXT("ERROR: %N:%l: on_data_available_request() -")
                   ACE_TEXT(" take_next_sample failed!\n")));
        }
        rcs_.signal();
        break;
      }
    }
  };

class ReaderListenerRoute : public OpenDDS::Model::NullReaderListener {
  public:
    ReaderListenerRoute(OpenDDS::Model::ReaderCondSync& rcs) : rcs_(rcs) {}
    virtual void on_data_available(DDS::DataReader_ptr reader);
  private:
    OpenDDS::Model::ReaderCondSync& rcs_;
    ACE_Thread_Mutex mutex_;
};

void
ReaderListenerRoute::on_data_available(DDS::DataReader_ptr reader) 
  {
    ACE_Guard<ACE_Thread_Mutex> g(mutex_);

    UATM::flightRoutesInfoDataReader_var reader_i =
      UATM::flightRoutesInfoDataReader::_narrow(reader);

    if (CORBA::is_nil(reader_i.in())) {
    ACE_ERROR((LM_ERROR,
               ACE_TEXT("ERROR: %N:%l: on_data_available_request() -")
               ACE_TEXT(" _narrow failed!\n")));
    ACE_OS::exit(-1);
  }

    UATM::flightRoutesInfo msg;
    DDS::SampleInfo info;

    // Read until no more messages
    while (true) {
      DDS::ReturnCode_t error = reader_i->take_next_sample(msg, info);
      if (error == DDS::RETCODE_OK) {
        std::cout << "SampleInfo.sample_rank = " << info.sample_rank << std::endl;
        if (info.valid_data) {
          std::cout << "----------------------------------" << std::endl
                    << "        flightRoutesInfo:" << std::endl
                    << "        -----------------" << std::endl
                    << "Flight Route ID: " << msg.flight_route_id << std::endl
                    << "Waypoints: " << msg.waypoints << std::endl
                    << "Fligt ID: " << msg.flight_id << std::endl
                    << "Estimated Time: " << msg.estimated_time.in() << std::endl
                    << "Status: " << msg.status << std::endl;
        } else {
            rcs_.signal();
            // break;
            std::cout << "Received sample, but no valid data." << std::endl;
        }
        // break;
      } else {
        if (error != DDS::RETCODE_NO_DATA) {
        ACE_ERROR((LM_ERROR,
                   ACE_TEXT("ERROR: %N:%l: on_data_available_request() -")
                   ACE_TEXT(" take_next_sample failed!\n")));
        }
        break;
      }
    }
  };

class ReaderListenerWeather : public OpenDDS::Model::NullReaderListener {
  public:
    ReaderListenerWeather(OpenDDS::Model::ReaderCondSync& rcs) : rcs_(rcs) {}
    virtual void on_data_available(DDS::DataReader_ptr reader);
  private:
    OpenDDS::Model::ReaderCondSync& rcs_;
    ACE_Thread_Mutex mutex_;
};

void
ReaderListenerWeather::on_data_available(DDS::DataReader_ptr reader) 
  {
    ACE_Guard<ACE_Thread_Mutex> g(mutex_);

    UATM::weatherInfoDataReader_var reader_i =
      UATM::weatherInfoDataReader::_narrow(reader);

    if (CORBA::is_nil(reader_i.in())) {
    ACE_ERROR((LM_ERROR,
               ACE_TEXT("ERROR: %N:%l: on_data_available_request() -")
               ACE_TEXT(" _narrow failed!\n")));
    ACE_OS::exit(-1);
  }

    UATM::weatherInfo msg;
    DDS::SampleInfo info;

    // Read until no more messages
    while (true) {
      DDS::ReturnCode_t error = reader_i->take_next_sample(msg, info);
      if (error == DDS::RETCODE_OK) {
        std::cout << "SampleInfo.sample_rank = " << info.sample_rank << std::endl;
        if (info.valid_data) {
          std::cout << "----------------------------------" << std::endl
                    << "        weatherInfo:" << std::endl
                    << "        -----------------" << std::endl
                    << "Weather Info ID: " << msg.weather_id << std::endl
                    << "Location: " << msg.location.in() << std::endl
                    << "Temperature: " << msg.temperature << std::endl
                    << "Wind Speed: " << msg.wind_speed << std::endl
                    << "Weather Condition: " << msg.weather_condition.in() << std::endl
                    << "Observation Time: " << msg.observation_time.in() << std::endl;
        } else {
            rcs_.signal();
            // break;
            std::cout << "Received sample, but no valid data." << std::endl;
        }
        // break;
      } else {
        if (error != DDS::RETCODE_NO_DATA) {
        ACE_ERROR((LM_ERROR,
                   ACE_TEXT("ERROR: %N:%l: on_data_available_request() -")
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
    UATM::uatmDCPS::DefaultUATMType model3(application, argc, argv);

    using OpenDDS::Model::UATM::uatmDCPS::Elements;

    ACE_SYNCH_MUTEX lock;
    ACE_Condition<ACE_SYNCH_MUTEX> condition(lock);

    DDS::DataReader_var reader_availability = model.reader(Elements::DataReaders::availabilityDR_FOP);
    OpenDDS::Model::ReaderCondSync rcs(reader_availability, condition);
    DDS::DataReaderListener_var listener(new ReaderListenerAvailability(rcs));
    reader_availability->set_listener(listener, OpenDDS::DCPS::DEFAULT_STATUS_MASK);
    
    DDS::DataReader_var reader_routes = model2.reader(Elements::DataReaders::FlightRoutesDR_FOP);
    OpenDDS::Model::ReaderCondSync rcs2(reader_routes, condition);
    DDS::DataReaderListener_var listener2(new ReaderListenerRoute(rcs2));
    reader_routes->set_listener(listener2, OpenDDS::DCPS::DEFAULT_STATUS_MASK);

    DDS::DataReader_var reader_weather = model3.reader(Elements::DataReaders::weatherDR_FOP);
    OpenDDS::Model::ReaderCondSync rcs3(reader_weather, condition);
    DDS::DataReaderListener_var listener3(new ReaderListenerWeather(rcs3));
    reader_weather->set_listener(listener3, OpenDDS::DCPS::DEFAULT_STATUS_MASK);

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