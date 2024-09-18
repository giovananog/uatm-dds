#ifdef ACE_AS_STATIC_LIBS
#include <dds/DCPS/transport/tcp/Tcp.h>
#endif
#include <model/Sync.h>
#include <ace/Log_Msg.h>
#include "../model/UATMTraits.h"


int ACE_TMAIN(int argc, ACE_TCHAR **argv)
{
  try
  {
    OpenDDS::Model::Application application(argc, argv);
    UATM::uatmDCPS::DefaultUATMType model(application, argc, argv);

    using OpenDDS::Model::UATM::uatmDCPS::Elements;

    DDS::DataWriter_var writer_request = model.writer(Elements::DataWriters::flightRequestInfoDW_FAS);
    UATM::flightRequestInfoDataWriter_var writer_request_var = UATM::flightRequestInfoDataWriter::_narrow(writer_request.in());

    if (CORBA::is_nil(writer_request_var.in())) {
        ACE_ERROR_RETURN((LM_ERROR,
                          ACE_TEXT("(%P|%t) ERROR: %N:%l: main() -")
                          ACE_TEXT(" _narrow failed!\n")),
                         -1);
    }

    OpenDDS::Model::WriterSync ws(writer_request);
    {
      UATM::flightRequestInfo fri;

      // Populate message and send
      fri.request_id = 23;
      fri.flight_id = 22;
      fri.requestor_id = 2;
      fri.requested_route_id = 3;
      fri.request_status = true;
      fri.request_time = "34343-24";

      DDS::ReturnCode_t error = writer_request_var->write(fri, DDS::HANDLE_NIL);

      if (error != DDS::RETCODE_OK) {
          ACE_ERROR((LM_ERROR,
                     ACE_TEXT("(%P|%t) ERROR: %N:%l: main() -")
                     ACE_TEXT(" write returned %d!\n"), error));
      }
    }
  } catch (const CORBA::Exception& e) {
    e._tao_print_exception("Exception caught in main():");
    return -1;

  } catch( const std::exception& ex) {
    ACE_ERROR_RETURN((LM_ERROR,
                      ACE_TEXT("(%P|%t) ERROR: %N:%l: main() -")
                      ACE_TEXT(" Exception caught: %C\n"),
                      ex.what()),
                     -1);
  }
  return 0;
}