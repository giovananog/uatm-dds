#include "../../model/UATMTraits.h" 
#include <tools/modeling/codegen/model/NullReaderListener.h> 
#include <model/Sync.h> 
#include <ace/Log_Msg.h> 
#include <dds/DCPS/WaitSet.h> 
#include "ReaderListenerRequest.h" // Listener for flight request data
#include "ReaderListenerRec.h" // Listener for record change data
#include "ReaderListenerAuth.h" // Listener for flight authorization data

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

    // Initialize OpenDDS application
    OpenDDS::Model::Application application(argc, argv);

    // Create instances of the UATM data model
    UATM::uatmDCPS::DefaultUATMType model(application, argc, argv);

    // Aliasing the model elements for ease of use
    using OpenDDS::Model::UATM::uatmDCPS::Elements;

    // Define a synchronization lock and condition variable
    ACE_SYNCH_MUTEX lock; // Mutex for thread synchronization
    ACE_Condition<ACE_SYNCH_MUTEX> condition(lock); // Condition variable based on the mutex

    // Setup and configure DataReader for flight authorization (model3)
    DDS::DataReader_var reader_auth = model.reader(Elements::DataReaders::flightAuthDR_PLM);
    OpenDDS::Model::ReaderCondSync rcs2(reader_auth, condition); // Synchronization for reader
    DDS::DataReaderListener_var listener2(new ReaderListenerAuth(rcs2)); // Create listener for flight authorization
    reader_auth->set_listener(listener2, OpenDDS::DCPS::DEFAULT_STATUS_MASK); // Set the listener on the reader

    // Setup and configure DataReader for record changes (model2)
    DDS::DataReader_var reader_rec = model.reader(Elements::DataReaders::changeRecDR_PLM);
    OpenDDS::Model::ReaderCondSync rcs3(reader_rec, condition); // Synchronization for reader
    DDS::DataReaderListener_var listener3(new ReaderListenerRec(rcs3)); // Create listener for record changes
    reader_rec->set_listener(listener3, OpenDDS::DCPS::DEFAULT_STATUS_MASK); // Set the listener on the reader

    // Setup and configure DataReader for flight assignments (model)
    DDS::DataReader_var reader_assign = model.reader(Elements::DataReaders::flightAssignDR_PLM);
    OpenDDS::Model::ReaderCondSync rcs(reader_assign, condition); // Synchronization for reader
    DDS::DataReaderListener_var listener(new ReaderListenerRequest(rcs)); // Create listener for flight assignments
    reader_assign->set_listener(listener, OpenDDS::DCPS::DEFAULT_STATUS_MASK); // Set the listener on the reader

  } catch (const CORBA::Exception& e) {
    // Catch and handle CORBA exceptions
    e._tao_print_exception("Exception caught in main():");
    return -1; // Return error code

  } catch( const std::exception& ex) {
    // Catch and handle standard exceptions
    ACE_ERROR_RETURN((LM_ERROR,
                      ACE_TEXT("(%P|%t) ERROR: %N:%l: main() -")
                      ACE_TEXT(" Exception caught: %C\n"),
                      ex.what()),
                     -1); // Return error code
  }

  return 0; // Return success
}
