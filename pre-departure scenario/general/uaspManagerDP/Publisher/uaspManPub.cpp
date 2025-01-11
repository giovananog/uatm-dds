
#include <iostream>                 // Standard input/output stream
#include <fstream>                  // File stream operations
#include <string>                   // String manipulation
#include <sstream>                  // String stream
#include <vector>                   // Dynamic array
#include <thread>                   // Threading functionality
#include <chrono>                   // Time duration and timestamps
#include <unordered_set>            // For storing unique elements efficiently
#include <model/Sync.h>             // Custom synchronization model
#include <ace/Log_Msg.h>            // ACE logging utility
#include "../../model/UATMTraits.h" // Includes specific traits for UATM (Urban Air Traffic Management)
#include "../utils/functions.h"     // Utility functions

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

    // Initialize the application with command-line arguments
    OpenDDS::Model::Application application(argc, argv);

    // Create instances of UATM models 
    UATM::uatmDCPS::DefaultUATMType model(application, argc, argv);

    // Define elements related to UATM data
    using OpenDDS::Model::UATM::uatmDCPS::Elements;

    // Initialize DataWriters for different data categories like route, authorization, and flight changes
    DDS::DataWriter_var writer_route = model.writer(Elements::DataWriters::routeDataDW_UASP);
    UATM::acceptableRouteDataWriter_var writer_route_var = UATM::acceptableRouteDataWriter::_narrow(writer_route.in());

    // Similar steps for flight authorization writer
    DDS::DataWriter_var writer_auth = model.writer(Elements::DataWriters::flightAuthDW_UASP);
    UATM::flightAuthorizationDataWriter_var writer_auth_var = UATM::flightAuthorizationDataWriter::_narrow(writer_auth.in());

    // Similar steps for change record writer
    DDS::DataWriter_var writer_rec = model.writer(Elements::DataWriters::changeRecDW_UASP);
    UATM::flightChangeRecDataWriter_var writer_rec_var = UATM::flightChangeRecDataWriter::_narrow(writer_rec.in());

    // Similar steps for tolpad request writer
    DDS::DataWriter_var writer_assign = model.writer(Elements::DataWriters::tolPadReqDW_UASP);
    UATM::tolPadRequestDataWriter_var writer_assign_var = UATM::tolPadRequestDataWriter::_narrow(writer_assign.in());
    
    if (CORBA::is_nil(writer_rec_var.in()) || CORBA::is_nil(writer_route_var.in()) || CORBA::is_nil(writer_auth_var.in()) || CORBA::is_nil(writer_assign_var.in()))
    {
      ACE_ERROR_RETURN((LM_ERROR,
                        ACE_TEXT("(%P|%t) ERROR: %N:%l: main() -")
                            ACE_TEXT(" _narrow failed!\n")),
                       -1);
    }

    // Set IDs and data structures to track sent requests and tolpads
    int assign_id = 1;
    int acceptable_route_id = 1;
    int recommendation_id = 1;
    std::unordered_set<std::string> sent_flight_ids;
    std::unordered_set<std::string> sent_flight_auth;
    std::unordered_set<std::string> sent_flight_recs;
    std::unordered_set<std::string> sent_tolpads;

    // Record the start time for the operation
    auto startTime = std::chrono::steady_clock::now();
    double duration = 100.0; // Set a duration to monitor the process
    int i = 0;

    // Main processing loop (runs indefinitely until duration is met)
    while (true)
    {
      auto currentTime = std::chrono::steady_clock::now();
      std::chrono::duration<double> elapsedTime = currentTime - startTime;

      // Check if the set duration has passed
      if (elapsedTime.count() >= duration)
      {
        // Close output files if time limit is reached
        std::ofstream outfile("uaspManagerDP/data/flows.txt", std::ofstream::trunc);
        outfile.close();
        std::ofstream outfile2("uaspManagerDP/data/requests.txt", std::ofstream::trunc);
        outfile2.close();
        std::ofstream outfile3("uaspManagerDP/data/restrictions.txt", std::ofstream::trunc);
        outfile3.close();
        std::ofstream outfile4("uaspManagerDP/data/tolpads.txt", std::ofstream::trunc);
        outfile4.close();
        std::ofstream outfile5("uaspManagerDP/data/weather.txt", std::ofstream::trunc);
        outfile5.close();
        break;
      }

      // Read requests and tolpad data from files
      std::vector<requestInfo> requests = read_requests_from_file("uaspManagerDP/data/requests.txt");
      std::vector<TolPad> tolpads = read_tolpads("uaspManagerDP/data/tolpads.txt");

      // Synchronize the writer for tolpad assignments
      OpenDDS::Model::WriterSync ws(writer_assign);
      {
        // Loop through tolpad requests to check for unassigned ones
        for (const auto &tolpad : tolpads)
        {
          // If tolpad is available and hasn't been assigned yet
          if (sent_tolpads.find(std::string(tolpad.resource_id)) == sent_tolpads.end() || tolpad.available == "1")
          {
            // Assign a flight to the available tolpad
            std::string flight_id = getAndUpdateFlightIDWithEmptyTolPad("uaspManagerDP/data/requests.txt", std::string(tolpad.resource_id));
            if (flight_id != "")
            {
              UATM::tolPadRequest tr;
              tr.assign_id = assign_id++;
              tr.flight_id = std::string(flight_id).c_str();
              tr.tol_pad_id = tolpad.resource_id.c_str();
              tr.assign_time = CORBA::string_dup(getCurrentTime().c_str());

              // Send the assignment to the DDS DataWriter
              DDS::ReturnCode_t error = writer_assign_var->write(tr, DDS::HANDLE_NIL);
              if (error != DDS::RETCODE_OK)
              {
                ACE_ERROR((LM_ERROR,
                           ACE_TEXT("(%P|%t) ERROR: %N:%l: main() -")
                               ACE_TEXT(" write returned %d!\n"),
                           error));
              }

              sent_tolpads.insert(std::string(tolpad.resource_id));
              break;
            }
          }
        }
      }

      // Synchronize writer for route assignments
      OpenDDS::Model::WriterSync ws2(writer_route);
      {
        // Loop through requests to check for unprocessed ones
        for (const auto &auth : requests)
        {
          // if (sent_flight_ids.find(std::string(auth.flight_id)) == sent_flight_ids.end() && auth.tolpad_id != "")
          // if (sent_flight_ids.find(std::string(auth.flight_id)) == sent_flight_ids.end())
          // {
          if (i < 2)
          {
            i++;

            std::cout << "\n\n enviou . \n\n";
            // Create a route assignment for the request
            UATM::acceptableRoute ar;
            ar.acceptable_route_id = acceptable_route_id++;
            ar.approved_by = "UASP";
            ar.estimated_time = "12";
            ar.timestamp = CORBA::string_dup(getCurrentTime().c_str());
            ar.flight_id = auth.flight_id.c_str();

            DDS::ReturnCode_t error = writer_route_var->write(ar, DDS::HANDLE_NIL);

            if (error != DDS::RETCODE_OK)
            {
              ACE_ERROR((LM_ERROR,
                         ACE_TEXT("(%P|%t) ERROR: %N:%l: main() -")
                             ACE_TEXT(" write returned %d!\n"),
                         error));
            }

            sent_flight_ids.insert(std::string(auth.flight_id.c_str()));
            break;
          }
        }
      }

      OpenDDS::Model::WriterSync ws3(writer_auth);
      {
        for (const auto &auth : requests)
        {
          if (sent_flight_auth.find(auth.flight_id) == sent_flight_auth.end() && auth.tolpad_id != "")
          {

            std::string flowsID, restrictionID, weatherID;
            bool flowOk = checkFlowConditions("uaspManagerDP/data/flows.txt", auth.departure_skyport_id, flowsID);
            bool restrictionOk = checkRestrictionConditions("uaspManagerDP/data/restrictions.txt", auth.departure_skyport_id, restrictionID);
            bool weatherOk = checkWeatherConditions("uaspManagerDP/data/weather.txt", auth.departure_skyport_id, weatherID);
            UATM::flightAuthorization fa;
            if (flowOk && restrictionOk && weatherOk)
              fa.authorization_status = 1;
            else
              fa.authorization_status = 0;

            fa.authorization_id = auth.request_id.c_str();
            fa.flight_id = auth.flight_id.c_str();
            fa.approved_route_id = "a";
            fa.authorization_time = CORBA::string_dup(getCurrentTime().c_str());
            fa.approved_departure_time = CORBA::string_dup(getCurrentTime().c_str());
            fa.approved_arrival_time = CORBA::string_dup(getCurrentTime().c_str());
            fa.tolpad_id = auth.tolpad_id.c_str();
            fa.pilot_id = auth.pilot_id.c_str();
            fa.evtol_id = auth.evtol_id.c_str();

            DDS::ReturnCode_t error = writer_auth_var->write(fa, DDS::HANDLE_NIL);
            if (error != DDS::RETCODE_OK)
            {
              ACE_ERROR((LM_ERROR,
                         ACE_TEXT("(%P|%t) ERROR: %N:%l: main() -")
                             ACE_TEXT(" write returned %d!\n"),
                         error));
            }

            sent_flight_auth.insert(auth.flight_id);
            break;
          }
        }
      }

      // Synchronize writer for flight change records
      OpenDDS::Model::WriterSync ws4(writer_rec);
      {
        // Loop through requests to send flight change records
        for (const auto &auth : requests)
        {
          // if (sent_flight_recs.find(auth.flight_id) == sent_flight_recs.end())
          if (i < 2)
          {
            i++;
            UATM::flightChangeRec fc;

            fc.recommendation_id = recommendation_id++;
            fc.flight_id = recommendation_id;
            fc.change_type = "change_type";
            fc.reason = "reason";
            fc.recommended_by = "recommended_by";
            fc.recommendation_time = CORBA::string_dup(getCurrentTime().c_str());

            DDS::ReturnCode_t error = writer_rec_var->write(fc, DDS::HANDLE_NIL);
            if (error != DDS::RETCODE_OK)
            {
              ACE_ERROR((LM_ERROR,
                         ACE_TEXT("(%P|%t) ERROR: %N:%l: main() -")
                             ACE_TEXT(" write returned %d!\n"),
                         error));
            }

            sent_flight_recs.insert(std::to_string(fc.flight_id));
            break;
          }
        }
      }
    }

    // End of the program
    ACE_DEBUG((LM_DEBUG,
               ACE_TEXT("(%P|%t) INFO: %N:%l: main() -")
                   ACE_TEXT(" finished processing\n")));
  }
  catch (const CORBA::Exception &ex)
  {
    ACE_ERROR_RETURN((LM_ERROR,
                      ACE_TEXT("(%P|%t) ERROR: %N:%l: main() -")
                          ACE_TEXT(" CORBA exception caught: %s\n"),
                      ex._name()),
                     -1);
  }

  // Return success
  return 0;
}
