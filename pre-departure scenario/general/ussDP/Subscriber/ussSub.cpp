#ifdef ACE_AS_STATIC_LIBS
#include <dds/DCPS/transport/tcp/Tcp.h>
#endif
  
#include "../../model/UATMTraits.h"
#include <tools/modeling/codegen/model/NullReaderListener.h>

#include <model/Sync.h>
#include <ace/Log_Msg.h>

#include <dds/DCPS/WaitSet.h>
#include "ReaderListenerRequest.h"

// Main entry point of the application
int ACE_TMAIN(int argc, ACE_TCHAR* argv[])
{
  try {
    // Initialize the application with the command-line arguments
    OpenDDS::Model::Application application(argc, argv);
    
    // Create a model for the UATM type (assumed to be some model related to air traffic management)
    UATM::uatmDCPS::DefaultUATMType model(application, argc, argv);

    // Use the model elements for data readers (this part shows a specific model for air traffic data)
    using OpenDDS::Model::UATM::uatmDCPS::Elements;

    // Create a synchronization lock and condition variable
    ACE_SYNCH_MUTEX lock;
    ACE_Condition<ACE_SYNCH_MUTEX> condition(lock);

    // Retrieve the DataReader for route data from the model
    DDS::DataReader_var reader_availability = model.reader(Elements::DataReaders::routeDataDR_USS);

    // Create a ReaderCondSync object for managing synchronization between the reader and condition variable
    OpenDDS::Model::ReaderCondSync rcs(reader_availability, condition);

    // Create the listener object that will handle the data available event for the DataReader
    DDS::DataReaderListener_var listener(new ReaderListenerRequest(rcs));

    // Set the listener for the DataReader, which will notify the listener when data is available
    reader_availability->set_listener(listener, OpenDDS::DCPS::DEFAULT_STATUS_MASK);

  } catch (const CORBA::Exception& e) {
    // Handle any CORBA-specific exceptions
    e._tao_print_exception("Exception caught in main():");
    return -1;

  } catch( const std::exception& ex) {
    // Handle general C++ exceptions
    ACE_ERROR_RETURN((LM_ERROR,
                      ACE_TEXT("(%P|%t) ERROR: %N:%l: main() -")
                      ACE_TEXT(" Exception caught: %C\n"),
                      ex.what()),
                     -1);
  }
  return 0;
}
