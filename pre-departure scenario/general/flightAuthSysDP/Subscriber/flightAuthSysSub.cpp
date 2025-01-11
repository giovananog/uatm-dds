#include "../../model/UATMTraits.h" 
#include <tools/modeling/codegen/model/NullReaderListener.h> 
#include <model/Sync.h> 
#include <ace/Log_Msg.h> 
#include <dds/DCPS/WaitSet.h> 
#include "ReaderListenerRequest.h" 

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
    TheServiceParticipant->set_security(true); ///
    
    // Create the application object and initialize it with command-line arguments
    OpenDDS::Model::Application application(argc, argv);

    // Initialize the UATM DCPS (Data-Centric Publish-Subscribe) model
    UATM::uatmDCPS::DefaultUATMType model(application, argc, argv);

    // Alias for the UATM model elements
    using OpenDDS::Model::UATM::uatmDCPS::Elements;

    // Mutex and condition variable for synchronization
    ACE_SYNCH_MUTEX lock;
    ACE_Condition<ACE_SYNCH_MUTEX> condition(lock);

    // Retrieve the data reader for the flight request data (from model)
    DDS::DataReader_var reader_request = model.reader(Elements::DataReaders::flightRequestDR_FAS);

    // Create a synchronization object that connects the data reader and condition
    OpenDDS::Model::ReaderCondSync rcs(reader_request, condition);

    // Create a listener for the data reader, using the synchronization object
    DDS::DataReaderListener_var listener(new ReaderListenerRequest(rcs));

    // Set the listener for the data reader with the default status mask
    reader_request->set_listener(listener, OpenDDS::DCPS::DEFAULT_STATUS_MASK);

  } catch (const CORBA::Exception& e) {
    // Catch and print CORBA exceptions, then exit the program with an error code
    e._tao_print_exception("Exception caught in main():");
    return -1;

  } catch( const std::exception& ex) {
    // Catch and log standard exceptions, then exit the program with an error code
    ACE_ERROR_RETURN((LM_ERROR,
                      ACE_TEXT("(%P|%t) ERROR: %N:%l: main() -")
                      ACE_TEXT(" Exception caught: %C\n"),
                      ex.what()),
                     -1);
  }
  return 0; // Return success if no exceptions were thrown
}
