#ifdef ACE_AS_STATIC_LIBS
#include <C:/Users/ongio_1lak36v/Downloads/OpenDDS-3.29.1/dds/DCPS/transport/tcp/Tcp.h>
#endif
#include <model/Sync.h>
#include <ace/Log_Msg.h>
#include "UATMTraits.h"

#include <model/Sync.h>


int ACE_TMAIN(int argc, ACE_TCHAR **argv)
{
  try
  {
    OpenDDS::Model::Application application(argc, argv);
    UATM::uatmDCPS::DefaultUATMType model(application, argc, argv);

    using OpenDDS::Model::UATM::uatmDCPS::Elements;

    DDS::DataWriter_var writer_auth = model.writer(Elements::DataWriters::airspaceRestDW_SKO);
    UATM::flightAuthorizationDataWriter_var writer_auth_var = UATM::flightAuthorizationDataWriter::_narrow(writer.in());

    if (CORBA::is_nil(writer_auth_var.in())) {
        ACE_ERROR_RETURN((LM_ERROR,
                          ACE_TEXT("(%P|%t) ERROR: %N:%l: main() -")
                          ACE_TEXT(" _narrow failed!\n")),
                         -1);
    }

    OpenDDS::Model::WriterSync ws(writer_auth);
    {
      UATM::flightAuthorization fa;

      // Populate message and send
      fa.authorization_id = 23;
      fa.flight_id = 2;
      fa.approved_route_id = 2;
      fa.authority = "343434";
      fa.authorization_time = "343434";
      fa.valid_until = "343434";

      DDS::ReturnCode_t error = writer_auth_var->write(fa, DDS::HANDLE_NIL);

      if (error != DDS::RETCODE_OK) {
          ACE_ERROR((LM_ERROR,
                     ACE_TEXT("(%P|%t) ERROR: %N:%l: main() -")
                     ACE_TEXT(" write returned %d!\n"), error));
      }
    
    DDS::DataWriter_var writer_rec = model.writer(Elements::DataWriters::airspaceRestDW_SKO);
    UATM::flightChangeRecDataWriter_var writer_rec_var = UATM::flightChangeRecDataWriter::_narrow(writer.in());

    if (CORBA::is_nil(writer_rec_var.in())) {
        ACE_ERROR_RETURN((LM_ERROR,
                          ACE_TEXT("(%P|%t) ERROR: %N:%l: main() -")
                          ACE_TEXT(" _narrow failed!\n")),
                         -1);
      }
    }

    OpenDDS::Model::WriterSync ws(writer_rec);
    {
      UATM::flightChangeRec fc;

      fc.recommendation_id = 23;
      fc.flight_id = 22;
      fc.change_type = "343434";
      fc.reason = "343434";
      fc.recommended_by = "343434";
      fc.recommendation_time = "343434";
      fc.request_status = "343434";

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