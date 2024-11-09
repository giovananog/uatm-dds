#ifdef ACE_AS_STATIC_LIBS
#include <dds/DCPS/transport/tcp/Tcp.h>
#endif

#include "../../model/UATMTraits.h"  // UATM traits for Urban Air Traffic Management model
#include <tools/modeling/codegen/model/NullReaderListener.h>  // Listener class for handling data reading

#include <model/Sync.h>  // Synchronization model for DDS
#include <ace/Log_Msg.h>  // ACE logging functionality

#include <dds/DCPS/WaitSet.h>  // DDS WaitSet for handling the synchronization of read events
#include "ReaderListenerRequest.h"  // Listener implementation to process incoming data

int ACE_TMAIN(int argc, ACE_TCHAR* argv[])
{
  try {
    // Initialize the application and model for UATM
    OpenDDS::Model::Application application(argc, argv);  // Initialize OpenDDS application with command-line arguments
    UATM::uatmDCPS::DefaultUATMType model(application, argc, argv);  // Set up UATM model

    using OpenDDS::Model::UATM::uatmDCPS::Elements;  // Access UATM model elements

    // Create a synchronization mechanism
    ACE_SYNCH_MUTEX lock;  // Define a mutex for synchronization
    ACE_Condition<ACE_SYNCH_MUTEX> condition(lock);  // Create a condition variable based on the mutex

    // Initialize the data reader for receiving route data from ANSP (Air Navigation Service Provider)
    DDS::DataReader_var reader_availability = model.reader(Elements::DataReaders::routeDataDR_ANSP);

    // Set up synchronization for reading data from the DDS
    OpenDDS::Model::ReaderCondSync rcs(reader_availability, condition);  // Create a condition-based reader synchronization object

    // Set up a listener for handling incoming data on the reader
    DDS::DataReaderListener_var listener(new ReaderListenerRequest(rcs));  // Create a custom listener object

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
