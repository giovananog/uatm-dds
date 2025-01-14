#include "../../model/UATMTraits.h"
#include <tools/modeling/codegen/model/NullReaderListener.h>
#include <model/Sync.h>
#include <ace/Log_Msg.h>
#include <dds/DCPS/WaitSet.h>
#include "ReaderListenerRequestANSP.h"

// Security configurations
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

int ACE_TMAIN(int argc, ACE_TCHAR* argv[])
{
  try {

    // Set security for participant
    TheServiceParticipant->set_security(true); 

    // Initialize the application and model for UATM
    OpenDDS::Model::Application application(argc, argv);  

    // Set up UATM model and acces model elements
    UATM::uatmDCPS::DefaultUATMType model(application, argc, argv);  
    using OpenDDS::Model::UATM::uatmDCPS::Elements;  

    // Create a synchronization mechanism
    ACE_SYNCH_MUTEX lock;  
    ACE_Condition<ACE_SYNCH_MUTEX> condition(lock);  

    // Initialize the data reader for receiving route data from ANSP (Air Navigation Service Provider)
    DDS::DataReader_var reader_availability = model.reader(Elements::DataReaders::routeDataDR_ANSP);

    // Set up synchronization for reading data from the DDS
    OpenDDS::Model::ReaderCondSync rcs(reader_availability, condition);  

    // Set up a listener for handling incoming data on the reader
    DDS::DataReaderListener_var listener(new ReaderListenerRequest(rcs));

    // Attach the listener to the data reader and specify the types of events it should listen for
    reader_availability->set_listener(listener, OpenDDS::DCPS::DEFAULT_STATUS_MASK);

  } catch (const CORBA::Exception& e) {
    // Handle CORBA-specific exceptions
    e._tao_print_exception("Exception caught in main():");
    return -1;

  } catch (const std::exception& ex) {
    // Handle standard exceptions
    ACE_ERROR_RETURN((LM_ERROR,
                      ACE_TEXT("(%P|%t) ERROR: %N:%l: main() -")
                      ACE_TEXT(" Exception caught: %C\n"),
                      ex.what()),
                     -1);
  }

  return 0;  // Return success
}
