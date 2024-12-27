// Including necessary headers for the application and transport layer
#ifdef ACE_AS_STATIC_LIBS
#include <dds/DCPS/transport/tcp/Tcp.h> // For TCP transport
#endif

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <thread>
#include <chrono>
#include <dds/DCPS/transport/tcp/Tcp.h>
#include "../../model/UATMTraits.h"
#include <model/Sync.h>
#include <unordered_set>
#include "../utils/functions.h"
#include <dds/DCPS/WaitSet.h>           // For managing wait sets in DDS

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

    // Initialize OpenDDS application
    OpenDDS::Model::Application application(argc, argv);
    UATM::uatmDCPS::DefaultUATMType model(application, argc, argv);

    // Define elements and obtain DataWriter for eVTOL availability
    using OpenDDS::Model::UATM::uatmDCPS::Elements;

    DDS::DataWriter_var writer = model.writer(Elements::DataWriters::evtolAvailabilityDW_EV);

    // Narrow DataWriter to a specific writer type
    UATM::availabilityInfoDataWriter_var writer_var = UATM::availabilityInfoDataWriter::_narrow(writer.in());

    // Check if narrowing succeeded
    if (CORBA::is_nil(writer_var.in()))
    {
      ACE_ERROR_RETURN((LM_ERROR,
                        ACE_TEXT("(%P|%t) ERROR: %N:%l: main() -")
                            ACE_TEXT(" _narrow failed!\n")),
                       -1);
    }

    // File storing eVTOL data and set to track sent eVTOLs
    std::string filename = "evtolManagerDP/data/evtols.txt";
    std::unordered_set<std::string> sent_evtols;

    // Set start time and duration for main loop
    auto startTime = std::chrono::steady_clock::now();
    double duration = 100.0;

    // Main loop to process eVTOL availability data
    while (true)
    {
      // Read eVTOL data from file
      std::vector<EVTOL> evtols = readEVTOLsFromFile(filename);
      auto currentTime = std::chrono::steady_clock::now();
      std::chrono::duration<double> elapsedTime = currentTime - startTime;

      // If duration is exceeded, reset file content and exit loop
      if (elapsedTime.count() >= duration)
      {
        std::ofstream file(filename, std::ios::trunc);
        std::string data =
            "evtol_id=eVTOL-1,skyport_id=Skyport-1,available=1\n"
            "evtol_id=eVTOL-2,skyport_id=Skyport-1,available=1\n"
            "evtol_id=eVTOL-3,skyport_id=Skyport-2,available=1\n";
        file << data;
        file.close();
        break;
      }

      // Acquire lock and process each eVTOL's availability
      OpenDDS::Model::WriterSync ws(writer);
      {
        for (const auto &evtol : evtols)
        {
          // Send availability data only if not sent before or availability has changed
          if (sent_evtols.find(std::string(evtol.evtol_id)) == sent_evtols.end() || evtol.available == 1)
          {
            UATM::availabilityInfo bfr;

            // Populate availability information for the eVTOL
            bfr.resource_id = CORBA::string_dup(evtol.evtol_id.c_str());
            bfr.resource_type = "evtol";
            bfr.available = evtol.available;
            bfr.skyport_id = CORBA::string_dup(evtol.skyport_id.c_str());
            bfr.availability_time = CORBA::string_dup(getCurrentTime().c_str());

            // Write data to DDS
            DDS::ReturnCode_t error = writer_var->write(bfr, DDS::HANDLE_NIL);

            // Error handling for failed write
            if (error != DDS::RETCODE_OK)
            {
              ACE_ERROR((LM_ERROR,
                         ACE_TEXT("(%P|%t) ERROR: %N:%l: main() -")
                             ACE_TEXT(" write returned %d!\n"),
                         error));
            }

            // Mark eVTOL as sent
            sent_evtols.insert(std::string(evtol.evtol_id));
            break;
          }
        }
      }

      // Wait for a defined period before processing the next batch
      std::this_thread::sleep_for(std::chrono::seconds(3));
    }
  }
  catch (const CORBA::Exception &e)
  {
    e._tao_print_exception("Exception caught in main():");
    return -1;
  }
  catch (const std::exception &ex)
  {
    ACE_ERROR_RETURN((LM_ERROR,
                      ACE_TEXT("(%P|%t) ERROR: %N:%l: main() -")
                          ACE_TEXT(" Exception caught: %C\n"),
                      ex.what()),
                     -1);
  }
  return 0;
}
