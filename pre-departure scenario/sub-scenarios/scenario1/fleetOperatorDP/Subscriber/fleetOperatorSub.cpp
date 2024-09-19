#ifdef ACE_AS_STATIC_LIBS
#include <dds/DCPS/transport/tcp/Tcp.h>
#endif
  
#include "../../model/UATMTraits.h"
#include <tools/modeling/codegen/model/NullReaderListener.h>

#include <model/Sync.h>
#include <ace/Log_Msg.h>
#include <dds/DCPS/WaitSet.h>
#include "ReaderListenerAvailability.h"
#include "ReaderListenerRequest.h"

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

    // Block until Publisher completes
    DDS::StatusCondition_var condition2 = reader_availability->get_statuscondition();
    condition2->set_enabled_statuses(DDS::SUBSCRIPTION_MATCHED_STATUS);

    DDS::WaitSet_var ws = new DDS::WaitSet;
    ws->attach_condition(condition2);

    while (true) {
      DDS::SubscriptionMatchedStatus matches;
      if (reader_availability->get_subscription_matched_status(matches) != DDS::RETCODE_OK) {
        ACE_ERROR_RETURN((LM_ERROR,
                          ACE_TEXT("ERROR: %N:%l: main() -")
                          ACE_TEXT(" get_subscription_matched_status failed!\n")),
                         1);
      }

      if (matches.current_count == 0 && matches.total_count > 0) {
        break;
      }

      DDS::ConditionSeq conditions;
      DDS::Duration_t timeout = { 60, 0 };
      if (ws->wait(conditions, timeout) != DDS::RETCODE_OK) {
        ACE_ERROR_RETURN((LM_ERROR,
                          ACE_TEXT("ERROR: %N:%l: main() -")
                          ACE_TEXT(" wait failed!\n")),
                         1);
      }
    }

    ws->detach_condition(condition2);

    
    DDS::DataReader_var reader_request = model2.reader(Elements::DataReaders::flightRequestDR_FOP);    
    OpenDDS::Model::ReaderCondSync rcs2(reader_request, condition);
    DDS::DataReaderListener_var listener2(new ReaderListenerRequest(rcs2));
    reader_request->set_listener(listener2, OpenDDS::DCPS::DEFAULT_STATUS_MASK);
    // listener2->on_data_available(reader_request);

    
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