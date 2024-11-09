#ifdef ACE_AS_STATIC_LIBS
#include <dds/DCPS/transport/tcp/Tcp.h>
#endif
  
#include "../../model/UATMTraits.h"
#include <tools/modeling/codegen/model/NullReaderListener.h>

#include <model/Sync.h>
#include <ace/Log_Msg.h>

#include <dds/DCPS/WaitSet.h>
#include "ReaderListenerRequest.h"
#include "ReaderListenerAvailability.h"
#include "ReaderListenerRest.h"
#include "ReaderListenerFlows.h"
#include "ReaderListenerWeather.h"

// Main function
int ACE_TMAIN(int argc, ACE_TCHAR* argv[])
{
  try {
    // Initialize the application and models for UATM
    OpenDDS::Model::Application application(argc, argv);
    UATM::uatmDCPS::DefaultUATMType model(application, argc, argv);
    UATM::uatmDCPS::DefaultUATMType model2(application, argc, argv);
    UATM::uatmDCPS::DefaultUATMType model3(application, argc, argv);
    UATM::uatmDCPS::DefaultUATMType model4(application, argc, argv);
    UATM::uatmDCPS::DefaultUATMType model5(application, argc, argv);

    // Define a shorthand for the Elements type in UATM
    using OpenDDS::Model::UATM::uatmDCPS::Elements;

    // Create a synchronization mechanism
    ACE_SYNCH_MUTEX lock;
    ACE_Condition<ACE_SYNCH_MUTEX> condition(lock);

    // Setup DataReaders and their listeners for different topics

    // Airspace rest data
    DDS::DataReader_var reader_rest = model4.reader(Elements::DataReaders::airspaceRestDR_UASP);
    OpenDDS::Model::ReaderCondSync rcs2(reader_rest, condition);
    DDS::DataReaderListener_var listener2(new ReaderListenerRest(rcs2));
    reader_rest->set_listener(listener2, OpenDDS::DCPS::DEFAULT_STATUS_MASK);

    // Flight request data
    DDS::DataReader_var reader_request = model.reader(Elements::DataReaders::flightRequestDR_UASP);
    OpenDDS::Model::ReaderCondSync rcs5(reader_request, condition);
    DDS::DataReaderListener_var listener5(new ReaderListenerRequest(rcs5));
    reader_request->set_listener(listener5, OpenDDS::DCPS::DEFAULT_STATUS_MASK);

    // Traffic flows data
    DDS::DataReader_var reader_flows = model3.reader(Elements::DataReaders::trafficFlowsDR_UASP);
    OpenDDS::Model::ReaderCondSync rcs(reader_flows, condition);
    DDS::DataReaderListener_var listener(new ReaderListenerFlows(rcs));
    reader_flows->set_listener(listener, OpenDDS::DCPS::DEFAULT_STATUS_MASK);

    // Weather information data
    DDS::DataReader_var reader_weather = model5.reader(Elements::DataReaders::weatherInfoDR_UASP);
    OpenDDS::Model::ReaderCondSync rcs3(reader_weather, condition);
    DDS::DataReaderListener_var listener3(new ReaderListenerWeather(rcs3));
    reader_weather->set_listener(listener3, OpenDDS::DCPS::DEFAULT_STATUS_MASK);

    // Pad availability data
    DDS::DataReader_var reader_availability = model2.reader(Elements::DataReaders::tolPadAvailabilityDR_UASP);
    OpenDDS::Model::ReaderCondSync rcs4(reader_availability, condition);
    DDS::DataReaderListener_var listener4(new ReaderListenerAvailability(rcs4));
    reader_availability->set_listener(listener4, OpenDDS::DCPS::DEFAULT_STATUS_MASK);

  } catch (const CORBA::Exception& e) {
    // Exception handling for CORBA exceptions
    e._tao_print_exception("Exception caught in main():");
    return -1;

  } catch( const std::exception& ex) {
    // Exception handling for standard C++ exceptions
    ACE_ERROR_RETURN((LM_ERROR,
                      ACE_TEXT("(%P|%t) ERROR: %N:%l: main() -")
                      ACE_TEXT(" Exception caught: %C\n"),
                      ex.what()),
                     -1);
  }
  return 0;
}
