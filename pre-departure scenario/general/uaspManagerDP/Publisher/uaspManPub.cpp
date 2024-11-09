#ifdef ACE_AS_STATIC_LIBS
#include <dds/DCPS/transport/tcp/Tcp.h>  // Include the TCP transport layer if ACE is statically linked
#endif

#include <iostream>        // Standard input/output stream
#include <fstream>         // File stream operations
#include <string>          // String manipulation
#include <sstream>         // String stream
#include <vector>          // Dynamic array
#include <thread>          // Threading functionality
#include <chrono>          // Time duration and timestamps
#include <unordered_set>   // For storing unique elements efficiently
#include <model/Sync.h>    // Custom synchronization model
#include <ace/Log_Msg.h>   // ACE logging utility
#include "../../model/UATMTraits.h"  // Includes specific traits for UATM (Urban Air Traffic Management)
#include "../utils/functions.h"  // Utility functions

// Main entry point of the application
int ACE_TMAIN(int argc, ACE_TCHAR **argv)
{
  try
  {
    // Initialize the application with command-line arguments
    OpenDDS::Model::Application application(argc, argv);

    // Create instances of UATM models (they represent different data writers for different traffic data)
    UATM::uatmDCPS::DefaultUATMType model(application, argc, argv);
    UATM::uatmDCPS::DefaultUATMType model2(application, argc, argv);
    UATM::uatmDCPS::DefaultUATMType model3(application, argc, argv);
    UATM::uatmDCPS::DefaultUATMType model4(application, argc, argv);

    // Define elements related to UATM data
    using OpenDDS::Model::UATM::uatmDCPS::Elements;

    // Initialize DataWriters for different data categories like route, authorization, and flight changes
    DDS::DataWriter_var writer_route = model.writer(Elements::DataWriters::routeDataDW_UASP);
    UATM::acceptableRouteDataWriter_var writer_route_var = UATM::acceptableRouteDataWriter::_narrow(writer_route.in());
    // Check if the writer is valid
    if (CORBA::is_nil(writer_route_var.in()))
    {
      ACE_ERROR_RETURN((LM_ERROR,
                        ACE_TEXT("(%P|%t) ERROR: %N:%l: main() -")
                            ACE_TEXT(" _narrow failed!\n")),
                       -1);
    }

    // Similar steps for flight authorization writer
    DDS::DataWriter_var writer_auth = model2.writer(Elements::DataWriters::flightAuthDW_UASP);
    UATM::flightAuthorizationDataWriter_var writer_auth_var = UATM::flightAuthorizationDataWriter::_narrow(writer_auth.in());
    if (CORBA::is_nil(writer_auth_var.in()))
    {
      ACE_ERROR_RETURN((LM_ERROR,
                        ACE_TEXT("(%P|%t) ERROR: %N:%l: main() -")
                            ACE_TEXT(" _narrow failed!\n")),
                       -1);
    }

    // Similar steps for change record writer
    DDS::DataWriter_var writer_rec = model3.writer(Elements::DataWriters::changeRecDW_UASP);
    UATM::flightChangeRecDataWriter_var writer_rec_var = UATM::flightChangeRecDataWriter::_narrow(writer_rec.in());
    if (CORBA::is_nil(writer_rec_var.in()))
    {
      ACE_ERROR_RETURN((LM_ERROR,
                        ACE_TEXT("(%P|%t) ERROR: %N:%l: main() -")
                            ACE_TEXT(" _narrow failed!\n")),
                       -1);
    }

    // Similar steps for tolpad request writer
    DDS::DataWriter_var writer_assign = model4.writer(Elements::DataWriters::tolPadReqDW_UASP);
    UATM::tolPadRequestDataWriter_var writer_assign_var = UATM::tolPadRequestDataWriter::_narrow(writer_assign.in());
    if (CORBA::is_nil(writer_assign_var.in()))
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
    std::unordered_set<std::string> sent_tolpads;

    // Record the start time for the operation
    auto startTime = std::chrono::steady_clock::now();
    double duration = 100.0;  // Set a duration to monitor the process

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
        bool sent = false;

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
              sent = true;
              break;
            }
          }
        }
      }

      // Synchronize writer for route assignments
      OpenDDS::Model::WriterSync ws2(writer_route);
      {
        bool sent = false;

        // Loop through requests to check for unprocessed ones
        for (const auto &auth : requests)
        {
          if (sent_flight_ids.find(auth.flight_id) == sent_flight_ids.end() && auth.tolpad_id != "")
          {
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
            sent = true;
            break;
          }
        }
      }

      // Synchronize writer for flight change records
      OpenDDS::Model::WriterSync ws4(writer_rec);
      {
        bool sent = false;

        // Loop through requests to send flight change records
        for (const auto &auth : requests)
        {
          if (sent_flight_ids.find(auth.flight_id) == sent_flight_ids.end() && auth.tolpad_id != "")
          {
            UATM::flightChangeRec fc;

            fc.recommendation_id = recommendation_id++;
            fc.flight_id = 1;
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

            sent = true;
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
