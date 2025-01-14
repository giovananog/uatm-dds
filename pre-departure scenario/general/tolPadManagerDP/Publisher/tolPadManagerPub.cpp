#include <string>
#include <vector>
#include <thread>
#include <chrono>
#include <unordered_set>
#include <dds/DCPS/transport/tcp/Tcp.h>
#include "../../model/UATMTraits.h"
#include "../Utils/functionsTPM.h"
#include <model/Sync.h>

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

int ACE_TMAIN(int argc, ACE_TCHAR **argv)
{
  try
  {
    // Set security for participant
    TheServiceParticipant->set_security(true); ///

    // Create the OpenDDS application model
    OpenDDS::Model::Application application(argc, argv);

    // Initialize the default UATM model for DCPS
    UATM::uatmDCPS::DefaultUATMType model(application, argc, argv);

    // Define a writer using the traffic availability writer
    using OpenDDS::Model::UATM::uatmDCPS::Elements;
    
    DDS::DataWriter_var writer = model.writer(Elements::DataWriters::tolPadAvailabilityDW_TP);

    // Narrow the writer to a specific data writer for availability info
    UATM::availabilityInfoDataWriter_var writer_var = UATM::availabilityInfoDataWriter::_narrow(writer.in());

    // If narrowing fails, output an error message and exit
    if (CORBA::is_nil(writer_var.in()))
    {
      ACE_ERROR_RETURN((LM_ERROR,
                        ACE_TEXT("(%P|%t) ERROR: %N:%l: main() -")
                            ACE_TEXT(" _narrow failed!\n")),
                       -1);
    }

    // Specify the file where tolPad data is read from
    std::string filename = "tolPadManagerDP/data/tolpads.txt";
    
    // Create a set to keep track of the tolPads already sent
    std::unordered_set<std::string> sent_tolpads;
    
    // Track the start time to manage the duration of the process
    auto startTime = std::chrono::steady_clock::now();
    double duration = 100.0; // Set the total duration to 100 seconds

    while (true)
    {
      // Read the tolPad data from the file
      std::vector<TolPad> tolPads = readTolPadsFromFile(filename);
      
      // Get the current time and calculate elapsed time
      auto currentTime = std::chrono::steady_clock::now();
      std::chrono::duration<double> elapsedTime = currentTime - startTime;

      // If elapsed time exceeds the duration, break the loop
      if (elapsedTime.count() >= duration)
      {
        break;
      }

      // Synchronize the writer access to ensure thread-safety
      OpenDDS::Model::WriterSync ws(writer);

      {
        // Iterate over each tolPad in the vector
        for (const auto &tolpad : tolPads)
        {
          // Check if the tolPad has not been sent or if it is available
          if (sent_tolpads.find(std::string(tolpad.resource_id)) == sent_tolpads.end() || tolpad.available == 1)
          {
            // Create an availability info object to send
            UATM::availabilityInfo bfr;
            bfr.resource_id = CORBA::string_dup(tolpad.resource_id.c_str());
            bfr.resource_type = "tolPad"; // Set the resource type
            bfr.available = tolpad.available; // Set availability status
            bfr.skyport_id = CORBA::string_dup(tolpad.skyport_id.c_str());
            bfr.availability_time = CORBA::string_dup(getCurrentTime().c_str()); // Set the current timestamp

            // Write the availability info using the writer
            DDS::ReturnCode_t error = writer_var->write(bfr, DDS::HANDLE_NIL);

            // Check for any write errors
            if (error != DDS::RETCODE_OK)
            {
              ACE_ERROR((LM_ERROR,
                         ACE_TEXT("(%P|%t) ERROR: %N:%l: main() -")
                             ACE_TEXT(" write returned %d!\n"),
                         error));
            }

            // Mark the tolPad as sent by adding its ID to the set
            sent_tolpads.insert(std::string(tolpad.resource_id));
            break; // Exit the loop after sending one tolPad
          }
        }

        // Sleep for 3 seconds before sending the next message
        std::this_thread::sleep_for(std::chrono::seconds(3));
      }
    }
  }
  catch (const CORBA::Exception &e)
  {
    // Catch CORBA exceptions and print the error message
    e._tao_print_exception("Exception caught in main():");
    return -1;
  }
  catch (const std::exception &ex)
  {
    // Catch standard exceptions and print the error message
    ACE_ERROR_RETURN((LM_ERROR,
                      ACE_TEXT("(%P|%t) ERROR: %N:%l: main() -")
                          ACE_TEXT(" Exception caught: %C\n"),
                      ex.what()),
                     -1);
  }
  
  // Return success
  return 0;
}
