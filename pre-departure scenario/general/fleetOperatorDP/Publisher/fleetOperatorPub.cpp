#include <fstream>
#include <string>
#include <vector>
#include <thread>
#include <chrono>
#include <unordered_set>
#include <model/Sync.h>
#include <ace/Log_Msg.h>
#include "../../model/UATMTraits.h"
#include "../utils/functions.h"

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
    
    // Initialize DDS application and UATM models.
    OpenDDS::Model::Application application(argc, argv);
    UATM::uatmDCPS::DefaultUATMType model(application, argc, argv);

    // Access writer elements within the UATM model namespace.
    using OpenDDS::Model::UATM::uatmDCPS::Elements;

    // Set up DataWriter for flight assignments.
    DDS::DataWriter_var writer_assign = model.writer(Elements::DataWriters::assignFlightDW_FOP);
    UATM::flightAssignDataWriter_var writer_assign_var = UATM::flightAssignDataWriter::_narrow(writer_assign.in());

    // Set up DataWriter for flight coordination.
    DDS::DataWriter_var writer_coord = model.writer(Elements::DataWriters::flightCoordDW_FOP);
    UATM::flightCoordinationDataWriter_var writer_coord_var = UATM::flightCoordinationDataWriter::_narrow(writer_coord.in());

    // Set up DataWriter for flight authorization requests.
    DDS::DataWriter_var writer_request = model.writer(Elements::DataWriters::uaspFlightRequestDW_FOP);
    UATM::flightAuthorizationRequestDataWriter_var writer_request_var = UATM::flightAuthorizationRequestDataWriter::_narrow(writer_request.in());

    if (CORBA::is_nil(writer_request_var.in()) || CORBA::is_nil(writer_assign_var.in()) || CORBA::is_nil(writer_coord_var.in()))
    {
      ACE_ERROR_RETURN((LM_ERROR,
                        ACE_TEXT("(%P|%t) ERROR: %N:%l: main() -")
                            ACE_TEXT(" _narrow failed!\n")),
                       -1);
    }


    // Initialize variables for unique identifiers and tracking.
    int flight_assign_id = 1;
    std::unordered_set<std::string> sent_coord;
    std::unordered_set<std::string> sent_auth;
    std::unordered_set<std::string> assigned_pilots;
    std::unordered_set<std::string> assigned_evtols;

    // Set the simulation start time and duration.
    auto startTime = std::chrono::steady_clock::now();
    double duration = 100.0;

    // Main simulation loop that continues until the duration is reached.
    while (true)
    {
      auto currentTime = std::chrono::steady_clock::now();
      std::chrono::duration<double> elapsedTime = currentTime - startTime;

      // File paths for resource availability, weather, routes, and flight requests.
      std::string resourceFile = "fleetOperatorDP/data/availabilities.txt";
      std::string weatherFile = "fleetOperatorDP/data/weather.txt";
      std::string routeFile = "fleetOperatorDP/data/routes.txt";
      std::string filename = "fleetOperatorDP/data/requests.txt";
      std::string flightFile = "fleetOperatorDP/data/requests.txt";

      // If elapsed time exceeds duration, truncate files and break the loop.
      if (elapsedTime.count() >= duration)
      {
        std::ofstream outfile(resourceFile, std::ofstream::trunc);
        outfile.close();
        std::ofstream outfile2(weatherFile, std::ofstream::trunc);
        outfile2.close();
        std::ofstream outfile3(routeFile, std::ofstream::trunc);
        outfile3.close();
        std::ofstream outfile4(flightFile, std::ofstream::trunc);
        outfile4.close();
        std::ofstream outfile5("fleetOperatorDP/data/authorizations.txt", std::ofstream::trunc);
        outfile5.close();
        break;
      }

      // Load bookings from file and prepare to write flight assignment data.
      std::vector<BookingData> bookings = readBookingsFromFile(filename);
      OpenDDS::Model::WriterSync ws(writer_assign);
      std::string evtolID, pilotID, flightID, originSkID, destSkID;

      {
        // If resources are available, assign flights to evtols and pilots.
        if (checkAvailability(resourceFile, evtolID, pilotID))
        {          
          if (findAndAssignFlight(flightFile, evtolID, pilotID, flightID, originSkID, destSkID))
          {
            UATM::flightAssign fa;

            // Set properties for the flight assignment data.
            fa.flight_assign_id = flight_assign_id++;
            fa.assign_time = CORBA::string_dup(getCurrentTime().c_str());
            fa.flight_id = CORBA::string_dup(flightID.c_str());
            fa.evtol_id = CORBA::string_dup(evtolID.c_str());
            fa.pilot_id = CORBA::string_dup(pilotID.c_str());
            fa.origin_skyport_id = CORBA::string_dup(originSkID.c_str());
            fa.destination_skyport_id = CORBA::string_dup(destSkID.c_str());

            removeAssignedResources(resourceFile, evtolID, pilotID);

            // Write the assignment to DDS.
            DDS::ReturnCode_t error = writer_assign_var->write(fa, DDS::HANDLE_NIL);

            if (error != DDS::RETCODE_OK)
            {
              ACE_ERROR((LM_ERROR,
                         ACE_TEXT("(%P|%t) ERROR: %N:%l: main() -")
                             ACE_TEXT(" write returned %d!\n"),
                         error));
            }
          } 
        }
      }

      // Synchronize and write flight coordination data.
      OpenDDS::Model::WriterSync ws2(writer_coord);
      {
        for (const auto &booking : bookings)
        {
          if (sent_coord.find(std::string(booking.flight_id)) == sent_coord.end() && !booking.evtol_id.empty() && !booking.pilot_id.empty() && !booking.route_id.empty() && !booking.weather_id.empty())
          {
            UATM::flightCoordination fc;
            fc.coordination_id = CORBA::string_dup(booking.booking_id.c_str());
            fc.flight_id = CORBA::string_dup(booking.flight_id.c_str());
            fc.origin_skyport_id = CORBA::string_dup(booking.origin_skyport_id.c_str());
            fc.destination_skyport_id = CORBA::string_dup(booking.destination_skyport_id.c_str());
            fc.evtol_id = CORBA::string_dup(booking.evtol_id.c_str());
            fc.pilot_id = CORBA::string_dup(booking.pilot_id.c_str());
            fc.route_id = CORBA::string_dup(booking.route_id.c_str());
            fc.weather_id = CORBA::string_dup(booking.weather_id.c_str());

            DDS::ReturnCode_t error = writer_coord_var->write(fc, DDS::HANDLE_NIL);

            if (error != DDS::RETCODE_OK)
            {
              ACE_ERROR((LM_ERROR,
                         ACE_TEXT("(%P|%t) ERROR: %N:%l: main() -")
                             ACE_TEXT(" write returned %d!\n"),
                         error));
            }

            sent_coord.insert(std::string(booking.flight_id));

            break;
          }
        }
      }

      // Synchronize and write flight authorization requests.
      OpenDDS::Model::WriterSync ws3(writer_request);
      {
        for (const auto &booking : bookings)
        {
          if (sent_coord.find(std::string(booking.flight_id)) != sent_coord.end() && sent_auth.find(std::string(booking.flight_id)) == sent_auth.end() && !booking.evtol_id.empty() && !booking.pilot_id.empty() && !booking.route_id.empty() && !booking.weather_id.empty())
          {
            std::vector<Route> routes = readRoutesFromFile(routeFile);

            UATM::flightAuthorizationRequest fr;
            Route *route = findRouteById(routes, booking.route_id);

            fr.auth_request_id = "a";
            fr.flight_id = CORBA::string_dup(booking.flight_id.c_str());
            fr.departure_skyport_id = CORBA::string_dup(route->origin_skyport_id.c_str());
            fr.destination_skyport_id = CORBA::string_dup(route->destination_skyport_id.c_str());
            fr.departure_time = CORBA::string_dup(getCurrentTime().c_str());
            fr.pilot_id = CORBA::string_dup(booking.pilot_id.c_str());
            fr.evtol_id = CORBA::string_dup(booking.evtol_id.c_str());

            DDS::ReturnCode_t error = writer_request_var->write(fr, DDS::HANDLE_NIL);

            if (error != DDS::RETCODE_OK)
            {
              ACE_ERROR((LM_ERROR,
                         ACE_TEXT("(%P|%t) ERROR: %N:%l: main() -")
                             ACE_TEXT(" write returned %d!\n"),
                         error));
            }

            sent_auth.insert(std::string(booking.flight_id));

            break;
          }
        }
      }

      // Pause to avoid overwhelming the system.
      std::this_thread::sleep_for(std::chrono::seconds(3));
    }
  }
  catch (const CORBA::Exception &e)
  {
    // Print exception details in case of a CORBA error.
    e._tao_print_exception("CORBA Exception caught in main():");
    return 1;
  }

  return 0;
}
