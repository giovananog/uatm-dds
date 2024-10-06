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
    UATM::uatmDCPS::DefaultUATMType model4(application, argc, argv);

    using OpenDDS::Model::UATM::uatmDCPS::Elements;

    DDS::DataWriter_var writer_route = model.writer(Elements::DataWriters::routeDataDW_UASP);

    UATM::acceptableRouteDataWriter_var writer_route_var = UATM::acceptableRouteDataWriter::_narrow(writer_route.in());

    if (CORBA::is_nil(writer_route_var.in()))
    {
      ACE_ERROR_RETURN((LM_ERROR,
                        ACE_TEXT("(%P|%t) ERROR: %N:%l: main() -")
                            ACE_TEXT(" _narrow failed!\n")),
                       -1);
    }

    DDS::DataWriter_var writer_auth = model2.writer(Elements::DataWriters::flightAuthDW_UASP);
    UATM::flightAuthorizationDataWriter_var writer_auth_var = UATM::flightAuthorizationDataWriter::_narrow(writer_auth.in());

    if (CORBA::is_nil(writer_auth_var.in()))
    {
      ACE_ERROR_RETURN((LM_ERROR,
                        ACE_TEXT("(%P|%t) ERROR: %N:%l: main() -")
                            ACE_TEXT(" _narrow failed!\n")),
                       -1);
    }

    DDS::DataWriter_var writer_rec = model3.writer(Elements::DataWriters::changeRecDW_UASP);
    UATM::flightChangeRecDataWriter_var writer_rec_var = UATM::flightChangeRecDataWriter::_narrow(writer_rec.in());

    if (CORBA::is_nil(writer_rec_var.in()))
    {
      ACE_ERROR_RETURN((LM_ERROR,
                        ACE_TEXT("(%P|%t) ERROR: %N:%l: main() -")
                            ACE_TEXT(" _narrow failed!\n")),
                       -1);
    }

    DDS::DataWriter_var writer_assign = model4.writer(Elements::DataWriters::tolPadReqDW_UASP);
    UATM::tolPadRequestDataWriter_var writer_assign_var = UATM::tolPadRequestDataWriter::_narrow(writer_assign.in());

    if (CORBA::is_nil(writer_assign_var.in()))
    {
      ACE_ERROR_RETURN((LM_ERROR,
                        ACE_TEXT("(%P|%t) ERROR: %N:%l: main() -")
                            ACE_TEXT(" _narrow failed!\n")),
                       -1);
    }

    int assign_id = 1;
    int acceptable_route_id = 1;
    int recommendation_id = 1;
    int i = 0;
    std::unordered_set<std::string> sent_flight_ids;
    std::unordered_set<std::string> sent_tolpads;

    while (true)
    {
      std::vector<requestInfo> requests = read_requests_from_file("uaspManagerDP/data/requests.txt");
      std::vector<TolPad> tolpads = read_tolpads("uaspManagerDP/data/tolpads.txt");

      OpenDDS::Model::WriterSync ws(writer_assign);
      {
        bool sent = false;
        for (const auto &tolpad : tolpads)
        {
          if (sent_tolpads.find(std::string(tolpad.resource_id)) == sent_tolpads.end())
          {
            std::string flight_id = getAndUpdateFlightIDWithEmptyTolPad("uaspManagerDP/data/requests.txt", std::string(tolpad.resource_id));
            if (flight_id != "")
            {
              UATM::tolPadRequest tr;
              tr.assign_id = assign_id++;
              tr.flight_id = std::string(flight_id).c_str();
              tr.tol_pad_id = tolpad.resource_id.c_str();
              tr.assign_time = CORBA::string_dup(getCurrentTime().c_str());

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

      std::this_thread::sleep_for(std::chrono::seconds(5));

      OpenDDS::Model::WriterSync ws2(writer_route);
      {
        bool sent = false;

        for (const auto &auth : requests)
        {
          if (sent_flight_ids.find(auth.flight_id) == sent_flight_ids.end())
          {

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

      std::this_thread::sleep_for(std::chrono::seconds(5));

      OpenDDS::Model::WriterSync ws4(writer_rec);
      {
        bool sent = false;

        for (const auto &auth : requests)
        {
          if (sent_flight_ids.find(auth.flight_id) == sent_flight_ids.end())
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

      std::this_thread::sleep_for(std::chrono::seconds(5));

      OpenDDS::Model::WriterSync ws3(writer_auth);
      {
        bool sent = false;
        for (const auto &auth : requests)
        {
          if (sent_flight_ids.find(auth.flight_id) == sent_flight_ids.end() && auth.tolpad_id != "")
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

            i++;
            sent = true;
            sent_flight_ids.insert(auth.flight_id);
            break;
          }
        }
      }

      std::this_thread::sleep_for(std::chrono::seconds(5));

      if (i == 3)
      {
        break;
      }
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