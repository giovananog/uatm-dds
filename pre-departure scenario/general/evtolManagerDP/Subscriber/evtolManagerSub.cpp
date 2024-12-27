#ifdef ACE_AS_STATIC_LIBS
#include <dds/DCPS/transport/tcp/Tcp.h>  // Include TCP transport header for DDS if ACE is built with static libraries
#endif
  
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <thread>
#include <chrono>
#include <unordered_set>
#include "../utils/functions.h"

#include "../../model/UATMTraits.h"  // Include UATM traits for the Urban Air Traffic Management model
#include <tools/modeling/codegen/model/NullReaderListener.h>  // Header for a null reader listener

#include <model/Sync.h>  // Synchronization model for DDS
#include <ace/Log_Msg.h>  // ACE logging functionality
#include <dds/DCPS/WaitSet.h>  // DDS WaitSet for synchronization of read events
#include "./ReaderListenerRequest.h"  // Header for the custom reader listener


// Main entry point for the application
int ACE_TMAIN(int argc, ACE_TCHAR* argv[])
{
  try {
    // Set security for participant
    TheServiceParticipant->set_security(true); ///
    
    // Create an application instance with arguments
    OpenDDS::Model::Application application(argc, argv);

    // Create the UATM model using the application instance
    UATM::uatmDCPS::DefaultUATMType model(application, argc, argv);

    // Define elements to be used from the UATM model
    using OpenDDS::Model::UATM::uatmDCPS::Elements;

    // add property qos policy on domain participant
    DDS::DomainParticipant_var participant = model.participant(Elements::Participants::evtolManagerDP);

    // Create a synchronization mutex and condition variable
    ACE_SYNCH_MUTEX lock;
    ACE_Condition<ACE_SYNCH_MUTEX> condition(lock);

    // Create a data reader for the flight assignment data (flightAssignDR_EV)
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
    return -1;  // Return -1 indicating an error occurred

  } catch (const std::exception& ex) {
    // Catch any standard exceptions and print the error message
    ACE_ERROR_RETURN((LM_ERROR,
                      ACE_TEXT("(%P|%t) ERROR: %N:%l: main() -")
                      ACE_TEXT(" Exception caught: %C\n"),
                      ex.what()),
                     -1);  // Return -1 indicating an error occurred
  }

  return 0;  // Return 0 indicating successful execution
}
