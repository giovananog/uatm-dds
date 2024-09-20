#ifdef ACE_AS_STATIC_LIBS
#include <dds/DCPS/transport/tcp/Tcp.h>
#endif
  
#include "../../model/UATMTraits.h"
#include <tools/modeling/codegen/model/NullReaderListener.h>

#include <model/Sync.h>
#include <ace/Log_Msg.h>

#include <dds/DCPS/WaitSet.h>
#include "ReaderListenerAvailability.h"
#include "ReaderListenerRoute.h"
#include "ReaderListenerWeather.h"

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