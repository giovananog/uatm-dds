#ifdef ACE_AS_STATIC_LIBS
#include <dds/DCPS/transport/tcp/Tcp.h>
#endif
  
#include "../model/UATMTraits.h"
#include < tools/modeling/codegen/model/NullReaderListener.h>

#include <model/Sync.h>
#include <ace/Log_Msg.h>

#include <dds/DCPS/WaitSet.h>


class ReaderListenerRec : public OpenDDS::Model::NullReaderListener {
  public:
    ReaderListenerRec(OpenDDS::Model::ReaderCondSync& rcs) : rcs_(rcs) {}
    virtual void on_data_available(DDS::DataReader_ptr reader);
  private:
    OpenDDS::Model::ReaderCondSync& rcs_;
    ACE_Thread_Mutex mutex_;
};

void
ReaderListenerRec::on_data_available(DDS::DataReader_ptr reader) 
  {
    ACE_Guard<ACE_Thread_Mutex> g(mutex_);

    UATM::flightChangeRecDataReader_var reader_i =
      UATM::flightChangeRecDataReader::_narrow(reader);

    if (CORBA::is_nil(reader_i.in())) {
    ACE_ERROR((LM_ERROR,
               ACE_TEXT("ERROR: %N:%l: on_data_available_request() -")
               ACE_TEXT(" _narrow failed!\n")));
    ACE_OS::exit(-1);
  }

    UATM::flightChangeRec msg;
    DDS::SampleInfo info;

    // Read until no more messages
    while (true) {
      DDS::ReturnCode_t error = reader_i->take_next_sample(msg, info);
      if (error == DDS::RETCODE_OK) {
        std::cout << "SampleInfo.sample_rank = " << info.sample_rank << std::endl;
        if (info.valid_data) {
          std::cout << "----------------------------------" << std::endl
                    << "        flightChangeRec:" << std::endl
                    << "        -----------------" << std::endl
                    << "Rec ID: " << msg.recommendation_id << std::endl
                    << "Flight ID: " << msg.flight_id << std::endl
                    << "Change Tyoe: " << msg.change_type.in() << std::endl
                    << "Reason: " << msg.reason.in() << std::endl
                    << "Rec by: " << msg.recommended_by.in() << std::endl
                    << "Req Status: " << msg.request_status << std::endl
                    << "Time: " << msg.recommendation_time.in() << std::endl;
        } else {
            rcs_.signal();
            std::cout << "Received sample, but no valid data." << std::endl;
        }
        break;
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

class ReaderListenerAuth : public OpenDDS::Model::NullReaderListener {
  public:
    ReaderListenerAuth(OpenDDS::Model::ReaderCondSync& rcs) : rcs_(rcs) {}
    virtual void on_data_available(DDS::DataReader_ptr reader);
  private:
    OpenDDS::Model::ReaderCondSync& rcs_;
    ACE_Thread_Mutex mutex_;
};

void
ReaderListenerAuth::on_data_available(DDS::DataReader_ptr reader) 
  {
    ACE_Guard<ACE_Thread_Mutex> g(mutex_);

    UATM::flightAuthorizationDataReader_var reader_i =
      UATM::flightAuthorizationDataReader::_narrow(reader);

    if (CORBA::is_nil(reader_i.in())) {
    ACE_ERROR((LM_ERROR,
               ACE_TEXT("ERROR: %N:%l: on_data_available_request() -")
               ACE_TEXT(" _narrow failed!\n")));
    ACE_OS::exit(-1);
  }

    UATM::flightAuthorization msg;
    DDS::SampleInfo info;

    // Read until no more messages
    while (true) {
      DDS::ReturnCode_t error = reader_i->take_next_sample(msg, info);
      if (error == DDS::RETCODE_OK) {
        std::cout << "SampleInfo.sample_rank = " << info.sample_rank << std::endl;
        if (info.valid_data) {
          std::cout << "----------------------------------" << std::endl
                    << "        acceptableRoute:" << std::endl
                    << "        -----------------" << std::endl
                    << "Auth ID: " << msg.authorization_id << std::endl
                    << "Flight ID: " << msg.flight_id << std::endl
                    << "Ap Route ID: " << msg.approved_route_id << std::endl
                    << "Authority: " << msg.authority.in() << std::endl
                    << "Auth Time: " << msg.authorization_time.in() << std::endl
                    << "Valid Until: " << msg.valid_until.in() << std::endl;
        } else {
            rcs_.signal();
            std::cout << "Received sample, but no valid data." << std::endl;
        }
        break;
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

int ACE_TMAIN(int argc, ACE_TCHAR* argv[])
{
  try {
    OpenDDS::Model::Application application(argc, argv);
    UATM::uatmDCPS::DefaultUATMType model(application, argc, argv);
    UATM::uatmDCPS::DefaultUATMType model2(application, argc, argv);

    using OpenDDS::Model::UATM::uatmDCPS::Elements;

    ACE_SYNCH_MUTEX lock;
    ACE_Condition<ACE_SYNCH_MUTEX> condition(lock);

    DDS::DataReader_var reader_rec = model.reader(Elements::DataReaders::changeRecDR_PLM);
    OpenDDS::Model::ReaderCondSync rcs(reader_rec, condition);
    DDS::DataReaderListener_var listener(new ReaderListenerRec(rcs));
    reader_rec->set_listener(listener, OpenDDS::DCPS::DEFAULT_STATUS_MASK);
    
    DDS::DataReader_var reader_auth = model2.reader(Elements::DataReaders::flightAuthDR_PLM);
    OpenDDS::Model::ReaderCondSync rcs2(reader_auth, condition);
    DDS::DataReaderListener_var listener2(new ReaderListenerAuth(rcs2));
    reader_auth->set_listener(listener2, OpenDDS::DCPS::DEFAULT_STATUS_MASK);

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