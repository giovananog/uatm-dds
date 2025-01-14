#include <string>
#include <vector>
#include <thread>
#include <chrono>
#include <unordered_set>
#include <dds/DCPS/transport/tcp/Tcp.h>
#include "../../model/UATMTraits.h"
#include <model/Sync.h>
#include "../Utils/functionsSKM.h"

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
    
    // Initialize the OpenDDS application
    OpenDDS::Model::Application application(argc, argv);
    // Initialize the DefaultUATMType model with given arguments
    UATM::uatmDCPS::DefaultUATMType model(application, argc, argv);

    // Using elements from the OpenDDS UATM model for convenience
    using OpenDDS::Model::UATM::uatmDCPS::Elements;

    // Create a DataWriter for skyport availability
    DDS::DataWriter_var writer = model.writer(Elements::DataWriters::skyportAvailabilityDW_SKM);
    // Narrow the DataWriter to a specific type
    UATM::availabilityInfoDataWriter_var writer_var = UATM::availabilityInfoDataWriter::_narrow(writer.in());

    // Check if the narrowing succeeded
    if (CORBA::is_nil(writer_var.in()))
    {
      // Log an error if the narrowing failed
      ACE_ERROR_RETURN((LM_ERROR,
                        ACE_TEXT("(%P|%t) ERROR: %N:%l: main() -")
                            ACE_TEXT(" _narrow failed!\n")),
                       -1);
    }

    // Path to the file containing skyport data
    std::string filename = "skyportManagerDP/data/skyports.txt";
    // Set to track which skyports have already been sent
    std::unordered_set<std::string> sent_skyports;
    // Record the start time of the process
    auto startTime = std::chrono::steady_clock::now();
    double duration = 100.0;  // Duration for the process (in seconds)

    // Main loop: continue until the specified duration is reached
    while (true)
    {
      // Read skyport data from the file
      std::vector<Skyport> skyports = readSkyportsFromFile(filename);
      // Get the current time
      auto currentTime = std::chrono::steady_clock::now();
      // Calculate elapsed time since the start
      std::chrono::duration<double> elapsedTime = currentTime - startTime;

      // If the elapsed time exceeds the duration, break out of the loop
      if (elapsedTime.count() >= duration)
      {
        break;
      }

      // Synchronize writing to the DDS Writer
      OpenDDS::Model::WriterSync ws(writer);
      {
        // Iterate through each skyport and send availability information
        for (const auto &skyport : skyports)
        {
          // If the skyport has not been sent yet, or if it is available
          if (sent_skyports.find(std::string(skyport.resource_id)) == sent_skyports.end())
          {
            // Prepare the availability information
            UATM::availabilityInfo fa;
            fa.resource_id = CORBA::string_dup(skyport.resource_id.c_str());
            fa.resource_type = "skyport";  // Resource type is "skyport"
            fa.available = skyport.available;  // Availability status
            fa.skyport_id = CORBA::string_dup(skyport.resource_id.c_str());  // Skyport ID
            fa.availability_time = CORBA::string_dup(getCurrentTime().c_str());  // Current time of availability

            // Write the availability information to the DDS Writer
            DDS::ReturnCode_t error = writer_var->write(fa, DDS::HANDLE_NIL);

            // If writing failed, log the error
            if (error != DDS::RETCODE_OK)
            {
              ACE_ERROR((LM_ERROR,
                         ACE_TEXT("(%P|%t) ERROR: %N:%l: main() -")
                             ACE_TEXT(" write returned %d!\n"),
                         error));
            }

            // Mark this skyport as sent
            sent_skyports.insert(std::string(skyport.resource_id));

            // Break after sending one skyport's availability info
            break;
          }
        }
      }
      // Wait for 2 seconds before sending the next skyport's availability info
      std::this_thread::sleep_for(std::chrono::seconds(2));
    }
  }
  catch (const CORBA::Exception &e)
  {
    // Handle any CORBA-related exceptions
    e._tao_print_exception("Exception caught in main():");
    return -1;
  }
  catch (const std::exception &ex)
  {
    // Handle other standard exceptions
    ACE_ERROR_RETURN((LM_ERROR,
                      ACE_TEXT("(%P|%t) ERROR: %N:%l: main() -")
                          ACE_TEXT(" Exception caught: %C\n"),
                      ex.what()),
                     -1);
  }
  return 0;
}
