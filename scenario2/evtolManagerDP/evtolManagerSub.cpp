#ifdef ACE_AS_STATIC_LIBS
#include <C:/Users/ongio_1lak36v/Downloads/OpenDDS-3.29.1/dds/DCPS/transport/tcp/Tcp.h>
#endif
  
#include "../model/UATMTraits.h"
#include <C:/Users/ongio_1lak36v/Downloads/OpenDDS-3.29.1/tools/modeling/codegen/model/NullReaderListener.h>

#include <model/Sync.h>
#include <ace/Log_Msg.h>
#include <dds/DCPS/WaitSet.h>


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

    UATM::flightAssignDataReader_var reader_i =
      UATM::flightAssignDataReader::_narrow(reader);

    if (CORBA::is_nil(reader_i.in())) {
    ACE_ERROR((LM_ERROR,
               ACE_TEXT("ERROR: %N:%l: on_data_available() -")
               ACE_TEXT(" _narrow failed!\n")));
    ACE_OS::exit(-1);
  }

    UATM::flightAssign msg;
    DDS::SampleInfo info;

    while (true) {
      DDS::ReturnCode_t error = reader_i->take_next_sample(msg, info);
      if (error == DDS::RETCODE_OK) {
        std::cout << "SampleInfo.sample_rank = " << info.sample_rank << std::endl;
        if (info.valid_data) {
          std::cout << "----------------------------------" << std::endl
                    << "        flightAssign:" << std::endl
                    << "        -----------------" << std::endl
                    << "Flight Assign ID: " << msg.flight_assign_id << std::endl
                    << "Assign Time: " << msg.assign_time << std::endl
                    << "Operator ID: " << msg.operator_id << std::endl
                    << "Assign Status: " << msg.assign_status << std::endl
                    << "Resources ID: " << msg.resources_id << std::endl;
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

    using OpenDDS::Model::UATM::uatmDCPS::Elements;

    ACE_SYNCH_MUTEX lock;
    ACE_Condition<ACE_SYNCH_MUTEX> condition(lock);

    DDS::DataReader_var reader_assign = model.reader(Elements::DataReaders::flightAssignDR_EV);
    OpenDDS::Model::ReaderCondSync rcs(reader_assign, condition);
    DDS::DataReaderListener_var listener(new ReaderListenerRequest(rcs));
    reader_assign->set_listener(listener, OpenDDS::DCPS::DEFAULT_STATUS_MASK);
    // listener->on_data_available(reader_assign);                           

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