#ifdef ACE_AS_STATIC_LIBS
#include <C:/Users/ongio_1lak36v/Downloads/OpenDDS-3.29.1/dds/DCPS/transport/tcp/Tcp.h>
#endif
  
#include "../model/UATMTraits.h"
#include <C:/Users/ongio_1lak36v/Downloads/OpenDDS-3.29.1/tools/modeling/codegen/model/NullReaderListener.h>

#include <model/Sync.h>
#include <ace/Log_Msg.h>

#include <dds/DCPS/WaitSet.h>


class ReaderListenerFlows : public OpenDDS::Model::NullReaderListener {
  public:
    ReaderListenerFlows(OpenDDS::Model::ReaderCondSync& rcs) : rcs_(rcs) {}
    // virtual void on_data_available_request(DDS::DataReader_ptr reader);
    virtual void on_data_available(DDS::DataReader_ptr reader);
  private:
    OpenDDS::Model::ReaderCondSync& rcs_;
    ACE_Thread_Mutex mutex_;
};

void
ReaderListenerFlows::on_data_available(DDS::DataReader_ptr reader) 
  {
    ACE_Guard<ACE_Thread_Mutex> g(mutex_);

    UATM::trafficFlowsInfoDataReader_var reader_i =
      UATM::trafficFlowsInfoDataReader::_narrow(reader);

    if (CORBA::is_nil(reader_i.in())) {
    ACE_ERROR((LM_ERROR,
               ACE_TEXT("ERROR: %N:%l: on_data_available_request() -")
               ACE_TEXT(" _narrow failed!\n")));
    ACE_OS::exit(-1);
  }

    UATM::trafficFlowsInfo msg;
    DDS::SampleInfo info;

    // Read until no more messages
    while (true) {
      DDS::ReturnCode_t error = reader_i->take_next_sample(msg, info);
      if (error == DDS::RETCODE_OK) {
        std::cout << "SampleInfo.sample_rank = " << info.sample_rank << std::endl;
        if (info.valid_data) {
          std::cout << "----------------------------------" << std::endl
                    << "        trafficFlowsInfo:" << std::endl
                    << "        -----------------" << std::endl
                    << "Flows ID: " << msg.flows_id << std::endl
                    << "Area: " << msg.area.in() << std::endl
                    << "Congestion Level: " << msg.congestion_level.in() << std::endl
                    << "Affected Routes: " << msg.affected_routes.in() << std::endl
                    << "Time: " << msg.timestamp.in() << std::endl;
        } else {
            rcs_.signal();
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

class ReaderListenerRest : public OpenDDS::Model::NullReaderListener {
  public:
    ReaderListenerRest(OpenDDS::Model::ReaderCondSync& rcs) : rcs_(rcs) {}
    virtual void on_data_available(DDS::DataReader_ptr reader);
  private:
    OpenDDS::Model::ReaderCondSync& rcs_;
    ACE_Thread_Mutex mutex_;
};

void
ReaderListenerRest::on_data_available(DDS::DataReader_ptr reader) 
  {
    ACE_Guard<ACE_Thread_Mutex> g(mutex_);

    UATM::airspaceRestrictionsDataReader_var reader_i =
      UATM::airspaceRestrictionsDataReader::_narrow(reader);

    if (CORBA::is_nil(reader_i.in())) {
    ACE_ERROR((LM_ERROR,
               ACE_TEXT("ERROR: %N:%l: on_data_available() -")
               ACE_TEXT(" _narrow failed!\n")));
    ACE_OS::exit(-1);
  }

    UATM::airspaceRestrictions msg;
    DDS::SampleInfo info;

    // Read until no more messages
    while (true) {
      DDS::ReturnCode_t error = reader_i->take_next_sample(msg, info);
      if (error == DDS::RETCODE_OK) {
        std::cout << "SampleInfo.sample_rank = " << info.sample_rank << std::endl;
        if (info.valid_data) {
          std::cout << "----------------------------------" << std::endl
                    << "        airspaceRestrictions:" << std::endl
                    << "        -----------------" << std::endl
                    << "Rest ID: " << msg.restriction_id << std::endl
                    << "Area: " << msg.area.in() << std::endl
                    << "Type: " << msg.restriction_type.in() << std::endl
                    << "Start Time: " << msg.start_time.in() << std::endl
                    << "End Time: " << msg.end_time.in() << std::endl
                    << "Approved by: " << msg.authority.in() << std::endl;
        } else {
            rcs_.signal();
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

    DDS::DataReader_var reader_flows = model.reader(Elements::DataReaders::trafficFlowsDR_UASP);
    OpenDDS::Model::ReaderCondSync rcs(reader_flows, condition);
    DDS::DataReaderListener_var listener(new ReaderListenerFlows(rcs));
    reader_flows->set_listener(listener, OpenDDS::DCPS::DEFAULT_STATUS_MASK);
    
    DDS::DataReader_var reader_rest = model2.reader(Elements::DataReaders::airspaceRestDR_UASP);
    OpenDDS::Model::ReaderCondSync rcs2(reader_rest, condition);
    DDS::DataReaderListener_var listener2(new ReaderListenerRest(rcs2));
    reader_rest->set_listener(listener2, OpenDDS::DCPS::DEFAULT_STATUS_MASK);

    DDS::DataReader_var reader_weather = model3.reader(Elements::DataReaders::weatherInfoDR_UASP);
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