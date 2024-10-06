#ifdef ACE_AS_STATIC_LIBS
#include <dds/DCPS/transport/tcp/Tcp.h>
#endif
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <thread>
#include <chrono>
#include <unordered_set>
#include <model/Sync.h>
#include <ace/Log_Msg.h>
#include "../../model/UATMTraits.h"
#include "../utils/functions.h"

int ACE_TMAIN(int argc, ACE_TCHAR **argv)
{
  try
  {
    OpenDDS::Model::Application application(argc, argv);
    UATM::uatmDCPS::DefaultUATMType model(application, argc, argv);
    UATM::uatmDCPS::DefaultUATMType model2(application, argc, argv);
    UATM::uatmDCPS::DefaultUATMType model3(application, argc, argv);

    using OpenDDS::Model::UATM::uatmDCPS::Elements;

    DDS::DataWriter_var writer_assign = model.writer(Elements::DataWriters::assignFlightDW_FOP);
    UATM::flightAssignDataWriter_var writer_assign_var = UATM::flightAssignDataWriter::_narrow(writer_assign.in());

    if (CORBA::is_nil(writer_assign_var.in()))
    {
      ACE_ERROR_RETURN((LM_ERROR,
                        ACE_TEXT("(%P|%t) ERROR: %N:%l: main() -")
                            ACE_TEXT(" _narrow failed!\n")),
                       -1);
    }

    DDS::DataWriter_var writer_coord = model3.writer(Elements::DataWriters::flightCoordDW_FOP);
    UATM::flightCoordinationDataWriter_var writer_coord_var = UATM::flightCoordinationDataWriter::_narrow(writer_coord.in());

    if (CORBA::is_nil(writer_coord_var.in()))
    {
      ACE_ERROR_RETURN((LM_ERROR,
                        ACE_TEXT("(%P|%t) ERROR: %N:%l: main() -")
                            ACE_TEXT(" _narrow failed!\n")),
                       -1);
    }

    DDS::DataWriter_var writer_request = model2.writer(Elements::DataWriters::uaspFlightRequestDW_FOP);
    UATM::flightAuthorizationRequestDataWriter_var writer_request_var = UATM::flightAuthorizationRequestDataWriter::_narrow(writer_request.in());

    if (CORBA::is_nil(writer_request_var.in()))
    {
      ACE_ERROR_RETURN((LM_ERROR,
                        ACE_TEXT("(%P|%t) ERROR: %N:%l: main() -")
                            ACE_TEXT(" _narrow failed!\n")),
                       -1);
    }

    int flight_assign_id = 1;
    int i = 0;

    std::unordered_set<std::string> sent_coord;
    std::unordered_set<std::string> sent_auth;
    std::unordered_set<std::string> assigned_pilots;
    std::unordered_set<std::string> assigned_evtols;

    while (true)
    {
      if (i == 3)
      {
        break;
      }
      std::string evtolID;
      std::string pilotID;
      std::string flightID;
      std::string resourceFile = "fleetOperatorDP/data/availabilities.txt";
      std::string weatherFile = "fleetOperatorDP/data/weather.txt";
      std::string routeFile = "fleetOperatorDP/data/routes.txt";
      std::string filename = "fleetOperatorDP/data/requests.txt";
      std::string flightFile = "fleetOperatorDP/data/requests.txt";
      std::vector<BookingData> bookings = readBookingsFromFile(filename);

      OpenDDS::Model::WriterSync ws(writer_assign);
      {
        if (checkAvailability(resourceFile, evtolID, pilotID))
        {
          if (findAndAssignFlight(flightFile, evtolID, pilotID, flightID))
          {
            UATM::flightAssign fa;

            fa.flight_assign_id = flight_assign_id++;
            fa.assign_time = CORBA::string_dup(getCurrentTime().c_str());
            fa.flight_id = CORBA::string_dup(flightID.c_str());
            fa.evtol_id = CORBA::string_dup(evtolID.c_str());
            fa.pilot_id = CORBA::string_dup(pilotID.c_str());

            removeAssignedResources(resourceFile, evtolID, pilotID);

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

      std::this_thread::sleep_for(std::chrono::seconds(3));

      OpenDDS::Model::WriterSync ws2(writer_coord);
      {
        bool sent = false;

        for (const auto &booking : bookings)
        {
          if (sent_coord.find(std::string(booking.flight_id)) == sent_coord.end() && !booking.evtol_id.empty() && !booking.pilot_id.empty() && !booking.route_id.empty() && !booking.weather_id.empty())
          {
            UATM::flightCoordination fc;
            fc.coordination_id = CORBA::string_dup(booking.booking_id.c_str());
            fc.flight_id = CORBA::string_dup(booking.flight_id.c_str());
            fc.skyport_id = CORBA::string_dup(booking.skyport_id.c_str());
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

            sent = true;
            i++;

            sent_coord.insert(std::string(booking.flight_id));

            break;
          }
        }
      }

      std::this_thread::sleep_for(std::chrono::seconds(3));

      OpenDDS::Model::WriterSync ws3(writer_request);
      {

        bool sent = false;

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

            sent = true;
            sent_auth.insert(std::string(booking.flight_id));
            break;
          }
        }
      }

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