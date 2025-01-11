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
    
    // Initialize OpenDDS application with command-line arguments
    OpenDDS::Model::Application application(argc, argv);

    // Initialize the UATM model using the application instance and command-line arguments
    UATM::uatmDCPS::DefaultUATMType model(application, argc, argv);

    // Alias for the UATM model elements
    using OpenDDS::Model::UATM::uatmDCPS::Elements;
    
    // Define a synchronization mutex and condition variable
    ACE_SYNCH_MUTEX lock;  // Mutex to protect shared resources
    ACE_Condition<ACE_SYNCH_MUTEX> condition(lock);  // Condition variable to wait and notify threads

    // Retrieve the DataReader for tolPadAssign from the model using the appropriate element
    DDS::DataReader_var reader_request = model.reader(Elements::DataReaders::tolPadAssignDR_TP);

    // Initialize the ReaderCondSync object, which coordinates the synchronization of the DataReader and the condition variable
    OpenDDS::Model::ReaderCondSync rcs(reader_request, condition);

    // Create a new instance of ReaderListenerRequest and assign it as the listener to the DataReader
    DDS::DataReaderListener_var listener(new ReaderListenerRequest(rcs));

    // Set the listener for the reader_request, specifying the status mask for the listener
    reader_request->set_listener(listener, OpenDDS::DCPS::DEFAULT_STATUS_MASK);

  } catch (const CORBA::Exception& e) {
    // Handle CORBA exceptions (for OpenDDS related errors)
    e._tao_print_exception("Exception caught in main():");
    return -1;  // Return -1 to indicate an error

  } catch( const std::exception& ex) {
    // Handle standard exceptions (e.g., exceptions thrown by the C++ Standard Library)
    ACE_ERROR_RETURN((LM_ERROR,
                      ACE_TEXT("(%P|%t) ERROR: %N:%l: main() -")
                      ACE_TEXT(" Exception caught: %C\n"),
                      ex.what()),
                     -1);  // Return -1 to indicate an error
  }
  return 0;  // Return 0 to indicate successful execution
}
