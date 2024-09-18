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
    UATM::uatmDCPS::DefaultUATMType model2(application, argc, argv);

    using OpenDDS::Model::UATM::uatmDCPS::Elements;

    DDS::DataWriter_var writer_coord = model.writer(Elements::DataWriters::flightCoordDW_FOP);
    UATM::flightCoordinationDataWriter_var writer_coord_var = UATM::flightCoordinationDataWriter::_narrow(writer_coord.in());

    if (CORBA::is_nil(writer_coord_var.in())) {
        ACE_ERROR_RETURN((LM_ERROR,
                          ACE_TEXT("(%P|%t) ERROR: %N:%l: main() -")
                          ACE_TEXT(" _narrow failed!\n")),
                         -1);
    }

    OpenDDS::Model::WriterSync ws(writer_coord);
    {
      UATM::flightCoordination fc;

      // Populate message and send
      fc.coordination_id = 23;
      fc.flight_id = 22;
      UATM::ArrayString involved_parties;

      fc.involved_parties[0] = "abc";
      fc.involved_parties[1] = "def";
      UATM::ArrayString coordination_details;

      fc.coordination_details[0] = "abc";
      fc.coordination_details[1] = "def";
      fc.recommendation_time = "343434";

      DDS::ReturnCode_t error = writer_coord_var->write(fc, DDS::HANDLE_NIL);

      if (error != DDS::RETCODE_OK) {
          ACE_ERROR((LM_ERROR,
                     ACE_TEXT("(%P|%t) ERROR: %N:%l: main() -")
                     ACE_TEXT(" write returned %d!\n"), error));
      }
    }
    
    DDS::DataWriter_var writer_assign = model2.writer(Elements::DataWriters::uaspFlightRequestDW_FOP);
    UATM::flightAuthorizationRequestDataWriter_var writer_assign_var = UATM::flightAuthorizationRequestDataWriter::_narrow(writer_assign.in());

    if (CORBA::is_nil(writer_assign_var.in())) {
        ACE_ERROR_RETURN((LM_ERROR,
                          ACE_TEXT("(%P|%t) ERROR: %N:%l: main() -")
                          ACE_TEXT(" _narrow failed!\n")),
                         -1);
      }

    OpenDDS::Model::WriterSync ws2(writer_assign);
    {
      UATM::flightAuthorizationRequest fr;

      // Populate message and send
      fr.auth_request_id = 23;
      fr.uasp_id = 22;
      fr.flight_id = 22;
      fr.request_status = true;
      fr.request_time = "34343-24";

      DDS::ReturnCode_t error = writer_assign_var->write(fr, DDS::HANDLE_NIL);

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