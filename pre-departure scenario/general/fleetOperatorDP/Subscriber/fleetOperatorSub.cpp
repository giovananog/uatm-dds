// Including necessary headers for the application and transport layer
#ifdef ACE_AS_STATIC_LIBS
#include <dds/DCPS/transport/tcp/Tcp.h> // For TCP transport
#endif

// Including model and listener headers
#include "../../model/UATMTraits.h"                          // Custom UATM model traits
#include <tools/modeling/codegen/model/NullReaderListener.h> // Placeholder for a Null Reader Listener

#include <model/Sync.h>  // Synchronization primitives
#include <ace/Log_Msg.h> // ACE logging utilities

#include <dds/DCPS/WaitSet.h>           // For managing wait sets in DDS
#include "ReaderListenerAvailability.h" // Listener for availability data
#include "ReaderListenerRoute.h"        // Listener for flight route data////.. çç
#include "ReaderListenerWeather.h"      // Listener for weather data
#include "ReaderListenerRequest.h"      // Listener for flight request data
#include "ReaderListenerRec.h"          // Listener for recommendation data
#include "ReaderListenerAuth.h"         // Listener for authentication data
#include <dds/DCPS/security/framework/Properties.h>

#if OPENDDS_CONFIG_SECURITY
#  include <dds/DCPS/security/framework/Properties.h>
#endif
#include <dds/DCPS/StaticIncludes.h>
#if OPENDDS_DO_MANUAL_STATIC_INCLUDES
#  ifndef OPENDDS_SAFETY_PROFILE
#    include <dds/DCPS/transport/udp/Udp.h>
#    include <dds/DCPS/transport/multicast/Multicast.h>
#    include <dds/DCPS/RTPS/RtpsDiscovery.h>
#    include <dds/DCPS/transport/shmem/Shmem.h>
#    if OPENDDS_CONFIG_SECURITY
#      include <dds/DCPS/security/BuiltInPlugins.h>
#    endif
#  endif
#  include <dds/DCPS/transport/rtps_udp/RtpsUdp.h>
#endif
#include <ace/Log_Msg.h>


// Main entry point for the application
int ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
  {
    // Set security for participant
    TheServiceParticipant->set_security(true); ///

    // Initialize the application model with arguments
    OpenDDS::Model::Application application(argc, argv);

    // Create multiple instances of DefaultUATMType models for different data types
    UATM::uatmDCPS::DefaultUATMType model(application, argc, argv);
    UATM::uatmDCPS::DefaultUATMType model2(application, argc, argv);
    UATM::uatmDCPS::DefaultUATMType model3(application, argc, argv);
    UATM::uatmDCPS::DefaultUATMType model4(application, argc, argv);
    UATM::uatmDCPS::DefaultUATMType model5(application, argc, argv);
    UATM::uatmDCPS::DefaultUATMType model6(application, argc, argv);

    // Define the elements for reading specific data types
    using OpenDDS::Model::UATM::uatmDCPS::Elements;

    OpenDDS::DCPS::security_debug.access_warn = true;
    OpenDDS::DCPS::security_debug.set_debug_level(100); 
    OpenDDS::DCPS::security_debug.parse_flags(ACE_TEXT("all"));

    // Initialize synchronization primitives
    ACE_SYNCH_MUTEX lock;
    ACE_Condition<ACE_SYNCH_MUTEX> condition(lock);

    // Setup and configure the listener for the flight request data reader
    DDS::DataReader_var reader_request = model4.reader(Elements::DataReaders::flightRequestDR_FOP);
    OpenDDS::Model::ReaderCondSync rcs4(reader_request, condition);
    DDS::DataReaderListener_var listener4(new ReaderListenerRequest(rcs4));
    reader_request->set_listener(listener4, OpenDDS::DCPS::DEFAULT_STATUS_MASK);

    // Setup and configure the listener for the availability data reader
    DDS::DataReader_var reader_availability = model.reader(Elements::DataReaders::availabilityDR_FOP);
    OpenDDS::Model::ReaderCondSync rcs(reader_availability, condition);
    DDS::DataReaderListener_var listener(new ReaderListenerAvailability(rcs));
    reader_availability->set_listener(listener, OpenDDS::DCPS::DEFAULT_STATUS_MASK);

    // Setup and configure the listener for the flight route data reader
    DDS::DataReader_var reader_routes = model2.reader(Elements::DataReaders::FlightRoutesDR_FOP);
    OpenDDS::Model::ReaderCondSync rcs2(reader_routes, condition);
    DDS::DataReaderListener_var listener2(new ReaderListenerRoute(rcs2));
    reader_routes->set_listener(listener2, OpenDDS::DCPS::DEFAULT_STATUS_MASK);

    // Setup and configure the listener for the flight authentication data reader
    DDS::DataReader_var reader_auth = model6.reader(Elements::DataReaders::flightAuthDR_FOP);
    OpenDDS::Model::ReaderCondSync rcs6(reader_auth, condition);
    DDS::DataReaderListener_var listener6(new ReaderListenerAuth(rcs6));
    reader_auth->set_listener(listener6, OpenDDS::DCPS::DEFAULT_STATUS_MASK);

    // Setup and configure the listener for the recommendation data reader
    DDS::DataReader_var reader_rec = model5.reader(Elements::DataReaders::recommendationDR_FOP);
    OpenDDS::Model::ReaderCondSync rcs5(reader_rec, condition);
    DDS::DataReaderListener_var listener5(new ReaderListenerRec(rcs5));
    reader_rec->set_listener(listener5, OpenDDS::DCPS::DEFAULT_STATUS_MASK);

    // Setup and configure the listener for the weather data reader
    DDS::DataReader_var reader_weather = model3.reader(Elements::DataReaders::weatherDR_FOP);
    OpenDDS::Model::ReaderCondSync rcs3(reader_weather, condition);
    DDS::DataReaderListener_var listener3(new ReaderListenerWeather(rcs3));
    reader_weather->set_listener(listener3, OpenDDS::DCPS::DEFAULT_STATUS_MASK);
  }
  catch (const CORBA::Exception &e)
  {
    // Handle CORBA exceptions
    e._tao_print_exception("Exception caught in main():");
    return -1;
  }
  catch (const std::exception &ex)
  {
    // Handle standard exceptions
    ACE_ERROR_RETURN((LM_ERROR,
                      ACE_TEXT("(%P|%t) ERROR: %N:%l: main() -")
                          ACE_TEXT(" Exception caught: %C\n"),
                      ex.what()),
                     -1);
  }
  return 0;
}
