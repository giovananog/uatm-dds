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

    DDS::DataWriter_var writer = model.writer(Elements::DataWriters::routeDataDW_UASP);

    UATM::acceptableRouteDataWriter_var writer_var = UATM::acceptableRouteDataWriter::_narrow(writer.in());

    if (CORBA::is_nil(writer_var.in()))
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

    while (true)
    {
      std::string tolPadID;
      std::string resourceFile = "uaspManagerDP/data/tolpads.txt";

      OpenDDS::Model::WriterSync ws(writer);
      {
        if (checkAvailability(resourceFile, tolPadID))
        {
          OpenDDS::Model::WriterSync ws2(writer_assign);
          {
            std::string flight_id = getAndUpdateFlightIDWithEmptyTolPad("uaspManagerDP/data/requests.txt", tolPadID.c_str());

            if (flight_id != "")
            {

              UATM::tolPadRequest tr;
              tr.assign_id = 1;
              tr.flight_id = flight_id.c_str();
              tr.tol_pad_id = tolPadID.c_str();
              tr.assign_time = "13212";

              DDS::ReturnCode_t error = writer_assign_var->write(tr, DDS::HANDLE_NIL);
              if (error != DDS::RETCODE_OK)
              {
                ACE_ERROR((LM_ERROR,
                           ACE_TEXT("(%P|%t) ERROR: %N:%l: main() -")
                               ACE_TEXT(" write returned %d!\n"),
                           error));
              }
            }
            else
            {
              std::cout << "all flight_id processed" << std::endl;
            }
          }
        }
        else
        {
          std::cout << "no tolpad available" << std::endl;
        }
      }

      std::this_thread::sleep_for(std::chrono::seconds(4));

      OpenDDS::Model::WriterSync ws2(writer);
      {
        UATM::acceptableRoute ar;

        ar.acceptable_route_id = "23";
        ar.approved_by = "UASP";
        ar.estimated_time = "12254-53";
        ar.timestamp = "2112-74";

        DDS::ReturnCode_t error = writer_var->write(ar, DDS::HANDLE_NIL);

        if (error != DDS::RETCODE_OK)
        {
          ACE_ERROR((LM_ERROR,
                     ACE_TEXT("(%P|%t) ERROR: %N:%l: main() -")
                         ACE_TEXT(" write returned %d!\n"),
                     error));
        }
      }

      std::this_thread::sleep_for(std::chrono::seconds(4));

      std::vector<UATM::flightAuthorization> authorizations = read_requests_from_file("uaspManagerDP/data/requests.txt");

      OpenDDS::Model::WriterSync ws3(writer);
      {
        UATM::flightAuthorization fa;

        for (const auto &auth : authorizations)
        {
          if (auth.authorization_status)
          {

            std::string flowsID, restrictionID, weatherID;

            bool flowOk = checkFlowConditions("uaspManagerDP/data/flows.txt", "Skyport-1", flowsID);
            bool restrictionOk = checkRestrictionConditions("uaspManagerDP/data/restrictions.txt", "Skyport-1", restrictionID);
            bool weatherOk = checkWeatherConditions("uaspManagerDP/data/weather.txt", "Skyport-1", weatherID);

            if (flowOk && restrictionOk && weatherOk)
              fa.authorization_status = 1;
            else
              fa.authorization_status = 0;

            fa.authorization_id = auth.authorization_id;
            fa.flight_id = auth.flight_id;
            fa.approved_route_id = "a";
            fa.authorization_time = "";
            fa.approved_departure_time = "a";
            fa.approved_arrival_time = "a";
            fa.tolpad_id = auth.tolpad_id;
            fa.pilot_id = auth.pilot_id;
            fa.evtol_id = auth.evtol_id;

            DDS::ReturnCode_t error = writer_auth_var->write(fa, DDS::HANDLE_NIL);

            if (error != DDS::RETCODE_OK)
            {
              ACE_ERROR((LM_ERROR,
                         ACE_TEXT("(%P|%t) ERROR: %N:%l: main() -")
                             ACE_TEXT(" write returned %d!\n"),
                         error));
            }

            break;
          }
        }
      }

      std::this_thread::sleep_for(std::chrono::seconds(4));

      OpenDDS::Model::WriterSync ws4(writer);
      {
        UATM::flightChangeRec fc;

        fc.recommendation_id = 23;
        fc.flight_id = 22;
        fc.change_type = "change_type";
        fc.reason = "reason";
        fc.recommended_by = "recommended_by";
        fc.recommendation_time = "34343-21";

        DDS::ReturnCode_t error = writer_rec_var->write(fc, DDS::HANDLE_NIL);

        if (error != DDS::RETCODE_OK)
        {
          ACE_ERROR((LM_ERROR,
                     ACE_TEXT("(%P|%t) ERROR: %N:%l: main() -")
                         ACE_TEXT(" write returned %d!\n"),
                     error));
        }
      }

      std::this_thread::sleep_for(std::chrono::seconds(4));
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