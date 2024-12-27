#ifdef ACE_AS_STATIC_LIBS
#include <dds/DCPS/transport/tcp/Tcp.h> // Includes the TCP transport layer for static linking
#endif

#include "../../model/UATMTraits.h" // Custom UATM model traits
#include <tools/modeling/codegen/model/NullReaderListener.h> // NullReaderListener (possibly for code generation purposes)
#include <model/Sync.h> // Synchronization utilities for thread safety
#include <ace/Log_Msg.h> // ACE logging utilities
#include <dds/DCPS/WaitSet.h> // DDS WaitSet for event handling
#include "ReaderListenerCoordination.h" // Listener for flight coordination data
#include "ReaderListenerAvailability.h" // Listener for availability data

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

// Main entry point
int ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
  {
    // Set security for participant
    TheServiceParticipant->set_security(true); ///

    // Initialize OpenDDS application with arguments
    OpenDDS::Model::Application application(argc, argv);

    // Create two default UATM models (for coordination and availability)
    UATM::uatmDCPS::DefaultUATMType model(application, argc, argv);
    UATM::uatmDCPS::DefaultUATMType model2(application, argc, argv);

    using OpenDDS::Model::UATM::uatmDCPS::Elements; // Reference to model elements

    // Create synchronization objects for thread-safe operations
    ACE_SYNCH_MUTEX lock; // Mutex to lock access
    ACE_Condition<ACE_SYNCH_MUTEX> condition(lock); // Condition variable tied to the mutex

    // Create a data reader for the flight coordination topic
    DDS::DataReader_var reader_coordination = model.reader(Elements::DataReaders::flightCoordDR_SKO);
    OpenDDS::Model::ReaderCondSync rcs(reader_coordination, condition); // Synchronization object for coordination
    DDS::DataReaderListener_var listener(new ReaderListenerCoordination(rcs)); // Create a listener for flight coordination
    reader_coordination->set_listener(listener, OpenDDS::DCPS::DEFAULT_STATUS_MASK); // Set listener for coordination data

    // Create a data reader for the availability topic
    DDS::DataReader_var reader_availability = model2.reader(Elements::DataReaders::availabilityDR_SKO);
    OpenDDS::Model::ReaderCondSync rcs2(reader_availability, condition); // Synchronization object for availability
    DDS::DataReaderListener_var listener2(new ReaderListenerAvailability(rcs2)); // Create a listener for availability data
    reader_availability->set_listener(listener2, OpenDDS::DCPS::DEFAULT_STATUS_MASK); // Set listener for availability data
  }
  catch (const CORBA::Exception &e)
  {
    // Handle CORBA exceptions
    e._tao_print_exception("Exception caught in main():");
    return -1; // Return with error code
  }
  catch (const std::exception &ex)
  {
    // Handle standard exceptions
    ACE_ERROR_RETURN((LM_ERROR,
                      ACE_TEXT("(%P|%t) ERROR: %N:%l: main() -")
                          ACE_TEXT(" Exception caught: %C\n"),
                      ex.what()),
                     -1); // Return with error code
  }
  return 0; // Success
}
