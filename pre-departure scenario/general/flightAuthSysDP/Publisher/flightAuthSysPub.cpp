#ifdef ACE_AS_STATIC_LIBS
#include <dds/DCPS/transport/tcp/Tcp.h>
#endif
#include <model/Sync.h>
#include <ace/Log_Msg.h>
#include "../../model/UATMTraits.h"
#include "../utils/functions.h"
#include <vector>
#include <dds/DCPS/WaitSet.h>
#include <string>
#include <thread>

int ACE_TMAIN(int argc, ACE_TCHAR **argv)
{
  try
  {
    OpenDDS::Model::Application application(argc, argv);
    UATM::uatmDCPS::DefaultUATMType model(application, argc, argv);

    using OpenDDS::Model::UATM::uatmDCPS::Elements;

    DDS::DataWriter_var writer_request = model.writer(Elements::DataWriters::flightRequestInfoDW_FAS);
    UATM::flightRequestInfoDataWriter_var writer_request_var = UATM::flightRequestInfoDataWriter::_narrow(writer_request.in());

    if (CORBA::is_nil(writer_request_var.in()))
    {
      ACE_ERROR_RETURN((LM_ERROR,
                        ACE_TEXT("(%P|%t) ERROR: %N:%l: main() -")
                            ACE_TEXT(" _narrow failed!\n")),
                       -1);
    }

    while (true)
    {

      OpenDDS::Model::WriterSync ws(writer_request);
      {
        UATM::flightRequestInfo fr;
        std::vector<flightRequestInfo> requests = readRequestsFromFile("flightAuthSysDP/data/requests.txt");

        for (auto &fri : requests)
        {
          if (fri.sent_req == 0)
          {
            fr.request_id = CORBA::string_dup(fri.auth_request_id.c_str());
            fr.flight_id = CORBA::string_dup(fri.flight_id.c_str());
            fr.departure_skyport_id = CORBA::string_dup(fri.departure_skyport_id.c_str());
            fr.destination_skyport_id = CORBA::string_dup(fri.destination_skyport_id.c_str());
            fr.departure_time = CORBA::string_dup(fri.departure_time.c_str());
            fr.pilot_id = CORBA::string_dup(fri.pilot_id.c_str());
            fr.evtol_id = CORBA::string_dup(fri.evtol_id.c_str());

            DDS::ReturnCode_t error = writer_request_var->write(fr, DDS::HANDLE_NIL);

            if (error != DDS::RETCODE_OK)
            {
              ACE_ERROR((LM_ERROR,
                         ACE_TEXT("(%P|%t) ERROR: %N:%l: main() -")
                             ACE_TEXT(" write returned %d!\n"),
                         error));
            }

            std::string flight_id_str = CORBA::string_dup(fri.flight_id.c_str());
            updateSentReq("flightAuthSysDP/data/requests.txt", flight_id_str);
            break;
          }
        }
      }
      std::this_thread::sleep_for(std::chrono::seconds(10));
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