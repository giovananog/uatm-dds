#include <model/Sync.h>  
#include <ace/Log_Msg.h>  
#include "../../model/UATMTraits.h"  
#include "../Utils/functionsFAS.h"  
#include <vector>  
#include <dds/DCPS/WaitSet.h>  
#include <string>  
#include <thread>  
#include <unordered_set>  

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
    TheServiceParticipant->set_security(true); 

    // Initializes the application with command-line arguments.
    OpenDDS::Model::Application application(argc, argv);
    UATM::uatmDCPS::DefaultUATMType model(application, argc, argv);

    using OpenDDS::Model::UATM::uatmDCPS::Elements;

    // Creates the writer for the flightRequestInfo type.
    DDS::DataWriter_var writer_request = model.writer(Elements::DataWriters::flightRequestInfoDW_FAS);
    UATM::flightRequestInfoDataWriter_var writer_request_var = UATM::flightRequestInfoDataWriter::_narrow(writer_request.in());

    // Checks if the narrowing of the writer was successful.
    if (CORBA::is_nil(writer_request_var.in()))
    {
      ACE_ERROR_RETURN((LM_ERROR,
                        ACE_TEXT("(%P|%t) ERROR: %N:%l: main() -")
                            ACE_TEXT(" _narrow failed!\n")),
                       -1);
    }

    std::unordered_set<std::string> sent_requests;  // Set to store sent request IDs.
    auto startTime = std::chrono::steady_clock::now();  // Captures the start time.
    double duration = 100.0;  // Defines the main loop duration.

    // Main loop that will continue until the duration is met.
    while (true)
    {
      auto currentTime = std::chrono::steady_clock::now();  // Gets the current time.
      std::chrono::duration<double> elapsedTime = currentTime - startTime;  // Calculates the elapsed time.

      // If the duration is reached, breaks out of the loop.
      if (elapsedTime.count() >= duration)
      {
        std::ofstream outfile("flightAuthSysDP/data/requests.txt", std::ofstream::trunc);
        outfile.close();  // Closes the requests file.
        break;  // Exits the loop.
      }

      // Reads requests from a file.
      std::vector<flightRequestInfo> requests = readRequestsFromFile("flightAuthSysDP/data/requests.txt");

      OpenDDS::Model::WriterSync ws(writer_request);  // Creates an object to synchronize the writer.
      {
        UATM::flightRequestInfo fr;  // Creates an instance of flightRequestInfo.

        // Iterates over the requests read from the file.
        for (auto &fri : requests)
        {
          // Checks if the request has already been sent.
          if (sent_requests.find(std::string(fri.flight_id)) == sent_requests.end())
          {
            // Fills the fields of the fr object with the request data.
            fr.request_id = CORBA::string_dup(fri.auth_request_id.c_str());
            fr.flight_id = CORBA::string_dup(fri.flight_id.c_str());
            fr.departure_skyport_id = CORBA::string_dup(fri.departure_skyport_id.c_str());
            fr.destination_skyport_id = CORBA::string_dup(fri.destination_skyport_id.c_str());
            fr.departure_time = CORBA::string_dup(fri.departure_time.c_str());
            fr.pilot_id = CORBA::string_dup(fri.pilot_id.c_str());
            fr.evtol_id = CORBA::string_dup(fri.evtol_id.c_str());

            // Sends the message through the writer.
            DDS::ReturnCode_t error = writer_request_var->write(fr, DDS::HANDLE_NIL);

            // Checks if there was an error sending the message.
            if (error != DDS::RETCODE_OK)
            {
              ACE_ERROR((LM_ERROR,
                         ACE_TEXT("(%P|%t) ERROR: %N:%l: main() -")
                             ACE_TEXT(" write returned %d!\n"),
                         error));
            }

            // Marks the request as sent by adding its ID to the set.
            std::string flight_id_str = CORBA::string_dup(fri.flight_id.c_str());
            sent_requests.insert(std::string(flight_id_str));

            break;  // Exits the loop after sending the first request.
          }
        }
      }
    }
  }
  catch (const CORBA::Exception &e)  // Catches CORBA exceptions.
  {
    e._tao_print_exception("Exception caught in main():");
    return -1;
  }
  catch (const std::exception &ex)  // Catches standard exceptions.
  {
    ACE_ERROR_RETURN((LM_ERROR,
                      ACE_TEXT("(%P|%t) ERROR: %N:%l: main() -")
                          ACE_TEXT(" Exception caught: %C\n"),
                      ex.what()),
                     -1);
  }
  return 0;
}
