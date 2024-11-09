#ifdef ACE_AS_STATIC_LIBS
#include <dds/DCPS/transport/tcp/Tcp.h> // Include TCP transport if using static libraries for ACE
#endif
  
#include "../../model/UATMTraits.h" // Custom UATM model traits header
#include <tools/modeling/codegen/model/NullReaderListener.h> // Null listener for code generation
#include <model/Sync.h> // Synchronization utilities
#include <ace/Log_Msg.h> // ACE logging utilities
#include <dds/DCPS/WaitSet.h> // DDS WaitSet for event management
#include "ReaderListenerRequest.h" // Listener for flight request data
#include "ReaderListenerRec.h" // Listener for record change data
#include "ReaderListenerAuth.h" // Listener for flight authorization data

// Main entry point of the application
int ACE_TMAIN(int argc, ACE_TCHAR* argv[])
{
  try {
    // Initialize OpenDDS application
    OpenDDS::Model::Application application(argc, argv);

    // Create instances of the UATM data model
    UATM::uatmDCPS::DefaultUATMType model(application, argc, argv);
    UATM::uatmDCPS::DefaultUATMType model2(application, argc, argv);
    UATM::uatmDCPS::DefaultUATMType model3(application, argc, argv);

    // Aliasing the model elements for ease of use
    using OpenDDS::Model::UATM::uatmDCPS::Elements;

    // Define a synchronization lock and condition variable
    ACE_SYNCH_MUTEX lock; // Mutex for thread synchronization
    ACE_Condition<ACE_SYNCH_MUTEX> condition(lock); // Condition variable based on the mutex

    // Setup and configure DataReader for flight authorization (model3)
    DDS::DataReader_var reader_auth = model3.reader(Elements::DataReaders::flightAuthDR_PLM);
    OpenDDS::Model::ReaderCondSync rcs2(reader_auth, condition); // Synchronization for reader
    DDS::DataReaderListener_var listener2(new ReaderListenerAuth(rcs2)); // Create listener for flight authorization
    reader_auth->set_listener(listener2, OpenDDS::DCPS::DEFAULT_STATUS_MASK); // Set the listener on the reader

    // Setup and configure DataReader for record changes (model2)
    DDS::DataReader_var reader_rec = model2.reader(Elements::DataReaders::changeRecDR_PLM);
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
