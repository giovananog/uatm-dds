#ifdef ACE_AS_STATIC_LIBS
#include <dds/DCPS/transport/tcp/Tcp.h> // Include TCP transport header for static linking
#endif

#include "../../model/UATMTraits.h" // Custom UATM model traits header
#include <tools/modeling/codegen/model/NullReaderListener.h> // Null listener for code generation
#include <model/Sync.h> // Synchronization utilities
#include <ace/Log_Msg.h> // ACE logging utilities
#include <dds/DCPS/WaitSet.h> // DDS WaitSet for managing events
#include "ReaderListenerRequest.h" // Reader listener for flight request data

// Main entry point for the application
int ACE_TMAIN(int argc, ACE_TCHAR* argv[])
{
  try {
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
