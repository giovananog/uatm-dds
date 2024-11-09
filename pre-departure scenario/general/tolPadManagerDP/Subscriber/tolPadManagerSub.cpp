#ifdef ACE_AS_STATIC_LIBS
#include <dds/DCPS/transport/tcp/Tcp.h>  // Include TCP transport for static libraries, if needed
#endif
  
#include "../../model/UATMTraits.h"  // Include UATM traits from a custom model
#include <tools/modeling/codegen/model/NullReaderListener.h>  // Include a NullReaderListener, probably for code generation purposes

#include <model/Sync.h>  // Include synchronization utilities for thread safety
#include <ace/Log_Msg.h>  // Include ACE logging utilities for error messages and logging

#include <dds/DCPS/WaitSet.h>  // Include DDS WaitSet to handle events
#include "ReaderListenerRequest.h"  // Include the header for the ReaderListenerRequest class

// The main entry point of the application
int ACE_TMAIN(int argc, ACE_TCHAR* argv[])
{
  try {
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
