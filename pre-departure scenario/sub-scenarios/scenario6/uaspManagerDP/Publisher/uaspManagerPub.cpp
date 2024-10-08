#ifdef ACE_AS_STATIC_LIBS
#include <dds/DCPS/transport/tcp/Tcp.h>
#endif
#include <model/Sync.h>
#include <ace/Log_Msg.h>
#include "../../model/UATMTraits.h"


int ACE_TMAIN(int argc, ACE_TCHAR **argv)
{
  try
  {
    OpenDDS::Model::Application application(argc, argv);
    UATM::uatmDCPS::DefaultUATMType model(application, argc, argv);
    UATM::uatmDCPS::DefaultUATMType model2(application, argc, argv);

    using OpenDDS::Model::UATM::uatmDCPS::Elements;

    DDS::DataWriter_var writer_auth = model.writer(Elements::DataWriters::flightAuthDW_UASP);
    UATM::flightAuthorizationDataWriter_var writer_auth_var = UATM::flightAuthorizationDataWriter::_narrow(writer_auth.in());

    if (CORBA::is_nil(writer_auth_var.in())) {
        ACE_ERROR_RETURN((LM_ERROR,
                          ACE_TEXT("(%P|%t) ERROR: %N:%l: main() -")
                          ACE_TEXT(" _narrow failed!\n")),
                         -1);
    }

    OpenDDS::Model::WriterSync ws(writer_auth);
    {
      UATM::flightAuthorization fa;

       
      fa.authorization_id = 23;
      fa.flight_id = 2;
      fa.approved_route_id = 2;
      fa.authority = "authority";
      fa.authorization_time = "343434-20";
      fa.valid_until = "343434-32";

      DDS::ReturnCode_t error = writer_auth_var->write(fa, DDS::HANDLE_NIL);

      if (error != DDS::RETCODE_OK) {
          ACE_ERROR((LM_ERROR,
                     ACE_TEXT("(%P|%t) ERROR: %N:%l: main() -")
                     ACE_TEXT(" write returned %d!\n"), error));
      }
    }

    DDS::DataWriter_var writer_rec = model2.writer(Elements::DataWriters::changeRecDW_UASP);
    UATM::flightChangeRecDataWriter_var writer_rec_var = UATM::flightChangeRecDataWriter::_narrow(writer_rec.in());

    if (CORBA::is_nil(writer_rec_var.in())) {
        ACE_ERROR_RETURN((LM_ERROR,
                          ACE_TEXT("(%P|%t) ERROR: %N:%l: main() -")
                          ACE_TEXT(" _narrow failed!\n")),
                         -1);
      }

    OpenDDS::Model::WriterSync ws2(writer_rec);
    {
      UATM::flightChangeRec fc;

      fc.recommendation_id = 23;
      fc.flight_id = 22;
      fc.change_type = "change_type";
      fc.reason = "reason";
      fc.recommended_by = "recommended_by";
      fc.recommendation_time = "34343-21";
      fc.request_status = "status";

      DDS::ReturnCode_t error = writer_rec_var->write(fc, DDS::HANDLE_NIL);

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