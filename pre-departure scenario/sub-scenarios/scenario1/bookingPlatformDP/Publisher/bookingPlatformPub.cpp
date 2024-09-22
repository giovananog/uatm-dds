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

    using OpenDDS::Model::UATM::uatmDCPS::Elements;

    DDS::DataWriter_var writer = model.writer(Elements::DataWriters::bookingFlightRequestDW_BP);

    UATM::bookingFlightRequestDataWriter_var writer_var = UATM::bookingFlightRequestDataWriter::_narrow(writer.in());

    if (CORBA::is_nil(writer_var.in())) {
        ACE_ERROR_RETURN((LM_ERROR,
                          ACE_TEXT("(%P|%t) ERROR: %N:%l: main() -")
                          ACE_TEXT(" _narrow failed!\n")),
                         -1);
    }

    OpenDDS::Model::WriterSync ws(writer);
    {
      UATM::bookingFlightRequest bfr;
      std::cout << "Before sending data" << std::endl;

      bfr.booking_id = 3;
      bfr.flight_id = 2;
      bfr.customer_id = "2";
      bfr.departure_time = "12345-12";
      bfr.arrival_time = "13245-45";
      bfr.route_id = 2;
      bfr.request_status = "pending";

      DDS::ReturnCode_t error = writer_var->write(bfr, DDS::HANDLE_NIL);

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