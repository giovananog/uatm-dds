#include "../utils/functions.h"
#include "../../model/UATMTraits.h"  
#include <tools/modeling/codegen/model/NullReaderListener.h>  
#include <model/Sync.h>  
#include <dds/DCPS/WaitSet.h>  
#include "./ReaderListenerRequest.h"  

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
    
    // Create an application instance with arguments
    OpenDDS::Model::Application application(argc, argv);

    // Create the UATM model using the application instance
    UATM::uatmDCPS::DefaultUATMType model(application, argc, argv);

    // Define elements to be used from the UATM model
    using OpenDDS::Model::UATM::uatmDCPS::Elements;

    // Create a synchronization mutex and condition variable
    ACE_SYNCH_MUTEX lock;
    ACE_Condition<ACE_SYNCH_MUTEX> condition(lock);

    // Create a data reader for the flight assignment data 
    DDS::DataReader_var reader_assign = model.reader(Elements::DataReaders::flightAssignDR_EV);

    // Create a ReaderCondSync object to synchronize reading operations
    OpenDDS::Model::ReaderCondSync rcs(reader_assign, condition);

    // Create a custom listener for handling data availability events
    DDS::DataReaderListener_var listener(new ReaderListenerRequest(rcs));

    // Set the listener for the reader and specify the status mask for the events to listen to
    reader_assign->set_listener(listener, OpenDDS::DCPS::DEFAULT_STATUS_MASK);

  } catch (const CORBA::Exception& e) {
    // Catch any CORBA exceptions and print the error message
    e._tao_print_exception("Exception caught in main():");
    return -1;  

  } catch (const std::exception& ex) {
    // Catch any standard exceptions and print the error message
    ACE_ERROR_RETURN((LM_ERROR,
                      ACE_TEXT("(%P|%t) ERROR: %N:%l: main() -")
                      ACE_TEXT(" Exception caught: %C\n"),
                      ex.what()),
                     -1);  
  }

  return 0;  
}
