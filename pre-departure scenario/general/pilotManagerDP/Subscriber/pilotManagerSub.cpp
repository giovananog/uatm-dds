#ifdef ACE_AS_STATIC_LIBS
#include <dds/DCPS/transport/tcp/Tcp.h>
#endif
  
#include "../../model/UATMTraits.h"
#include <tools/modeling/codegen/model/NullReaderListener.h>
#include <model/Sync.h>
#include <ace/Log_Msg.h>
#include <dds/DCPS/WaitSet.h>
#include "ReaderListenerRequest.h"
#include "ReaderListenerRec.h"
#include "ReaderListenerAuth.h"


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

    DDS::DataReader_var reader_auth = model3.reader(Elements::DataReaders::flightAuthDR_PLM);
    OpenDDS::Model::ReaderCondSync rcs2(reader_auth, condition);
    DDS::DataReaderListener_var listener2(new ReaderListenerAuth(rcs2));
    reader_auth->set_listener(listener2, OpenDDS::DCPS::DEFAULT_STATUS_MASK);
    
    DDS::DataReader_var reader_rec = model2.reader(Elements::DataReaders::changeRecDR_PLM);
    OpenDDS::Model::ReaderCondSync rcs3(reader_rec, condition);
    DDS::DataReaderListener_var listener3(new ReaderListenerRec(rcs3));
    reader_rec->set_listener(listener3, OpenDDS::DCPS::DEFAULT_STATUS_MASK);

    DDS::DataReader_var reader_assign = model.reader(Elements::DataReaders::flightAssignDR_PLM);
    OpenDDS::Model::ReaderCondSync rcs(reader_assign, condition);
    DDS::DataReaderListener_var listener(new ReaderListenerRequest(rcs));
    reader_assign->set_listener(listener, OpenDDS::DCPS::DEFAULT_STATUS_MASK);

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